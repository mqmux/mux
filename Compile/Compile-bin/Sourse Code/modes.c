#include <windows.h>
#include <stdio.h>
int main(int argc, char* argv[])
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hOut, &info); //获取当前缓冲区大小
	if (argc < 2) {
		printf("Usage: %s [Width][Height] [buffer_Width][buffer_Height] 设置控制台宽高,缓冲区大小,传一个参数返回缓冲区宽度\n", argv[0]);
		return 0;
	}
	if (argc == 2) return info.dwSize.X;
	// 获取标准输出设备句柄
	int S_X = atoi(argv[1]);
	int S_Y = atoi(argv[2]);
	SMALL_RECT rc = {0,0,S_X,S_Y};
	// 获取窗口缓冲区信息
	
	int B_X = S_X+1;
	int B_Y = info.dwSize.Y;
	if (argc > 3) {
		B_X = atoi(argv[3]);
		if (argc > 4) B_Y = atoi(argv[4]); }
	if (B_X <= S_X) B_X = S_X+1;
	if (B_Y <= info.dwSize.Y) B_Y = info.dwSize.Y+1;
	if ( S_X < 0 || S_Y < 0 ) {
		if (B_X < info.dwSize.X) B_X = info.dwSize.Y;
		if (B_Y < info.dwSize.Y) B_Y = info.dwSize.Y;
		COORD size = { B_X,B_Y };
		SetConsoleScreenBufferSize(hOut, size);
		return 0;
	}
	COORD size = { B_X,B_Y };
	//printf("%d,%d,%d,%d \n",info.dwSize.X,info.dwSize.Y,S_X,S_Y);
	SetConsoleScreenBufferSize(hOut, size); // 设置缓冲区大小
	SetConsoleWindowInfo(hOut, TRUE, &rc);
	SetConsoleScreenBufferSize(hOut, size); // 设置缓冲区大小
	CloseHandle(hOut);
	return 0;
}
