#include<Windows.h>
#include<stdio.h>

int main(int argc, char* argv[])
{
    if (argc > 1) {
        HWND hwnd = GetConsoleWindow();
        if (argc == 3) hwnd = (HWND)atoi(argv[2]);
        HINSTANCE hInstance = GetModuleHandle((LPCWSTR)argv[0]);
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadImage(hInstance, (LPCWSTR)argv[1], IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
    }
    else {
        printf("Usage: %s [IconFile] [hwnd] 设置控制台图标,后面的句柄(10进制)不填则默认控制台自身 \n", argv[0]);
    }
    return 0;
}

//HICON hIcon = (HICON)LoadImage(hInstance, TEXT("small.ico"), IMAGE_ICON, 0, 0, 0);
//SendMessage(hwnd, WM_SETICON, ICON_BIG, hIcon );
/*
SendMessage(hwnd, WM_SETICON, TRUE,  (LPARAM)LoadIcon((HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),MAKEINTRESOURCE(idi)));
SendMessage(hwnd, WM_SETICON, FALSE, (LPARAM)LoadIcon((HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),MAKEINTRESOURCE(idi)));
 */
 /* #include <iostream>
 #include <windows.h>
 #include <stdlib.h>
 #include <bitset>

 using namespace std;

 int main(int argc, char *argv[])
 {

     //设置程序图标
     HWND hwnd = GetForegroundWindow();
     HINSTANCE hInstance = GetModuleHandle(argv[0]);
     //使用外部资源
     SendMessage(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)LoadIcon(hInstance,TEXT("gdg.ico")));

     //使用系统标准图标
     //SendMessage(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)LoadIcon(NULL,MAKEINTRESOURCE(IDI_WARNING)));
     //SendMessage(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)LoadIcon(NULL,TEXT("gdg.ico")));

     return 0;
 } */


 /*
 HANDLE LoadImage(
     HINSTANCE hinst,     // 若加载程序外部资源传NULL,否则一般传AfxGetInstanceHandle()
     LPCTSTR lpszName,    // 图片名称或全路径
     UINT uType,          // 图片类型:IMAGE_BITMAP或IMAGE_ICON或IMAGE_CURSOR
     int cxDesired,
     int cyDesired,
     UINT fuLoad          // 一般为LR_DEFAULTCOLOR | LR_CREATEDIBSECTION
 );

 cxDesired， cyDesired：
 按Image的实际尺寸加载：cxDesired=0，cyDesired=0并且fuLoad不指定LR_DEFAULTSIZE。
 按指定大小加载： cxDesired，cyDesired不为0并且fuLoad不指定LR_DEFAULTSIZE，Image会根据实际大小缩放。
 按Windows系统默认的Icon或Cursor大小(一般为32x32)加载Icon或Cursor：cxDesired=0，cyDesired=0， fuLoad指定LR_DEFAULTSIZE，Image会根据实际大小缩放。
 注意: LR_DEFAULTSIZE只对Icon和Cursor起作用，对Bitmap不起作用。

 fuLoad:
 若从独立于程序的Image文件(.bmp,.ico)加载，请指定标志LR_LOADFROMFILE。

 示例
 1.加载Icon资源
 按Windows系统默认的Icon大小加载

 HICON hIcon = (HICON)::LoadImage(
         AfxGetInstanceHandle(),
         MAKEINTRESOURCE(IDI_ICON1),
         IMAGE_ICON,
         0, 0,
         LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

 2.加载本地磁盘的Icon文件
 按实际大小加载

 HICON hIcon = (HICON)::LoadImage(
         AfxGetInstanceHandle(),
         TEXT("D:\\Butterfly.ico"),
         IMAGE_ICON,
         0, 0,
         LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);

 3.加载本地磁盘的Bitmap文件
 按指定大小加载

 HBITMAP hBitmap = (HBITMAP)::LoadImage(
         NULL,
         TEXT("D:\\001.bmp"),
         IMAGE_BITMAP,
         50, 50,
         LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE); */

         /*

         SendMessage(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)LoadIcon(NULL,IDI_APPLICATION));

         IDI_APPLICATION
         MAKEINTRESOURCE(32512)	默认程序图标
         IDI_ASTERISK
         MAKEINTRESOURCE(32516)	Asterisk图标，
         与IDI_INFORMATION相同
         IDI_ERROR
         MAKEINTRESOURCE(32513)	Hand-shaped图标
         IDI_EXCLAMATION
         MAKEINTRESOURCE(32515)	感叹号图标，
         与IDI_WARNING相同
         IDI_HAND
         MAKEINTRESOURCE(32513)	Hand-shaped图标，
         与IDI_ERROR相同
         IDI_INFORMATION
         MAKEINTRESOURCE(32516)	Asterisk图标
         IDI_QUESTION
         MAKEINTRESOURCE(32514)	疑问号图标
         IDI_SHIELD
         MAKEINTRESOURCE(32518)	安全伞图标
         IDI_WARNING
         MAKEINTRESOURCE(32515)	感叹号图标
         IDI_WINLOGO
         MAKEINTRESOURCE(32517)	默认程序图标，
         Win2000：Windows logo图标

         */