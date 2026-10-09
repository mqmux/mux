//#include <stdio.h>
#include <windows.h>
#include <WinUser.h>
//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" ) // 设置入口地址，为了隐藏控制台黑框，调试时黑框不会消失，运行exe文件时才能隐藏黑框


//遍历全部子窗口的子窗口 , Z序遍历
HWND findChildWindowHandleByName(HWND parent, int level, TCHAR* winNameBuf)
{
	HWND child = NULL;
	HWND hWnd = NULL;
	TCHAR buf[MAX_PATH];
	DWORD pid = 0, tid = 0;
	do {
		child = FindWindowEx(parent, child, NULL, NULL);
		int ret = GetWindowText(child, buf, MAX_PATH);
		buf[ret] = 0;
		tid = GetWindowThreadProcessId(child, &pid);
		for (int i = 0; i < level; ++i)
			//printf("\t");
		//printf("%s ,  pid:%d, tid:%d\n", buf, pid, tid);
		if (memcmp(winNameBuf, buf, sizeof(winNameBuf)) == 0)
		{
			hWnd = child;
			return hWnd;
		}
		if (child)
			hWnd = findChildWindowHandleByName(child, level + 1, winNameBuf);
		if (hWnd != NULL)
		{
			return hWnd;
		}
	} while (child);
	return NULL;
}

//遍历全部 explore 下的窗口 , Z序遍历
HWND findWindowHandleByName(TCHAR* winNameBuf)
{
	HWND child = NULL;
	HWND hWnd = NULL;
	TCHAR buf[MAX_PATH];
	DWORD pid = 0, tid = 0;

	do {
		//查找 Explore 下的一个窗口,若是能找到则根据 Explore 下的child 继续找
		child = FindWindowEx(NULL, child, NULL, NULL);
		int ret = GetWindowText(child, buf, MAX_PATH);
		buf[ret] = 0;
		tid = GetWindowThreadProcessId(child, &pid);
		//printf("%s ,  pid:%d, tid:%d\n", buf, pid, tid);
		if (memcmp(winNameBuf, buf, sizeof(winNameBuf)) == 0)
		{
			hWnd = child;
			return hWnd;
		}
		//遍历子窗口们
		if (child)
			hWnd = findChildWindowHandleByName(child, 1, winNameBuf);
		if (hWnd != NULL)
		{
			return hWnd;
		}
	} while (child);
	return NULL;
}

int main(int argc, char* argv[])
{
	HWND hWnd = NULL;
	HWND hWnd2 = NULL;
	TCHAR desktopNameBuf[] = { L"FolderView" };		// 桌面句柄名

	// 遍历桌面句柄
	hWnd = findWindowHandleByName(desktopNameBuf);
	if (hWnd != NULL)
	{
		if (argc > 1) return (int)IsWindowVisible(hWnd);
		// 切换显示与隐藏桌面
		if (IsWindowVisible(hWnd))
		{
			ShowWindow(hWnd, SW_HIDE);
			return 2;
		}
		else
		{
			ShowWindow(hWnd, SW_SHOW);
			return 3;
		}

		// 模拟按键 Win+D
		//Sleep(100);
		//keybd_event(VK_LWIN, 0, 0, 0);
		//keybd_event('D', 0, 0, 0);
		//Sleep(100);
		//keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
		//keybd_event('D', 0, KEYEVENTF_KEYUP, 0);
	}

	// 根据任务栏类名寻找句柄
	hWnd = FindWindowEx(NULL, NULL, L"Shell_TrayWnd", NULL);
	if (hWnd != NULL)
	{
		// 寻找是否有第二个屏幕的任务栏
		hWnd2 = FindWindowEx(NULL, NULL, L"Shell_SecondaryTrayWnd", NULL);
		if (hWnd2 != NULL)
		{
			if (argc > 1) return (int)IsWindowVisible(hWnd);
			// 切换显示与隐藏任务栏
			if (IsWindowVisible(hWnd))
			{
				ShowWindow(hWnd, SW_HIDE);
				ShowWindow(hWnd2, SW_HIDE);		// 第二屏幕任务栏需单独隐藏
				return 4;
			}
			else
			{
				ShowWindow(hWnd, SW_SHOW);		// 显示第一屏幕任务栏后，第二屏幕任务栏会自动显示
				return 5;
			}
		}
	}

	return 404;
}

