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
struct _Client
{
    SOCKET s = INVALID_SOCKET;
    int num = 0;
    char name[1024];
    char time[32];//[2024/12/25 12:10:34:345] 23+1
};
_Client _client_;
std::vector<_Client> clients;
int sendmsg(SOCKET sock, char* data) {
    int len = strlen(data) + 1;
    if (len > 1024) return 0;
    send(sock, (char*)&len, sizeof(int), 0);
    send(sock, data, len, 0);
    return 1;
}
int recvmsg(SOCKET sock, char* data) {
    int recvLen;
    int data_recv = recv(sock, (char*)&recvLen, sizeof(int), 0);
    if (data_recv <= 0) return data_recv;
    if (recvLen > 1024) return -1024;
    data_recv = recv(sock, data, recvLen, 0);
    if (data_recv <= 0) return data_recv;
    if (data_recv == recvLen) return 2;
    return 1;
}
