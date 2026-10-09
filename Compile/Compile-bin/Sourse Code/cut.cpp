#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ImageWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 注册全屏窗口类
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = TEXT("MyFullScreenWindowClass");
    RegisterClass(&wc);

    // 创建全屏窗口
    HWND hwnd = CreateWindow(
        TEXT("MyFullScreenWindowClass"), // 窗口类名
        TEXT("Screen Capture"), // 窗口标题
        WS_POPUP | WS_VISIBLE, // 窗口样式
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), // 窗口位置和大小
        NULL, NULL, hInstance, NULL // 其他参数
    );

    // 注册图片窗口类
    wc.lpfnWndProc = ImageWndProc;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = TEXT("MyImageWindowClass");
    RegisterClass(&wc);

    // 消息循环
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static BOOL isDragging = FALSE; // 是否正在拖动
    static POINT startPos; // 拖动开始位置
    static HWND hCaptureWnd = NULL; // 截图窗口句柄

    switch (msg)
    {
    case WM_CREATE:
<<<<<<< HEAD
        SetCursor(LoadCursor(NULL, IDC_CROSS)); // 设置鼠标样式为十字
=======
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
        // 创建透明窗口
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hwnd, 0, (255 * 20) / 100, LWA_ALPHA); // 设置背景透明度为20%
        break;
<<<<<<< HEAD

=======
    
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
    case WM_LBUTTONDOWN:
        // 开始拖动
        isDragging = TRUE;
        startPos.x = LOWORD(lParam);
        startPos.y = HIWORD(lParam);
<<<<<<< HEAD
=======
        SetCursor(LoadCursor(NULL, IDC_CROSS)); // 设置鼠标样式为十字
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
        // 设置矩形区域透明度和颜色
        //SetLayeredWindowAttributes(hCaptureWnd, RGB(0, 0, 0), (255 * 10) / 100, LWA_COLORKEY | LWA_ALPHA); // 设置矩形区域透明度为10%
        break;
    case WM_PAINT:
    {
<<<<<<< HEAD
        // 绘制白色边框矩形
=======
// 绘制白色边框矩形
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SelectObject(hdc, GetStockObject(BACKGROUND_BLUE)); // 选择空白画刷
        Rectangle(hdc, startPos.x, startPos.y, LOWORD(lParam), HIWORD(lParam)); // 绘制矩形
        SetLayeredWindowAttributes(hCaptureWnd, RGB(0, 0, 0), 0, LWA_COLORKEY | LWA_ALPHA); // 设置矩形区域透明度为30%
        EndPaint(hwnd, &ps);
        break;
    }
<<<<<<< HEAD

=======
        
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
    case WM_MOUSEMOVE:
        if (isDragging)
        {
            // 计算矩形位置和大小
            int left = min(startPos.x, LOWORD(lParam));
            int top = min(startPos.y, HIWORD(lParam));
            int width = abs(startPos.x - LOWORD(lParam));
            int height = abs(startPos.y - HIWORD(lParam));

            // 创建或更新截图窗口
            if (hCaptureWnd == NULL)
            {
                hCaptureWnd = CreateWindow(
                    TEXT("STATIC"), NULL, WS_VISIBLE | WS_POPUP | WS_BORDER | WS_EX_TRANSPARENT,
                    left - GetSystemMetrics(SM_CXSIZEFRAME),
                    top - GetSystemMetrics(SM_CYSIZEFRAME),
                    width + GetSystemMetrics(SM_CXSIZEFRAME) * 2,
                    height + GetSystemMetrics(SM_CYSIZEFRAME) * 2,
                    hwnd, NULL, NULL, NULL);
                SetWindowLong(hCaptureWnd, GWL_EXSTYLE,
                    GetWindowLong(hCaptureWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_DLGFRAME);
            }
            SetWindowPos(hCaptureWnd, HWND_TOPMOST,
                left - GetSystemMetrics(SM_CXSIZEFRAME),
                top - GetSystemMetrics(SM_CYSIZEFRAME),
                width + GetSystemMetrics(SM_CXSIZEFRAME) * 2,
                height + GetSystemMetrics(SM_CYSIZEFRAME) * 2,
                SWP_SHOWWINDOW);

<<<<<<< HEAD
            // 设置窗口透明度和透明色
            SetLayeredWindowAttributes(hCaptureWnd, 0, 10, LWA_ALPHA);
            SetLayeredWindowAttributes(hwnd, 0, 10, LWA_ALPHA);
=======
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
            // 请求重绘窗口
            InvalidateRect(hCaptureWnd, NULL, FALSE);
        }
        break;
    case WM_LBUTTONUP:
        if (isDragging)
        {
            // 停止拖动
            isDragging = FALSE;
            SetCursor(LoadCursor(NULL, IDC_ARROW)); // 恢复鼠标样式

            // 获取屏幕和截图 DC
            HDC hScreenDC = GetDC(NULL);
            HDC hCaptureDC = GetDC(hCaptureWnd);

<<<<<<< HEAD
            // 获取 DPI 缩放比例
            int screenW = ::GetSystemMetrics(SM_CXSCREEN);
            int screenH = ::GetSystemMetrics(SM_CYSCREEN);
            HWND hwd = ::GetDesktopWindow();
            HDC hdc = ::GetDC(hwd);
            int widthg = ::GetDeviceCaps(hdc, DESKTOPHORZRES);
            int heightg = ::GetDeviceCaps(hdc, DESKTOPVERTRES);
            int mdpi = (96 * (widthg * 100 / screenW)) / 100;

            //HWND hd = GetDesktopWindow();
            //int mdpi = GetDpiForWindow(hd);

=======

            // 获取 DPI 缩放比例
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
            int dpiX = GetDeviceCaps(hScreenDC, LOGPIXELSX);
            int dpiY = GetDeviceCaps(hScreenDC, LOGPIXELSY);

            // 计算截图区域大小，并根据 DPI 缩放大小
            int width = abs(startPos.x - LOWORD(lParam)) * dpiX / 96;
            int height = abs(startPos.y - HIWORD(lParam)) * dpiY / 96;

<<<<<<< HEAD
            // 创建兼容位图，并根据 DPI 缩放大小
            HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

=======

            // 创建兼容位图，并根据 DPI 缩放大小
            HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);


>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
            // 将截图区域复制到兼容位图，并根据 DPI 缩放大小
            HDC hBitmapDC = CreateCompatibleDC(hScreenDC);
            HGDIOBJ hOldBitmap = SelectObject(hBitmapDC, hBitmap);

            // 设置缩放模式为最佳算法
            SetStretchBltMode(hBitmapDC, HALFTONE);

<<<<<<< HEAD
            // 使用实际的 DPI 值来计算目标区域的宽度和高度
            StretchBlt(hBitmapDC, 0, 0, width, height,
                hScreenDC,
                min(startPos.x, LOWORD(lParam)) * mdpi / 96,
                min(startPos.y, HIWORD(lParam)) * mdpi / 96,
                abs(startPos.x - LOWORD(lParam)) * mdpi / 96,
                abs(startPos.y - HIWORD(lParam)) * mdpi / 96,
                SRCCOPY);

=======

            // 获取系统默认的 DPI 值
            int defaultDpiX = GetDeviceCaps(hScreenDC, LOGPIXELSX);
            int defaultDpiY = GetDeviceCaps(hScreenDC, LOGPIXELSY);

            // 使用系统默认的 DPI 值来计算目标区域的宽度和高度
            StretchBlt(hBitmapDC, 0, 0, width * defaultDpiX / dpiX, height * defaultDpiY / dpiY,
                hScreenDC,
                min(startPos.x, LOWORD(lParam)),
                min(startPos.y, HIWORD(lParam)),
                width, height,
                SRCCOPY);



>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
            SelectObject(hBitmapDC, hOldBitmap);

            // 将截图保存到剪贴板
            OpenClipboard(NULL);
            EmptyClipboard();
            SetClipboardData(CF_BITMAP, hBitmap);
            CloseClipboard();

            // 创建图片窗口，并根据 DPI 缩放位置和大小
            HWND hImageWnd = CreateWindow(
                TEXT("MyImageWindowClass"), NULL,
                WS_VISIBLE | WS_POPUP,
                min(startPos.x, LOWORD(lParam)) * dpiX / 96,
                min(startPos.y, HIWORD(lParam)) * dpiY / 96,
                width, height,
                hwnd, NULL, NULL, NULL);

<<<<<<< HEAD
=======

>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
            // 设置图片窗口为无边框且可拖动
            SetWindowLong(hImageWnd, GWL_EXSTYLE,
                GetWindowLong(hImageWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

<<<<<<< HEAD
=======

>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
            // 发送图片给图片窗口
            SendMessage(hImageWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

            // 清空截图窗口
            DestroyWindow(hCaptureWnd);
            hCaptureWnd = NULL;

            // 关闭全屏窗口
            ShowWindow(hwnd, SW_HIDE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK ImageWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static BOOL isDragging = FALSE; // 是否正在拖动
    static POINT oldPos; // 旧的鼠标位置
    static HBITMAP hBitmap = NULL; // 截图的位图句柄

    switch (msg)
    {
    case WM_CREATE:
        // 设置图片窗口透明度为100%
        SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
        break;
    case WM_LBUTTONDOWN:
        // 开始拖动
        isDragging = TRUE;
        oldPos.x = LOWORD(lParam);
        oldPos.y = HIWORD(lParam);
<<<<<<< HEAD
        SetLayeredWindowAttributes(hwnd, 0, (255 * 90) / 100, LWA_ALPHA); // 设置透明度为10%
=======
        SetLayeredWindowAttributes(hwnd, 0, (255 * 10) / 100, LWA_ALPHA); // 设置透明度为10%
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
        break;
    case WM_MOUSEMOVE:
        if (isDragging)
        {
            // 计算新的窗口位置
            POINT newPos;
            GetCursorPos(&newPos);
            SetWindowPos(hwnd, NULL,
                newPos.x - oldPos.x,
                newPos.y - oldPos.y,
                0, 0,
                SWP_NOSIZE | SWP_NOZORDER);
        }
        break;
    case WM_LBUTTONUP:
        // 停止拖动
        isDragging = FALSE;
        SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA); // 恢复透明度
        break;
<<<<<<< HEAD
    case WM_RBUTTONDOWN:
        // 右键按下，关闭窗口
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        break;
    case WM_PAINT:
    {
        // 绘制图片到窗口
=======
    case WM_PAINT:
    {
// 绘制图片到窗口
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HDC hBitmapDC = CreateCompatibleDC(hdc);
        HGDIOBJ hOldBitmap = SelectObject(hBitmapDC, hBitmap);
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
            hBitmapDC, 0, 0, SRCCOPY);
        SelectObject(hBitmapDC, hOldBitmap);
        DeleteDC(hBitmapDC);
        EndPaint(hwnd, &ps);
        break;
    }
<<<<<<< HEAD

=======
        
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
    case STM_SETIMAGE:
    {
        // 接收并保存截图的位图句柄
        hBitmap = (HBITMAP)lParam;
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

<<<<<<< HEAD

void DrawSelectedAreaFromBitmap(HWND hCaptureWnd, int left, int top, int width, int height)
{
    // 获取窗口的设备上下文
    HDC hdc = GetDC(hCaptureWnd);

    // 创建一个内存设备上下文
    HDC memDC = CreateCompatibleDC(hdc);

    // 创建一个位图对象，大小和选定区域相同
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);

    // 将位图对象选入内存设备上下文
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    // 从亮色位图中抠出选定区域
    BitBlt(memDC, 0, 0, width, height, hdc, left, top, SRCCOPY);

    // 将抠出的图像绘制到窗口上
    BitBlt(hdc, left, top, width, height, memDC, 0, 0, SRCCOPY);

    // 清理
    SelectObject(memDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memDC);
    ReleaseDC(hCaptureWnd, hdc);
}
=======
>>>>>>> 53be284a8ca3abd23cb9b5002e1b466f38200344
