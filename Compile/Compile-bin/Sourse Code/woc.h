#ifndef WOC_H
#define WOC_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <direct.h>
#pragma comment(lib, "ws2_32.lib")
#define MAX_BUFFER_SIZE 1024
#define BUFFER_SIZE 1024
#define NICKNAME_LEN 1024
#define ERROR_EXIT(msg) {perror(msg); exit(EXIT_FAILURE);}
#define History_SIZE 4096
#define Passwd_History_SIZE 2048
char History[History_SIZE];
char Passwd_History[Passwd_History_SIZE];
char Cli_num[25];
int Control_Server_port = 0;
SOCKET sclient;//控制端描述符
SOCKET slisten;
int selected = 0;//选择
int History_selected = 0; //历史连接数量
char Password[MAX_BUFFER_SIZE];
int OutTime = 200;
//帮助信息
void HELP(char** argv) {
    printf("Usage: %s [port] 服务端模式\n", argv[0]);
    printf("   or: %s [port] [port] [key] 控制端模式\n", argv[0]);
    printf("   or: %s [ip] [port] [name] 客户端模式\n", argv[0]);
    printf("\n");
    printf("   #[num]: 选择客户端 /: 打印在线客户端\n");
    printf("   #: 打印历史连接 ##: 清空历史连接记录 //:ls key\n");
}
struct _Client
{
    SOCKET s = INVALID_SOCKET;
    int num = 0;
    char name[1024];
    char time[32];//[2024/12/25 12:10:34:345] 23+1
};
_Client _client_;
std::vector<_Client> clients; // 客户端套接字列表

//客户端心跳包
void Server_Loop_Send_W()
{
    while (1) {
        Sleep(60000);
        for (size_t i = 0; i < clients.size(); ++i) {
            send(clients[i].s, " ", strlen(" ") + 1, 0);
        }
        if (Control_Server_port != 0) send(slisten, " ", strlen(" ") + 1, 0);
    }
}
struct tm* sysTime_t = NULL;
void Now_TIME(time_t now) {
    sysTime_t = localtime(&now);
}
int SEND(SOCKET sock, char* data) {
    int len = strlen(data) + 1;
    send(sock, (char*)&len, sizeof(int), 0);
    send(sock, data, len, 0);
    return 0;
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
    slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数分别为协议族，类型，协议号 AF_INET代表TCP/IP 
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
    char correct[] = "correct";
    char correct_error[] = "Password error";
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
        RECV(sclient, com);
        Now_TIME(time(NULL));
        sprintf(title, "[%d-%02d-%02d %02d:%02d:%02d.%03d] \0",
            1900 + sysTime_t->tm_year, 1 + sysTime_t->tm_mon, sysTime_t->tm_mday,
            sysTime_t->tm_hour, sysTime_t->tm_min, sysTime_t->tm_sec, (int)(time(NULL) % 1000)
        );
        while ((strlen(Passwd_History) + strlen(com) + strlen(title) + 2) > Passwd_History_SIZE) {
            char* pos = strchr(Passwd_History, '\n');
            if (pos != nullptr) {
                int index = pos - Passwd_History + 1; // +1 to also remove the '\n'
                strcpy(Passwd_History, Passwd_History + index);
            }
        }
        strcat(Passwd_History, title);
        strcat(Passwd_History, com);
        strcat(Passwd_History, "\n");
        if (strcmp(com, Password)) {
            SEND(sclient, correct_error);
            Sleep(1000);
            continue;
        }
        SEND(sclient, correct);
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
                if (buffer[1] == '/') {
                    printf("%s", Passwd_History);
                    send(sclient, Passwd_History, strlen(Passwd_History), 0);
                    printf("[%d]$ ", selected);
                    sprintf(com, "[%d]$ \0", selected);
                    send(sclient, com, strlen(com)+1, 0);
                    continue;
                }
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

                int count = 0;
                int number = clients.size() - 1;
                while (number != 0) {
                    number = number / 10;
                    ++count;
                }
                for (size_t i = 0; i < clients.size(); ++i) {
                    printf("#%*d %*s R%*d [%s]\n", -count, i, -max_length_Cli_name, clients[i].name, -length_num_max, clients[i].num, clients[i].time);
                    sprintf(com, "#%*d %*s R%*d [%s]\n\0", -count, i, -max_length_Cli_name, clients[i].name, -length_num_max, clients[i].num, clients[i].time);
                    send(sclient, com, strlen(com), 0);
                }
                printf("[%d]$ ", selected);
                memset(com, '\0', sizeof(com));
                sprintf(com, "[%d]$ ", selected);
                send(sclient, com, strlen(com)+1, 0);
                continue;
            }
            if (buffer[0] == '#')
            {
                /*if (buffer[1] == '#') {
                    History[0] = '\0';
                    printf("[%d]$ ", selected);
                    memset(com, '\0', sizeof(com));
                    sprintf(com, "[%d]$ ", selected);
                    send(sclient, com, strlen(com), 0);
                    continue;
                }*/
                if (buffer[1] == '\0') {  //NULL , '\0' ,0 的ASCII码值都是 0 , '0'是48
                    printf("%s", History);
                    send(sclient, History, strlen(History), 0);
                    printf("[%d]$ ", selected);
                    sprintf(com, "[%d]$ ", selected);
                    send(sclient, com, strlen(com)+1, 0);
                    continue;
                }
                selected = atoi(buffer + 1);
                printf("[%d]$ ", selected);
                memset(com, '\0', sizeof(com));
                sprintf(com, "[%d]$ ", selected);
                send(sclient, com, strlen(com)+1, 0);
            }
            else {
                if (clients.size() <= selected || (send(clients[selected].s, buffer, sizeof(buffer), 0) <= 0))
                {
                    printf("[%d]$ ", selected);
                    memset(com, '\0', sizeof(com));
                    sprintf(com, "[%d]$ ", selected);
                    send(sclient, com, strlen(com)+1, 0);
                }
            }
        }
    }
    //控制台返回数据
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
            if (buffer[1] == '/') {
                printf("%s", Passwd_History);
                printf("[%d]$ ", selected);
                continue;
            }
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

            int count = 0;
            int number = clients.size() - 1;
            while (number != 0) {
                number = number / 10;
                ++count;
            }
            for (size_t i = 0; i < clients.size(); ++i) {
                printf("#%*d %*s R%*d [%s]\n", -count, i, -max_length_Cli_name, clients[i].name, -length_num_max, clients[i].num, clients[i].time);
            }
            printf("[%d]$ ", selected);
            continue;
        }
        if (buffer[0] == '#')
        {
            /* if (buffer[1] == '#') {
                 History[0] = '\0';
                 printf("[%d]$ ", selected);
                 continue;
             }*/
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
                if (Control_Server_port != 0) send(sclient, buffer, strlen(buffer)+1, 0);
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
        //printf("[%c][%c][%c]\n", _client_.name[0], _client_.name[1], _client_.name[2]);
        if (_client_.name[0] == 'G' && _client_.name[1] == 'E' && _client_.name[2] == 'T') {
            closesocket(server);
            Sleep(1000);
            return;
        }
        memset(Cli_num, 0, sizeof(Cli_num));
        _itoa(++History_selected, Cli_num, 10);
        send(client, Cli_num, strlen(Cli_num)+1, 0);

        Now_TIME(time(NULL));
        sprintf(title, "%d-%02d-%02d %02d:%02d:%02d.%03d\0",
            1900 + sysTime_t->tm_year, 1 + sysTime_t->tm_mon, sysTime_t->tm_mday,
            sysTime_t->tm_hour, sysTime_t->tm_min, sysTime_t->tm_sec, (int)(time(NULL) % 1000)
        );
        strcpy(_client_.time, title);
        _client_.s = client;
        _client_.num = History_selected;
        clients.push_back(_client_); // 添加到客户端列表
        sprintf(
            title,
            "[%s] R%d #%d %s 已连接",
            _client_.time,
            _client_.num,
            clients.size() - 1,
            _client_.name
        );
        SetConsoleTitle(title);
        while ((strlen(History) + strlen(title) + 2) > History_SIZE) {
            char* pos = strchr(History, '\n');
            if (pos != nullptr) {
                int index = pos - History + 1; // +1 to also remove the '\n'
                strcpy(History, History + index);
            }
        }
        strcat(History, title);
        strcat(History, "\n\0");
        /*if ((strlen(History) + strlen(title) + 2) > History_SIZE) {
            strcpy(History, title);
            strcat(History, "\n\0");
        }
        else {
            strcat(History, title);
            strcat(History, "\n\0");
        }*/
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
        if (clients[selected].s == fd.fd) printf("%s", buffer);
        if (Control_Server_port != 0 && clients[selected].s == fd.fd) send(sclient, buffer, strlen(buffer)+1, 0);
    }
    else // 客户端已断开连接
    {
        closesocket(fd.fd);
        // 删除客户端
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
                "[%d-%02d-%02d %02d:%02d:%02d.%03d] R%d #%d %s 已断开\0",
                1900 + sysTime_t->tm_year, 1 + sysTime_t->tm_mon, sysTime_t->tm_mday,
                sysTime_t->tm_hour, sysTime_t->tm_min, sysTime_t->tm_sec, (int)(time(NULL) % 1000),
                clients[i].num,
                i,
                clients[i].name
            );
            SetConsoleTitle(buffer);
            while ((strlen(History) + strlen(buffer) + 2) > History_SIZE) {
                char* pos = strchr(History, '\n');
                if (pos != nullptr) {
                    int index = pos - History + 1; // +1 to also remove the '\n'
                    strcpy(History, History + index);
                }
            }
            strcat(History, buffer);
            strcat(History, "\n\0");
            /*if ((strlen(History) + strlen(buffer) + 2) > History_SIZE) {
                strcpy(History, buffer);
                strcat(History, "\n\0");
            }
            else {
                strcat(History, buffer);
                strcat(History, "\n\0");
            }*/
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
#endif

