#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <Commctrl.h>
#pragma comment(lib, "Comctl32.lib")
HFONT hFont;
HWND hwnd;
int screenWidth, screenHeight;
int digitWidth = 50;
int digitHeight = 80;
int colonWidth = 20;

bool isDragging = false;
bool istop = false;
int mx, my;

int trans1 = 180;
int trans2 = 100;


std::wstring timeStr;
std::wstring formatNumber(int num);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void SaveWindowPosition(HWND hwnd) {
    // 获取窗口位置和大小信息
    RECT rc;
    GetWindowRect(hwnd, &rc);
    int x = rc.left, y = rc.top, w = rc.right - rc.left, h = rc.bottom - rc.top;

    // 打开注册表项
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\MyApp", 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        // 保存窗口位置和大小信息
        RegSetValueEx(hKey, L"WindowX", 0, REG_DWORD, (LPBYTE)&x, sizeof(DWORD));
        RegSetValueEx(hKey, L"WindowY", 0, REG_DWORD, (LPBYTE)&y, sizeof(DWORD));
        RegSetValueEx(hKey, L"WindowWidth", 0, REG_DWORD, (LPBYTE)&w, sizeof(DWORD));
        RegSetValueEx(hKey, L"WindowHeight", 0, REG_DWORD, (LPBYTE)&h, sizeof(DWORD));

        // 关闭注册表项
        RegCloseKey(hKey);
    }
}

void RestoreWindowPosition(HWND hwnd) {
    // 打开注册表项
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\MyApp", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        // 读取窗口位置和大小信息
        DWORD x, y, w, h, type, size;
        size = sizeof(DWORD);
        if (RegQueryValueEx(hKey, L"WindowX", NULL, &type, (LPBYTE)&x, &size) == ERROR_SUCCESS) {
            size = sizeof(DWORD);
            if (RegQueryValueEx(hKey, L"WindowY", NULL, &type, (LPBYTE)&y, &size) == ERROR_SUCCESS) {
                size = sizeof(DWORD);
                if (RegQueryValueEx(hKey, L"WindowWidth", NULL, &type, (LPBYTE)&w, &size) == ERROR_SUCCESS) {
                    size = sizeof(DWORD);
                    if (RegQueryValueEx(hKey, L"WindowHeight", NULL, &type, (LPBYTE)&h, &size) == ERROR_SUCCESS) {
                        // 设置窗口位置和大小
                        SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
                    }
                }
            }
        }

        // 关闭注册表项
        RegCloseKey(hKey);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    char bmp[8][255];
    int b1 = 0;
    int b = 0;
    for (int i = 0; i < strlen(lpCmdLine); i++) {
        if (lpCmdLine[i] == '"') {
            i++;
            if (i >= strlen(lpCmdLine)) break;
            while (lpCmdLine[i] != '"') {
                bmp[b1][b] = lpCmdLine[i];
                //printf("bmp[%d][%d]=[%d]:%c\n",b1,b,i,lpCmdLine[i]);
                i++;
                b++;
            }
            bmp[b1][b] = '\0';
            i++;
            if (i >= strlen(lpCmdLine)) break;
        }
        if (lpCmdLine[i] == ' ') {
            bmp[b1][b] = '\0';
            i++;
            if (i >= strlen(lpCmdLine)) break;
            while (lpCmdLine[i] == ' ') {
                i++;
            }
            b = 0;
            b1++;
        }
        if (lpCmdLine[i] == '"') {
            i++;
            if (i >= strlen(lpCmdLine)) break;
            while (lpCmdLine[i] != '"') {
                bmp[b1][b] = lpCmdLine[i];
                //printf("bmp[%d][%d]=[%d]:%c\n",b1,b,i,lpCmdLine[i]);
                i++;
                b++;
            }
            bmp[b1][b] = '\0';
            i++;
            if (i >= strlen(lpCmdLine)) break;
        }
        bmp[b1][b] = lpCmdLine[i];
        //printf("bmp[%d][%d]=[%d]:%c\n",b1,b,i,lpCmdLine[i]);
        b++;
    }
    bmp[b1][b] = '\0';
    /* for(int i=0;i<=b1;i++){
    printf("bmp[%d]:[%s]\n",i,bmp[i]);
    }
    printf("b1:[%d]\n",b1); */

    if (lpCmdLine[0] != '\0') trans1 = atoi(bmp[0]);
    if (b1 > 0) trans2 = atoi(bmp[1]);
    // 获取屏幕宽度和高度
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    InitCommonControls(); // 添加这行代码
    // 设置字体
    hFont = CreateFont(digitHeight, 0, 0, 0, FW_BOLD, FALSE,
        FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("微软雅黑"));

    // 创建窗口
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // 设置背景为白色
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"DigitalClock";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"窗口注册失败！", L"Error", MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOOLWINDOW, // 设置窗口样式，使之置顶、透明 | WS_EX_TOOLWINDOW | WS_EX_APPWINDOW
        L"DigitalClock",
        L"Digital Clock",
        WS_POPUP, // 无边框窗口
        (screenWidth - digitWidth * 5 - colonWidth) / 2,
        (screenHeight - digitHeight) / 2, // 计算窗口位置
        digitWidth * 5 + colonWidth,
        digitHeight,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hwnd) {
        MessageBox(NULL, L"创建窗口失败！", L"Error", MB_ICONERROR);
        return 0;
    }

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), trans1, LWA_COLORKEY | LWA_ALPHA); // 设置窗口透明
    //SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    SetTimer(hwnd, 1, 1000, NULL); // 设置定时器

    MSG msg;

    // 添加以下消息循环代码：

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static POINT cursorOffset; // 鼠标点击位置相对于窗口左上角的偏移量
    switch (msg)
    {
    case WM_CREATE:
        RestoreWindowPosition(hwnd);
        break;
    case WM_CLOSE:
        //SaveWindowPosition(hwnd);
        DestroyWindow(hwnd);
        break;
    case WM_PAINT:
    {
        RECT rect;
        GetClientRect(hwnd, &rect);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SelectObject(hdc, hFont);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        DrawText(hdc, timeStr.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_NCHITTEST:
    {
        UINT nHitTest;
        nHitTest = ::DefWindowProc(hwnd, msg, wParam, lParam);
        //如果鼠标左键按下, GetAsyncKeyState 的返回值小于0

        if (nHitTest == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
        {
            SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), trans2, LWA_COLORKEY | LWA_ALPHA); // 设置窗口透明
            nHitTest = HTCAPTION;
        }
        else {
            SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), trans1, LWA_COLORKEY | LWA_ALPHA); // 设置窗口透明
        }
        return nHitTest;
    }
    case WM_RBUTTONDOWN:
        if (istop) {
            SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            istop = false;
        }
        else {
            SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            istop = true;
        }
    case WM_TIMER:
    {
        // 获取当前时间
        time_t currentTime = time(NULL);
        tm timeStruct;
        localtime_s(&timeStruct, &currentTime);
        std::wstring hourStr = formatNumber(timeStruct.tm_hour);
        std::wstring minuteStr = formatNumber(timeStruct.tm_min);
        std::wstring secondStr = formatNumber(timeStruct.tm_sec);

        // 拼接时间字符串
        timeStr = hourStr + L":" + minuteStr + L":" + secondStr;

        InvalidateRect(hwnd, NULL, true); // 刷新窗口
        break;
    }

    case WM_DESTROY:
        SaveWindowPosition(hwnd);
        DeleteObject(hFont);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

std::wstring formatNumber(int num) {
    std::wstring str = std::to_wstring(num);
    if (str.length() == 1)
        str = L"0" + str;
    return str;
}
