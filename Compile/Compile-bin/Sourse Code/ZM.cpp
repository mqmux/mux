#include<iostream>
#include<vector>
#include <io.h>
//#include <unistd.h>
#include <Windows.h>
#include<string>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

//https://www.cnblogs.com/yhlboke-1992/p/9315263.html

#define MAX_PATH 80
void getFiles(string path, vector<string>& files);


void MoveWindow(int posx, int posy)
{
    RECT rectClient, rectWindow;
    HWND hWnd = GetConsoleWindow();
    GetClientRect(hWnd, &rectClient);
    GetWindowRect(hWnd, &rectWindow);
    MoveWindow(hWnd, posx, posy, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, TRUE);
}

void MoveCenter()
{
    RECT rectClient, rectWindow;
    HWND hWnd = GetConsoleWindow();
    GetClientRect(hWnd, &rectClient);
    GetWindowRect(hWnd, &rectWindow);
    int posx, posy;
    posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2,
        posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2,

        MoveWindow(hWnd, posx, posy, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, TRUE);
}


int number = 1; //定义全局变量
char agg3[64]; //定义全局变量
CRITICAL_SECTION Critical;      //定义临界区句柄

/* unsigned long __stdcall ThreadProc1(void* lp)
{
    char command[100];
    while(1){
        cin.getline(command,100);
        system(command);
        memset(command,0,sizeof(command));
    }

    return 0;
} */
LPCWSTR string2LPCWSTR(std::string str)
{
    size_t size = str.length();
    int wLen = ::MultiByteToWideChar(CP_UTF8,
        0,
        str.c_str(),
        -1,
        NULL,
        0);
    wchar_t* buffer = new wchar_t[wLen + 1];
    memset(buffer, 0, (wLen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, (LPWSTR)buffer, wLen);
    return buffer;
}

unsigned long __stdcall ThreadProc2(void* lp)
{
    system(agg3);
    return 0;
}

int main(int argc, char* argv[])
{

    InitializeCriticalSection(&Critical);   //初始化临界区对象

    //CreateThread(NULL, 0, ThreadProc1, NULL, 0, NULL);


    if (argc == 2) {
        SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)argv[1], SPIF_SENDCHANGE);
    }
    else if (argc == 4) {
        /* int Width{ GetSystemMetrics(SM_CXSCREEN) };
        int Heigth{ GetSystemMetrics(SM_CYSCREEN) };
        MoveWindow(Width/2-200,0);

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  */
        //SetConsoleTitle(time());// 窗口标题
        /* SMALL_RECT rc = {0,0,70,15}; // 重置窗口大小 33,0
        COORD sizes = {0,0};
        SetConsoleWindowInfo(hOut,true ,&rc);
        SetConsoleScreenBufferSize(hOut,sizes); // 重新设置缓冲区大小 */
        strcpy(agg3, argv[3]);
        int time = atoi(argv[2]);
        /* HWND hwnd;
        hwnd=FindWindow("ConsoleWindowClass",NULL);
        if(hwnd)
        {
            ShowWindow(hwnd,SW_HIDE);				//不显示
        }
        */
        CreateThread(NULL, 0, ThreadProc2, NULL, 0, NULL);

        vector<string> files;
        char  buffer[MAX_PATH];
        //getcwd(buffer, MAX_PATH);
        strcpy(buffer, argv[1]);
        //printf("buffer:%s\n",buffer);
        string filePath = buffer;
        getFiles(filePath, files);
        //char str[30];
        int size = files.size();
        int num = 0;
        int number = 0;
        char m_s[25];
        char m_m[25];
        char m_d[25];


        while (1) {
            for (int i = 0; i < size; i++)
            {
                //cout<<++num<<':'<<files[i].c_str()<<endl;
                SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)files[i].c_str(), SPIF_SENDCHANGE);
                for (int j = 0; j < time; j++) {
                    SYSTEMTIME sys;
                    GetLocalTime(&sys);
                    char cmd[1024] = { 0 };
                    sprintf(cmd, "%02d:%02d:%02d -- ", sys.wHour, sys.wMinute, sys.wSecond);
                    if (number / 3600 < 10) strcat(cmd, "0");
                    itoa(number / 3600, m_d, 10);
                    strcat(cmd, m_d);
                    strcat(cmd, ":");

                    if (number % 3600 / 60 < 10) strcat(cmd, "0");
                    itoa(number % 3600 / 60, m_m, 10);
                    strcat(cmd, m_m);
                    strcat(cmd, ":");

                    itoa(number % 60, m_s, 10);
                    if (number % 60 < 10) strcat(cmd, "0");
                    strcat(cmd, m_s);
                    //strcat(cmd,"& copy con nul");
                        //SetConsoleTitle(cmd);// 窗口标题
                    SetConsoleTitle(string2LPCWSTR(cmd));
                    Sleep(1000);
                    number++;
                }
            }
        }
    }
    else {
        printf("Usage: %s [path] [time] [argument] 设置桌面壁纸\n", argv[0]);
    }
    return 0;
}

void getFiles(string path, vector<string>& files)
{
    long hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    string Jpg[] = { "\\*.jpg","\\*.png" };
    for (int i = 0; i < sizeof(Jpg) / sizeof(Jpg[0]); i++) {
        if ((hFile = _findfirst(p.assign(path).append(Jpg[i]).c_str(), &fileinfo)) != -1)
        {
            do
            {
                if ((fileinfo.attrib & _A_SUBDIR))
                {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                        getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
                }
                else
                {
                    files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                }
            } while (_findnext(hFile, &fileinfo) == 0);
            _findclose(hFile);
        }
    }
}
