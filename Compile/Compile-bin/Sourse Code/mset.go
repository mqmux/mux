package main

import (
	//"bufio"
	"database/sql"
	//"flag"
	"fmt"
	"log"
	"os"
	"strings"

	_ "github.com/go-sql-driver/mysql"
	"github.com/olekukonko/tablewriter"
)

func main() {
	// 配置 MySQL 连接信息
	//dsn := "root:yourpassword@tcp(127.0.0.1:3306)/your_database"
	dsn := "mmux_cctv:yourpassword@tcp(mysql.sqlpub.com:3306)/mmux_cctv"
	// 连接数据库
	db, err := sql.Open("mysql", dsn)
	if err != nil {
		log.Fatal("数据库连接失败:", err)
	}
	defer db.Close()

	// 测试连接
	if err := db.Ping(); err != nil {
		log.Fatal("无法连接到数据库:", err)
	}
	// fmt.Println("成功连接到数据库")

	var query string

	if len(os.Args) > 1 {
		// 获取命令行参数并分割成 key=value 格式
		//arg := os.Args[1]
		args := os.Args[1:]
		arg := strings.Join(args, " ")
		if arg == "=" {
			query = "SELECT * FROM var;"
			mysql_chat(db, query)
			return
		}
		//parts := strings.Split(arg, "=")
		parts := strings.SplitN(arg, "=", 2) // 分割成最多2个部分
		if len(parts) == 2 {
			key := parts[0]
			value := parts[1]

			// 输出 key 和 value
			fmt.Printf("[%v][%v]\n", key, value)

			if key == "" {
				fmt.Println("键值为空")
				return
			}

			// 查询表中是否有指定 KEY 的记录
			var count int
			err = db.QueryRow("SELECT COUNT(*) FROM var WHERE `KEY` = ?", key).Scan(&count)
			if err != nil {
				log.Fatal("查询失败:", err)
			}
			if count > 0 {
				if value == "" {
					// 执行删除操作
					result, err := db.Exec("DELETE FROM var WHERE `KEY` = ?", key)
					if err != nil {
						log.Fatal("删除失败:", err)
					}
					rowsAffected, err := result.RowsAffected()
					if err != nil {
						log.Fatal("获取影响的行数失败:", err)
					}
					if rowsAffected > 0 {
						fmt.Println("记录已删除")
					} else {
						fmt.Println("没有找到匹配的记录")
					}
					return
				}
				// 如果记录存在，则执行 UPDATE
				_, err = db.Exec("UPDATE var SET `VALUE` = ? WHERE `KEY` = ?", value, key)
				if err != nil {
					log.Fatal("更新失败:", err)
				}
				fmt.Println("记录已更新")
			} else {
				// 如果记录不存在，则执行 INSERT
				_, err = db.Exec("INSERT INTO var (`KEY`, `VALUE`) VALUES (?, ?)", key, value)
				if err != nil {
					log.Fatal("插入失败:", err)
				}
				fmt.Println("新记录已插入")
			}
			return
		}

		if len(parts) == 1 {
			var value string
			err := db.QueryRow("SELECT value FROM var WHERE `KEY` = ?", arg).Scan(&value)
			if err != nil {
				if err == sql.ErrNoRows {
					fmt.Println("没有找到对应的记录")
				} else {
					log.Fatal("查询失败:", err)
				}
			} else {
				fmt.Print(value)
			}
			return
		}
	}

	fmt.Println("Usage: mset varname=value")
	fmt.Println("   or: mset varname (it will show value)")
	query = "SELECT * FROM msg ORDER BY id DESC LIMIT 10;"
	mysql_chat(db, query)
	
	/* fmt.Println("[注册]\n")
	// 获取作者输入
	fmt.Print("名字: ")
	reader := bufio.NewReader(os.Stdin)
	author_name, _ := reader.ReadString('\n')
	author_name = strings.TrimSpace(author_name) // 去掉换行符
	if author_name == "" {
		return
	}
	
	fmt.Print("邮箱: ")
	reader = bufio.NewReader(os.Stdin)
	author, _ := reader.ReadString('\n')
	author = strings.TrimSpace(author) // 去掉换行符
	if author == "" {
		return
	}
	
	// 获取信息输入
	fmt.Print("密码: ")
	info, _ := reader.ReadString('\n')
	info = strings.TrimSpace(info) // 去掉换行符
	if info == "" {
		return
	}

	// 构建 SQL 插入语句
	query = fmt.Sprintf("INSERT INTO user (`name`,`email`, `key`) VALUES ('%s', '%s', '%s');", author_name, author, info)
	mysql_chat(db, query) */
}

func mysql_chat(db *sql.DB, query string) {
	// 判断 SQL 语句类型
	queryType := strings.ToUpper(strings.Fields(query)[0]) // 获取 SQL 关键字（第一个单词）

	if queryType == "SELECT" {
		// 处理 SELECT 语句
		executeSelectQuery(db, query)
	} else {
		// 处理 INSERT/UPDATE/DELETE 语句
		executeModifyQuery(db, query)
	}
}

// **处理 SELECT 语句**
func executeSelectQuery(db *sql.DB, query string) {
	rows, err := db.Query(query)
	if err != nil {
		log.Fatal("查询失败:", err)
	}
	defer rows.Close()

	// 获取列名
	columns, err := rows.Columns()
	if err != nil {
		log.Fatal("无法获取列名:", err)
	}

	// 初始化 tablewriter
	table := tablewriter.NewWriter(os.Stdout)
	table.SetHeader(columns) // 设置表头
	table.SetBorder(true)    // 启用边框

	// 读取数据
	for rows.Next() {
		values := make([]sql.RawBytes, len(columns))
		valuePtrs := make([]interface{}, len(columns))
		for i := range values {
			valuePtrs[i] = &values[i]
		}

		err := rows.Scan(valuePtrs...)
		if err != nil {
			log.Fatal("扫描行失败:", err)
		}

		rowData := make([]string, len(columns))
		for i, val := range values {
			if val == nil {
				rowData[i] = "NULL"
			} else {
				rowData[i] = string(val)
			}
		}

		table.Append(rowData)
	}

	// 渲染表格
	table.Render()

	// 检查查询错误
	if err = rows.Err(); err != nil {
		log.Fatal("查询过程中出现错误:", err)
	}
}

// **处理 INSERT/UPDATE/DELETE 语句**
func executeModifyQuery(db *sql.DB, query string) {
	//result, err := db.Exec(query)
	_, err := db.Exec(query)
	if err != nil {
		log.Fatal("执行失败:", err)
	}

	// 获取影响的行数
	// rowsAffected, err := result.RowsAffected()
	// if err != nil {
	// 	log.Fatal("无法获取受影响的行数:", err)
	// }

	// fmt.Printf("SQL 执行成功，影响了 %d 行数据。\n", rowsAffected)
}
