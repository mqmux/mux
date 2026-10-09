#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <io.h>
#include <ctype.h>
#include <time.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include "mux.h"

#pragma comment(lib, "Ws2_32.lib")
#define BUF_SIZE 1024
long long read_image_file_index = 0;
char log_str[1024];

void write_log(char* filename) {
    FILE* log_file = fopen(filename, "a");
    if (log_file == NULL) {
        log_file = fopen(filename, "w");
        if (log_file == NULL) {
            perror("Error creating log file");
            return;
        }
    }
    fprintf(log_file, "%s", log_str);
    fclose(log_file);
}

char* utf8_to_gbk(const char* utf8_str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
    wchar_t* wstr = (wchar_t*)malloc(sizeof(wchar_t) * len);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wstr, len);

    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* gbk_str = (char*)malloc(len);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, gbk_str, len, NULL, NULL);
    free(wstr);
    return gbk_str;
}
char* url_decode(const char* str) {
    char* decoded = (char*)malloc(strlen(str) + 256);
    if (!decoded) return NULL;

    int j = 0;
    for (int i = 0; str[i]; ++i) {
        if (str[i] == '%' && isxdigit(str[i + 1]) && isxdigit(str[i + 2])) {
            // 解码十六进制字符
            char hex[3] = { str[i + 1], str[i + 2], '\0' };
            decoded[j++] = strtol(hex, NULL, 16);
            i += 2;
        }
        else if (str[i] == '+') {
            // 将 + 转换为空格
            decoded[j++] = ' ';
        }
        else {
            // 复制非编码字符
            decoded[j++] = str[i];
        }
    }
    decoded[j] = '\0'; // 添加字符串结束符
    return decoded;
}

const char* get_content_type(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return "application/octet-stream";  // 默认为二进制流
    // 将扩展名转换为小写
    char ext_lowercase[16]; // 扩展名最长为15个字符
    int i = 0;
    while (ext[i] && i < 15) {
        ext_lowercase[i] = tolower(ext[i]);
        i++;
    }
    ext_lowercase[i] = '\0';
    if (strcmp(ext_lowercase, ".html") == 0 || strcmp(ext_lowercase, ".htm") == 0) {
        return "text/html";
    }
    else if (strcmp(ext_lowercase, ".jpg") == 0 || strcmp(ext_lowercase, ".jpeg") == 0) {
        return "image/jpeg";
    }
    else if (strcmp(ext_lowercase, ".gif") == 0) {
        return "image/gif";
    }
    else if (strcmp(ext_lowercase, ".png") == 0) {
        return "image/png";
    }
    else if (strcmp(ext_lowercase, ".pdf") == 0) {
        return "application/pdf";
    }
    else if (strcmp(ext_lowercase, ".mp4") == 0) {
        return "video/mp4";
    }
    else if (strcmp(ext_lowercase, ".bmp") == 0) {
        return "image/bmp";
    }
    else if (strcmp(ext_lowercase, ".ico") == 0) {
        return "image/x-icon";
    }
    return "application/octet-stream";  // 默认为二进制流
}

void return_error(SOCKET client_socket) {
    //const char* error_message = "<html><head><title>404 Not Found</title></head><body><h1><center>404 Not Found</center></h1></body></html>";
    const char* error_message = R"(
        <html>
        <head>
            <title>404 Not Found</title>
            <style>
                body {
                    background-color: black;
                    color: white;
                }
                h1 {
                    text-align: center;
                }
            </style>
        </head>
        <body>
            <h1>404 Not Found</h1>
        </body>
        </html>
    )";

    char response_header[BUF_SIZE];
    sprintf(response_header, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n%s", strlen(error_message), error_message);
    send(client_socket, response_header, strlen(response_header), 0);
    closesocket(client_socket);
}

bool isValidFilename(const char* filename) {
    if (filename[1] == ':' || strncmp(filename, "\\\\", 2) == 0 || strncmp(filename, "//", 2) == 0) {
        printf("Absolute or network path: %s\n", filename);
        return false;
    }
    if (strstr(filename, "..\\") != NULL || strstr(filename, "../") != NULL) {
        printf("Have parent directory path: %s\n", filename);
        return false;
    }
    const char* invalid_chars = "|><:*?\"";
    if (strpbrk(filename, invalid_chars) != NULL) {
        printf("Have invalid characters: %s\n", filename);
        return false;
    }
    return true;
}

void read_image_file(char* filename, SOCKET client_socket) {
    FILE* image_file = fopen(filename, "rb");
    if (image_file == NULL) {
        strcpy(filename, "404.html");
        image_file = fopen(filename, "rb");
        if (image_file == NULL) { return_error(client_socket); return; }
    }
    // 获取文件大小
    fseek(image_file, 0, SEEK_END);
    size_t file_size = ftell(image_file);
    fseek(image_file, 0, SEEK_SET);

    // 生成响应头
    char response_header[1024];
    const char* content_type = get_content_type(filename);
    sprintf(response_header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n", content_type, file_size);
    printf("%s", response_header);
    send(client_socket, response_header, strlen(response_header), 0);

    char buffer[BUF_SIZE];
    clock_t last_print_time = clock();
    printf("[%lld]", ++read_image_file_index);
    int bytes_read; printf("[%s][>>>>>>>>>>.", filename);
    while ((bytes_read = fread(buffer, 1, BUF_SIZE, image_file)) > 0) {
        clock_t current_time = clock();
        double time_diff = (current_time - last_print_time);
        if (time_diff > 1) {
            printf(">");
            last_print_time = current_time;
        }
        if (send(client_socket, buffer, bytes_read, 0) == SOCKET_ERROR) break;
    }
    printf("]\n");
    fclose(image_file);
    closesocket(client_socket);
}
int ListFiles(const char* path, const char* targetFilename) {
    struct _finddata_t file;
    long hFile;
    char currentPath[MAX_PATH];
    char filePath[MAX_PATH];
    char fileandPath[MAX_PATH];
    strcpy(currentPath, path);
    strcat(currentPath, "/*.*");
    if ((hFile = _findfirst(currentPath, &file)) == -1L) return 0;
    do {
        if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0) continue;
        if (file.attrib & _A_SUBDIR) {
            strcpy(filePath, path);
            strcat(filePath, "/");
            strcat(filePath, file.name);
            if (ListFiles(filePath, targetFilename) == 1) {
                _findclose(hFile); return 1;
            }
        }
        else {
            sprintf(fileandPath, "%s/%s", path + 2, file.name);
            if (fileandPath[0] == '/')  strcpy(fileandPath, file.name);
            printf("[%s][%s]\n", targetFilename, fileandPath);
            if (strcmp(fileandPath, targetFilename) == 0) {
                _findclose(hFile); return 1;
            }
        }
    } while (_findnext(hFile, &file) == 0);
    _findclose(hFile);
    return 0;
}

int main(int argc, char* argv[]) {
    char filename[1024];
    char passwd[1024];
    int HTTP_PORT = 7777;
    strcpy(filename, "http.log");
    passwd[0] = 0;
    int wait_time = 1000;
    switch (argc)
    {
    case 6:
        wait_time = atoi(argv[5]);
    case 5: // http [ip] [port] [filename] [sendfile passwd]
    {
        SOCKET client = init_client(argv[1], atoi(argv[2]));
        send(client, argv[4], strlen(argv[4]) + 1, 0);
        RECV(client, passwd);
        if (passwd[0] == 'O' && passwd[1] == 'K') _SendFile(client, argv[3], wait_time); // 发送 filename
        return 0;
    }
    case 4:
        strcpy(filename, argv[3]);
    case 3:
        if (strchr(argv[1], '.') != NULL) return 1;
        strcpy(passwd, argv[2]);
    case 2:
        HTTP_PORT = atoi(argv[1]);
        break;
    default:
        printf("http [port] [recvfile passwd] [log filename]服务器\n");
        printf("http [ip] [port] [filename] [sendfile passwd] [等待时间] 上传文件\n");
        return 0;
    }
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) { WSACleanup(); return 1; }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(HTTP_PORT);
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) { closesocket(server_socket); WSACleanup(); return 1; }
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) { closesocket(server_socket); WSACleanup(); return 1; }
    char buffer[BUF_SIZE];
    while (1) {
        SOCKET client_socket = accept(server_socket, NULL, NULL);

        int timeout = 5000; // 以毫秒为单位（例如5秒）
        setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        if (client_socket == INVALID_SOCKET) { closesocket(server_socket); WSACleanup(); return 404; }

        int bytes_received = recv(client_socket, buffer, BUF_SIZE, 0);

        if (bytes_received == SOCKET_ERROR) {
            int err_code = WSAGetLastError();
            if (err_code == WSAETIMEDOUT) {
                printf("发生了套接字超时。未接收到数据。\n");
                continue; // 重新开始循环以接受新连接
            }
            else {
                printf("接收数据时发生错误: %d\n", err_code);
                closesocket(client_socket);
                continue; // 转到下一个循环处理其他客户端
            }
        }

        if (passwd[0] != 0 && !strcmp(buffer, passwd)) {
            SEND(client_socket, "OK");//发送key
            _RecvFile(client_socket);
            continue;
        }

        time_t now = time(NULL);
        char* timeStr = ctime(&now);
        timeStr[strlen(timeStr) - 1] = '\0'; // 去掉换行符
        printf("[ %s ][<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<]\n", timeStr);
        printf("%.*s\n", bytes_received, buffer);
        if (bytes_received <= BUF_SIZE) buffer[bytes_received] = '\0';

        char method[BUF_SIZE], path[BUF_SIZE];
        if (sscanf(buffer, "%s /%s", method, path) != 2) {
            strcpy(path, "404.html");
        }
        char* decoded = url_decode(path);
        char* gbk_str = utf8_to_gbk(decoded);
        strcpy(path, gbk_str); free(decoded); free(gbk_str);
        printf("method=%s;path=%s;\n\n", method, path);
        sprintf(log_str, "[%lld][%s][%s][%s][", read_image_file_index, timeStr, method, path);
        if (!isValidFilename(path)) {
            strcpy(path, "404.html");
            //if (!ListFiles(".", path)) { return_error(client_socket); continue; }
        }
        else if (strcmp(method, "GET") == 0) {
            if (path[0] == 'H' && path[1] == 'T') {
                strcpy(path, "index.html");
            }
            read_image_file(path, client_socket);
        }
        else {
            return_error(client_socket);
        }
        //timeStr method path read_image_file_index
        strcat(log_str, path);
        strcat(log_str, "]\n");
        write_log(filename);
    }
    closesocket(server_socket);
    WSACleanup(); return 0;
}