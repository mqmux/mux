package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"strings"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

var clients = make(map[*websocket.Conn]bool) // 连接的客户端映射
var broadcast = make(chan Message)           // 广播消息通道

// 定义WebSocket消息结构
type Message struct {
	Author string `json:"author"`
	Text   string `json:"text"`
}

type send_Message struct {
	Author string `json:"author"`
	Text   string `json:"text"`
	Time   string `json:"time"`
}

type MemoryDB struct {
	sync.RWMutex
	data map[string]string // 用于存储id和data字符串
}

// 递归解析嵌套的JSON数据
func getValueByPath(data interface{}, path []string) (interface{}, bool) {
	if len(path) == 0 {
		return data, true
	}

	switch v := data.(type) {
	case map[string]interface{}:
		if val, ok := v[path[0]]; ok {
			return getValueByPath(val, path[1:])
		}
	}
	return nil, false
}

// UpdateRecord 更新指定 ID 的记录
func (db *MemoryDB) UpdateRecord(id string, data string) {
	db.Lock()         // 加锁以防止数据竞争
	defer db.Unlock() // 解锁

	// 更新记录
	db.data[id] = data
}

// 创建新的内存数据库
func NewMemoryDB() *MemoryDB {
	return &MemoryDB{
		data: make(map[string]string),
	}
}

// 创建记录
func (db *MemoryDB) CreateRecord(id string, data string) error {
	db.Lock()         // 加锁，保证并发安全
	defer db.Unlock() // 解锁
	db.data[id] = data
	return nil
}

// 根据 ID 获取记录
func (db *MemoryDB) GetRecordByID(id string) (string, bool) {
	db.RLock()         // 读锁
	defer db.RUnlock() // 解锁
	data, exists := db.data[id]
	return data, exists
}

// 获取所有记录
func (db *MemoryDB) GetAllRecords() map[string]string {
	db.RLock()         // 读锁
	defer db.RUnlock() // 解锁
	return db.data
}

// 删除记录
func (db *MemoryDB) DeleteRecordByID(id string) bool {
	db.Lock()         // 加锁
	defer db.Unlock() // 解锁
	if _, exists := db.data[id]; exists {
		delete(db.data, id)
		return true
	}
	return false
}

// 最近的消息队列，最多存储10个消息
var recentMessages = make([]send_Message, 10)
var messageIndex = 0
var messageCount = 0

// 升级HTTP连接为WebSocket连接
var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

func main() {
	// 默认端口号
	port := "8080"

	// 检查是否有命令行参数传递端口号
	if len(os.Args) > 1 {
		port = os.Args[1]
		log.Println("使用的端口号:", port)
	} else {
		log.Println("未指定端口号，默认使用端口 8080")
	}

	db := NewMemoryDB()
	// 定义HTTP路由
	http.HandleFunc("/ws", handleConnections)

	// 设置静态文件目录
	fs := http.FileServer(http.Dir("./static"))
	http.Handle("/static/", http.StripPrefix("/static/", fs))

	// 设置默认路由，访问根路径 "/" 时返回 index.html
	http.Handle("/", http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		// 检查请求的路径是否为 "/"
		if r.URL.Path == "/" {
			http.ServeFile(w, r, "./static/index.html")
			return
		}
		// 如果不是根路径，则使用静态文件处理器处理请求
		fs.ServeHTTP(w, r)
	}))

	// 注册获取最近消息的接口
	http.HandleFunc("/getRecentMessages", handleGetRecentMessages)

	// 注册发送消息给所有客户端的接口
	http.HandleFunc("/broadcastMessage", handleBroadcastMessage)

	// 处理创建记录的 HTTP 路由
	http.HandleFunc("/create", func(w http.ResponseWriter, r *http.Request) {
		var input map[string]interface{} // 用于解析传入的 JSON

		// 解析 JSON 数据
		if err := json.NewDecoder(r.Body).Decode(&input); err != nil {
			http.Error(w, "Invalid input", http.StatusBadRequest)
			return
		}

		// 从输入中提取id和data字段
		id, ok := input["id"].(string)
		if id == "" {
			http.Error(w, "ID is empty", http.StatusBadRequest)
			return
		}
		if !ok {
			http.Error(w, "Invalid or missing 'id'", http.StatusBadRequest)
			return
		}

		// 处理 data 字段
		var data string
		if d, ok := input["data"]; ok {
			// 检查 data 是否是一个 JSON 对象
			if dataObj, isObject := d.(map[string]interface{}); isObject {
				// 将整个对象序列化为 JSON 字符串
				jsonData, err := json.Marshal(dataObj)
				if err != nil {
					http.Error(w, "Invalid 'data'", http.StatusBadRequest)
					return
				}
				data = string(jsonData)
			} else {
				// 如果不是对象，尝试将其转为字符串
				data = fmt.Sprintf("%v", d)
			}
		} else {
			http.Error(w, "Missing 'data'", http.StatusBadRequest)
			return
		}

		// 存储记录
		db.CreateRecord(id, data)

		// 返回成功响应
		w.WriteHeader(http.StatusOK)
		fmt.Fprintf(w, "Record with ID %s created successfully", id)
	})

	http.HandleFunc("/records/", func(w http.ResponseWriter, r *http.Request) {
		path := strings.TrimPrefix(r.URL.Path, "/records/")
		keys := strings.Split(path, "/")

		id := keys[0]

		if len(keys) == 0 || keys[0] == "" {
			http.Error(w, "ID is empty", http.StatusBadRequest)
			return
		}
		if r.Method == http.MethodGet {
			data, exists := db.GetRecordByID(id)
			if !exists {
				http.Error(w, "Record not found", http.StatusNotFound)
				return
			}

			// 解析返回的 JSON
			var inputData map[string]interface{}
			err := json.Unmarshal([]byte(data), &inputData)
			if err != nil {
				w.Header().Set("Content-Type", "application/json")
				w.Write([]byte(data))
				return
			}

			result, found := getValueByPath(inputData, keys[1:])
			if !found {
				w.WriteHeader(http.StatusNotFound)
				json.NewEncoder(w).Encode(map[string]string{"error": "Path not found in the JSON"})
				return
			}

			// 返回原始 JSON 数据
			beautifiedJSON, err := json.MarshalIndent(result, "", "    ")
			if err != nil {
				w.WriteHeader(http.StatusInternalServerError)
				json.NewEncoder(w).Encode(map[string]string{"error": "Failed to marshal response"})
				return
			}

			w.Header().Set("Content-Type", "application/json; charset=UTF-8")
			w.WriteHeader(http.StatusOK)
			w.Write(beautifiedJSON)
		} else if r.Method == http.MethodPut {
			var input map[string]interface{}
			if err := json.NewDecoder(r.Body).Decode(&input); err != nil {
				http.Error(w, "Invalid input", http.StatusBadRequest)
				return
			}

			data, _ := json.Marshal(input["data"])
			db.UpdateRecord(id, string(data))
			w.WriteHeader(http.StatusOK)
			fmt.Fprintf(w, "Record with ID %s updated successfully", id)
		} else if r.Method == http.MethodDelete {
			if db.DeleteRecordByID(id) {
				fmt.Fprintf(w, "Record with ID %s deleted successfully", id)
			} else {
				http.Error(w, "Record not found", http.StatusNotFound)
			}
		} else {
			http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
		}
	})

	// 获取所有记录的 HTTP 路由
	http.HandleFunc("/records", func(w http.ResponseWriter, r *http.Request) {
		if r.Method == http.MethodPost {
			var input map[string]interface{} // 用于解析传入的 JSON
			if err := json.NewDecoder(r.Body).Decode(&input); err != nil {
				http.Error(w, "Invalid input", http.StatusBadRequest)
				return
			}
			id, ok := input["id"].(string)
			if !ok {
				http.Error(w, "Invalid or missing 'id'", http.StatusBadRequest)
				return
			}
			var data string
			if d, ok := input["data"]; ok {
				if dataObj, isObject := d.(map[string]interface{}); isObject {
					jsonData, err := json.Marshal(dataObj)
					if err != nil {
						http.Error(w, "Invalid 'data'", http.StatusBadRequest)
						return
					}
					data = string(jsonData)
				} else {
					data = fmt.Sprintf("%v", d)
				}
			} else {
				http.Error(w, "Missing 'data'", http.StatusBadRequest)
				return
			}
			db.CreateRecord(id, data)
			w.WriteHeader(http.StatusOK)
			fmt.Fprintf(w, "Record with ID %s created successfully", id)
			return
		}

		allRecords := db.GetAllRecords()
		response := make(map[string]interface{})
		for id, data := range allRecords {
			var recordData interface{}

			// 尝试解析 JSON 数据
			if err := json.Unmarshal([]byte(data), &recordData); err != nil {
				// 如果解析失败，直接将 data 作为字符串存储
				response[id] = data
			} else {
				// 如果成功解析，将解析后的数据存入响应中
				response[id] = recordData
			}
		}

		// 设置响应头为 JSON 格式
		w.Header().Set("Content-Type", "application/json")

		// 使用 json.MarshalIndent 美化 JSON 输出
		beautifiedJSON, err := json.MarshalIndent(response, "", "    ")
		if err != nil {
			http.Error(w, "Internal Server Error", http.StatusInternalServerError)
			return
		}

		// 发送美化后的 JSON 响应
		w.Write(beautifiedJSON)
	})

	// 删除记录的 HTTP 路由
	http.HandleFunc("/delete/", func(w http.ResponseWriter, r *http.Request) {
		id := r.URL.Path[len("/delete/"):] // 从 URL 中提取 ID
		if id == "" {
			http.Error(w, "ID is empty", http.StatusBadRequest)
			return
		}

		if db.DeleteRecordByID(id) {
			fmt.Fprintf(w, "Record with ID %s deleted successfully", id)
		} else {
			http.Error(w, "Record not found", http.StatusNotFound)
		}
	})

	// 启动广播协程
	go handleMessages()

	// 启动HTTP服务器并监听在指定端口
	addr := ":" + port
	log.Printf("HTTP server started on %s\n", addr)
	err := http.ListenAndServe(addr, nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}

// 处理发送消息给所有客户端的HTTP请求
func handleBroadcastMessage(w http.ResponseWriter, r *http.Request) {
	// 解析请求体中的JSON数据
	var msg Message
	err := json.NewDecoder(r.Body).Decode(&msg)
	if err != nil {
		log.Printf("Error decoding JSON: %v", err)
		http.Error(w, "Bad Request", http.StatusBadRequest)
		return
	}

	// 将消息发送到广播通道
	broadcast <- msg

	// 返回成功响应
	w.WriteHeader(http.StatusOK)
}

// 处理获取最近消息的HTTP请求
func handleGetRecentMessages(w http.ResponseWriter, r *http.Request) {
	// 设置响应头为JSON格式
	w.Header().Set("Content-Type", "application/json")

	// 将最近消息列表转换为JSON并发送给客户端
	err := json.NewEncoder(w).Encode(recentMessages)
	if err != nil {
		log.Printf("Error encoding recent messages: %v", err)
		http.Error(w, "Internal Server Error", http.StatusInternalServerError)
		return
	}
}

// 处理WebSocket连接
func handleConnections(w http.ResponseWriter, r *http.Request) {
	// 升级连接为WebSocket
	ws, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer ws.Close()

	// 获取客户端的远程地址信息

	log.Printf("新连接的WebSocket地址：%s\n", ws.RemoteAddr().String())

	// 将新连接的客户端添加到映射中
	clients[ws] = true
	index := 0
	// 发送最近的消息给客户端
	for i := 0; i < messageCount; i++ {

		if messageCount == 10 {
			index = (messageIndex + i) % 10 // 计算当前应发送的消息索引
		} else {
			index = i // 如果消息数量未满10，则直接使用索引
		}

		// 打印即将发送的消息内容
		//log.Printf("recentMessages[%d]: %v", index, recentMessages[index])

		err := ws.WriteJSON(recentMessages[index])
		if err != nil {
			log.Printf("发送信息给客户端失败: %v", err)
			break
		}
	}

	// 循环处理客户端的消息
	for {
		var msg Message
		//var msg interface{} // 使用 interface{} 接收任意 JSON 数据
		// 读取客户端发来的消息
		err := ws.ReadJSON(&msg)
		if err != nil {
			// 检查具体的错误类型来判断是否是连接断开的情况
			if websocket.IsCloseError(err, websocket.CloseNormalClosure, websocket.CloseGoingAway) {
				log.Printf("客户端 %v 断开连接：%v", ws.RemoteAddr().String(), err)
			} else {
				log.Printf("读取消息时发生错误：%v", err)
			}

			//log.Printf("error: %v", err)
			delete(clients, ws)
			break
		}
		// 打印收到的消息内容
		log.Printf("{'author':'%v','text':'%v'}", msg.Author, msg.Text)
		// 将消息发送到广播通道
		broadcast <- msg
	}
}

// 处理广播消息
func handleMessages() {
	for {
		// 从广播通道获取消息
		msg := <-broadcast

		// 如果收到 "cls" 消息，则清空 recentMessages
		if msg.Text == "cls" {
			recentMessages = make([]send_Message, 10) // 清空 recentMessages
			messageIndex = 0                          // 重置 messageIndex
			messageCount = 0                          // 重置 messageCount
			continue                                  // 跳过后续代码，直接进入下一次循环
		}

		smsg := send_Message{
			Author: msg.Author,
			Text:   msg.Text,
			Time:   time.Now().Format("2006-01-02 15:04:05"),
		}

		// 将消息存入最近消息队列
		recentMessages[messageIndex] = smsg
		messageIndex = (messageIndex + 1) % 10 // 循环使用数组，确保不超出索引范围

		// 如果消息数量未满10，则增加计数
		if messageCount < 10 {
			messageCount++
		}

		// 遍历连接的客户端，将消息发送给每个客户端
		for client := range clients {
			if client.WriteJSON(smsg) != nil {
				log.Printf("error sending message to client")
				client.Close()
				delete(clients, client)
			}
		}
	}
}
