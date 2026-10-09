#include<Windows.h>
#include<stdio.h>
//#include<iostream>

BOOL m(0), k(0), a(0), m1(0);

//std::string get_time()
//{
//	SYSTEMTIME sys;
//	GetLocalTime(&sys);
//	char time[1024] = { 0 };
//	sprintf_s(time, "[%4d/%02d/%02d %02d:%02d:%02d]", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
//	return std::string(time);
//}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	/*
	typedef struct tagKBDLLHOOKSTRUCT {
	DWORD     vkCode;		// 按键代号
	DWORD     scanCode;		// 硬件扫描代号，同 vkCode 也可以作为按键的代号。
	DWORD     flags;		// 事件类型，一般按键按下为 0 抬起为 128。
	DWORD     time;			// 消息时间戳
	ULONG_PTR dwExtraInfo;	// 消息附加信息，一般为 0。
	}KBDLLHOOKSTRUCT,*LPKBDLLHOOKSTRUCT,*PKBDLLHOOKSTRUCT;
	*/

	/*KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;		// 包含低级键盘输入事件信息
	char data[1024];
	DWORD code = ks->vkCode;
	//std::string t = get_time();
	char state[20];
	if (wParam == WM_KEYDOWN)
	{
		strcpy_s(state, "按下");
	}
	else if (wParam == WM_KEYUP)
	{
		strcpy_s(state, "抬起");
	}
	//sprintf_s(data, "%s 键代码:%d %s", t.c_str(), code, state);
	sprintf_s(data, "%d %s", code, state);
	std::cout << data << std::endl;
	*/
	if (k == 1)return 1;	// 吃掉消息
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	/*
	typedef struct tagMOUSEHOOKSTRUCT {
		POINT   pt;					// Point数据
		HWND    hwnd;				// 接收鼠标消息的窗体的句柄
		UINT    wHitTestCode;		// 指定点击测试值
		ULONG_PTR dwExtraInfo;		// 指定和该消息相关联的附加信息。
	} MOUSEHOOKSTRUCT, FAR* LPMOUSEHOOKSTRUCT, * PMOUSEHOOKSTRUCT;
	

	//std::string t = get_time();
	char data[1024];

	MOUSEHOOKSTRUCT* ms = (MOUSEHOOKSTRUCT*)lParam;
	POINT pt = ms->pt;

	char state[20] = "未识别";

	if (wParam == WM_LBUTTONDOWN)	// 其余回到文档去找
	{
		strcpy_s(state, "左键按下");
	}
	else if (wParam == WM_LBUTTONUP)
	{
		strcpy_s(state, "左键抬起");
	}
	else if (wParam == WM_MOUSEMOVE)
	{
		strcpy_s(state, "移动");
	}
	else if (wParam == WM_RBUTTONDOWN)
	{
		strcpy_s(state, "右键按下");
	}
	else if (wParam == WM_RBUTTONUP)
	{
		strcpy_s(state, "右键抬起");
	}

	//sprintf_s(data, "%s 键代码:x:%d y:%d %s", t.c_str(), pt.x, pt.y, state);
	sprintf_s(data, "[%d,%d] %s", pt.x, pt.y, state);

	std::cout << data << std::endl;
	*/
	if (m1 || wParam != WM_MOUSEMOVE){
		if(m==1) return 1;	// 吃掉消息
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main(int argc, char* argv[])
{
	if (argc != 2 ) {
	printf("Usage: %s [m|k|a][m] m:鼠标 k:键盘 a:键盘鼠标\n", argv[0]);
		return 0;
	}
	if (argv[1][0] == 'm') m = 1;
	if (argv[1][0] == 'k') k = 1;
	if (argv[1][0] == 'a') m = 1, k = 1;
	if (argv[1][1] == 'm') m1 = 1;
	HINSTANCE hM = GetModuleHandle(NULL), hK = GetModuleHandle(NULL);
	HHOOK g_Hook = NULL, g_Hook2=NULL;
	if (k == 1) g_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hK, 0);	//WH_KEYBOARD_LL 13
	if (m == 1) g_Hook2 = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hM, 0);	//WH_MOUSE_LL 14

	// 消息循环是必须的，Windows直接在你自己的进程中调用你的hook回调.要做这个工作,
	//需要一个消息循环.没有其他机制可以让您的主线程进行回调,
	//回调只能在您调用Get / PeekMessage()以使Windows可以控制时发生.

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (k == 1) UnhookWindowsHookEx(g_Hook);
	if (m == 1) UnhookWindowsHookEx(g_Hook2);

	return 0;
}
