#include <windows.h>
#include<stdio.h>

int basetoint(char argv[]) {
	int bit = 10;
	if (argv[0] == '0' && argv[1] == 'x' || argv[1] == 'X') bit = 16;
	if (argv[0] == '0' && argv[1] != 'x' && argv[1] != 'X') bit = 8;
	char* stop;
	return strtol(argv, &stop, bit);//将八进制数1054转成十进制，后面均为非法字符 printf("%s\n", stop);
}

int main(int argc, char* argv[])
{
	RECT rect;
	HWND hWnd=NULL;
	//HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
	//HWND hWnd=GetForegroundWindow();
	if (argc > 1 && argc < 7) {
		if (argv[1][0] == '0' && argv[1][1] == '\0')
			hWnd = GetConsoleWindow();
		else {
			hWnd = (HWND)basetoint(argv[1]);
		}
		GetWindowRect(hWnd, &rect);
		int size_one = rect.left;
		int size_two = rect.top;
		int size_three = rect.right - rect.left;
		int size_four = rect.bottom - rect.top;
		if (argv[2] != NULL) {
			size_one = basetoint(argv[2]);
			if (argv[3] != NULL) {
				size_two = basetoint(argv[3]);
				if (argv[4] != NULL) {
					size_three = basetoint(argv[4]);
					if (argv[5] != NULL) size_four = basetoint(argv[5]);
				}
			}
		}
		else {
			printf("%d %d %d %d", rect.left, rect.top, rect.right, rect.bottom);
		}
		MoveWindow(hWnd, size_one, size_two, size_three, size_four, TRUE);
		//SetWindowPos(hWnd, NULL, size_one, size_two, 0, 0, SWP_NOSIZE);
		//[句柄,左上角横坐标(X),左上角竖坐标(Y),窗口宽度(width),窗口高度(height),值为真]
	}
	else {
		hWnd = GetConsoleWindow();
		GetWindowRect(hWnd, &rect);
		printf("%d %d %d %d", rect.left, rect.top, rect.right, rect.bottom);
	}
	int Width = GetSystemMetrics(SM_CXSCREEN);
	int Heigth = GetSystemMetrics(SM_CYSCREEN);
	int mouse_pos = Width;
	mouse_pos = mouse_pos << 16 | Heigth;
	return mouse_pos;
}

