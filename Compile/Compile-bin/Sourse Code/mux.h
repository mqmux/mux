#pragma once

#include <stdio.h>
#include <set>
//#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cmath>
#include <io.h>
#include <fstream>
#include <direct.h>
#include <string>
#include <fstream>
#define MAX(x,y) x>y?x:y
#define STR(X) #X //转字符串
#define CAT(X,Y) X##Y //拼接字符、函数名、变量等等
#define KEY_SIZE 1024
#define RECV_SIZE 1024
#define SEND_SIZE 1024
#define FILENAME_SIZE 1024
#define FILE_SIZE 1024
#define bufferSize 2048
#pragma comment(lib, "ws2_32.lib")

std::set<std::string> ignoredPaths;

void print(char* data, int len) {
    for (int i = 0; i < len; i++)
        printf("[%c|%d]", data[i], data[i]);
    printf("\n");
}

int SEND(SOCKET sock, char* data) {
    int len = strlen(data) + 1;
    if (len > SEND_SIZE) return -1;
    strcat(data, "\0");
    send(sock, (char*)&len, sizeof(int), 0);
    send(sock, data, len, 0);
    Sleep(1);
    return 0;
}
int SEND(SOCKET sock, const char* data) {
    char buf[SEND_SIZE];
    strcpy(buf, data);
    int len = strlen(buf) + 1;
    if (len > SEND_SIZE) return -1;
    strcat(buf, "\0");
    send(sock, (char*)&len, sizeof(int), 0);
    send(sock, buf, len, 0);
    Sleep(1);
    return 0;
}
void SEND(SOCKET sock, int len) {
    send(sock, (char*)&len, sizeof(int), 0);
    Sleep(1);
}
int RECV(SOCKET sock, char* data) {
    int recvLen;
    int _recv = recv(sock, (char*)&recvLen, sizeof(int), 0);
    if (_recv <= 0) return _recv;
    if (recvLen > RECV_SIZE) return -1;
    _recv = recv(sock, data, recvLen, 0);//接收数据
    if (_recv == recvLen) return 1;//返回1代表接收成功
    return _recv;
}
int RECV(SOCKET sock) {
    int recvLen = 0;
    if (recv(sock, (char*)&recvLen, sizeof(int), 0) <= 0) return -1;
    return recvLen;
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
    bind(server, (struct sockaddr*)&addr, sizeof(addr)); //bind
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
    connect(client, (struct sockaddr*)&addr, sizeof(addr)); //connect
    return client;
}

void LoadGitignore()
{
    std::ifstream file(".gitignore");
    if (!file) {
        printf(".gitignore file not found, skipping...");
        return;
    }
    ignoredPaths.clear();
    std::string line;
    while (std::getline(file, line))
    {
        ignoredPaths.insert(line);
    }
}

int ListFiles(const char* path, const char* root, char** filenames, int& file_cnt, int deep)
{
    struct _finddata_t file;
    long hFile;
    char buf[FILENAME_SIZE];
    char tmp_filename_pah[FILENAME_SIZE];
    strcpy(buf, path);
    strcat(buf, "\\*");
    if ((hFile = _findfirst(buf, &file)) != -1L) {
        do {
            if (file.attrib & _A_SUBDIR) {
                if (strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0) {
                    char subPath[FILENAME_SIZE];
                    strcpy(subPath, path);
                    strcat(subPath, "\\");
                    strcat(subPath, file.name);
                    if (ignoredPaths.find(subPath + 1) != ignoredPaths.end()) continue; // Skip if path is in .gitignore
                    deep++;
                    ListFiles(subPath, root, filenames, file_cnt, deep);
                    deep--;
                }
            }
            else {
                /*for (std::set<std::string>::iterator it = ignoredPaths.begin(); it != ignoredPaths.end(); ++it)
                {
                    printf("[ignoredPaths:%s]\n", it->c_str());
                }*/
                filenames[file_cnt] = new char[FILENAME_MAX];
                if (deep == 0) {
                    if (ignoredPaths.find(file.name) != ignoredPaths.end()) continue; // Skip if file is in .gitignore
                    strcpy(filenames[file_cnt], file.name);
                }
                else {
                    sprintf(tmp_filename_pah, "%s\\%s", path + strlen(root) + 1, file.name);
                    if (ignoredPaths.find(tmp_filename_pah) != ignoredPaths.end()) continue; // Skip if file is in .gitignore
                    sprintf(filenames[file_cnt], "%s\\%s", path + strlen(root) + 1, file.name);
                }
                file_cnt++;
                if (file_cnt > FILE_SIZE) return 1;
            }
        } while (_findnext(hFile, &file) == 0);
        _findclose(hFile);
    }
    return 0;
}

//CRC32算法，文件摘要算法
uint32_t crc32(std::ifstream& file) {
    uint32_t crc_table[256];
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
        crc_table[i] = crc;
    }

    uint32_t crc = 0xFFFFFFFF;
    char buf[1024];
    while (file.read(buf, sizeof(buf)), file.gcount() > 0)
        for (auto q = buf, qend = buf + file.gcount(); q < qend; ++q)
            crc = crc_table[(crc ^ *q) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFF;
}
bool CreateDirectoryRecursively(char* path) {
    char* subpath = _strdup(path);
    char* cursor = strchr(subpath + ((subpath[0] == '\\') ? 1 : 0), '\\');
    while (cursor != NULL) {
        *cursor = '\0';
        if (!CreateDirectory(subpath, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
            free(subpath);
            return false;
        }
        *cursor = '\\';
        cursor = strchr(cursor + 1, '\\');
    }
    free(subpath);
    return true;
}
void RecvFile(SOCKET client)
{
    char file[FILE_SIZE] = { 0 };
    char buffer[FILE_SIZE] = { 0 };
    int readLen = 0;
    int haveSend = 0;
    RECV(client, file);
    int file_size = RECV(client);
    if (RECV(client) == 444) {
        printf("%4d * %s NOT EXIST\n", 404, file); fflush(stdout);
        return;
    }
    std::ofstream desFile;
    CreateDirectoryRecursively(file);
    desFile.open(file, std::ios::binary);
    if (!desFile)
    {
        printf("%4d * %s RUNING\n", 404, file); fflush(stdout);
        SEND(client, 444);
        return;
        /*while(!desFile){
            strcat(file, ".tmp");
            desFile.open(file, std::ios::binary);
        }
        printf("%3d - %s\n", 0, file); fflush(stdout);*/
    }
    else SEND(client, 1);
    do
    {
        if (file_size == 0) break;
        readLen = recv(client, buffer, FILE_SIZE, 0);
        haveSend += readLen;
        if (readLen == 0 || haveSend > file_size)
        {
            break;
        }
        else
        {
            desFile.write(buffer, readLen);
            if (haveSend == file_size) break;
        }
    } while (true);
    desFile.close();
}

void SendFile(SOCKET client, char* file)
{
    int haveSend = 0;
    char buffer[FILE_SIZE] = { 0 };
    int readLen = 0;
    SEND(client, file);
    Sleep(100);
    struct stat stat_buf;
    int rc = stat(file, &stat_buf);
    int file_size = rc == 0 ? stat_buf.st_size : -1;
    SEND(client, file_size);
    std::ifstream srcFile;
    srcFile.open(file, std::ios::binary);
    if (!srcFile) {
        SEND(client, 444);
        return;
    }
    SEND(client, 1);
    if (RECV(client) == 444) {
        printf("%4d * %s Can't SEND\n", 404, file); fflush(stdout);
        return;
    }
    int tmptime = 0;
    while (!srcFile.eof()) {
        srcFile.read(buffer, FILE_SIZE);
        readLen = srcFile.gcount();
        send(client, buffer, readLen, 0);
        haveSend += readLen;
    }
    srcFile.close();
}
void gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD coord;
    coord.X = x + csbi.dwCursorPosition.X;
    coord.Y = y + csbi.dwCursorPosition.Y;
    SetConsoleCursorPosition(hConsole, coord);
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


void _RecvFile(SOCKET m_Client)
{
    char file[bufferSize] = { 0 };
    char buffer[bufferSize] = { 0 };
    int readLen = 0;
    long long haveSend = 0;
    readLen = RECV(m_Client, file);
    printf("RECV:%s | ", file); fflush(stdout);
    int filesize = RECV(m_Client);
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
    printf("FILESIZE:%1.2f%s [%dB]\n", chave, speed_B, filesize); fflush(stdout);
    Sleep(100);
    SYSTEMTIME st = { 0 };
    int tmptime = 0;
    double speed = 0;
    unsigned long long lasthavesend = 0;
    std::ofstream desFile;
    desFile.open(file, std::ios::binary);
    int REC = 0;
    if (!desFile)
    {
        printf("Open File Error\n"); fflush(stdout);
        return;
    }
    do
    {
        readLen = recv(m_Client, buffer, bufferSize, 0);
        haveSend += readLen;
        if (readLen == 0) {
            if (REC > 10) break;
            Sleep(80);
            REC++;
        }
        if (readLen == 0)
        {
            //printf("\nreadLen=%d;haveSend=%lld;filesize=%d\n", readLen, haveSend, filesize);
            //if (haveSend != filesize) printf("readLen=%d;haveSend=%lld;filesize=%d\n", readLen, haveSend, filesize);
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
    printf("[=======================================>][100.00%%][0.00B/S][%lldB]   \n", haveSend); fflush(stdout);
    gotoxy(0, 1);
}

void _SendFile(SOCKET m_Client, char* file,int wait_time)
{
    char buffer[bufferSize] = { 0 };
    int readLen = 0;
    std::string srcFileName = file;  //这是用户端要发送的路径
    //int index = srcFileName.find_last_of("\\");
    //std::string filename = srcFileName.substr(index + 1, -1);
    //printf("SEND:%s | ", filename.c_str()); fflush(stdout);
    printf("SEND:%s | ", srcFileName.c_str()); fflush(stdout);
    //SEND(m_Client, filename.c_str());
    SEND(m_Client, srcFileName.c_str());
    std::ifstream fin(srcFileName);
    fin.seekg(0, fin.end);
    long long filesize = fin.tellg();
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
    printf("FILESIZE:%1.2f%s [%lldB]\n", chave, speed_B, filesize); fflush(stdout);
    SEND(m_Client, filesize);
    Sleep(100);
    std::ifstream srcFile;
    srcFile.open(srcFileName.c_str(), std::ios::binary);
    if (!srcFile) {
        return;
    }
    SYSTEMTIME st = { 0 };
    int tmptime = 0;
    unsigned long long haveSend = 0;
    double speed = 0;
    unsigned long long lasthavesend = 0;
    //Sleep(1000);
    while (!srcFile.eof()) {
        srcFile.read(buffer, bufferSize);
        readLen = srcFile.gcount();
        send(m_Client, buffer, readLen, 0);
        //Sleep(1);
        haveSend += readLen;
        GetLocalTime(&st);
        if (st.wSecond != tmptime) {
            speed = haveSend - lasthavesend;
            Schedule(haveSend, speed, filesize);
            tmptime = st.wSecond;
            lasthavesend = haveSend;
        }
    }
    //printf("\nhaveSend=%lld;readLen=%d;filesize=%lld\n", haveSend, readLen, filesize); fflush(stdout);
    srcFile.close();
    fin.close();
    printf("[=======================================>][100.00%%][0.00B/S][%lldB]", haveSend); fflush(stdout);
    gotoxy(0, 1);
    Sleep(wait_time); //最后一个数据包接收不到，大概率是发送端提前断开
}
int _SEND(SOCKET sock, const char* data) {
    int len = strlen(data) + 1;
    if (len > SEND_SIZE) return -1;
    send(sock, (char*)&len, sizeof(int), 0);
    send(sock, data, len, 0);
    Sleep(100);
    //send(sock, data, len, 0);
    return 0;
}
int _RECV(SOCKET sock, char* data) {
    data[0] = '\0';
    int recvLen = 0;
    int REC = 0;
    int _recv = 0;
    while (REC < 100) {
        if (recv(sock, (char*)&recvLen, sizeof(int), 0) != 0) break;
        Sleep(10);
        REC++;
    }
    //printf("REC=%d,recvLen=%d\n", REC, recvLen);
    REC = 0;
    if (recvLen <= 0) return recvLen;
    if (recvLen > RECV_SIZE) return -1024;
    while (REC < 100) {
        _recv = recv(sock, data, recvLen, 0);//接收数据
        //printf("[%d]%d:%s\n", REC,_recv, data);
        if (_recv != 0) break;
        Sleep(10);
        REC++;
    }
    //printf("REC=%d,data=%s\n", REC, data);
    if (_recv == recvLen) return recvLen;//返回1代表接收成功
    return _recv;
}