package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"path/filepath"

	"github.com/google/uuid"
)

const UPLOAD_DIR = "uploads"

// 只存储一个 Token（初始示例）
var CURRENT_TOKEN = "" // 默认 Token，可修改

func init() {
	// 确保上传目录存在
	if err := os.MkdirAll(UPLOAD_DIR, os.ModePerm); err != nil {
		log.Fatalf("无法创建上传目录: %v", err)
	}
}

func main() {
	// 解析命令行参数
	port := flag.Int("port", 12025, "服务器端口号")
	flag.Parse()

	// 静态文件服务
	http.Handle("/static/", http.StripPrefix("/static/", http.FileServer(http.Dir(UPLOAD_DIR))))

	// 上传文件接口
	http.HandleFunc("/upload", verifyToken(uploadFile))

	// 更新 Token 接口
	http.HandleFunc("/update_token", verifyToken(updateToken))

	fmt.Printf("服务器正在 0.0.0.0:%d 上运行...\n", *port)
	log.Fatal(http.ListenAndServe(fmt.Sprintf("0.0.0.0:%d", *port), nil))
}

// verifyToken 中间件函数
func verifyToken(next http.HandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		token := r.Header.Get("X-Token")
		if token != CURRENT_TOKEN {
			http.Error(w, "Invalid or missing Token", http.StatusForbidden)
			return
		}
		next.ServeHTTP(w, r)
	}
}

// uploadFile 处理函数
func uploadFile(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "只支持 POST 请求", http.StatusMethodNotAllowed)
		return
	}

	file, handler, err := r.FormFile("file")
	if err != nil {
		http.Error(w, fmt.Sprintf("获取文件失败: %v", err), http.StatusBadRequest)
		return
	}
	defer file.Close()

	ext := filepath.Ext(handler.Filename)
	uniqueName := fmt.Sprintf("%s%s", uuid.New().String(), ext)
	filePath := filepath.Join(UPLOAD_DIR, uniqueName)

	out, err := os.Create(filePath)
	if err != nil {
		http.Error(w, fmt.Sprintf("创建文件失败: %v", err), http.StatusInternalServerError)
		return
	}
	defer out.Close()

	_, err = io.Copy(out, file)
	if err != nil {
		http.Error(w, fmt.Sprintf("保存文件失败: %v", err), http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(map[string]string{
		"url":     fmt.Sprintf("/static/%s", uniqueName),
		"message": "File uploaded successfully",
	})
}

// TokenUpdateRequest Pydantic 模型，用于 Token 修改请求
type TokenUpdateRequest struct {
	NewToken string `json:"new_token"` // 新 Token
}

// updateToken 处理函数
func updateToken(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "只支持 POST 请求", http.StatusMethodNotAllowed)
		return
	}

	var req TokenUpdateRequest
	err := json.NewDecoder(r.Body).Decode(&req)
	if err != nil {
		http.Error(w, fmt.Sprintf("解析请求体失败: %v", err), http.StatusBadRequest)
		return
	}

	if req.NewToken == "" {
		http.Error(w, "Token cannot be empty", http.StatusBadRequest)
		return
	}

	// 修改 Token（只能有一个 Token，不能新增）
	CURRENT_TOKEN = req.NewToken // 更新 Token

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(map[string]string{
		"message":  "Token updated successfully",
		"new_token": CURRENT_TOKEN, // 返回新 Token（仅用于调试）
	})
} 