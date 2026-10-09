#include <Windows.h>

// 窗口类名
//const wchar_t* JYBroadcastWindowClassName = L"JYBroadcastWindowClass";

// 窗口名称
//const wchar_t* JYBroadcastWindowTitle = L"教室";

// 消息处理回调函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        // 如果是创建窗口消息，尝试找到窗口，并将其窗口化
    {
        HWND jyBroadcastWnd = FindWindow("win_bmp", NULL);

        if (jyBroadcastWnd != NULL) {
            SetParent(jyBroadcastWnd, hwnd); // 将窗口设置为当前窗口的子窗口
            SetWindowText(jyBroadcastWnd, "IO"); // 设置窗口的标题栏
        }

        // 禁止窗口大小调整和关闭按钮
        SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_SYSMENU));
    }
    break;
    case WM_CLOSE:
        PostQuitMessage(0); // 发送退出程序的消息
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam); // 其他消息交给默认的窗口消息处理函数处理
    }

    return 0;
}

// 程序入口函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 注册窗口类
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "JYPlayerWindowClass";
    RegisterClassEx(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowEx(0, "JYPlayerWindowClass", "JY Player", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "窗口创建失败。", "错误", MB_ICONERROR);
        return 0;
    }

    // 显示窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 消息循环
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
