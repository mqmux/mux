#include <Windows.h>

int main(int argc, char* argv[]) {
	// 控制台输入输出句柄
	HANDLE inputHandle, outputHandle;
	// 控制台模式
	DWORD  inConsoleMode, outConsoleMode;

	inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleMode(inputHandle, &inConsoleMode);
	// 获取输出的控制台模式
	GetConsoleMode(outputHandle, &outConsoleMode);

	if (argc > 1) {
		// 禁用快速编辑模式
		inConsoleMode &= ~ENABLE_QUICK_EDIT_MODE;
		// 应用
		SetConsoleMode(inputHandle, inConsoleMode);
	}
	else {
		// 启用快速编辑模式
		inConsoleMode |= ENABLE_QUICK_EDIT_MODE;
		// 应用
		SetConsoleMode(inputHandle, inConsoleMode);
	}
	return 0;
}

/*
  此程序将会演示如何使用 SetCurrentConsoleFontEx 函数更改控制台字体为“楷体”。
  有一点需要注意，更改的字体必须是等宽字体，不是等宽字体的修改后都会变成默认字体。
  g++ t.cpp -finput-charset=GBK
*/

/* HWND hwnd=FindWindow("H-SMILE-FRAME",0);
	ShowWindow(hwnd,SW_HIDE);
	hwnd=FindWindow("H-SMILE-FRAME","ToDesk");
	ShowWindow(hwnd,SW_HIDE); */
	//任务管理器里面显示的不一定是程序的真实标题
	//推荐使用SPY++或者Process Monitor查看窗口的标题
	/*SW_FORCEMINIMIZE：在WindowNT5.0中最小化窗口，即使拥有窗口的线程被挂起也会最小化。在从其他线程最小化窗口时才使用这个参数。nCmdShow=11。
	SW_HIDE：隐藏窗口并激活其他窗口。nCmdShow=0。
	SW_MAXIMIZE：最大化指定的窗口。nCmdShow=3。
	SW_MINIMIZE：最小化指定的窗口并且激活在Z序中的下一个顶层窗口。nCmdShow=6。
	SW_RESTORE：激活并显示窗口。如果窗口最小化或最大化，则系统将窗口恢复到原来的尺寸和位置。在恢复最小化窗口时，应用程序应该指定这个标志。nCmdShow=9。
	SW_SHOW：在窗口原来的位置以原来的尺寸激活和显示窗口。nCmdShow=5。
	SW_SHOWDEFAULT：依据在STARTUPINFO结构中指定的SW_FLAG标志设定显示状态，STARTUPINFO 结构是由启动应用程序的程序传递给CreateProcess函数的。nCmdShow=10。
	SW_SHOWMAXIMIZED：激活窗口并将其最大化。nCmdShow=3。
	SW_SHOWMINIMIZED：激活窗口并将其最小化。nCmdShow=2。
	SW_SHOWMINNOACTIVE：窗口最小化，激活窗口仍然维持激活状态。nCmdShow=7。
	SW_SHOWNA：以窗口原来的状态显示窗口。激活窗口仍然维持激活状态。nCmdShow=8。
	SW_SHOWNOACTIVATE：以窗口最近一次的大小和状态显示窗口。激活窗口仍然维持激活状态。nCmdShow=4。
	SW_SHOWNORMAL：激活并显示一个窗口。如果窗口被最小化或最大化，系统将其恢复到原来的尺寸和大小。应用程序在第一次显示窗口的时候应该指定此标志。nCmdShow=1。
	*/