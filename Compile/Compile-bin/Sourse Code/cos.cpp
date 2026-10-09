#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include<stdio.h>
#include <direct.h>
#pragma comment(lib,"ws2_32.lib")//链接这个库

int SEND(SOCKET sock, char* data) {
    int len = strlen(data) + 1;
    send(sock, (char*)&len, sizeof(int), 0);
    send(sock, data, len, 0);
    return 0;
}

int RECV(SOCKET sock, char* data) {
    int recvLen;
    int _recv = recv(sock, (char*)&recvLen, sizeof(int), 0);
    //printf("_recv:[%d]\n", _recv);
    if (_recv <= 0) return _recv;
    if (recvLen > 1024) return 1024;
    _recv = recv(sock, data, recvLen, 0);
    //printf("recvLen:[%d] ==  _recv:[%d]\n", recvLen, _recv);
    if (_recv == recvLen) return 1;
    return _recv;
    /*char* buffer = new char[recvLen];
    int totalReceived = 0;
    while (totalReceived < recvLen) {
        int received = recv(sock, buffer + totalReceived, recvLen - totalReceived, 0);
        std::cout << "RRR:[" << buffer << "]" << std::endl;
        if (received <= 0) {
            break;
        }
        totalReceived += received;
    }
    std::cout << "Received: " << buffer << std::endl;
    delete[] buffer;*/
}

DWORD WINAPI Server_Listen_Thread(LPVOID lp);
bool isend = true;
const int len = 1024;
SOCKET slisten;
SOCKET sclient;

DWORD WINAPI Server_Loop_Send(LPVOID lp)
{
    char tmp[2] = " ";
    while (1) {
        Sleep(200000);
        //send(sclient, " ", strlen(" "), 0);
        SEND(sclient, tmp);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 't' && argv[1][2] == '\0') {
        char Current_path[MAX_PATH];
        char* pwd = _getcwd(Current_path, MAX_PATH);
        printf("[%s]$ ", Current_path);
        return 0;
    }
    //调用winsock 
    WORD sockVersion = MAKEWORD(2, 2);//请求使用的winsock版本 
    WSADATA wsaData;   // 实际返回的winsock版本  
    if (WSAStartup(sockVersion, &wsaData) != 0) {
        return 0;
    }
    if (argc == 3 && !strchr(argv[1], '.')) {//服务端/被控端
        //创建socket 
        slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//参数分别为协议族，类型，协议号 AF_INET代表TCP/IP 
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
        char* recData = new char[len];
        char* com = new char[len];
        FILE* fp = NULL;
        int ret = 1;
        char buf[256];
        char mony[256] = "$ ";
        char* s;
        const char* sendData;
        char sedd[len - 8];

        while (1) {
            //listen
            if (listen(slisten, 5) == SOCKET_ERROR) { //5为queuesize,缓存区大小 
                printf("listen error !");
                return 0;
            }

            //由于使用的是TCP ，socket stream,要循环接收数据 
            sockaddr_in remoteAddr;
            int nAddrlen = sizeof(remoteAddr);

            sclient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);//accept会新建一个socket 
            if (sclient == INVALID_SOCKET) {
                printf("accept error !");
                return 0;
            }

            strcpy_s(sedd, "console");
            sendData = sedd;
            send(sclient, sendData, 7, 0);
            ret = 1;
            RECV(sclient, recData);
            char correct_error[] = "Password error";
            if (strcmp(recData, argv[2])) {
                SEND(sclient, correct_error);
                Sleep(1000);
                continue;
            }
            char correct[] = "correct";
            SEND(sclient, correct);
            printf("$ ");
            while (ret > 0) {
                memset(mony, '\0', sizeof(mony));
                memset(recData, '\0', sizeof(recData));
                memset(com, '\0', sizeof(com));
                //ret = recv(sclient, recData, len, 0);
                ret = RECV(sclient, recData);
                if (ret > 0) {
                    if (strlen(recData) == 0) continue;
                    for (int i = 0; i < strlen(recData); i++) {
                        if (recData[i] != ' ') {
                            com[0] = 'B';
                            break;
                        }
                    }
                    if (com[0] != 'B') {
                        continue;
                    }
                    printf("%s\n", recData);
                }
                else {
                    printf("\n");
                    break;
                }
                if (!strcmp(recData, "cls")) {
                    system("cls");
                    strcpy(com, "echo;&");
                    strcat(com, _pgmptr);
                    strcat(com, " -t");
                }
                else {
                    strcpy(com, recData);
                    strcat(com, " 2>&1&echo;&");
                    strcat(com, _pgmptr);
                    strcat(com, " -t");
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
                    //Sleep(35);
                }
                buf[strlen(buf) - 3] = '\0';
                s = buf;
                memcpy(mony, s + 1, strlen(buf) - 1);
                SetCurrentDirectoryA(mony);
                _pclose(fp);
            }
            //接收数据
            closesocket(sclient);
        }
        closesocket(slisten);
    }
    else if (argc == 4) {
        while (1) {
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
                Sleep(5000);
            }

            const char* sendData;
            char sedd[len];
            char* recData = new char[len];

            bool opinion = true;
            while (opinion) {
                int ret = recv(sclient, recData, 8, 0);
                if (ret > 0) {
                    recData[ret] = 0x00;
                    if (strcmp(recData, "console")) {
                        closesocket(sclient);
                        Sleep(5000);
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
                            Sleep(5000);
                        }
                    }
                    else {
                        SEND(sclient, argv[3]);//correct
                        RECV(sclient, sedd);
                        if (strcmp(sedd, "correct")) {
                            printf("%s", sedd);
                            return 404;
                        }
                        opinion = false;
                    }
                }
                else {
                    closesocket(sclient);
                    Sleep(5000);
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
                        Sleep(5000);
                    }
                }
            }
            LPVOID* lp_link = (LPVOID*)&sclient;
            CreateThread(NULL, 0, Server_Listen_Thread, lp_link, 0, NULL);
            CreateThread(NULL, 0, Server_Loop_Send, lp_link, 0, NULL);

            printf("$ ");
            for (int i = 0; i < sizeof(sedd); i++) sedd[i] = '\0';
            while (isend) {
                gets_s(sedd);
                if (sedd[0] == '\0') {
                    printf("$ ");
                    continue;
                }
                if (!strcmp(sedd, "cls")) system("cls");
                sendData = sedd;
                //send(sclient, sendData, strlen(sendData), 0);
                SEND(sclient, sedd);
            }
            closesocket(sclient);
            isend = true;
        }
    }
    else {
        printf("Usage: %s [htons] [key] 服务器 \n", argv[0]);
        printf("   or: %s [ip] [htons] [key] 客户端\n", argv[0]);
        return 0;
    }

    WSACleanup();
    return 0;
}

DWORD WINAPI Server_Listen_Thread(LPVOID lp)
{
    SOCKET* s = (SOCKET*)lp;
    char revData[len + 1];//buffer
    int ret = 1;
    while (ret > 0) {
        memset(revData, '\0', sizeof(revData));
        ret = recv(*s, revData, len, 0);
        if (ret > 0) {
            printf("%s", revData);
        }
    }
    closesocket(*s);
    isend = false;
    return 0;
}
