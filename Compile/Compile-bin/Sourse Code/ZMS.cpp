#include <windows.h>
#include <stdio.h>

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowExW(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		// 找它的下一个窗口，类名为WorkerW，隐藏它
		HWND hWorkerw = FindWindowExW(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);
		return FALSE;
	}
	return TRUE;
}

/* int main(int argc, char *argv[])
{
	// 视频路径、1920和1080，要根据实际情况改。建议使用GetSystemMetrics函数获取分辨率属性
	LPCWSTR lpParameter = L" .\\video.mp4  -noborder -x 1920 -y 1080 -loop 0";
	STARTUPINFOW si{ 0 };
	PROCESS_INFORMATION pi{ 0 };

	// 下面是我电脑上ffplay的路径，要根据实际情况改
	if (CreateProcessW(L".\\ffmpeg-n5.1-latest-win64-lgpl-5.1\\bin\\ffplay.exe", (LPWSTR)lpParameter, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		Sleep(2500);												// 等待视频播放器启动完成。可用循环获取窗口尺寸来代替Sleep()

		HWND hProgman = FindWindowW(L"Progman", 0);				// 找到PM窗口
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// 给它发特殊消息
		HWND hFfplay = FindWindowW(L"SDL_app", 0);				// 找到视频窗口
		SetParent(hFfplay, hProgman);							// 将视频窗口设置为PM的子窗口
		EnumWindows(EnumWindowsProc, 0);						// 找到第二个WorkerW窗口并隐藏它
	}

	return 0;
} */

int Char2Wchar(wchar_t* wcharStr, const char* charStr) {
	int len = MultiByteToWideChar(CP_ACP, 0, charStr, strlen(charStr), NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, charStr, strlen(charStr), wcharStr, len);
	wcharStr[len] = '\0';
	return len;
}

int main(int argc, char* argv[])
{
	if (argc != 2 && argc != 3) {
		printf("Usage: %s [hwnd]\n", argv[0]);
		printf("   or: %s [win_class] [win_title]\n", argv[0]);
		return 0;
	}
	HWND hProgman = FindWindowW(L"Progman", 0);				// 找到PM窗口
	SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// 给它发特殊消息
	// 找到窗口
	HWND hFfplay = NULL;
	if (isdigit(argv[1][0]) != 0) {
		int bit = 10;
		if (argv[1][0] == '0' && argv[1][1] == 'x' || argv[1][1] == 'X') bit = 16;
		char* stop;
		int ans = strtol(argv[1], &stop, bit);
		hFfplay = (HWND)ans;
	}
	else if (isalpha(argv[1][0]) != 0) {
		wchar_t arg1[100];
		wchar_t arg2[100];
		Char2Wchar(arg1, argv[1]);
		if (argc == 2) {
			hFfplay = FindWindowW(arg1, 0);
		}
		else {
			Char2Wchar(arg2, argv[2]);
			if (argv[1][0] == 'n' && argv[1][1] == 'u' && argv[1][2] == 'l' && argv[1][3] == 'l') {
				hFfplay = FindWindowW(0, arg2);
			}
			else hFfplay = FindWindowW(arg1, arg2);
		}
	}
	printf("%#X\n", hFfplay);
	SetWindowLong(hFfplay, GWL_STYLE, GetWindowLong(hFfplay, GWL_STYLE) & ~(WS_CAPTION | WS_SIZEBOX));//去掉窗口边框
	SetParent(hFfplay, hProgman);							// 将视频窗口设置为PM的子窗口
	EnumWindows(EnumWindowsProc, 0);						// 找到第二个WorkerW窗口并隐藏它
	return 0;
}
