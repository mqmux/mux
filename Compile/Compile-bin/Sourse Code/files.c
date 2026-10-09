#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int bufferSize = 1024;
SOCKET m_Client;

void RecvFile();
void SendFile(char* file);
void Schedule(unsigned long long haveSend, double speed, unsigned long long filesize);
bool isIPAddressValid(const char* pszIPAddr);
int writen(int fd, const char* msg, int size);
int sendMsg(int cfd, char* msg, int len);
int readn(int fd, char* buf, int size);
int recvMsg(int cfd, char** msg);

void gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD coord;
    coord.X = x + csbi.dwCursorPosition.X;
    coord.Y = y + csbi.dwCursorPosition.Y;
    SetConsoleCursorPosition(hConsole, coord);
}

int main(int argc, char* argv[])
{
    //初始化WSA  
    WORD sockVersion = MAKEWORD(2, 2);
    if (isIPAddressValid(argv[1])) {
        WSADATA data;
        if (WSAStartup(sockVersion, &data) != 0)
        {
            return 0;
        }
        //客户端
        m_Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_Client == INVALID_SOCKET)
        {
            printf("invalid socket!");
            return 0;
        }

        sockaddr_in serAddr;
        serAddr.sin_family = AF_INET;
        serAddr.sin_port = htons(atoi(argv[2]));
        serAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
        while (connect(m_Client, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
        {  //连接失败 
            Sleep(1000);
        }
        if (argc == 3)
            RecvFile();
        else
            SendFile(argv[3]);
        closesocket(m_Client);
    }
    else if (argc > 1) { //--------------------------------------------------
        //服务器
        WSADATA wsaData;
        if (WSAStartup(sockVersion, &wsaData) != 0)
        {
            return 0;
        }

        //创建套接字  
        SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (slisten == INVALID_SOCKET)
        {
            printf("socket error !");
            return 0;
        }

        //绑定IP和端口  
        sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(atoi(argv[1]));
        sin.sin_addr.S_un.S_addr = INADDR_ANY;
        if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
        {
            printf("bind error !");
        }

        //开始监听  
        if (listen(slisten, 5) == SOCKET_ERROR)
        {
            printf("listen error !");
            return 0;
        }

        //循环接收数据  
        sockaddr_in remoteAddr;
        int nAddrlen = sizeof(remoteAddr);
        char revData[255];
        //while (true)
        //{
        printf("WAIT...\r");
        m_Client = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
        if (m_Client == INVALID_SOCKET)
        {
            printf("accept error !");
            //continue;
            return 1;
        }
        //printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
        if (argc == 2)
            RecvFile();
        else
            SendFile(argv[2]);
        closesocket(m_Client);
        //}
        closesocket(slisten);
    }
    else {
        printf("Usage: %s [htons] [filename] 服务器\n", argv[0]);
        printf("   or: %s [ip] [htons] [filename] 客户端\n", argv[0]);
        printf("  and: 有 [filename] 就是发送,没有就是接收\n");
        return 0;
    }
    WSACleanup();
    return 0;
}



void RecvFile()
{
    char file[bufferSize] = { 0 };
    char buffer[bufferSize] = { 0 };
    int readLen = 0;
    unsigned long long haveSend = 0;

    //readLen = recv(m_Client, file, bufferSize, 0);
    char** recvMsg_n=new char*[100];
    recvMsg(m_Client, recvMsg_n);
    strcpy(file,recvMsg_n[0]);

    printf("RECV:%s | ", file);
    char filesizes[256];
    //recv(m_Client, filesizes, bufferSize, 0);
    recvMsg(m_Client, recvMsg_n);
    strcpy(filesizes, recvMsg_n[0]);
    delete[] recvMsg_n[0];
    delete[] recvMsg_n;

    unsigned long long  filesize = _atoi64(filesizes);
    char speed_B[3] = "B";
    double chave;
    if (filesize > 1024) {
        chave = filesize / 1024.00;
        speed_B[0] = 'K'; speed_B[1] = 'B'; speed_B[2] = '\0';
        if (chave > 1024) {
            chave /= 1024.00;
            speed_B[0] = 'M'; speed_B[1] = 'B'; speed_B[2] = '\0';
            if (chave > 1024) {
                chave /= 1024.00;
                speed_B[0] = 'G'; speed_B[1] = 'B'; speed_B[2] = '\0';
            }
        }
    }
    else chave = filesize;
    printf("FILESIZE:%1.2f%s [%sB]\n", chave, speed_B, filesizes); fflush(stdout);

    SYSTEMTIME st = { 0 };
    int tmptime = 0;
    double speed = 0;
    unsigned long long lasthavesend = 0;
    ofstream desFile;
    desFile.open(file, ios::binary);
    if (!desFile)
    {
        return;
    }
    do
    {
        readLen = recv(m_Client, buffer, bufferSize, 0);
        haveSend += readLen;
        if (readLen == 0)
        {
            break;
        }
        else
        {
            desFile.write(buffer, readLen);
        }
        GetLocalTime(&st);
        if (st.wSecond != tmptime) {
            speed = haveSend - lasthavesend;
            Schedule(haveSend, speed, filesize);
            tmptime = st.wSecond;
            lasthavesend = haveSend;
        }
    } while (true);
    desFile.close();
    gotoxy(0, 1);
}

void Schedule(unsigned long long haveSend, double speed, unsigned long long filesize) {
    char speed_B[3] = "B";
    double percent = 0;
    char unit[2] = "S";
    double RemainingTime = (filesize - haveSend) / speed;
    if (RemainingTime >= 60) {
        RemainingTime /= 60;
        unit[0] = 'M';
        unit[1] = '\0';
    }
    if (speed >= 1024) {
        speed /= 1024.00;
        speed_B[0] = 'K'; speed_B[1] = 'B'; speed_B[2] = '\0';
        if (speed >= 1024) {
            speed /= 1024.00;
            speed_B[0] = 'M'; speed_B[1] = 'B'; speed_B[2] = '\0';
        }
    }
    if (haveSend != 0 && filesize != 0) {
        if (haveSend >= filesize)
            percent = 100;
        else
            percent = (haveSend / (filesize * 0.01));
    }
    int ProgressBar = percent / 2.5;
    printf("[");
    for (int i = 0; i < ProgressBar; i++) printf("=");
    printf(">");
    for (int i = 0; i < 39 - ProgressBar; i++) printf(" ");
    printf("][%1.2f%%][%1.2f%s/S][%1.2f%s]   \n", percent, speed, speed_B, RemainingTime, unit); fflush(stdout);
    gotoxy(0, -1);
}

void SendFile(char* file)
{
    char buffer[bufferSize] = { 0 };
    int readLen = 0;
    string srcFileName = file;  //这是用户端要发送的路径
    int index = srcFileName.find_last_of("\\");
    string filename = srcFileName.substr(index + 1, -1);

    int filename_len = strlen(filename.c_str());
    //send(m_Client, filename.c_str(), filename_len, 0);
    char* sendMsg_filename = new char[filename_len];
    strcpy(sendMsg_filename, filename.c_str());
    sendMsg(m_Client, sendMsg_filename, filename_len);

    printf("SEND:%s | ", sendMsg_filename);
    ifstream fin(srcFileName);
    fin.seekg(0, fin.end);
    unsigned long long filesize = fin.tellg();
    char speed_B[3] = "B";
    double chave;
    if (filesize > 1024) {
        chave = filesize / 1024.00;
        speed_B[0] = 'K'; speed_B[1] = 'B'; speed_B[2] = '\0';
        if (chave > 1024) {
            chave /= 1024.00;
            speed_B[0] = 'M'; speed_B[1] = 'B'; speed_B[2] = '\0';
            if (chave > 1024) {
                chave /= 1024.00;
                speed_B[0] = 'G'; speed_B[1] = 'B'; speed_B[2] = '\0';
            }
        }
    }
    else chave = filesize;
    //printf("FILESIZE:%1.2f%s [%lldB]\n", chave, speed_B, filesize); fflush(stdout);

    char filesizes[256];
    sprintf(filesizes, "%lld", filesize);
    //send(m_Client, filesizes, bufferSize, 0);
    int filesizes_len = strlen(filesizes);
    char* filesizes_filename = new char[filesizes_len];
    strcpy(filesizes_filename, filesizes);
    sendMsg(m_Client, filesizes_filename, filesizes_len);
    printf("FILESIZE:%1.2f%s [%sB]\n", chave, speed_B, filesizes_filename); fflush(stdout);

    ifstream srcFile;
    srcFile.open(srcFileName.c_str(), ios::binary);
    if (!srcFile) {
        return;
    }
    SYSTEMTIME st = { 0 };
    int tmptime = 0;
    unsigned long long haveSend = 0;
    double speed = 0;
    unsigned long long lasthavesend = 0;
    while (!srcFile.eof()) {
        srcFile.read(buffer, bufferSize);
        readLen = srcFile.gcount();
        send(m_Client, buffer, readLen, 0);
        haveSend += readLen;
        GetLocalTime(&st);
        if (st.wSecond != tmptime) {
            speed = haveSend - lasthavesend;
            Schedule(haveSend, speed, filesize);
            tmptime = st.wSecond;
            lasthavesend = haveSend;
        }
    }
    srcFile.close();
    fin.close();
    delete sendMsg_filename;
    delete filesizes_filename;
    gotoxy(0, 1);
}

bool isIPAddressValid(const char* pszIPAddr)
{
    if (!pszIPAddr) return false; //若pszIPAddr为空  
    char IP1[100], cIP[4];
    int len = strlen(pszIPAddr);
    int i = 0, j = len - 1;
    int k, m = 0, n = 0, num = 0;
    //去除首尾空格(取出从i-1到j+1之间的字符):  
    while (pszIPAddr[i++] == ' ');
    while (pszIPAddr[j--] == ' ');

    for (k = i - 1; k <= j + 1; k++)
    {
        IP1[m++] = *(pszIPAddr + k);
    }
    IP1[m] = '\0';

    char* p = IP1;

    while (*p != '\0')
    {
        if (*p == ' ' || *p < '0' || *p>'9') return false;
        cIP[n++] = *p; //保存每个子段的第一个字符，用于之后判断该子段是否为0开头  

        int sum = 0;  //sum为每一子段的数值，应在0到255之间  
        while (*p != '.' && *p != '\0')
        {
            if (*p == ' ' || *p < '0' || *p>'9') return false;
            sum = sum * 10 + *p - 48;  //每一子段字符串转化为整数  
            p++;
        }
        if (*p == '.') {
            if ((*(p - 1) >= '0' && *(p - 1) <= '9') && (*(p + 1) >= '0' && *(p + 1) <= '9'))//判断"."前后是否有数字，若无，则为无效IP，如“1.1.127.”  
                num++;  //记录“.”出现的次数，不能大于3  
            else
                return false;
        };
        if ((sum > 255) || (sum > 0 && cIP[0] == '0') || num > 3) return false;//若子段的值>255或为0开头的非0子段或“.”的数目>3，则为无效IP  

        if (*p != '\0') p++;
        n = 0;
    }
    if (num != 3) return false;
    return true;
}

/*
函数描述: 发送指定的字节数
函数参数:
    - fd: 通信的文件描述符(套接字)
    - msg: 待发送的原始数据
    - size: 待发送的原始数据的总字节数
函数返回值: 函数调用成功返回发送的字节数, 发送失败返回-1
*/
int writen(int fd, const char* msg, int size)
{
    const char* buf = msg;
    int count = size;
    while (count > 0)
    {
        int len = send(fd, buf, count, 0);
        if (len == -1)
        {
            //closesocket(fd);
            return -1;
        }
        else if (len == 0)
        {
            continue;
        }
        buf += len;
        count -= len;
    }
    return size;
}

/*
函数描述: 发送带有数据头的数据包
函数参数:
    - cfd: 通信的文件描述符(套接字)
    - msg: 待发送的原始数据
    - len: 待发送的原始数据的总字节数
函数返回值: 函数调用成功返回发送的字节数, 发送失败返回-1
*/
int sendMsg(int cfd, char* msg, int len)
{
    if (msg == NULL || len <= 0 || cfd <= 0)
    {
        return -1;
    }
    // 申请内存空间: 数据长度 + 包头4字节(存储数据长度)
    char* data = (char*)malloc(len + 4);
    int bigLen = htonl(len);
    memcpy(data, &bigLen, 4);
    memcpy(data + 4, msg, len);
    // 发送数据
    int ret = writen(cfd, data, len + 4);
    // 释放内存
    free(data);
    return ret;
}


/*
函数描述: 接收指定的字节数
函数参数:
    - fd: 通信的文件描述符(套接字)
    - buf: 存储待接收数据的内存的起始地址
    - size: 指定要接收的字节数
函数返回值: 函数调用成功返回发送的字节数, 发送失败返回-1
*/
int readn(int fd, char* buf, int size)
{
    char* pt = buf;
    int count = size;
    while (count > 0)
    {
        int len = recv(fd, pt, count, 0);
        if (len == -1)
        {
            return -1;
        }
        else if (len == 0)
        {
            return size - count;
        }
        pt += len;
        count -= len;
    }
    return size;
}

/*
函数描述: 接收带数据头的数据包
函数参数:
    - cfd: 通信的文件描述符(套接字)
    - msg: 一级指针的地址，函数内部会给这个指针分配内存，用于存储待接收的数据，这块内存需要使用者释放
函数返回值: 函数调用成功返回接收的字节数, 发送失败返回-1
*/
int recvMsg(int cfd, char** msg)
{
    // 接收数据
    // 1. 读数据头
    int len = 0;
    readn(cfd, (char*)&len, 4);
    len = ntohl(len);
    //printf("数据块大小: %d\n", len);

    // 根据读出的长度分配内存，+1 -> 这个字节存储\0
    char* buf = (char*)malloc(len + 1);
    int ret = readn(cfd, buf, len);
    if (ret != len)
    {
        //closesocket(cfd);
        free(buf);
        return -1;
    }
    buf[len] = '\0';
    *msg = buf;

    return ret;
}