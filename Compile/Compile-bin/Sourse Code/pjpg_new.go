package main

import (
	"crypto/rand"
	"database/sql"
	"encoding/hex"
	"flag"
	"fmt"
	"io"
	"net/http"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"time"

	_ "github.com/mattn/go-sqlite3"

	"github.com/gin-gonic/gin"
)

const (
	UPLOAD_DIR = "uploads"
)

var (
	currentToken string
	tokenMutex   sync.RWMutex // 保护 token 的读写
	db           *sql.DB
)

// FileRecord 文件记录结构
type FileRecord struct {
	ID           int64
	RandomName   string
	OriginalName string
	ActualName   string
	FilePath     string
	CreatedAt    string
}

func init() {
	// 初始化上传目录
	if err := os.MkdirAll(UPLOAD_DIR, 0755); err != nil {
		panic(fmt.Sprintf("创建上传目录失败: %v", err))
	}
	// 生成初始 Token
	currentToken = generateSecureToken(10)
	fmt.Printf("初始 Token: %s\n", currentToken)

	// 初始化数据库
	var err error
	db, err = sql.Open("sqlite3", "./files.db")
	if err != nil {
		panic(fmt.Sprintf("打开数据库失败: %v", err))
	}

	// 创建文件表
	_, err = db.Exec(`
		CREATE TABLE IF NOT EXISTS files (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			random_name TEXT NOT NULL,
			original_name TEXT NOT NULL,
			actual_name TEXT NOT NULL,
			file_path TEXT NOT NULL,
			created_at DATETIME DEFAULT CURRENT_TIMESTAMP
		)
	`)
	if err != nil {
		panic(fmt.Sprintf("创建表失败: %v", err))
	}
}

func generateSecureToken(length int) string {
	b := make([]byte, length)
	if _, err := rand.Read(b); err != nil {
		panic(err)
	}
	return hex.EncodeToString(b)
}

func verifyToken() gin.HandlerFunc {
	return func(c *gin.Context) {
		token := c.GetHeader("X-Token")
		tokenMutex.RLock()
		validToken := currentToken
		tokenMutex.RUnlock()

		if token == "" {
			c.AbortWithStatusJSON(http.StatusUnauthorized, gin.H{"error": "Missing token"})
			return
		}

		if token != validToken {
			c.AbortWithStatusJSON(http.StatusForbidden, gin.H{"error": "Invalid token"})
			return
		}
		c.Next()
	}
}

type TokenUpdateRequest struct {
	NewToken string `json:"new_token" binding:"required"`
}

// 判断文件是否应该直接在浏览器中显示
func shouldDisplayInline(filename string) bool {
	ext := strings.ToLower(filepath.Ext(filename))
	// 常见的可以直接在浏览器中显示的文件类型
	displayableExts := map[string]bool{
		".jpg":  true,
		".jpeg": true,
		".png":  true,
		".gif":  true,
		".pdf":  true,
		".txt":  true,
		".html": true,
		".htm":  true,
		".svg":  true,
		".webp": true,
	}
	return displayableExts[ext]
}

func main() {
	// 解析命令行参数
	port := flag.Int("port", 10000, "服务器端口号")
	flag.Parse()

	router := gin.Default()

	// 文件上传端点（需要 Token 验证）
	router.POST("/upload", verifyToken(), func(c *gin.Context) {
		// 获取上传的文件
		file, header, err := c.Request.FormFile("file")
		if err != nil {
			c.JSON(http.StatusBadRequest, gin.H{"error": "Missing file"})
			return
		}
		defer file.Close()

		// 生成日期目录
		now := time.Now()
		dateDir := now.Format("2006-01-02")
		uploadPath := filepath.Join(UPLOAD_DIR, dateDir)

		// 创建日期目录
		if err := os.MkdirAll(uploadPath, 0755); err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to create directory"})
			return
		}

		// 生成唯一文件名
		ext := filepath.Ext(header.Filename)
		randomName := generateSecureToken(16)
		actualName := randomName + ext
		filePath := filepath.Join(uploadPath, actualName)

		// 创建目标文件
		out, err := os.Create(filePath)
		if err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to create file"})
			return
		}
		defer out.Close()

		// 复制文件内容
		if _, err = io.Copy(out, file); err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to save file"})
			return
		}

		// 保存文件记录到数据库
		_, err = db.Exec(
			"INSERT INTO files (random_name, original_name, actual_name, file_path) VALUES (?, ?, ?, ?)",
			randomName,
			header.Filename,
			actualName,
			filepath.Join(dateDir, actualName),
		)
		if err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to save file record"})
			return
		}

		c.JSON(http.StatusOK, gin.H{
			"url":     randomName,
			"message": "File uploaded successfully",
		})
	})

	// 文件下载端点
	router.GET("/static/:filename", func(c *gin.Context) {
		requestedName := c.Param("filename")

		// 从数据库查询文件信息
		var originalName, actualName, filePath string
		err := db.QueryRow(
			"SELECT original_name, actual_name, file_path FROM files WHERE random_name = ? OR actual_name = ?",
			requestedName,
			requestedName,
		).Scan(&originalName, &actualName, &filePath)

		if err != nil {
			c.JSON(http.StatusNotFound, gin.H{"error": "File not found"})
			return
		}

		// 检查文件是否存在
		fullPath := filepath.Join(UPLOAD_DIR, filePath)
		if _, err := os.Stat(fullPath); os.IsNotExist(err) {
			c.JSON(http.StatusNotFound, gin.H{"error": "File not found"})
			return
		}

		// 根据文件类型设置 Content-Disposition
		if shouldDisplayInline(originalName) {
			c.Header("Content-Disposition", fmt.Sprintf("inline; filename=%s", originalName))
		} else {
			c.Header("Content-Disposition", fmt.Sprintf("attachment; filename=%s", originalName))
		}
		c.File(fullPath)
	})

	// 清理旧文件端点（需要 Token 验证）
	router.POST("/cleanup", verifyToken(), func(c *gin.Context) {
		var req struct {
			Days int `json:"days"`
		}
		if err := c.ShouldBindJSON(&req); err != nil {
			// 只在JSON解析错误时打印请求体
			body, _ := io.ReadAll(c.Request.Body)
			fmt.Printf("JSON binding error: %v\nReceived request body: %s\n", err, string(body))
			c.JSON(http.StatusBadRequest, gin.H{
				"error":         "Invalid request",
				"details":       err.Error(),
				"received_body": string(body),
			})
			return
		}

		// 验证天数必须大于等于0
		if req.Days < 0 {
			c.JSON(http.StatusBadRequest, gin.H{
				"error":         "Days cannot be negative",
				"received_days": req.Days,
			})
			return
		}

		// 计算截止日期（包含今天）
		now := time.Now()
		cutoffDate := time.Date(now.Year(), now.Month(), now.Day(), 0, 0, 0, 0, now.Location())
		if req.Days > 0 {
			cutoffDate = cutoffDate.AddDate(0, 0, -req.Days)
		}
		cutoffStr := cutoffDate.Format("2006-01-02")

		// 查询需要删除的文件记录
		rows, err := db.Query("SELECT file_path FROM files WHERE date(created_at) <= date(?)", cutoffStr)
		if err != nil {
			fmt.Printf("Database query error: %v\n", err)
			c.JSON(http.StatusInternalServerError, gin.H{
				"error":       "Failed to query files",
				"details":     err.Error(),
				"cutoff_date": cutoffStr,
			})
			return
		}
		defer rows.Close()

		var deletedFiles int
		var failedFiles int

		// 删除文件
		dateDirs := make(map[string]bool) // 记录包含文件的日期目录
		for rows.Next() {
			var filePath string
			if err := rows.Scan(&filePath); err != nil {
				failedFiles++
				continue
			}

			fullPath := filepath.Join(UPLOAD_DIR, filePath)
			if err := os.Remove(fullPath); err != nil {
				failedFiles++
				continue
			}
			deletedFiles++

			// 记录文件所在的日期目录
			dateDir := filepath.Dir(filePath)
			dateDirs[dateDir] = true
		}

		// 删除数据库记录
		_, err = db.Exec("DELETE FROM files WHERE date(created_at) <= date(?)", cutoffStr)
		if err != nil {
			fmt.Printf("Database delete error: %v\n", err)
			c.JSON(http.StatusInternalServerError, gin.H{
				"error":       "Failed to delete database records",
				"details":     err.Error(),
				"cutoff_date": cutoffStr,
			})
			return
		}

		// 检查并删除空的日期目录
		var deletedDirs int
		for dateDir := range dateDirs {
			dirPath := filepath.Join(UPLOAD_DIR, dateDir)
			// 检查目录是否为空
			entries, err := os.ReadDir(dirPath)
			if err != nil {
				continue
			}
			if len(entries) == 0 {
				if err := os.Remove(dirPath); err == nil {
					deletedDirs++
				}
			}
		}

		c.JSON(http.StatusOK, gin.H{
			"message":       "Cleanup completed",
			"deleted_files": deletedFiles,
			"failed_files":  failedFiles,
			"deleted_dirs":  deletedDirs,
			"cutoff_date":   cutoffStr,
			"days":          req.Days,
		})
	})

	// Token 更新端点（需要 Token 验证）
	router.POST("/update_token", verifyToken(), func(c *gin.Context) {
		var req TokenUpdateRequest
		if err := c.ShouldBindJSON(&req); err != nil {
			c.JSON(http.StatusBadRequest, gin.H{"error": "Invalid request"})
			return
		}

		if req.NewToken == "" {
			c.JSON(http.StatusBadRequest, gin.H{"error": "Token cannot be empty"})
			return
		}

		tokenMutex.Lock()
		currentToken = req.NewToken
		tokenMutex.Unlock()

		c.JSON(http.StatusOK, gin.H{
			"message":   "Token updated successfully",
			"new_token": currentToken,
		})
	})

	// 获取当前 Token（测试用）
	router.GET("/current_token", func(c *gin.Context) {
		tokenMutex.RLock()
		defer tokenMutex.RUnlock()
		c.JSON(http.StatusOK, gin.H{"token": currentToken})
	})

	fmt.Printf("服务启动: http://0.0.0.0:%d\n", *port)
	if err := router.Run(fmt.Sprintf(":%d", *port)); err != nil {
		panic(fmt.Sprintf("服务器启动失败: %v", err))
	}
}
