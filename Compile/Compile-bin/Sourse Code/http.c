/*
CONSOLE MINI HTTP SERVER, COPYRIGHT@2017~2019 BY HAPPY, VERSION 1.0
MISV.EXE
FRI AUG 18 2017 18:16:01 GMT+0800
////////////////////////////////////////////////////////////////////
说明：
	跨平台代码，可在Windows、Linux、安卓等多种嵌入式设备上直译。
	兼容GCC、VS编译器。 如使用VS请先改名为misv.cpp方可正常编译。
	编译时，可通过定义USE_FAST_MODE、USE_SLEEP_MODE 宏实现优化。

编译：
	gcc misv.c -lWs2_32  -o misv.exe       REM Windows下编译
	gcc misv.c -lpthread -o misv           REM Linux下编译
	gcc misv.c                             REM 安卓及嵌入式编译
	cl  misv.cpp /MD /Ox /out:misv.exe     REM VS下编译
////////////////////////////////////////////////////////////////////
*/

#define _CRT_SECURE_NO_WARNINGS

////////////////////////////////////////////////////////////////////
// 服务器名称
#define HTTP_SERVER_NAME "MISV"

// 服务器端口
#define HTTP_SERVER_PORT 7777

// 服务器根目录WWW
#define HTTP_SERVER_ROOT "."

// 服务器默认目录主页
#define HTTP_SERVER_HOMEPAGE "index.html"
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// 是否 启用 高速响应
//#define USE_FAST_MODE

// 是否 启用 智能休眠
#define USE_SLEEP_MODE
////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
#include <direct.h>
#include <Winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#endif

#if defined(_MSC_VER)
#pragma comment(lib,"Ws2_32.lib")
#else
#include <stdbool.h>
#endif

// 队列等待最大长度(不超过5)
#define MAX_BACKLOG_SIZE 5
// 服务器最小休眠冲击量（MIN-Requests per second）
#define MIN_HTTP_RPS 1
// 最大字元长
#define MAX_BUFF_SIZE 1024*4
// 最小字元长
#define MIN_BUFF_SIZE 128
// 文件发送错误
#define FILE_SEND_ERROR -1
// 文件发送错误
#define HTTP_GENERAL_ERROR 1
// 文件路径阈值
#define MAX_PATH_SIZE 1024

#if defined(USE_FAST_MODE)
// 定义VS控制台启动方式（对VS编译器，改后缀为.cpp编译时，实现无窗化启动）
#if defined(_MSC_VER)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif
#endif

// 定义服务器默认路径分隔符 "\" 或 "/"
#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
#define HTTP_SERVER_PATHCHARACTER "\\"
#else
#define HTTP_SERVER_PATHCHARACTER "/"
// 定义宏名错误值
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
// 跨平台数据类型
typedef int SOCKET;
typedef unsigned long DWORD;
typedef void* LPVOID;
typedef const struct sockaddr* LPSOCKADDR;
#ifndef byte
typedef unsigned char byte;
#endif
#endif
#ifndef NULL
#define NULL (void*)0
#endif

// 跨平台宏函数
#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
#define GETCWD _getcwd
#define SLEEP(x) Sleep(x)
#define CLOSESOCKET closesocket
#else
#define GETCWD getcwd
#define SLEEP(x) usleep(x*1000)
#define CLOSESOCKET close
#endif

// 判断小写字母宏函数
#define ISLOWERLETTER(x) ('a'<=(x)&&(x)<='z')

// 声明 服务器当前目录
char currentPath[MAX_PATH_SIZE];

// HTTP协议关键词
static const char* HTTP_KEY_WORDS[] = { "GET", "POST", "HEAD", "EDIT", NULL };

// 定义404返回页面
static const char responsePage404[] =
"<html>"
"<head>"
"</head>"
"<title>404 Not Found</title>"
"<body>"
"  <h1 align='center'><font color=#FF1493>Sorry,404 Error!</font></h1>"
"    <script>"
"      document.write('<hr/>' + '<font color=#9400D3>' + new Date() + '</font>' + '<hr/>' + '<font color=#A9A9A9>' + 'File not found.' + '</font>');"
"  </script>"
"</body>"
"</html>";
static const int responsePage404Strlen = sizeof(responsePage404) / sizeof(char);

// 通用 关键词识别函数
int HTTP_IdentifyKey(char* inStr, char** inKeyWords, const char* endChars)
{
	int SN = 0;
	while (inKeyWords[SN] != NULL)
	{
		char* op = inStr, * kp = inKeyWords[SN];

		while (*kp != '\0')
		{
			if (
				((ISLOWERLETTER(*op)) ? (*op - 32) : (*op)) != ((ISLOWERLETTER(*kp)) ? (*kp - 32) : (*kp))
				)
			{
				break;
			}
			op++;
			kp++;
		}

		if (*kp == '\0')
		{
			if (*op == '\0')
			{
				return SN;
			}
			while (*endChars != '\0')
			{
				if (*op == *(endChars++))
				{
					return SN;
				}
			}
		}

		SN++;
	}
	return -1;
}

// 错误打印函数
int HTTP_PrintError(const char* perr)
{
#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
	fprintf(stderr, "%s: %d\n", perr, WSAGetLastError());
#else
	perror(perr);
#endif
	return 0;
}

// 发送200报头
int HTTP_Response200(SOCKET soc, int contentLength)
{
	char* responseBuf = (char*)malloc(MIN_BUFF_SIZE * sizeof(char));

	sprintf
	(
		responseBuf
		,
		"HTTP/1.1 200 OK\r\n"
		"Server: %s\r\n"
		"Connection: keep-alive\r\n"
		"Content-Type: text/html\r\n"
		"Content-Length: %d\r\n\r\n"
		,
		HTTP_SERVER_NAME, contentLength
	);

	send(soc, responseBuf, strlen(responseBuf), 0);

	free(responseBuf);
	return 0;
}

// 发送404报头
int HTTP_Response404(SOCKET soc)
{
	char* responseBuf = (char*)malloc(MIN_BUFF_SIZE * sizeof(char));

	sprintf
	(
		responseBuf
		,
		"HTTP/1.1 404 NOT FOUND\r\n"
		"Server: %s\r\n"
		"Connection: keep-alive\r\n"
		"Content-Type: text/html\r\n\r\n"
		,
		HTTP_SERVER_NAME
	);

	send(soc, responseBuf, strlen(responseBuf), 0);

	free(responseBuf);
	return 0;
}

// 发送请求的资源
int HTTP_SendFile(SOCKET soc, FILE* fp, bool removeUTF8BOM)
{
	// 重置文件流位置
	rewind(fp);

	// 分配数据拾取器
	byte* pickUpBuf = (byte*)malloc(MAX_BUFF_SIZE * sizeof(byte*));
	size_t freadSize = 0;

	if (removeUTF8BOM)
	{
		// 去除UTF8 BOM头 （必要的兼容性处理）
		freadSize = fread(pickUpBuf, sizeof(byte), 3, fp);
		if (freadSize == 3)
		{
			if (
				(*(pickUpBuf + 0) != 0xEF) ||
				(*(pickUpBuf + 1) != 0xBB) ||
				(*(pickUpBuf + 2) != 0xBF)
				)
			{
				rewind(fp);
			}
		}
	}

	// 发送文件二进制数据
	while (!feof(fp))
	{
		freadSize = fread(pickUpBuf, sizeof(byte), MAX_BUFF_SIZE, fp);
		if (send(soc, (const char*)pickUpBuf, freadSize, 0) == SOCKET_ERROR)
		{
#ifndef USE_FAST_MODE
			HTTP_PrintError("Send file Failed");
#endif
			return FILE_SEND_ERROR;
		}
	}

	free(pickUpBuf);
	return 0;
}

// 服务器核心函数
DWORD
#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
WINAPI
#endif
HTTP_ServerCore(LPVOID lpvsoc)
{
	DWORD ret = 0;
	SOCKET soc = (SOCKET)lpvsoc;

	// 分配客户请求接受容器
	char* receiveBuf = (char*)malloc(MAX_BUFF_SIZE * sizeof(char));

	int receiveCount = recv(soc, receiveBuf, MAX_BUFF_SIZE, 0);
	if (receiveCount == SOCKET_ERROR)
	{
#ifndef USE_FAST_MODE
		HTTP_PrintError("Receive failed");
#endif
		ret = HTTP_GENERAL_ERROR;
		CLOSESOCKET(soc);
		return ret;
		//goto JMP_END;
	}
	else
	{
		// 置结束符 '\0'
		receiveBuf[receiveCount] = '\0';

#ifndef USE_FAST_MODE
		// 接收成功，打印客户请求报文
		fprintf(stdout, "Receive data from client: \n%s\n", receiveBuf);
#endif
	}

	// 解析 HTTP协议
	char* httpMethod = strtok(receiveBuf, " \t");
	if (httpMethod == NULL)
	{
#ifndef USE_FAST_MODE
		fprintf(stdout, "Receive request data error\n");
#endif
		ret = HTTP_GENERAL_ERROR;
		CLOSESOCKET(soc);
		return ret;
		//goto JMP_END;
	}

	// 获取 客户端HTTP请求 协议值
	int httpMethodValue = HTTP_IdentifyKey(httpMethod, (char**)HTTP_KEY_WORDS, " \t");

	// 对于 不支持的协议，则断开连接
	if (httpMethodValue == -1)
	{
#ifndef USE_FAST_MODE
		fprintf(stdout, "Not support this method '%s'\n", httpMethod);
#endif
		ret = HTTP_GENERAL_ERROR;
		CLOSESOCKET(soc);
		return ret;
		//goto JMP_END;
	}

	// 解析 URL地址
	char* httpURL = strtok(NULL, " \t");
	if (httpURL == NULL)
	{
#ifndef USE_FAST_MODE
		fprintf(stdout, "Receive data error\n");
#endif
		ret = HTTP_GENERAL_ERROR;
		CLOSESOCKET(soc);
		return ret;
		//goto JMP_END;
	}

	// 解析 请求文件路径
	char* httpFilePath = strchr(httpURL, '/');
	if (httpFilePath == NULL)
	{
		httpFilePath = httpURL;
	}
#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
	else
	{
		char* p = httpFilePath;
		while (*p != '\0')
		{
			if (*p == '/')
			{
				*p = (HTTP_SERVER_PATHCHARACTER)[0];
			}
			p++;
		}
	}
#endif

#ifndef USE_FAST_MODE
	// 输出 客户端请求摘要
	fprintf
	(
		stdout
		,
		"HTTP method: %s\n"
		"File svpath: %s\n"
		,
		httpMethod
		,
		httpFilePath
	);
#endif

	// 拼接 请求文件路径
	char* requestFilePath = (char*)malloc(MAX_PATH_SIZE * sizeof(char));
	sprintf
	(
		requestFilePath
		,
		"%s%s%s%s%s"
		,
		currentPath, HTTP_SERVER_PATHCHARACTER, HTTP_SERVER_ROOT, HTTP_SERVER_PATHCHARACTER, httpFilePath
	);

	// 如果请求的是目录，则跳转到该目录主页“index.html”
	int httpFilePathLen = strlen(httpFilePath);
	if (*(httpFilePath + httpFilePathLen - 1) == (HTTP_SERVER_PATHCHARACTER)[0])
	{
		strcat(requestFilePath, HTTP_SERVER_HOMEPAGE);
	}

	// 以二进制方式 打开服务器文件流
	FILE* fp = fopen(requestFilePath, "rb");

	// 如果 读取文件失败
	if (fp == NULL)
	{
		// 如果是HEAD方法，则发送404报头
		if (httpMethodValue == 2)
		{
			HTTP_Response404(soc);
		}
		// 如果是 GET方法，则发送404页面
		else if (httpMethodValue == 0)
		{
			send(soc, responsePage404, responsePage404Strlen, 0);
		}
#ifndef USE_FAST_MODE
		// 打印 网站404错误，并执行跳转
		fprintf(stdout, "File not found.\n");
#endif
		ret = HTTP_GENERAL_ERROR;
		CLOSESOCKET(soc);
		return ret;
		//goto JMP_END;
	}

	// 测量 要请求的文件长度
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	int fpSize = ftell(fp);

	// 如果是HEAD方法，则发送200报头
	if (httpMethodValue == 2)
	{
		HTTP_Response200(soc, fpSize);
		goto JMP_END;
	}

	// 如果是GET方法则发送请求的资源
	if (httpMethodValue == 0)
	{
#ifndef USE_FAST_MODE
		// 显示 要请求的文件长度
		fprintf(stdout, "File length: %d\n", fpSize);
#endif

		if (HTTP_SendFile(soc, fp, true) != FILE_SEND_ERROR)
		{
#ifndef USE_FAST_MODE
			fprintf(stdout, "File send OK\n");
		}
		else
		{
			fprintf(stdout, "File send failed\n");
#endif
		}
	}
	fclose(fp);

JMP_END:
	// 释放 套接口
	CLOSESOCKET(soc);

#ifndef USE_FAST_MODE
	// 显示 标尾信息
	fprintf
	(
		stdout
		,
		"Close socket(%d)\n"
		"[<==]\n"
		,
		soc
	);
#endif
	free(receiveBuf);
	free(requestFilePath);
	return ret;
}

// MAIN主函数
int main()
{
	// 获取服务器 当地时间
	time_t timeNow = time(&timeNow);
	char* nowTime = ctime(&timeNow);

	// 显示 标头信息
	fprintf
	(
		stdout,
		"%s"
		"[===* Welcome to use %s *===]\n"
		"[PORT: %d]\n"
		"[>>>]\n"
		,
		nowTime
		,
		HTTP_SERVER_NAME
		,
		HTTP_SERVER_PORT
	);

	//获取当前文件所在目录
	GETCWD(currentPath, MAX_PATH_SIZE);

#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
	// 启动 安全套接字
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		fprintf(stdout, "Failed to create wsocket\n");
		return HTTP_GENERAL_ERROR;
	}
#endif

	// 创建 连接套接口、监听套接口
	SOCKET soc, socListen;
	socListen = socket(AF_INET, SOCK_STREAM, 0);
	if (socListen == INVALID_SOCKET)
	{
		HTTP_PrintError("Creat listen socket failed");
		return HTTP_GENERAL_ERROR;
	}

	// 声明 服务器地址，客户端地址
	struct sockaddr_in serverSockaddrIn, clientSockaddrIn;

	// 配置 服务器信息
	serverSockaddrIn.sin_family = AF_INET;
	serverSockaddrIn.sin_port = htons(HTTP_SERVER_PORT);
	serverSockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);

	// 绑定 监听套接口 与 服务器地址
	if (bind(socListen, (LPSOCKADDR)&serverSockaddrIn, sizeof(serverSockaddrIn)) == SOCKET_ERROR)
	{
		HTTP_PrintError("Blind server failed");
		return HTTP_GENERAL_ERROR;
	}
	printf("-----------");
	// 通过 监听套接口 监听
	if (listen(socListen, MAX_BACKLOG_SIZE) == SOCKET_ERROR)
	{
		HTTP_PrintError("Listen socket failed");
		return HTTP_GENERAL_ERROR;
	}

	// 声明 计时器变量
	clock_t preClock = clock();
	int countTimes = 0;
	int requestsPerSecond = 1000;

	// 监听 网页资源请求
	while (true)
	{
		int clientSockaddrInLen = sizeof(clientSockaddrIn);

		// 返回 客户连接套接口
		soc = accept(socListen, (struct sockaddr*)&clientSockaddrIn, &clientSockaddrInLen);

		if (soc == INVALID_SOCKET)
		{
			HTTP_PrintError("Accept failed");
			break;
		}

#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
		// 创建 请求线程
		DWORD ThreadID;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HTTP_ServerCore, (LPVOID)soc, 0, &ThreadID);
#else
		pthread_t newPthreadT;
		pthread_create(&newPthreadT, NULL, (void*)HTTP_ServerCore, (void*)(intptr_t)soc);
#endif

#if defined(USE_SLEEP_MODE)
		// 计次器
		countTimes++;

		// 每隔300毫秒检测一次服务器冲击量
		if (clock() - preClock > CLOCKS_PER_SEC * 0.3)
		{
			requestsPerSecond = CLOCKS_PER_SEC * countTimes / (clock() - preClock);
			countTimes = 0;
			preClock = clock();
		}

		// 如果冲击量过小，则休眠CPU占用率
		if (requestsPerSecond < MIN_HTTP_RPS)
		{
			SLEEP(3);
		}
#endif
	}

	// 关闭 安全套接口
	CLOSESOCKET(socListen);

#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__) || defined(_MSC_VER)
	// 关闭 安全套接字
	WSACleanup();
#endif

	return 0;
}