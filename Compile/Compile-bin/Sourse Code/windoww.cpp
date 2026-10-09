// -lmingw32
#define WS_EX_LAYERED 0x00080000
#include <windows.h>
#include <Winbase.h>
#include <stdlib.h>
#include <stdio.h>

typedef BOOL(WINAPI* lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
lpfn g_pSetLayeredWindowAttributes;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 手动拖动相关变量
static BOOL g_bDragging = FALSE;
static POINT g_ptDragStart;

// 字符串转换辅助函数
void CharToWide(const char* src, wchar_t* dst, int maxLen) {
    MultiByteToWideChar(CP_ACP, 0, src, -1, dst, maxLen);
}

void SaveWindowPosition(HWND hwnd) {
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\MyApp", 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, L"WindowHwnd", 0, REG_DWORD, (LPBYTE)&hwnd, sizeof(DWORD));
        RegCloseKey(hKey);
    }
}

static HBITMAP hBitmap;
int WindowWidth = 1708, WindowHeight = 960;
static int cxSource, cySource;
BOOL Draw = false;
int topmost = 0;      // 0：不置顶，1：置顶
int trans = 255;
int trans1 = 180;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstanc,
    LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szAppName[] = TEXT("win_bmp");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    LONG lWindowLong;
    HMODULE hUser32;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("错误"), TEXT("wrong"), MB_ICONERROR);
        return 0;
    }

    // 解析命令行参数（改进版，支持引号和空格）
    char* argv[8] = { 0 };
    int argc = 0;
    char* cmd = lpCmdLine;
    BOOL inQuote = FALSE;
    char token[512] = { 0 };
    int tokenPos = 0;

    while (*cmd && argc < 8) {
        if (*cmd == '"') {
            inQuote = !inQuote;
            cmd++;
            continue;
        }
        if (!inQuote && *cmd == ' ') {
            if (tokenPos > 0) {
                token[tokenPos] = '\0';
                argv[argc] = _strdup(token);
                argc++;
                tokenPos = 0;
            }
            cmd++;
            continue;
        }
        token[tokenPos++] = *cmd;
        cmd++;
    }
    if (tokenPos > 0) {
        token[tokenPos] = '\0';
        argv[argc] = _strdup(token);
        argc++;
    }

    // 处理图片文件
    if (argc > 0 && argv[0] != NULL) {
        wchar_t wPath[MAX_PATH];
        CharToWide(argv[0], wPath, MAX_PATH);
        hBitmap = (HBITMAP)LoadImageW(NULL, wPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        if (hBitmap) {
            BITMAP bitmap;
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);
            cxSource = bitmap.bmWidth;
            cySource = bitmap.bmHeight;
            if (cxSource > 7680 || cxSource < 1 || cySource > 4320 || cySource < 1) {
                cxSource = 600;
                cySource = 300;
                WindowWidth = cxSource;
                WindowHeight = cySource;
                Draw = true;
            }
            else {
                Draw = false;
            }
        }
        else {
            // 加载失败，显示提示文字
            Draw = true;
            cxSource = 600;
            cySource = 300;
            WindowWidth = cxSource;
            WindowHeight = cySource;
        }
    }
    else {
        Draw = true;
        cxSource = 600;
        cySource = 300;
        WindowWidth = cxSource;
        WindowHeight = cySource;
    }

    if (Draw) trans = 200;
    if (argc > 1) trans = atoi(argv[1]);
    if (argc > 2) trans1 = atoi(argv[2]);
    if (argc > 3) topmost = atoi(argv[3]);
    if (argc > 4) WindowWidth = atoi(argv[4]);
    if (argc > 5) WindowHeight = atoi(argv[5]);

    // 如果宽度高度未指定或无效，则使用图片原始尺寸
    if (WindowWidth <= 0 || WindowHeight <= 0) {
        WindowWidth = cxSource;
        WindowHeight = cySource;
    }

    // 屏幕尺寸保护
    if (WindowWidth < 1 || WindowHeight < 1) {
        int scrWidth = GetSystemMetrics(SM_CXSCREEN);
        int scrHeight = GetSystemMetrics(SM_CYSCREEN);
        WindowWidth = scrWidth;
        WindowHeight = scrHeight;
    }

    hwnd = CreateWindow(szAppName,
        TEXT(""),
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowWidth,
        WindowHeight,
        NULL,
        NULL,
        hInstance,
        NULL);

    lWindowLong = GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, lWindowLong);
    hUser32 = GetModuleHandle(TEXT("USER32.DLL"));
    if (hUser32 == NULL) return 0;
    g_pSetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
    g_pSetLayeredWindowAttributes(hwnd, 0, trans, LWA_ALPHA);
    FreeLibrary(hUser32);

    // 根据 topmost 设置窗口置顶状态
    if (topmost)
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    else
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);
    SaveWindowPosition(hwnd);

    // 释放复制的参数字符串
    for (int i = 0; i < argc; i++) free(argv[i]);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 1;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc, hdcMem;
    PAINTSTRUCT ps;
    RECT  rect;

    switch (message)
    {
    case WM_ERASEBKGND:
    {
        if (Draw) { return DefWindowProc(hwnd, message, wParam, lParam); }
        else return 1;
    }
    case WM_CREATE:
    {
        int scrWidth, scrHeight;
        scrWidth = GetSystemMetrics(SM_CXSCREEN);
        scrHeight = GetSystemMetrics(SM_CYSCREEN);
        GetWindowRect(hwnd, &rect);
        rect.left = (scrWidth - (rect.right - rect.left)) / 2;
        rect.top = (scrHeight - (rect.bottom - rect.top)) / 2;
        SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
        return 0;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        if (Draw) {
            SetTextColor(hdc, RGB(176, 196, 222));
            SetBkColor(hdc, RGB(122, 122, 122));
            SetBkMode(hdc, OPAQUE);
            SetBkMode(hdc, TRANSPARENT);
            HFONT hFont = CreateFont(
                -20, -9, 0, 0, 400,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
                DEFAULT_QUALITY,
                FF_DONTCARE,
                TEXT("微软雅黑")
            );
            HGDIOBJ hOldGdiobj = SelectObject(hdc, (HGDIOBJ)hFont);
            GetClientRect(hwnd, &rect);
            DrawText(hdc, TEXT("拖入或传参[*.bmp] [0-255] [0-255] [0/1置顶] [宽度] [高度]"), -1, &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            SelectObject(hdc, hOldGdiobj);
            DeleteObject(hOldGdiobj);
        }
        else {
            if (hBitmap) {
                SelectObject(hdcMem, hBitmap);
                SetStretchBltMode(hdc, STRETCH_HALFTONE);
                StretchBlt(hdc, 0, 0, WindowWidth, WindowHeight, hdcMem, 0, 0, cxSource, cySource, SRCCOPY);
            }
            DeleteDC(hdcMem);
        }
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_KEYDOWN:
    {
        DestroyWindow(hwnd);
        break;
    }

    // 手动拖动实现，解决无法贴顶问题
    case WM_LBUTTONDOWN:
    {
        GetCursorPos(&g_ptDragStart);
        GetWindowRect(hwnd, &rect);
        g_ptDragStart.x -= rect.left;
        g_ptDragStart.y -= rect.top;
        g_bDragging = TRUE;
        SetCapture(hwnd);
        ::SetLayeredWindowAttributes(hwnd, 0, trans1, LWA_ALPHA);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (g_bDragging)
        {
            POINT ptCursor;
            GetCursorPos(&ptCursor);
            int x = ptCursor.x - g_ptDragStart.x;
            int y = ptCursor.y - g_ptDragStart.y;
            SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {
        if (g_bDragging)
        {
            g_bDragging = FALSE;
            ReleaseCapture();
            ::SetLayeredWindowAttributes(hwnd, 0, trans, LWA_ALPHA);
        }
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}