package main

import (
	"database/sql"
	"encoding/json"
	"flag"
	"io"
	"log"
	"net/http"
	"runtime"
	"strings"
	"sync"
	"time"

	"github.com/gorilla/mux"
	_ "github.com/mattn/go-sqlite3"
)

var db *sql.DB
var token *string
var dbMutex sync.Mutex // 数据库操作互斥锁

// 锁定状态管理
type LockState struct {
	ID        string
	Timestamp time.Time
	Timeout   time.Duration // 添加超时时间字段
}

var (
	lockMutex sync.Mutex
	lockState *LockState
)

// 检查是否可以执行请求
func canExecuteRequest(id string, shouldLock bool) bool {
	lockMutex.Lock()
	defer lockMutex.Unlock()

	// 如果ID为空，直接允许执行
	if id == "" {
		return true
	}

	// 检查现有锁是否超时
	if lockState != nil {
		if time.Since(lockState.Timestamp) > lockState.Timeout {
			lockState = nil // 如果超时，清除锁定状态
		}
	}

	// 如果没有锁定状态，且请求要求锁定
	if lockState == nil && shouldLock {
		lockState = &LockState{
			ID:        id,
			Timestamp: time.Now(),
			Timeout:   time.Second * 5, // 设置5秒超时
		}
		return true
	}

	// 如果已经有锁定状态
	if lockState != nil {
		// 如果是同一个ID，且请求要求解锁
		if lockState.ID == id && !shouldLock {
			lockState = nil
			return true
		}
		// 如果是同一个ID，且请求要求锁定
		if lockState.ID == id && shouldLock {
			lockState.Timestamp = time.Now() // 更新锁定时间
			return true
		}
		// 如果是不同ID，且当前请求要求锁定
		if lockState.ID != id && shouldLock {
			return false
		}
		// 如果是不同ID，且当前请求要求解锁
		if lockState.ID != id && !shouldLock {
			return false
		}
	}

	return true
}

// 智能分割SQL语句
func splitSQLStatements(sql string) []string {
	var statements []string
	var current strings.Builder
	inString := false
	stringChar := byte(0)

	for i := 0; i < len(sql); i++ {
		char := sql[i]

		// 处理字符串字面量
		if char == '\'' || char == '"' {
			if !inString {
				inString = true
				stringChar = char
			} else if char == stringChar {
				inString = false
			}
		}

		// 只有在不在字符串中时才处理分号
		if char == ';' && !inString {
			stmt := strings.TrimSpace(current.String())
			if stmt != "" {
				statements = append(statements, stmt)
			}
			current.Reset()
		} else {
			current.WriteByte(char)
		}
	}

	// 处理最后一个语句
	if stmt := strings.TrimSpace(current.String()); stmt != "" {
		statements = append(statements, stmt)
	}

	return statements
}

func main() {
	// 定义命令行参数
	dbFile := flag.String("db", "sqlite.db", "Database file name")
	port := flag.String("port", "8080", "Server port")
	token = flag.String("token", "", "Authorization token for database operations")

	// 定义短格式参数
	flag.StringVar(dbFile, "d", "sqlite.db", "Database file name (short)")
	flag.StringVar(port, "p", "8080", "Server port (short)")
	flag.StringVar(token, "t", "", "Authorization token for database operations (short)")

	// 解析命令行参数
	flag.Parse()

	var err error
	// 创建并打开数据库
	db, err = sql.Open("sqlite3", *dbFile)

	db.SetMaxOpenConns(1)                  // 设置最大连接数为1
	db.SetMaxIdleConns(1)                  // 设置最大空闲连接数为1
	db.SetConnMaxLifetime(time.Minute * 5) // 设置最大连接生命周期

	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	// 初始化路由
	r := mux.NewRouter()
	r.HandleFunc("/execute", executeSQL).Methods("POST", "OPTIONS")

	// 启动服务器
	log.Printf("Server is running on port %s", *port)
	log.Fatal(http.ListenAndServe(":"+*port, r))
}

func executeSQL(w http.ResponseWriter, r *http.Request) {
	// 限制请求体大小为
	r.Body = http.MaxBytesReader(w, r.Body, 8192)

	// 允许跨域请求
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS")
	w.Header().Set("Access-Control-Allow-Headers", "Content-Type, Authorization")

	// 处理 OPTIONS 请求
	if r.Method == http.MethodOptions {
		w.WriteHeader(http.StatusOK)
		return
	}

	// 检查 token
	if *token != "" {
		reqToken := r.Header.Get("Authorization")
		log.Printf("[Authorization] [%s], Len:[%d]", reqToken, len(reqToken))
		if !strings.HasPrefix(reqToken, "Bearer ") || len(reqToken) <= 7 || reqToken[7:] != *token {
			log.Printf("[NOT][%s]", reqToken)
			http.Error(w, "Unauthorized", http.StatusUnauthorized)
			return
		}
	}

	var query struct {
		SQL  string `json:"sql"`
		Lock bool   `json:"lock"`
		ID   string `json:"id"`
	}
	if err := json.NewDecoder(r.Body).Decode(&query); err != nil {
		bodyBytes, _ := io.ReadAll(r.Body)
		log.Printf("解码错误: %v, 请求体内容: %s", err, string(bodyBytes))
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	// 检查锁定状态
	if !canExecuteRequest(query.ID, query.Lock) {
		log.Printf("请求被锁定，ID: %s", query.ID)
		error_str := "Request blocked by lock ID:" + lockState.ID
		http.Error(w, error_str, http.StatusLocked)
		return
	}

	if query.SQL == "" {
		log.Println("空查询")
		http.Error(w, "SQL query cannot be empty", http.StatusBadRequest)
		return
	}
	log.Printf("[QUERY] %s", query.SQL)

	// 使用互斥锁确保数据库操作的串行执行
	dbMutex.Lock()
	defer dbMutex.Unlock()

	// 分割多条 SQL 语句
	statements := splitSQLStatements(query.SQL)

	var results []map[string]interface{}
	for _, stmt := range statements {
		stmt = strings.TrimSpace(stmt)
		if stmt == "" {
			continue
		}

		// 判断是否是 SELECT 语句
		if strings.HasPrefix(strings.ToUpper(stmt), "SELECT") {
			rows, err := db.Query(stmt)
			if err != nil {
				log.Printf("执行查询错误: %v", err)
				http.Error(w, err.Error(), http.StatusInternalServerError)
				return
			}
			defer rows.Close()

			columns, err := rows.Columns()
			if err != nil {
				log.Printf("获取列错误: %v", err)
				http.Error(w, err.Error(), http.StatusInternalServerError)
				return
			}

			// 创建一个映射以记录列顺序
			order := map[string]int{}
			for i, col := range columns {
				order[col] = i
			}

			for rows.Next() {
				values := make([]interface{}, len(columns))
				for i := range values {
					values[i] = new(interface{})
				}

				if err := rows.Scan(values...); err != nil {
					log.Printf("扫描错误: %v", err)
					http.Error(w, err.Error(), http.StatusInternalServerError)
					return
				}

				row := make(map[string]interface{})
				for col, idx := range order {
					row[col] = *(values[idx].(*interface{}))
				}
				results = append(results, row)
			}
		} else {
			// 执行非 SELECT 语句
			_, err := db.Exec(stmt)
			if err != nil {
				log.Printf("执行非查询语句错误: %v", err)
				http.Error(w, err.Error(), http.StatusInternalServerError)
				return
			}
		}
	}

	log.Printf("response %d rows.", len(results))

	w.Header().Set("Content-Type", "application/json")

	if err := json.NewEncoder(w).Encode(results); err != nil {
		log.Printf("编码响应错误: %v", err)
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
	runtime.GC()
}

// 过滤空语句
func filterEmptyStatements(statements []string) []string {
	var result []string
	for _, stmt := range statements {
		if strings.TrimSpace(stmt) != "" {
			result = append(result, stmt)
		}
	}
	return result
}
