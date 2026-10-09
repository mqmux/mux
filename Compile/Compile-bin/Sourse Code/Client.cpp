#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include<windows.h>
#include <stdlib.h>
#include <stdio.h>
#include<conio.h>
#include <iostream>
#include <cstring>
#include <fstream>
#pragma comment(lib,"ws2_32.lib")

using namespace std;
//===============================全局常量=================
const int BUF_SIZE = 2048;
const int NICKNAME_LEN = 512;

//===============================全局变量=================
SOCKET  sockCli;//服务端和客户端的Socket
SOCKADDR_IN  addrSer, addrCli;//服务端、客户端的地址包
int naddr = sizeof(SOCKADDR_IN);
int cli_stop = 0;

char sendbuf[BUF_SIZE];//发送缓冲区
char inputbuf[BUF_SIZE];//输入缓冲区
char recvbuf[BUF_SIZE];//接受缓冲区
char client_name[NICKNAME_LEN] = "travaler";//用户名
char ServIP[50];//服务器IP地址
int Ser_htons;//端口号
bool recvs = false;
int sy(0), ey(0), eey(0);
//===============================函数声明==================
DWORD WINAPI Client_Receive_Thread(LPVOID lp);
void setColor(int color);
int endlie;
DWORD WINAPI Server_Listen_Thread(LPVOID lp);
void movestr(int s, int e, int f) {
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
    GetConsoleScreenBufferInfo(outputHandle, &consoleScreenBufferInfo);
    //printf("窗口大小: x=%hd, y=%hd\n", consoleScreenBufferInfo.srWindow.Right + 1, consoleScreenBufferInfo.srWindow.Bottom + 1);
    SMALL_RECT movePos{ 0, s, consoleScreenBufferInfo.srWindow.Right,e };
    COORD newPos{ 0, f };
    CHAR_INFO charInfo;
    charInfo.Char.AsciiChar = ' ';
    charInfo.Attributes = consoleScreenBufferInfo.wAttributes;
    ScrollConsoleScreenBufferA(outputHandle, &movePos, NULL, newPos, &charInfo);
}

void gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //COORD coordScreen = {0, 0}; //光标位置
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

int gotoxy(int x) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    if (x == 0)
        return csbi.dwCursorPosition.X;
    else
        return csbi.dwCursorPosition.Y;
}

int main(int argc, char* argv[])
{
    switch (argc) {
    case 1://0
        cout << "请输入服务器IP地址:"; cin.getline(ServIP, 50);
    case 2://1
        cout << "请输入端口号:"; cin >> Ser_htons; cin.get();
    case 3://2
        cout << "请输入你的昵称:"; cin.getline(client_name, NICKNAME_LEN);
    case 4://3
        switch (argc) {
        case 4://3
            strcpy_s(client_name, argv[3]);
        case 3://2
            Ser_htons = atoi(argv[2]);
        case 2://1
            strcpy_s(ServIP, argv[1]);
        case 1://0
            break;
        }
        break;
    default:
        return 0;
    }

    /*if (argc == 4) {
        strcpy_s(ServIP, argv[1]);
        Ser_htons = atoi(argv[2]);
        strcpy_s(client_name, argv[3]);
    }
    else {
        cout << "请输入服务器IP地址:"; cin.getline(ServIP, 50);
        cout << "请输入端口号:"; cin >> Ser_htons; cin.get();
        cout << "请输入你的昵称:"; cin.getline(client_name, NICKNAME_LEN);
    }*/

    //载入socket库
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        setColor(0x0c);
        cout << "载入socket库失败" << endl;
        return 0;
    }

    LPVOID* lp = (LPVOID*)&sockCli;
    HANDLE hThread = CreateThread(NULL, 0, Client_Receive_Thread, lp, 0, NULL);

    while (1) {
        //创建socket
        sockCli = socket(AF_INET, SOCK_STREAM, 0);
        //初始化本地地址
        addrCli.sin_family = AF_INET;
        addrCli.sin_addr.s_addr = inet_addr("127.0.0.1");
        addrCli.sin_port = htons(12248);
        //初始化服务器地址
        addrSer.sin_family = AF_INET;
        addrSer.sin_addr.s_addr = inet_addr(ServIP);
        addrSer.sin_port = htons(Ser_htons);

        //连接到服务端

        //将服务器的地址包与本客户端的套接字连接
        if (connect(sockCli, (SOCKADDR*)&addrSer, sizeof(addrCli)) != SOCKET_ERROR)//连接成功
        {
            recvbuf[0] = '\0';
            recv(sockCli, recvbuf, sizeof(recvbuf), 0);
            if (strlen(recvbuf) == 0) {
                Sleep(60000);
                continue;
            }
            //先告诉服务端自己的名字
            send(sockCli, client_name, sizeof(client_name), 0);
            setColor(0x0a);
            endlie = gotoxy(0);
            int tmphang = gotoxy(1);
            movestr(tmphang, tmphang, tmphang + 2);
            printf("\n");
            cout << recvbuf << endl;

            movestr(tmphang + 1, tmphang + 1, tmphang);
            movestr(tmphang + 2, tmphang + 2, tmphang + 1);
            gotoxy(endlie, tmphang + 1);
            setColor(0x07);
        }
        //=========================================
        //创建接收数据线程
        recvs = true;

        int nrecv = 1;
        while (1)
        {
            memset(recvbuf, '\0', strlen(recvbuf));
            nrecv = recv(sockCli, recvbuf, sizeof(recvbuf), 0);

            if (nrecv <= 0 && errno != EINTR) { break; }
            if (nrecv > 0)//如果接收到数据
            {
                if (strlen(recvbuf) == 0)
                    continue;
                if (*recvbuf == '[') {
                    //-------------
                    ey = gotoxy(1);
                    int x = gotoxy(0);
                    int jump = 64;
                    movestr(sy, ey, ey + jump);//把输入中的消息移动到下面
                    //-------------------
                    cout << "\n" << recvbuf << '\n';
                    //--------------
                    eey = gotoxy(1);
                    movestr(ey + 1, eey - 1, sy);//收到消息放到上面
                    movestr(ey + jump, eey + ey - sy + jump, sy + eey - ey - 1);//输入中的消息移动到收到的消息的下面
                    gotoxy(x, ey + eey - ey - 1);
                    sy += (eey - ey - 1);
                    //----------------		
                    continue;
                }
                int length = 100;
                char name[1024];
                if (strncmp(recvbuf, "/file|", 6) == 0) {
                    char delims[] = "|";
                    char* result = NULL;
                    result = strtok(recvbuf, delims);
                    int i = 1;
                    while (result != NULL) {
                        if (i == 2) strcpy_s(name, result);
                        if (i == 3) length = atoi(result);
                        result = strtok(NULL, delims);
                        i++;
                    }
                    //-------------
                    ey = gotoxy(1);
                    int x = gotoxy(0);
                    int jump = 64;
                    movestr(sy, ey, ey + jump);//把输入中的消息移动到下面
                    //-------------------
                    cout << '\n' << "RECV:" << name << "[" << length << "B] ... \n";
                    //--------------
                    eey = gotoxy(1);
                    movestr(ey + 1, eey - 1, sy);//收到消息放到上面
                    movestr(ey + jump, eey + ey - sy + jump, sy + eey - ey - 1);//输入中的消息移动到收到的消息的下面
                    gotoxy(x, ey + eey - ey - 1);
                    sy += (eey - ey - 1);
                    //----------------

                    const int bufferSize = 1024;
                    char buffer[bufferSize] = { 0 };
                    int readLen = 0;
                    int haveSend = 0;

                    ofstream desFile;
                    desFile.open(name, ios::binary);
                    if (!desFile)
                    {
                        return 0;
                    }
                    do
                    {
                        readLen = recv(sockCli, buffer, bufferSize, 0);
                        if (readLen == 0)
                        {
                            break;
                        }
                        else
                        {
                            desFile.write(buffer, readLen);
                            haveSend += readLen;
                            //printf("data:%db  \r", haveSend);
                            if (haveSend >= length) {
                                break;
                            }
                        }
                    } while (true);
                    desFile.close();
                    //-------------
                    ey = gotoxy(1);
                    x = gotoxy(0);
                    jump = 64;
                    movestr(sy, ey, ey + jump);//把输入中的消息移动到下面
                    //-------------------
                    printf("\nDATA:%dB      \n", haveSend);
                    //--------------
                    eey = gotoxy(1);
                    movestr(ey + 1, eey - 1, sy);//收到消息放到上面
                    movestr(ey + jump, eey + ey - sy + jump, sy + eey - ey - 1);//输入中的消息移动到收到的消息的下面
                    gotoxy(x, ey + eey - ey - 1);
                    sy += (eey - ey - 1);
                    //----------------
                    continue;
                }
                if (strncmp(recvbuf, "echo:", 5) == 0) {
                    ey = gotoxy(1);
                    int x = gotoxy(0);
                    int jump = 64;
                    movestr(sy, ey, ey + jump);
                    printf("\n");

                    //printf(recvbuf);
                    system(recvbuf);

                    eey = gotoxy(1);
                    movestr(ey + 1, eey - 1, sy);
                    movestr(ey + jump, eey + ey - sy + jump, sy + eey - ey - 1);//输入中的消息移动到收到的消息的下面
                    gotoxy(x, ey + eey - ey - 1);
                    sy += (eey - ey - 1);
                }
                else {
                    cout << "strlen(recvbuf):" << strlen(recvbuf) << endl;
                    cout << "[else:]" << recvbuf << '\n';
                }
            }
        }
        recvs = false;
        cli_stop = 0;
        while (cli_stop < 60) {
            cli_stop++;
            Sleep(1000);
        }
    }
    closesocket(sockCli);   //关闭套接字
    WSACleanup();           //释放套接字资源;
    return 0;
}

DWORD WINAPI Client_Receive_Thread(LPVOID lp)
{
    //SOCKET* s = (SOCKET*)lp;
    //循环接收输入
    //char* strs = new char[2048];
    while (1)
    {
        if (recvs) {
            sy = gotoxy(1);
            printf("                             \r");
            cout << "[" << client_name << "]:"; cin.getline(inputbuf, BUF_SIZE);
            if (strlen(inputbuf) == 0) continue;
            strcpy_s(sendbuf, "[");
            strcat_s(sendbuf, client_name);
            strcat_s(sendbuf, "]:");
            strcat_s(sendbuf, inputbuf);
            send(sockCli, sendbuf, strlen(sendbuf), 0);
            memset(inputbuf, '\0', strlen(inputbuf));
        }
        else {
            printf("connecting... \r");
            Sleep(10);
            cli_stop = 60;
        }
    }
    //closesocket(sockCli);   //关闭套接字
    return 0;
}
void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

