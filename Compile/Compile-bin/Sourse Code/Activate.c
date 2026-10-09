//#include <iostream>
#include <windows.h>

// SwitchToThisWindow 方法
void ActiveAnyWindow_SwitchToThisWindow(HWND hWnd)
{
    // 第二个参数传TRUE：如果窗口被极小化了，则恢复
    SwitchToThisWindow(hWnd, TRUE);
}

// SetForegroundWindow 方法
void ActiveAnyWindow_SetForegroundWindow(HWND hWnd)
{
    const HWND hForeWnd = ::GetForegroundWindow();
    ::AttachThreadInput(::GetWindowThreadProcessId(hForeWnd, nullptr), ::GetCurrentThreadId(), TRUE);
    if (!::SetForegroundWindow(hWnd))
    {
        //std::cout << "SetForegroundWindow GLE=%d", ::GetLastError();
    }
    else
    {
        //std::cout << "SetForegroundWindow OK";
    }
    AttachThreadInput(::GetWindowThreadProcessId(hForeWnd, nullptr), ::GetCurrentThreadId(), FALSE);
}

// BringWindowToTop 方法
void ActiveAnyWindow_BringWindowToTop(HWND hWnd)
{
    const HWND hForeWnd = ::GetForegroundWindow();
    ::AttachThreadInput(::GetWindowThreadProcessId(hForeWnd, nullptr), ::GetCurrentThreadId(), TRUE);
    if (!::BringWindowToTop(hWnd))
    {
        //std::cout << "BringWindowToTop GLE=%d", ::GetLastError();
    }
    else
    {
        //std::cout << "BringWindowToTop OK";
    }
    AttachThreadInput(::GetWindowThreadProcessId(hForeWnd, nullptr), ::GetCurrentThreadId(), FALSE);
}

int main()
{
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, 1);
    ActiveAnyWindow_BringWindowToTop(hWnd);
    SetWindowTextA(hWnd, "");
}
