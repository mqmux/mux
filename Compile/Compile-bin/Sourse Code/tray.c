#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include<conio.h>

#if defined (_WIN32) || defined (_WIN64)
#define TRAY_WINAPI 1
#elif defined (__linux__) || defined (linux) || defined (__linux)
#define TRAY_APPINDICATOR 1
#elif defined (__APPLE__) || defined (__MACH__)
#define TRAY_APPKIT 1
#endif

#include "tray.h"

#if TRAY_APPINDICATOR
//#define TRAY_ICON1 "indicator-messages"
//#define TRAY_ICON2 "indicator-messages-new"
#elif TRAY_APPKIT
//#define TRAY_ICON1 "icon.png"
//#define TRAY_ICON2 "icon.png"
#elif TRAY_WINAPI
//#define TRAY_ICON1 "icon.ico"
//#define TRAY_ICON2 "icons.ico"
#endif

HANDLE g_hProcess = NULL; //目标进程的句柄
HWND g_hwnd = NULL;
static struct tray tray;
char filename[256];
char TRAY_ICON1[256];
char TRAY_ICON2[256];

typedef struct
{
    HWND hwndWindow; // 窗口句柄
    DWORD dwProcessID; // 进程ID
}EnumWindowsArg;
	
///< 枚举窗口回调函数
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    EnumWindowsArg *pArg = (EnumWindowsArg *)lParam;
    DWORD dwProcessID = 0;
    // 通过窗口句柄取得进程ID
    GetWindowThreadProcessId(hwnd, &dwProcessID);
    if (dwProcessID == pArg->dwProcessID)
    {
        pArg->hwndWindow = hwnd;
        // 找到了返回FALSE
        return FALSE;
    }
    // 没找到，继续找，返回TRUE
    return TRUE;
}
///< 通过进程ID获取窗口句柄
HWND GetWindowHwndByPID(DWORD dwProcessID)
{
    HWND hwndRet = NULL;
    EnumWindowsArg ewa;
    ewa.dwProcessID = dwProcessID;
    ewa.hwndWindow = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)&ewa);
    if (ewa.hwndWindow)
    {
        hwndRet = ewa.hwndWindow;
    }
    return hwndRet;
}

static void toggle_cb(struct tray_menu *item) {
  printf("toggle cb\n");
  
  item->checked = !item->checked;
  tray_update(&tray);
}

static void hello_cb(struct tray_menu *item) {
  (void)item;
  //--------
		HWND hd=GetDesktopWindow();  
		hd=GetWindow(hd,GW_CHILD);  
		while(hd!=NULL)  
		{
			if (g_hwnd == hd)
			{
				printf("  ------- %#X\n",hd);
				return;
			}
			hd=GetNextWindow(hd,GW_HWNDNEXT); 
		}
  //--------
  printf("hello cb\n");
//================
	char szFileName[256];
	strcpy(szFileName,filename);
    STARTUPINFO si;
    memset(&si, 0, sizeof(si));
    PROCESS_INFORMATION pi;
    BOOL bRet = CreateProcess(
            NULL,
            szFileName,
            NULL,
            NULL,
            FALSE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi
    );
    g_hProcess = pi.hProcess;
	printf("进程句柄 %#X\n",g_hProcess);
	printf("进程ID %d\n",pi.dwProcessId);
	Sleep(1000);
	g_hwnd=GetWindowHwndByPID(pi.dwProcessId);
	printf("当前窗口句柄 %X\n",g_hwnd);
// ================
  if (strcmp(tray.icon, TRAY_ICON1) == 0) {
    tray.icon = TRAY_ICON2;
  } else {
    tray.icon = TRAY_ICON1;
  }
  tray_update(&tray);
}

static void quit_cb(struct tray_menu *item) {
  (void)item;
  printf("quit cb\n");
  // --
  printf(" - %#X\n",g_hProcess);
  printf(" - %X\n",g_hwnd);
  SendMessage(g_hwnd, WM_CLOSE, 0, 0);
	if (g_hProcess)
	{
		DWORD dwEC = 0;
		BOOL b = GetExitCodeProcess(g_hProcess, &dwEC);
		if (b)
		{
			if (!TerminateProcess(g_hProcess, dwEC))
			{ printf("强行关闭 - 找不到进程"); }
		}
		CloseHandle(g_hProcess);
	}
	// --
  tray_exit();
}

static void submenu_cb(struct tray_menu *item) {
  (void)item;
  printf("submenu: clicked on %s\n", item->text);
  tray_update(&tray);
}

// Test tray init
static struct tray tray = {
    .icon = TRAY_ICON1,
    .menu =
        (struct tray_menu[]){
            {.text = "Termux", .cb = hello_cb},
            {.text = "Checked", .checked = 1, .cb = toggle_cb},
            {.text = "Disabled", .disabled = 1},
            {.text = "-"},
            {.text = "SubMenu",
             .submenu =
                 (struct tray_menu[]){
                     {.text = "FIRST", .checked = 1, .cb = submenu_cb},
                     {.text = "SECOND",
                      .submenu =
                          (struct tray_menu[]){
                              {.text = "THIRD",
                               .submenu =
                                   (struct tray_menu[]){
                                       {.text = "7", .cb = submenu_cb},
                                       {.text = "-"},
                                       {.text = "8", .cb = submenu_cb},
                                       {.text = NULL}}},
                              {.text = "FOUR",
                               .submenu =
                                   (struct tray_menu[]){
                                       {.text = "5", .cb = submenu_cb},
                                       {.text = "6", .cb = submenu_cb},
                                       {.text = NULL}}},
                              {.text = NULL}}},
                     {.text = NULL}}},
            {.text = "-"},
            {.text = "Quit", .cb = quit_cb},
            {.text = NULL}},
};

int main(int argc, char* argv[])
{
	if( argc != 4 ){
		printf("Usage: tray [filename][TRAY_ICON1][TRAY_ICON2]\n");
		return 0;
	}
	strcpy(filename,argv[1]);
	strcpy(TRAY_ICON1,argv[2]);
	strcpy(TRAY_ICON2,argv[3]);
	if (tray_init(&tray) < 0) {
		printf("failed to create tray\n");
		return 1;
	}
	//==============
	char szFileName[256];
	strcpy(szFileName,filename);
    STARTUPINFO si;
    memset(&si, 0, sizeof(si));
    PROCESS_INFORMATION pi;
    BOOL bRet = CreateProcess(
            NULL,
            szFileName,
            NULL,
            NULL,
            FALSE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi
    );
    g_hProcess = pi.hProcess;
	printf("进程句柄 %#X\n",g_hProcess);
	printf("进程ID %d\n",pi.dwProcessId);
	Sleep(1000);
	g_hwnd=GetWindowHwndByPID(pi.dwProcessId);
	printf("当前窗口句柄 %X\n",g_hwnd);
	//===============
	while (tray_loop(1) == 0) {
		printf("iteration\n");
	}
	return 0;
}
