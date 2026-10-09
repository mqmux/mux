#include <stdio.h>
#include <Windows.h>
//#define HELP_INFO  L"About: Registered hotkey
#define HELP_INFO  "\n\
Usage: Hotkey.exe  [command]\n\
       Hotkey.exe  [Hwnd]\n\
       Hotkey.exe [/h] hide commandline \n\
       Hotkey.exe [/w] commandline \n\
       Hotkey.exe  /w  C:\\PHP\\php-cgi.exe -b 127.0.0.1:9123 \n\
       Hotkey.exe  /w  python  d:\\test.py \n\
	   \n\
[ALT+C] to console windows of started program.\n\
[ALT+V] to show the interface of Hotkey.exe\n\
the /w is optional which  means waiting for \n\
termination of the program launched by the commandline\n"

struct ProcessWindow
{
	DWORD dwProcessId;
	HWND hwndWindow;
};

BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{
	ProcessWindow* pProcessWindow = (ProcessWindow*)lParam;

	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	// 判断是否是指定进程的主窗口
	if (pProcessWindow->dwProcessId == dwProcessId && IsWindowVisible(hWnd) && GetParent(hWnd) == NULL)
	{
		pProcessWindow->hwndWindow = hWnd;

		return FALSE;
	}

	return TRUE;
}

int main(int argc, char* argv[])
{
	HWND hWnd = GetConsoleWindow();

	BOOL bool_hide = true;
	int bWait = 0;
	DWORD exitcode = 0;
	WCHAR stopchar = L' ';
	WCHAR* lpszCmd = GetCommandLineW();


	if (lpszCmd[0] == L'"')
		stopchar = L'"';
	do {
		lpszCmd++;
	} while ((lpszCmd[0] != stopchar) && (lpszCmd[0] != 0));

	if (lpszCmd[0] != 0)
	{
		do {
			lpszCmd++;
		} while ((lpszCmd[0] != 0) && ((lpszCmd[0] == L' ')));
	};
	if (lpszCmd[0] == 0)
	{
		printf(HELP_INFO);
		//MessageBoxW(0, HELP_INFO, L"Incorrect usage", 0);
		ExitProcess(0);
	};

	if ((lpszCmd[0] == L'/') && (lpszCmd[1] == L'w' || lpszCmd[1] == L'W') && (lpszCmd[2] == L' '))
	{
		bWait = 1;
		lpszCmd += 3;
	};
	//skip the space
	while ((lpszCmd[0] != 0) && (lpszCmd[0] == ' '))
		lpszCmd++;
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	if ((lpszCmd[0] == L'/') && (lpszCmd[1] == L'h' || lpszCmd[1] == L'H') && (lpszCmd[2] == L' '))
	{
		lpszCmd += 3;
		si.wShowWindow = SW_HIDE;
	}
	else si.wShowWindow = SW_SHOW;

	// 注册两个热键 Ctrl+F1 , Ctrl+F2
	if (argv[1][0] == '0' && argv[1][1] == 'x' || argv[1][1] == 'X') {
		char* stop;
		int ans = strtol(argv[1], &stop, 16);
		hWnd = (HWND)ans;
	}
	else {
		if (0 == RegisterHotKey(
			NULL,
			1,
			MOD_ALT | MOD_NOREPEAT,
			0x43))  // is 'C'
		{
			printf("RegisterHotKey error : %d", GetLastError()); fflush(stdout);
			return GetLastError();
		}
	}



	if (0 == RegisterHotKey(
		NULL,
		2,
		MOD_ALT | MOD_NOREPEAT,
		0x56))  // is 'V'
	{
		printf("RegisterHotKey error : %d", GetLastError()); fflush(stdout);
		return GetLastError();
	}

	// 消息循环
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0)) {
		switch (msg.message) {
		case WM_HOTKEY:
		{
			if (1 == msg.wParam) {
				if (CreateProcessW(NULL, lpszCmd,
					NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
				{
					ProcessWindow procwin;
					procwin.dwProcessId = pi.dwProcessId;
					procwin.hwndWindow = NULL;
					if (bWait)
						WaitForSingleObject(pi.hProcess, INFINITE);
					hWnd = 0;
					while (!hWnd) {
						WaitForSingleObject(pi.hProcess, 500);
						EnumWindows(EnumWindowCallBack, (LPARAM)&procwin);
						hWnd = procwin.hwndWindow;
					}
					printf("ALT+C : %#X - %d - %#X\n", pi.hProcess, pi.dwProcessId, hWnd); fflush(stdout);
					bool_hide = true;
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
				}
				else
					exitcode = GetLastError();
			}
			else if (2 == msg.wParam) {
				printf("ALT+V - %d\n", bool_hide); fflush(stdout);
				if (hWnd)
				{
					if (bool_hide) {
						ShowWindow(hWnd, SW_HIDE);
						bool_hide = FALSE;
					}
					else {
						ShowWindow(hWnd, SW_SHOW);
						bool_hide = TRUE;
					}
				}
			}
			break;
		}
		default:
			break;
		}
	}
	return 0;
}
