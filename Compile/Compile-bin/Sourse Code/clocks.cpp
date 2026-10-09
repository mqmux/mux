
//参数：透明度 点击后透明度 字体高 字体宽 RGB[255 255 255] 1:倒计时/文字

#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <Commctrl.h>
#pragma comment(lib, "Comctl32.lib")
HFONT hFont;
HWND hwnd;
int screenWidth, screenHeight;
int digitHeight = 30;//窗口高度
int digitWidth = 11;
int colonWidth = 130;//窗口宽度：digitWidth * 5 + colonWidth
int digitlen = 48;

bool isDragging = false;
bool istop = false;
int mx, my;
int rgb1 = 255, rgb2 = 255, rgb3 = 255;

int trans1 = 180;
int trans2 = 100;
int Countdown = 3;
char digit[1024];

time_t startTime = time(NULL);
std::wstring timeStr;
std::wstring formatNumber(int num);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::wstring charTowstring(char* str);
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
        RegSetValueEx(hKey, L"WindowHwnd", 0, REG_DWORD, (LPBYTE)&hwnd, sizeof(DWORD));
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
    char bmp[16][255];
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

    tm timeStart;
    localtime_s(&timeStart, &startTime);

    /* for(int i=0;i<=b1;i++){
    printf("bmp[%d]:[%s]\n",i,bmp[i]);
    }
    printf("b1:[%d]\n",b1); */

    if (lpCmdLine[0] != '\0') trans1 = atoi(bmp[0]);
    if (b1 > 0) trans2 = atoi(bmp[1]);
    if (b1 > 1) digitHeight = atoi(bmp[2]);
    if (b1 > 2) digitWidth = atoi(bmp[3]);
    if (b1 > 3) rgb1 = atoi(bmp[4]);
    if (b1 > 4) rgb2 = atoi(bmp[5]);
    if (b1 > 5) rgb3 = atoi(bmp[6]);
    if (b1 > 6) {
        if (bmp[7][0] == '1' && bmp[7][1] == '\0') {
            digitlen = 8;
            colonWidth = digitWidth;
            Countdown = 1;
        }
        else if (bmp[7][0] == '0' && bmp[7][1] == '\0') {
            digitlen = 8;
            colonWidth = digitWidth;
            Countdown = 0;
        }
        else {
            Countdown = 2;
            strcpy(digit, bmp[7]);
            if (b1 > 7) {
                for (int i = 8; i <= b1; i++) {
                    strcat(digit, " ");
                    strcat(digit, bmp[i]);
                }
            }
            digitlen = strlen(digit);
            if (digitlen < 70) colonWidth = 7*digitWidth; else colonWidth = (digitlen / 10) * digitWidth;
        }
    }
    // 获取屏幕宽度和高度
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    InitCommonControls(); // 添加这行代码
    // 设置字体
    hFont = CreateFont(
        digitHeight,
        digitWidth,
        0,
        0,
        FW_BOLD,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, //输出精度 OUT_OUTLINE_PRECIS
        CLIP_DEFAULT_PRECIS, //剪裁精度
        PROOF_QUALITY, //PROOF_QUALITY,//DEFAULT_QUALITY、DRAFT_QUALITY、PROOF_QUALITY 或 NONANTIALIASED_QUALITY
        VARIABLE_PITCH,
        TEXT("微软雅黑")
    );
    /*HFONT CreateFont(
        int cHeight, //字体的逻辑高度
        int cWidth, //字体的逻辑宽度
        int cEscapement, //指定移位向量相对X轴的偏转角度
        int cOrientation, //指定字符基线相对X轴的偏转角度
        int cWeight, //设置字体粗细程度
        DWORD bItalic, //是否启用斜体
        DWORD bUnderline, //是否启用下划线
        DWORD bStrikeOut, //是否启用删除线
        DWORD iCharSet, //指定字符集
        DWORD iOutPrecision, //输出精度
        DWORD iClipPrecision, //剪裁精度
        DWORD iQuality, //输出质量
        DWORD iPitchAndFamily, //字体族
        LPCSTR pszFaceName //字体名
    );*/

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
    //colonWidth = digitWidth ;
    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOOLWINDOW, // 设置窗口样式，使之置顶、透明 | WS_EX_TOOLWINDOW | WS_EX_APPWINDOW
        //WS_DLGFRAME | WS_THICKFRAME | WS_POPUP, 清除普通窗口的 WS_POPUP 样式，并设置 WS_CHILD 样式
        L"DigitalClock",
        L"Digital Clock",
        WS_POPUP, // 无边框窗口
        (screenWidth - digitWidth * digitlen - colonWidth) / 2,
        (screenHeight - digitHeight) / 2, // 计算窗口位置
        digitWidth * digitlen + colonWidth,
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
    return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static POINT cursorOffset; // 鼠标点击位置相对于窗口左上角的偏移量
    switch (msg)
    {
    case WM_ERASEBKGND:
        return true;
    case WM_CREATE:
        SaveWindowPosition(hwnd);
        //RestoreWindowPosition(hwnd);
        break;
    case WM_CLOSE:
        SaveWindowPosition(hwnd);
        DestroyWindow(hwnd);
        break;
    case WM_PAINT:
    {
        RECT rect;
        GetClientRect(hwnd, &rect);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 创建内存 DC 和位图
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
        SelectObject(memDC, memBitmap);

        // 填充背景
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); // 使用你的背景颜色
        FillRect(memDC, &rect, hBrush);
        DeleteObject(hBrush);

        // 在内存 DC 上进行绘制操作
        SelectObject(memDC, hFont);
        SetBkMode(memDC, TRANSPARENT);
        SetTextColor(memDC, RGB(rgb1, rgb2, rgb3));
        DrawText(memDC, timeStr.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // 将内存 DC 的内容复制到窗口 DC
        BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);

        // 删除内存 DC 和位图
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
        /*RECT rect;
        GetClientRect(hwnd, &rect);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SelectObject(hdc, hFont);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        DrawText(hdc, timeStr.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(hwnd, &ps);*/
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
    {
        if (istop) {
            SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            istop = false;
        }
        else {
            SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            istop = true;
        }
    }
    case WM_TIMER:
    {
        // 获取当前时间
        time_t currentTime = time(NULL);
        tm timeStruct;
        localtime_s(&timeStruct, &currentTime);

        std::wstring hourStr, minuteStr, secondStr;
        if (Countdown == 2) {
            timeStr = charTowstring(digit);
            KillTimer(hwnd, 1);
        }
        else if (Countdown == 0) {
            hourStr = formatNumber(timeStruct.tm_hour);
            minuteStr = formatNumber(timeStruct.tm_min);
            secondStr = formatNumber(timeStruct.tm_sec);// 拼接时间字符串
            timeStr = hourStr + L":" + minuteStr + L":" + secondStr;
        }
        else if (Countdown == 1) {
            double s = difftime(currentTime, startTime);
            int hh, mm, ss;
            hh = s / (60 * 60);          //一个小时有3600秒，除以3600之后就能得到“时”，int型抹去小数点后的数
            mm = (s - hh * (60 * 60)) / 60;  //先减去“时”占的秒数，再用之前的办法求“分”
            ss = s - hh * (60 * 60) - mm * 60; //直接减去“时”和“分”占的秒数，剩下的当然就是秒
            hourStr = formatNumber(hh);
            minuteStr = formatNumber(mm);
            secondStr = formatNumber(ss);
            // 拼接时间字符串
            timeStr = hourStr + L":" + minuteStr + L":" + secondStr;
        }
        else {
            timeStr = L"透明度 点击后透明度 字体高 字体宽 RGB[255 255 255] [0|1|...]";
            KillTimer(hwnd, 1);
        }
        InvalidateRect(hwnd, NULL, true); // 刷新窗口
        break;
    }
    case WM_DESTROY:
    {
        SaveWindowPosition(hwnd);
        DeleteObject(hFont);
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

std::wstring charTowstring(char* str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    if (len == 0)
        return std::wstring(L"");
    wchar_t* wct = new wchar_t[len];
    if (!wct)
        return std::wstring(L"");

    MultiByteToWideChar(CP_ACP, 0, str, -1, wct, len);
    std::wstring wstr(wct);
    delete[] wct;
    wct = NULL;
    return wstr;
}

std::wstring formatNumber(int num) {
    std::wstring str = std::to_wstring(num);
    if (str.length() == 1)
        str = L"0" + str;
    return str;
}
