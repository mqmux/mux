#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{

	HWND hwnd;
	const int NUM = 500;

	/// 锁定指定窗口，禁止它更新
	/// GetDesktopWindow() 用于返回桌面窗口的句柄
	
	hwnd=FindWindow("ConsoleWindowClass",NULL);
	if(hwnd)
	{
		ShowWindow(hwnd,SW_HIDE);				//设置指定窗口的显示状态
	}
	
	if (LockWindowUpdate(hwnd = GetDesktopWindow()))
	{
		HDC hdcScr = GetDCEx(hwnd, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);
		///创建兼容DC
		HDC hdcMem = CreateCompatibleDC(hdcScr);
		
		/// 获取系统当前的分辨率，宽像素 和 高像素
		int cxClient = GetSystemMetrics(SM_CXSCREEN) / 10;
		int cyClient = GetSystemMetrics(SM_CYSCREEN) / 10;

		/// 创建兼容位图
		HBITMAP hBitmap = CreateCompatibleBitmap(hdcScr, cxClient, cyClient);

		SelectObject(hdcMem, hBitmap);

		srand((int)GetCurrentTime());

		int iKeep[NUM][4];
		int	x1, x2, y1, y2;

		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < NUM; ++j)
			{
				if (0 == i)
				{
					iKeep[j][0] = x1 = cxClient * (rand() % 10);
					iKeep[j][1] = y1 = cyClient * (rand() % 10);
					iKeep[j][2] = x2 = cxClient * (rand() % 10);
					iKeep[j][3] = y2 = cyClient * (rand() % 10);
				}
				else
				{
					x1 = iKeep[NUM - 1 - j][0];
                    y1 = iKeep[NUM - 1 - j][1];
                    x2 = iKeep[NUM - 1 - j][2];
                    y2 = iKeep[NUM - 1 - j][3];
				}

				/// 对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境
				BitBlt(hdcMem,  0,  0, cxClient, cyClient, hdcScr, x1, y1, SRCCOPY);
				BitBlt(hdcScr, x1, y1, cxClient, cyClient, hdcScr, x2, y2, SRCCOPY);
				BitBlt(hdcScr, x2, y2, cxClient, cyClient, hdcMem,  0,  0, SRCCOPY);

				Sleep(10);
			}
		}

		DeleteDC(hdcMem);
        ReleaseDC(hwnd, hdcScr);
        DeleteObject(hBitmap);
             
        LockWindowUpdate(NULL);
	}

	return FALSE;
}

/*


#include <windows.h>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址

#define NUM 32767
 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static int iKeep[NUM][4];
	HDC        hdcScr, hdcMem;
	int        cx, cy;
	HBITMAP    hBitmap;
	HWND       hwnd;
	int        i, j, x1, y1, x2, y2;
	
	hwnd=FindWindow("ConsoleWindowClass",NULL);
	if(hwnd)
	{
		ShowWindow(hwnd,SW_HIDE);				//设置指定窗口的显示状态
	}
	
	if (LockWindowUpdate(hwnd = GetDesktopWindow()))
	{
		hdcScr = GetDCEx(hwnd, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);
		hdcMem = CreateCompatibleDC(hdcScr);
		cx = GetSystemMetrics(SM_CXSCREEN) / 10;
		cy = GetSystemMetrics(SM_CYSCREEN) / 10;
		hBitmap = CreateCompatibleBitmap(hdcScr, cx, cy);
 
		SelectObject(hdcMem, hBitmap);
 
		srand((int)GetCurrentTime());
 
		for (i = 0; i < 2; i++)
		for (j = 0; j < NUM; j++)
		{
			LockWindowUpdate(hwnd = GetDesktopWindow());
			if (i == 0)
			{
				iKeep[j][0] = x1 = cx * (rand() % 10);
				iKeep[j][1] = y1 = cy * (rand() % 10);
				iKeep[j][2] = x2 = cx * (rand() % 10);
				iKeep[j][3] = y2 = cy * (rand() % 10);
			}
			else
			{
				x1 = iKeep[NUM - 1 - j][0];
				y1 = iKeep[NUM - 1 - j][1];
				x2 = iKeep[NUM - 1 - j][2];
				y2 = iKeep[NUM - 1 - j][3];
			}
			BitBlt(hdcMem, 0, 0, cx, cy, hdcScr, x1, y1, SRCCOPY);
			BitBlt(hdcScr, x1, y1, cx, cy, hdcScr, x2, y2, SRCCOPY);
			BitBlt(hdcScr, x2, y2, cx, cy, hdcMem, 0, 0, SRCCOPY);
		}
 
		DeleteDC(hdcMem);
		ReleaseDC(hwnd, hdcScr);
		DeleteObject(hBitmap);
	}
	return 0;
}



#include <iostream>
#include <Windows.h>
void fuction(char *str){
	const char* path;
	path = str;
	SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)path, SPIF_SENDCHANGE);
}

int main(int argc,char *argv[])
{
	system("mode con: cols=50 lines=4");
	char* str = new(char);
	
	if(argc>1) {
		HWND hwnd;
		
		hwnd=FindWindow("ConsoleWindowClass",NULL);
		if(hwnd)
		{
			ShowWindow(hwnd,SW_HIDE);				//设置指定窗口的显示状态
		}
		
		str=argv[1];
		fuction(str);
		
	}else{
		while(1){
			std::cout<<"输入/拖入图片路径:"<<std::endl;
			std::cin>>str;
			fuction(str);
			system("cls");
		}
	}
	
	return 0;
}


*/