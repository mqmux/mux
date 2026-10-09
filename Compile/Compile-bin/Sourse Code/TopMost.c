#include <windows.h>
#include<stdio.h>
int main(int argc, char* argv[])
{
	//HWND hWnd = ::GetForegroundWindow();
	HWND hWnd = GetConsoleWindow();
	if (argc < 2) {
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
		return 0;
	}
	if (!strcmp("-h", argv[1])) {
		printf("Usage: %s 置顶本窗口\n", argv[0]);
		printf("   or: %s [-] 打印本窗口句柄\n", argv[0]);
		printf("   or: %s [-n] 取消本窗口置顶\n", argv[0]);
		printf("   or: %s [-a] [hwnd] 取消置顶指定窗口句柄 \n", argv[0]);
		printf("   or: %s [-a] [hwnd] [time] time=1无间隔置顶 0一次置顶 \n", argv[0]);
		printf("   or: %s [-w] [hwnd] 去掉窗口边框\n", argv[0]);
		printf("   or: %s [-h] 帮助信息 \n", argv[0]);
		return 0;
	}
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!strcmp("-", argv[1])) { printf("%ld,%#lx", hWnd, hWnd); return 0; }
	if (!strcmp("-n", argv[1])) {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
		return 0;
	}
	char* stop = new char[20];
	HWND hwnd = NULL;
	if (argv[2][1] == 'X' || argv[2][1] == 'x')
		hwnd = (HWND)strtol(argv[2], &stop, 16);
	else
		hwnd = (HWND)atoi(argv[2]);
	if (hwnd == 0) hwnd = hWnd;
	printf("%#X", hwnd);
	if (!strcmp("-a", argv[1])) {
		if (argc == 4) {
			int time = 500;
			time = atoi(argv[3]);
			while (time) {
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
				if (time != 1) Sleep(time);
			}
			return 0;
		}
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
		return 0;
	}
	if (!strcmp("-w", argv[1])) {
		SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~(WS_CAPTION | WS_SIZEBOX));//去掉窗口边框
		return 0;
	}
	return 0;
}

/* void TopWindow(HWND &hWnd) {
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
int main() {
	HWND hWnd=GetForegroundWindow();
	while(1){
		TopWindow(hWnd);
		Sleep(100);
	}
}

//将当前线程附到新的置顶线程上，再置顶
#include <windows.h>

// windows置顶窗体终极方案

BOOL OnForceShow(HWND hWnd)
{
	HWND hForeWnd = NULL;
	DWORD dwForeID = 0;
	DWORD dwCurID = 0;

	hForeWnd = ::GetForegroundWindow();
	dwCurID = ::GetCurrentThreadId();
	dwForeID = ::GetWindowThreadProcessId(hForeWnd, NULL);
	::AttachThreadInput(dwCurID, dwForeID, TRUE);
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetForegroundWindow(hWnd);
	// 将前台窗口线程贴附到当前线程（也就是程序A中的调用线程）
	::AttachThreadInput(dwCurID, dwForeID, FALSE);

	return TRUE;
}


int main(int argc, char *argv[])
{

	HWND hWnd = ::GetForegroundWindow();
	if (OnForceShow(hWnd))
		return TRUE;

	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
	return FALSE;
}


*/