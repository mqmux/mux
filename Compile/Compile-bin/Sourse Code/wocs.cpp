#include "woc.h"

int main(int argc, char* argv[])
{
    if (argc == 1) { printCWD(); return 0; }
    if ((argc == 2 && argv[1][0] >= '0' && argv[1][0] <= '9') || (argc == 4 && (!strchr(argv[1], '.')))) // 作为服务器运行
    {
        int port = atoi(argv[1]);
        if (argc == 4) {
            Control_Server_port = atoi(argv[2]);
            strcpy(Password, argv[3]);
        }
        SOCKET server = init_server(port);
        std::thread t(send_threads); // 创建发送消息的线程
        std::thread s(Server_Loop_Send_W); // 创建发送消息的线程
        if (Control_Server_port != 0) {
            std::thread c(Control_Server); // 创建发送消息的线程
            c.detach();
        }
        t.detach();
        s.detach();
        while (true)
        {
            std::vector<WSAPOLLFD> fds;
            fds.push_back({ server, POLLIN, 0 });
            for (_Client& client : clients)
            {
                fds.push_back({ client.s, POLLIN, 0 });
            }
            int ret = WSAPoll(fds.data(), fds.size(), 60000);
            //printf("RET=%d\n", ret);
            if (ret > 0)
            {
                if (fds[0].revents & POLLIN) // 有新的连接
                {
                    handle_new_connection(server);
                }
                for (size_t i = 1; i < fds.size(); ++i) // 检查每个客户端
                {
                    if (fds[i].revents & POLLHUP) // 客户端断开连接
                    {
                        handle_client_disconnection(fds[i]);
                    }
                    else if (fds[i].revents & POLLIN) // 客户端有数据可读
                    {
                        handle_client_data(fds[i]);
                    }
                }
            }
            else if (ret < 0) {
                SetConsoleTitle("Poll error");
                continue;;
            }
            else {
                //printf("poll timeout");
                for (int i = 0; i < clients.size(); ++i) {
                    if (clients[i].s == INVALID_SOCKET) {
                        closesocket(clients[i].s);
                        clients.erase(clients.begin() + i);
                    }
                }
            }
        }
        closesocket(server);
        WSACleanup();
    }
    else if (argc == 4) // 作为客户端运行
    {
        if (strlen(argv[3]) > 256) ERROR_EXIT("Parameter 3 Exceeds the length");
        FILE* fp = NULL;
        int loop_index = 0;
        int connect1 = 0;
        int error = 0;
        char buffer[BUFFER_SIZE];
        char com[BUFFER_SIZE];
        char mony[256] = "$ ";
        const char* ip = argv[1];
        int port = atoi(argv[2]);
        //u_long mode = 1;

        struct timeval timeout;
        timeout.tv_sec = 120000; // 设置超时时间为10秒
        timeout.tv_usec = 0;

        WSADATA wsaData;
        int ws = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (ws != 0) return 0;
        SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr;
        while (1) {
            client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (client == INVALID_SOCKET) {
                printf("invalid socket!");
                return 0;
            }
            if (setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) { //设置堵塞超时机制
                perror("Error setting socket receive timeout");
            }
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(argv[1]);
            addr.sin_port = htons(atoi(argv[2]));
            connect1 = 0;
            while (connect(client, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                sprintf(buffer,"CONNECT %d ", connect1);
                SetConsoleTitle(buffer);
                Sleep(1000);
                if(++connect1 > 5) break;
            }
            if (connect1 > 5) {
                closesocket(client);
                continue;
            }

            //if (ioctlsocket(client, FIONBIO, &mode) != NO_ERROR) //设置非堵塞
            //    printf("ioctlsocket failed with\n");

            // TODO: 处理客户端的逻辑
            if (send(client, argv[3], strlen(argv[3]), 0) <= 0) { //发送客户端名字
                ERROR_EXIT("send error");
            }
            //memset(buffer, '\0', sizeof(buffer));
            connect1 = 0;
            while (1) {
                int ret = recv(client, buffer, sizeof(buffer), 0);
                //printf("ret=%d\n", ret);
                if (ret == SOCKET_ERROR) {
                    error = WSAGetLastError();
                    if (error == WSAETIMEDOUT) {
                        //printf("Connection timed out\n");
                        connect1 = 6;
                        break;
                    }
                }
                if (ret > 0) break;
                if (ret == 0) {
                    connect1 = 6;
                    break;
                }
                sprintf(buffer, "RECV SERVER %d  ", connect1);
                SetConsoleTitle(buffer);
                Sleep(1000);
                if (++connect1 > 5) break;
            }
            if (connect1 > 5) {
                closesocket(client);
                continue;
            }
            //sprintf(com, "R%s %s %s %s", buffer, argv[1], argv[2], argv[3]);
            //SetConsoleTitle(com);
            printf("\r                                                         \r$ ");
            
            while (1)
            {
                memset(buffer, '\0', sizeof(buffer));
                memset(com, '\0', sizeof(com));
                connect1 = 0;
                while (1) {
                    int ret = recv(client, buffer, sizeof(buffer), 0);
                    //printf("ret=%d\n", ret);
                    if (ret == SOCKET_ERROR) {
                        error = WSAGetLastError();
                        if (error == WSAETIMEDOUT) {
                            //printf("Connection timed out\n");
                            connect1 = 6;
                            break;
                        }
                    }
                    if (ret > 0) break;
                    if (ret == 0) {
                        connect1 = 6;
                        break;
                    }
                    sprintf(buffer, "RECV COMMAND %d  ", connect1);
                    SetConsoleTitle(buffer);
                    Sleep(1000);
                    if (++connect1 > 5) break;
                }
                if (connect1 > 5) {
                    closesocket(client);
                    break;
                }
                //接收到信息
                if (strlen(buffer) == 0) continue;
                for (int i = 0; i < strlen(buffer); i++) {
                    if (buffer[i] != ' ') {
                        com[0] = 'B';
                        break;
                    }
                }
                if (com[0] != 'B') continue;
                printf("%s\n", buffer);
                if (!strcmp(buffer, "cls")) {
                    system("cls");
                    strcpy(com, _pgmptr);
                }
                else {
                    strcpy(com, buffer);
                    strcat(com, " 2>&1&echo;&");
                    strcat(com, _pgmptr);
                }
                memset(buffer, '\0', sizeof(buffer));
                fp = _popen(com, "r");
                if (!fp) {
                    exit(EXIT_FAILURE);
                }
                if (!fp) {
                    return 1;
                }
                loop_index = 0;
                while (fgets(buffer, sizeof(buffer) - 1, fp) != 0) {
                    loop_index++;
                    printf("%s",buffer);
                    if (send(client, buffer, strlen(buffer), 0) <= 0) ERROR_EXIT("send error");
                }
                buffer[strlen(buffer) - 3] = '\0';
                if (loop_index != 0) {
                    memcpy(mony, buffer + 1, strlen(buffer) - 1);
                    SetCurrentDirectoryA(mony);
                }
                memset(mony, '\0', sizeof(mony));
                _pclose(fp);
            }
        }
    }
    else {
        HELP(argv);
    }
    return 0;
}