#include "wc_fc.h"
void server_(int argc, char* argv[]) {
    if (argc == 4) {
        Control_Server_port = atoi(argv[2]);
        strcpy(Password, argv[3]);
    }
    SOCKET server;
    std::vector<WSAPOLLFD> fds;
    std::thread t(send_threads); // 创建发送消息的线程
    std::thread s(Server_Loop_Send_W); // 创建心跳包发送消息的线程
    if (Control_Server_port != 0) {
        std::thread c(Control_Server); // 创建发送消息的线程
        c.detach();
    }
    t.detach();
    s.detach();

    while (1) {
        server = init_server(argc, argv); //初始化
        int muxWSAPoll = 0;
        while (1)
        {
            fds.clear();
            fds.push_back({ server, POLLIN, 0 });
            for (_Client& client : clients)
            {
                fds.push_back({ client.s, POLLIN, 0 });
            }
            int ret = WSAPoll(fds.data(), fds.size(), 30000); //开启监听
            //printf("WSAPoll RET=%d , fds.size = %d\n", ret , fds.size());
            if (ret > 0) //事件发生
            {
                if (fds[0].revents & POLLIN) // 新的连接
                {
                    handle_new_connection(server);
                }
                check_client_connection(fds); //检查所有套接字，接收信息或者断开
            }
            else { 
                check_client_connection(fds);
            }
            char buffer[256];
            sprintf(buffer, "COUNT = %d , RET=%d , FDS.SIZE = %d , MUX.SIZE = %d", muxWSAPoll++, ret, fds.size(), clients.size());
            SetConsoleTitle(buffer);
            if (muxWSAPoll == INT_MAX) muxWSAPoll = 0;
        }
        closesocket(server);
    }
}
void client_(int argc, char* argv[]) {

    if (strlen(argv[3]) > 256) {
        printf("Parameter 3 Exceeds the length");
        return;
    }
    FILE* fp = NULL;
    int tmp = 0;
    char buffer[BUFFER_SIZE];
    char com[BUFFER_SIZE];
    struct timeval timeout;
    timeout.tv_sec = 120000; // 设置超时时间为2分钟
    timeout.tv_usec = 0;
    WSADATA wsaData;
    int ws = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ws != 0) return;
    SOCKET client;
    sockaddr_in addr;
    while (1) {
        client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //获取套接字
        if (client == INVALID_SOCKET) {
            printf("invalid socket!");
            return;
        }
        if (setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) { //设置堵塞超时机制
            perror("Error setting socket receive timeout");
        }
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(argv[1]);
        addr.sin_port = htons(atoi(argv[2]));
        tmp = 0;
        while (connect(client, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            sprintf(buffer, "CONNECT %d ", tmp);
            SetConsoleTitle(buffer);
            Sleep(1000);
            if (++tmp > 5) break;
        }
        if (tmp > 5) { closesocket(client); continue; }
        sendmsg(client, argv[3]); //发送客户端名字
        tmp = 0;
        while (1) {
            buffer[0] = '\0'; int ret = recvmsg(client, buffer); //接收响应
            if (ret == SOCKET_ERROR) {if (WSAGetLastError() == WSAETIMEDOUT) { tmp = 6; break; }}
            if (ret > 0) { if (buffer[0] != '#') { tmp = 6; } break; }
            if (ret == 0) { tmp = 6; break; }
            sprintf(buffer, "RECV SERVER %d  ", tmp);
            SetConsoleTitle(buffer); Sleep(1000);
            if (++tmp > 5) break;
        }
        if (tmp > 5) { closesocket(client); continue; }
        printf("\r$ ");
        while (1)
        {
            buffer[0] = '\0'; com[0] = '\0'; tmp = 0;
            while (1) {
                int ret = recvmsg(client, buffer); //接收命令
                if (ret == SOCKET_ERROR) {
                    if (WSAGetLastError() == WSAETIMEDOUT) { tmp = 6; break; }
                }
                if (ret > 0) break;
                if (ret == 0) { tmp = 6; break; }
                sprintf(buffer, "RECV COMMAND %d  ", tmp);
                SetConsoleTitle(buffer); Sleep(1000);
                if (++tmp > 5) break;
            }
            if (tmp > 5) { closesocket(client); break; }
            if (strlen(buffer) == 0) continue;
            for (int i = 0; i < strlen(buffer); i++) { if (buffer[i] != ' ') { com[0] = 'B'; break; } }
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
            fp = _popen(com, "r"); //执行命令
            if (!fp) {
                continue;
            }
            tmp = 0;
            while (fgets(buffer, sizeof(buffer) - 1, fp) != 0) { //循环命令返回值
                tmp++; 
                printf("%s",buffer);
                send(client, buffer, strlen(buffer), 0);
            }
            if (tmp != 0) {
                buffer[strlen(buffer) - 3] = '\0';
                SetCurrentDirectoryA(buffer+1);
            }
            _pclose(fp);
        }
    }
}