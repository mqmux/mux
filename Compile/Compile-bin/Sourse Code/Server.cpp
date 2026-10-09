#define _CRT_NONSTDC_NO_DEPRECATE 1//strupr等不安全函数
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <cstring>
#include <fstream>
#include <conio.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

//===============================全局常量=================
const int BUF_SIZE = 2048;
const int NICKNAME_LEN = 512;
const int MAX_CLIENT_COUNT = 100; //最大连接数
//===============================全局变量=================
SOCKET sockSer, sockCli;
SOCKADDR_IN addrCli, addrSer;
int naddr = sizeof(SOCKADDR_IN);
char sendbuf[BUF_SIZE];//发送缓冲区
char inputbuf[BUF_SIZE];//输入缓冲区
char recvbuf[BUF_SIZE];//接受缓冲区
char localIP[50];//本地IP地址
int clientCount = 0;
int back_socket[MAX_CLIENT_COUNT + 1];
int Ser_htons;
int sy(0), ey(0), eey(0);
struct Client
{
    SOCKET s;
    SOCKADDR_IN sin;
    char name[NICKNAME_LEN];
}Cli[MAX_CLIENT_COUNT];

//===============================函数声明==================

DWORD WINAPI Server_Listen_Thread(LPVOID lp);
DWORD WINAPI Recv_Thread(LPVOID lp);

void getLocalIP(char localIp[], int n);
void setColor(int color);
bool AllisNum(char str[]);  //判断是否为位数字
int clientCount_num();
void main_command();
//----------------
int endlie;
int tmphang;
void movestr(int s, int e, int f) {
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
    GetConsoleScreenBufferInfo(outputHandle, &consoleScreenBufferInfo);
    SMALL_RECT movePos{ 0, s, consoleScreenBufferInfo.srWindow.Right,e };
    COORD newPos{ 0, f };
    CHAR_INFO charInfo;
    charInfo.Char.AsciiChar = ' ';
    charInfo.Attributes = consoleScreenBufferInfo.wAttributes;
    ScrollConsoleScreenBufferA(outputHandle, &movePos, NULL, newPos, &charInfo);
}

void gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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

DWORD WINAPI Server_Loop_Send(LPVOID lp)
{
    for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
        if (back_socket[i] != 0) {
            send(Cli[i].s, "echo:>nul", strlen("echo:>nul"), 0);
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        Ser_htons = atoi(argv[1]);
    }
    else {
        cout << "Usage: " << argv[0] << " [port] \n\
       \n\
  输入 /exit 退出选择其他客户端\n\
  输入客户端代号选择与客户端对话\n\
  输入 /0 发送消息给所有人 /q踢出房间 \n\
  输入 /file|+文件 发送文件 /ls 查看客户端列表 \n";
        return 0;
    }
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        setColor(0x0c);
        cout << "载入socket库失败" << endl;
        return 0;
    }

    for (int i = 0; i <= MAX_CLIENT_COUNT; i++)
        back_socket[i] = 0;//记录断开的套接字数组下标
    getLocalIP(localIP, sizeof(localIP) / sizeof(char));
    setColor(0x03);
    cout << "[" << localIP << ":" << Ser_htons << "]作为聊天室服务端已经开启" << endl;
    setColor(0x07);

    //创建socket
    sockSer = socket(AF_INET, SOCK_STREAM, 0);
    //初始化本地地址
    addrSer.sin_family = AF_INET;
    addrSer.sin_addr.s_addr = INADDR_ANY;//服务器bind时需要使用地址通配
    addrSer.sin_port = htons(Ser_htons);

    //绑定socket和本地地址
    bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR));

    //创建接受连接线程
    LPVOID* lp_link = (LPVOID*)&sockSer;
    HANDLE hT_Accept_Link = CreateThread(NULL, 0, Server_Listen_Thread, lp_link, 0, NULL);
    CreateThread(NULL, 0, Server_Loop_Send, lp_link, 0, NULL);

    //循环接收输入
    main_command();

    closesocket(sockCli);   //关闭套接字
    closesocket(sockSer);   //关闭套接字
    WSACleanup();           //释放套接字资源;

    return 0;
}

DWORD WINAPI Server_Listen_Thread(LPVOID lp)
{
    SOCKET* s = (SOCKET*)lp;
    SYSTEMTIME sysTime = { 0 };
    while (1)
    {
        //监听socket
        listen(*s, 5);
        //接受连接
        //
        for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
            if (back_socket[i] == 0) {
                clientCount = i;
                break;
            }
            else {
                clientCount = 0;
            }
        }
        if (clientCount == 0) {
            Sleep(2000);
            continue;
        }
        //堵塞在accept
        Cli[clientCount].s = accept(*s, (SOCKADDR*)&Cli[clientCount].sin, &naddr);//从服务器套接口接受连接请求，将接受到的地址保存在缓冲区
        //此时创建了一个新的套接口与客户端的套接口连接，之后的通信都从这个专属套接口执行

        if (Cli[clientCount].s != INVALID_SOCKET)//连接成功
        {
            char tmp[10];
            _itoa_s(clientCount_num() + 1, tmp, 10);
            strcpy_s(sendbuf, "[连接成功],当前连接数:");
            strcat_s(sendbuf, tmp);
            send(Cli[clientCount].s, sendbuf, strlen(sendbuf), 0);

            //接收客户端名字
            recv(Cli[clientCount].s, Cli[clientCount].name, BUF_SIZE, 0);

            setColor(0x0a);
            back_socket[clientCount] = clientCount;

            LPVOID* lp_recv = (LPVOID*)&Cli[clientCount];
            //gotoxy(0, input_num + 1);
            /*endlie = gotoxy(0);
            tmphang = gotoxy(1);
            movestr(tmphang, tmphang, tmphang + 2);*/
            //-------------
            ey = gotoxy(1);
            int x = gotoxy(0);
            int jump = 64;
            movestr(sy, ey, ey + jump);//把输入中的消息移动到下面
            //-------------------
            //cout << "\n(" << clientCount << ")" << "[" << Cli[clientCount].name << "](" << inet_ntoa(Cli[clientCount].sin.sin_addr) << ")连接成功！当前连接数：" << clientCount_num() << endl;
            GetSystemTime(&sysTime);
            // << "year:" << sysTime.wYear << " month:" << sysTime.wMonth << " day:" << sysTime.wDay
            //   << " hour:" << sysTime.wHour + 8 << " minute:" << sysTime.wMinute << " second:" << sysTime.wSecond << " milliseconds:" << sysTime.wMilliseconds;
            cout << "\n(" << clientCount << ")" << "[" << Cli[clientCount].name << "][" << sysTime.wHour + 8 << ":" << sysTime.wMinute << ":" << sysTime.wSecond << "." << sysTime.wMilliseconds << "]连接成功！当前连接数:" << clientCount_num() << endl;
            //--------------
            eey = gotoxy(1);
            movestr(ey + 1, eey - 1, sy);//收到消息放到上面
            movestr(ey + jump, eey + ey - sy + jump, sy + eey - ey - 1);//输入中的消息移动到收到的消息的下面
            gotoxy(x, ey + eey - ey - 1);
            sy += (eey - ey - 1);
            //----------------
            /*movestr(tmphang + 1, tmphang + 1, tmphang);
            movestr(tmphang + 2, tmphang + 2, tmphang + 1);
            gotoxy(endlie, tmphang + 1);*/

            setColor(0x07);
            CloseHandle(CreateThread(NULL, 0, Recv_Thread, lp_recv, 0, NULL));

            Sleep(1000);
        }

    }

}
//============================线程函数============================
DWORD WINAPI Recv_Thread(LPVOID lp) {
    SOCKET* s = (SOCKET*)lp;
    int nrecv;
    int num = clientCount;
    char tmp[10];
    itoa(clientCount_num(), tmp, 10);
    strcpy(recvbuf, "[");
    strcat(recvbuf, Cli[num].name);
    strcat(recvbuf, "]进入了聊天室,当前连接数:");
    strcat(recvbuf, tmp);
    for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
        if (back_socket[i] != 0 && back_socket[i] != num) {
            send(Cli[i].s, recvbuf, strlen(recvbuf), 0);
        }
    }
    memset(recvbuf, 0, sizeof(recvbuf));
    while (1)
    {
        nrecv = recv(Cli[num].s, recvbuf, sizeof(recvbuf), 0);
        if (nrecv <= 0 && errno != EINTR) {
            back_socket[num] = 0;
            setColor(0x0c);
            strcpy(recvbuf, "[");
            strcat(recvbuf, Cli[num].name);
            strcat(recvbuf, "]退出了聊天室,当前连接数:");
            itoa(clientCount_num(), tmp, 10);
            strcat(recvbuf, tmp);
            //gotoxy(0, input_num + 1);
            /*endlie = gotoxy(0);
            int tmphang = gotoxy(1);
            movestr(tmphang, tmphang, tmphang + 2);*/
            //-------------
            ey = gotoxy(1);
            int x = gotoxy(0);
            int jump = 64;
            movestr(sy, ey, ey + jump);//把输入中的消息移动到下面
            //-------------------

            printf("\n");
            SYSTEMTIME sysTime = { 0 };
            GetSystemTime(&sysTime);
            cout << "(" << num << ")" << "[" << Cli[num].name << "][" << sysTime.wHour + 8 << ":" << sysTime.wMinute << ":" << sysTime.wSecond << "." << sysTime.wMilliseconds << "]已断开,当前连接数:" << clientCount_num() << endl;
            //--------------
            eey = gotoxy(1);
            movestr(ey + 1, eey - 1, sy);//收到消息放到上面
            movestr(ey + jump, eey + ey - sy + jump, sy + eey - ey - 1);//输入中的消息移动到收到的消息的下面
            gotoxy(x, ey + eey - ey - 1);
            sy += (eey - ey - 1);
            //----------------
            /*movestr(tmphang + 1, tmphang + 1, tmphang);
            movestr(tmphang + 2, tmphang + 2, tmphang + 1);
            gotoxy(endlie, tmphang + 1);*/
            setColor(0x07);
            for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                if (back_socket[i] != 0 && back_socket[i] != num) {
                    send(Cli[i].s, recvbuf, strlen(recvbuf), 0);
                }
            }
            memset(recvbuf, 0, sizeof(recvbuf));

            break;
        }
        if (nrecv > 0)//如果接收到数据
        {
            /*endlie = gotoxy(0);
            tmphang = gotoxy(1);
            movestr(tmphang, tmphang, tmphang + 2);*/
            //-------------
            ey = gotoxy(1);
            int x = gotoxy(0);
            int jump = 64;
            movestr(sy, ey, ey + jump);//把输入中的消息移动到下面
            //-------------------
            cout << "\n(" << num << ")" << recvbuf << endl;
            //--------------
            eey = gotoxy(1);
            movestr(ey + 1, eey - 1, sy);//收到消息放到上面
            movestr(ey + jump, eey + ey - sy + jump, sy + eey - ey - 1);//输入中的消息移动到收到的消息的下面
            gotoxy(x, ey + eey - ey - 1);
            sy += (eey - ey - 1);
            //----------------
            /*movestr(tmphang + 1, tmphang + 1, tmphang);
            movestr(tmphang + 2, tmphang + 2, tmphang + 1);
            gotoxy(endlie, tmphang + 1);*/
            //转发每个客户端
            for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                if (back_socket[i] != 0 && back_socket[i] != num) {
                    send(Cli[i].s, recvbuf, strlen(recvbuf), 0);
                }
            }
            memset(recvbuf, 0, sizeof(recvbuf));
        }
    }
    setColor(0x07);
    return 0;
}

//========================================================================
int clientCount_num() {
    int n = 0;
    for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
        if (back_socket[i] != 0)
            n++;
    }
    return n;
}

//获取本地IPv4
void getLocalIP(char localIp[], int n)
{
    gethostname(localIp, n);
    HOSTENT* host = gethostbyname(localIp);
    //以下未研究
    in_addr PcAddr;
    for (int i = 0;; i++)
    {
        char* p = host->h_addr_list[i];
        if (NULL == p)
        {
            break;
        }
        memcpy(&(PcAddr.S_un.S_addr), p, host->h_length);
        strcpy(localIp, inet_ntoa(PcAddr));
    }
}
//设置颜色
void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
//发送消息
void main_command()
{
    sy = gotoxy(1);
    clock_t t;
    const int timeout = 180000; // clock() 返回毫秒
    int key, count, wcount = 0, i = 0;
    char title[100];
    while (1) {
        t = clock() + timeout;
        key = 0;
        while (t > clock())
        {
            count = (t - clock()) / 1000;
            sprintf(title, "WAIT : %d.%d    ", wcount, count);
            SetConsoleTitle(title);
            //printf("WAIT:%d.%d    \r", wcount, count);
            if (kbhit()) //没有的话, 试试_kbhit()
            {
                _getch(); //vc6没有getch, 只有_getch, 有的编译器这2还有不同
                if (kbhit())_getch();
                key = 1;
                break;
            }
        }
        wcount++;
        if (key) {
            break;
        }
        else {
            for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                if (back_socket[i] != 0) {
                    send(Cli[i].s, "echo:>nul", 10, 0);
                }
            }
        }
    }
    while (1)
    {
        int command[MAX_CLIENT_COUNT + 1] = { 0 };
        //memset(a,0,sizeof(a));
        char a[100];
        memset(a, 0, sizeof(a));
        sy = gotoxy(1);
        cout << "$ "; cin.getline(a, 100);
        if (strlen(a) == 0) continue;
        char delims[] = " ";
        char* result = NULL;
        result = strtok(a, delims);

        int i = 1;
        while (result != NULL) {
            command[i] = atoi(result);
            result = strtok(NULL, delims);
            i++;
        }

        if (!strcmp(a, "/ls")) {
            for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                if (back_socket[i] != 0 && back_socket[i] != clientCount) {
                    cout << '|' << "(" << i << ")" << Cli[i].name;
                }
            }
            cout << '|' << endl;
            continue;
        }
        else if (!strcmp(a, "/0")) {
            while (1) {
                memset(inputbuf, 0, sizeof(inputbuf));
                sy = gotoxy(1);
                cout << ">[所有人:]";
                cin.getline(inputbuf, BUF_SIZE);
                //退出点
                if (strlen(inputbuf) == 0) continue;
                if (strcmp(inputbuf, "/exit") == 0) break;
                if (strncmp(inputbuf, "/file|", 6) == 0)
                {
                    char len[100], * name = new char[100];
                    char delims[] = "|";
                    char* result = NULL;
                    result = strtok(inputbuf, delims);
                    int i = 1;
                    while (result != NULL)
                    {
                        if (i == 2) strcpy(name, result);
                        result = strtok(NULL, delims);
                        i++;
                    }
                    fstream file;
                    file.open(name, ios::binary | ios::in | ios::ate);   //打开时指针在文件尾
                    if (!file.good()) {
                        cout << "文件不存在\n";
                        continue;
                    }
                    char* ext = strrchr(name, '\\');
                    if (ext)
                    {
                        *ext = '\0';
                        ext++;
                    }
                    if (ext != NULL) name = ext;
                    delete ext;
                    int length = file.tellg();
                    char* imgData = new char[length];
                    file.seekg(0);
                    file.read(imgData, length);  //二进制只能用这个读
                    itoa(length, len, 10);
                    strcat(inputbuf, "|");
                    strcat(inputbuf, name);
                    strcat(inputbuf, "|");
                    strcat(inputbuf, len);
                    for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                        if (back_socket[i] != 0) {
                            send(Cli[i].s, inputbuf, strlen(inputbuf), 0);
                        }
                    }
                    cout << "SEND:" << name << "[" << length << "B]\n";
                    //--------
                    int haveSend = 0;
                    const int bufferSize = 1024;
                    char buffer[bufferSize] = { 0 };
                    int readLen = 0;
                    SYSTEMTIME st = { 0 };
                    int tmptime = 0;
                    //---------
                    ifstream srcFile;
                    srcFile.open(name, ios::binary);
                    st = { 0 };
                    if (!srcFile) {
                        return;
                    }
                    while (!srcFile.eof()) {
                        srcFile.read(buffer, bufferSize);
                        readLen = srcFile.gcount();
                        for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                            if (back_socket[i] != 0) {
                                send(Cli[i].s, buffer, readLen, 0);
                            }
                            haveSend += readLen;
                            GetLocalTime(&st);
                            if (st.wSecond != tmptime) {
                                printf("data:%dB  \r", haveSend);
                                tmptime = st.wSecond;
                            }
                        }
                    }
                    printf("DATA:%dB      \n", haveSend);
                    srcFile.close();

                    memset(name, '\0', sizeof(name));
                    continue;
                }

                //给每个客户端发送留言
                strcpy(sendbuf, "echo:");
                strcat(sendbuf, inputbuf);
                if (strcmp(inputbuf, "/loop") == 0) {
                    for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                        if (back_socket[i] != 0) {
                            send(Cli[i].s, "echo:>nul", strlen("echo:>nul"), 0);
                        }
                    }
                    clock_t t;
                    const int timeout = 180000; // clock() 返回毫秒
                    int key, count, wcount = 0, i = 0;
                    char title[100];
                    while (1) {
                        t = clock() + timeout;
                        key = 0;
                        while (t > clock())
                        {
                            count = (t - clock()) / 1000;
                            sprintf(title, "WAIT : %d.%d    ", wcount, count);
                            SetConsoleTitle(title);
                            //printf("WAIT:%d.%d    \r", wcount, count);
                            if (kbhit()) //没有的话, 试试_kbhit()
                            {
                                _getch(); //vc6没有getch, 只有_getch, 有的编译器这2还有不同
                                if (kbhit())_getch();
                                key = 1;
                                break;
                            }
                        }
                        wcount++;
                        if (key) {
                            break;
                        }
                        else {
                            for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                                if (back_socket[i] != 0) {
                                    send(Cli[i].s, "echo:>nul", 10, 0);
                                }
                            }
                        }
                    }
                    continue;
                }
                for (int i = 1; i <= MAX_CLIENT_COUNT; i++) {
                    if (back_socket[i] != 0) {
                        send(Cli[i].s, sendbuf, strlen(sendbuf), 0);
                    }
                }
            }
            continue;
        }
        else if (!(*a >= '0' && *a <= '9')) {
            continue;
        }

        while (1)
        {
            memset(inputbuf, 0, sizeof(inputbuf));
            sy = gotoxy(1);
            cout << ">";
            for (int j = 1; j <= MAX_CLIENT_COUNT; j++) {
                if (!(command[j] == 0))
                    cout << "[" << command[j] << "]";
            }
            cin.getline(inputbuf, BUF_SIZE);
            if (strcmp(inputbuf, "/q") == 0) {
                for (int j = 1; j <= MAX_CLIENT_COUNT; j++) {
                    if (!(command[j] == 0)) {
                        int tmp_q = command[j];
                        back_socket[tmp_q] = 0;
                        closesocket(Cli[tmp_q].s);
                    }
                }
            }
            if (strlen(inputbuf) == 0) continue;
            if (strcmp(inputbuf, "/exit") == 0) break;
            if (strncmp(inputbuf, "/file|", 6) == 0)
            {
                char len[100], * name = new char[100];
                char delims[] = "|";
                char* result = NULL;
                result = strtok(inputbuf, delims);
                int i = 1;
                while (result != NULL)
                {
                    if (i == 2) strcpy(name, result);
                    result = strtok(NULL, delims);
                    i++;
                }
                fstream file;
                file.open(name, ios::binary | ios::in | ios::ate);   //打开时指针在文件尾
                if (!file.good()) {
                    cout << "\n文件不存在\n";
                    continue;
                }
                char* ext = strrchr(name, '\\');
                if (ext)
                {
                    *ext = '\0';
                    ext++;
                }
                if (ext != NULL) name = ext;
                delete ext;
                int length = file.tellg();
                char* imgData = new char[length];
                file.seekg(0);
                //读文件
                file.read(imgData, length);  //二进制只能用这个读
                itoa(length, len, 10);
                strcat(inputbuf, "|");
                strcat(inputbuf, name);
                strcat(inputbuf, "|");
                strcat(inputbuf, len);

                delete imgData;
                //发送文件配置信息
                for (int j = 1; j <= MAX_CLIENT_COUNT; j++)
                {
                    if (command[j] != 0)
                    {
                        int tmp = command[j];
                        send(Cli[tmp].s, inputbuf, strlen(inputbuf), 0);
                    }
                }
                cout << "SEND:" << name << "[" << length << "B]\n";
                //--------
                int haveSend = 0;
                const int bufferSize = 1024;
                char buffer[bufferSize] = { 0 };
                int readLen = 0;
                SYSTEMTIME st = { 0 };
                int tmptime = 0;
                //---------
                //开始传输

                ifstream srcFile;
                srcFile.open(name, ios::binary);
                if (!srcFile) {
                    return;
                }

                while (!srcFile.eof()) {
                    srcFile.read(buffer, bufferSize);
                    readLen = srcFile.gcount();
                    for (int j = 1; j <= MAX_CLIENT_COUNT; j++)
                    {
                        if (command[j] != 0)
                        {
                            int tmp = command[j];
                            send(Cli[tmp].s, buffer, readLen, 0);
                        }
                    }
                    haveSend += readLen;
                    GetLocalTime(&st);
                    if (st.wSecond != tmptime) {
                        printf("data:%dB  \r", haveSend);
                        tmptime = st.wSecond;
                    }
                }
                srcFile.close();
                printf("DATA:%dB      \n", haveSend);
                memset(name, '\0', sizeof(name));
                continue;
            }

            for (int j = 1; j <= MAX_CLIENT_COUNT; j++)
            {
                if (command[j] != 0)
                {
                    int tmp = command[j];
                    strcpy(sendbuf, "echo:");
                    strcat(sendbuf, inputbuf);
                    send(Cli[tmp].s, sendbuf, strlen(sendbuf), 0);
                }
            }
        }
    }
}