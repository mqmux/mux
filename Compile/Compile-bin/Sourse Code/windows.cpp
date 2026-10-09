// -lmingw32 
#define WS_EX_LAYERED 0x00080000
#include <windows.h>
#include <Winbase.h>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
//#include <stdio.h>
//#include <dwmapi.h>
//#pragma comment(lib,"Dwmapi.lib")  
typedef BOOL(WINAPI* lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
lpfn g_pSetLayeredWindowAttributes;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static HBITMAP hBitmap;
static int cxSource, cySource;
BOOL Draw = false;
int Rounded = 1;
int trans = 255;
int trans1 = 180;
int wait_time = 512;
HWND hProgress; //进度条句柄
int progress = 0; //进度条进度
HINSTANCE hInst; //应用程序实例句柄
const int PROGRESS_HEIGHT = 30;
int g_progress = 0;

// 绘制条形方块
void DrawProgress(HDC hdc, RECT rcClient, int progress)
{
    int width = rcClient.right - rcClient.left; // 不再减去空隙
    int height = 15; // 条形方块高度
    int barWidth = width * progress / wait_time;
    RECT rcBar = { rcClient.left, rcClient.bottom - height,
        rcClient.left + barWidth, rcClient.bottom };
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // 绿色
    FillRect(hdc, &rcBar, hBrush);
    DeleteObject(hBrush);
}
// 定时器消息处理函数
void OnTimer(HWND hWnd, UINT_PTR idEvent)
{
    g_progress++;
    if (g_progress > wait_time) {
        KillTimer(hWnd, 1);  // 停止定时器
        PostQuitMessage(0);
    }
    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}
void SetWindowEllispeFrame1(HWND hwnd, int nWidthEllipse, int nHeightEllipse)
{
    HRGN hRgn;
    RECT rect;

    GetWindowRect(hwnd, &rect);
    hRgn = CreateRoundRectRgn(0, 0, rect.right - rect.left, rect.bottom - rect.top, nWidthEllipse, nHeightEllipse);
    SetWindowRgn(hwnd, hRgn, TRUE);
}
void SetWindowEllispeFrame2(HWND hwnd, int nWidthEllipse, int nHeightEllipse)
{
    HRGN hRgn;
    RECT rect;
    HDC hdc, hdcMem;

    hdc = GetDC(hwnd);
    hdcMem = CreateCompatibleDC(hdc);
    ReleaseDC(hwnd, hdc);

    GetWindowRect(hwnd, &rect);

    // 画一个圆角矩形。
    BeginPath(hdcMem);
    RoundRect(hdcMem, 0, 0, rect.right - rect.left, rect.bottom - rect.top, nWidthEllipse, nHeightEllipse);
    EndPath(hdcMem);

    hRgn = PathToRegion(hdcMem); // 最后把路径转换为区域。

    SetWindowRgn(hwnd, hRgn, TRUE);
}
void SetWindowEllispeFrame3(HWND hwnd, int nWidthEllipse, int nHeightEllipse)
{
    HRGN hRgn;
    RECT rect;
    int nHeight, nWidth;


    GetWindowRect(hwnd, &rect);
    nHeight = rect.bottom - rect.top;    // 计算高度
    nWidth = rect.right - rect.left;        // 计算宽度

    POINT point[8] = {
        {0, nHeightEllipse},                // left-left-top
        {nWidthEllipse, 0},                 // left-top-left
        {nWidth - nWidthEllipse, 0},
        {nWidth, nHeightEllipse},        // right-top
        {nWidth, nHeight - nHeightEllipse}, // right-bottom-right
        {nWidth - nWidthEllipse, nHeight},  // right-bottom-bottom
        {nWidthEllipse, nHeight},              // left-bottom
        {0, nHeight - nHeightEllipse}
    };

    hRgn = CreatePolygonRgn(point, 8, WINDING);
    SetWindowRgn(hwnd, hRgn, TRUE);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstanc,
    LPSTR lpCmdLine, int nShowCmd)
{
    //HINSTANCE hInstance = GetModuleHandle(0);
    hInst = hInstance; //赋值给全局变量
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
    //wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("错误"), TEXT("worng"), MB_ICONERROR);
        return 0;
    }

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

    if (b1 > 2) wait_time = atoi(bmp[3]);
    if (b1 > 3) Rounded = atoi(bmp[4]);

    BITMAP bitmap;
    hBitmap = (HBITMAP)LoadImage(NULL,
        (LPCTSTR)bmp[0],//TEXT("C:\\Users\\Administrator\\Desktop\\399.bmp"),
        IMAGE_BITMAP,
        0,
        0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);
    cxSource = bitmap.bmWidth;
    cySource = bitmap.bmHeight;
    if (cxSource > 7680 || cxSource < 1 || cySource>4320 || cySource < 1) {
        cxSource = 500;
        cySource = 300;
        Draw = true;
    }
    if (lpCmdLine[0] == '\0') {
        cxSource = 500;
        cySource = 300;
        Draw = true;
    }
    hwnd = CreateWindow(szAppName,
        TEXT(""),
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        cxSource,
        cySource,
        NULL,
        NULL,
        hInstance,
        NULL);

    lWindowLong = GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, lWindowLong); /*为窗口添加WS_EX_LAYERED标志位*/
    hUser32 = GetModuleHandle(TEXT("USER32.DLL")); /*获得user32.dll的句柄*/
    if (hUser32 == NULL) return 0;
    g_pSetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32, "SetLayeredWindowAttributes");  /*获得SetLayeredWindowAttributes在user32.dll的地址*/


    if (Draw) trans = 200;
    if (b1 > 0) trans = atoi(bmp[1]);
    if (b1 > 1) trans1 = atoi(bmp[2]);
    g_pSetLayeredWindowAttributes(hwnd, 0, trans, 2);  /*设置窗口的透明属性*/

    FreeLibrary(hUser32);    /*释放user32.dll*/

    //MARGINS m = { -1 };
    //DwmExtendFrameIntoClientArea(hwnd, &m);//指定颜色透明
    // 设置定时器，每250毫秒更新一次


    ShowWindow(hwnd, nShowCmd);
    //ShowWindow(hwnd,SW_SHOW|nShowCmd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    // 删除定时器
    KillTimer(hwnd, 1);
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc, hdcMem;
    PAINTSTRUCT ps;
    RECT  rect;

    switch (message)
    {
        //构建窗体时
    case WM_CREATE:
    {
        SetTimer(hwnd, 1, 1, (TIMERPROC)OnTimer);
        //SetTimer(hwnd, 1, 1, NULL);
        /*hProgress = CreateWindowEx(WS_EX_STATICEDGE, PROGRESS_CLASS, NULL,
            WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
            0, 0, 0, 0,
            hwnd, NULL, hInst, NULL);
        long style = GetWindowLong(hProgress, GWL_EXSTYLE);
        style &= ~WS_EX_STATICEDGE;*/
        //SetWindowLong(hProgress, GWL_EXSTYLE, style);

        //SendMessage(hProgress, PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 255, 255));
        //SendMessage(hProgress, PBM_SETBKCOLOR, 0, (LPARAM)RGB(0, 0, 0));
        //SendMessage(hProgress, PBM_SETRANGE32, 0, wait_time);
        //SendMessage(hProgress, PBM_SETPOS, 0, 0);
        int scrWidth, scrHeight;
        //获得屏幕尺寸
        scrWidth = GetSystemMetrics(SM_CXSCREEN);
        scrHeight = GetSystemMetrics(SM_CYSCREEN);
        //获取窗体尺寸
        GetWindowRect(hwnd, &rect);
        rect.left = (scrWidth - rect.right) / 2;
        rect.top = (scrHeight - rect.bottom) / 2;
        SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
        if (Rounded)SetWindowEllispeFrame1(hwnd, 11, 11);//圆角

        hdc = BeginPaint(hwnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);
        BitBlt(hdc, 0, 0, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
        DeleteDC(hdcMem);
        EndPaint(hwnd, &ps);
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        /*hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);
        BitBlt(hdc, 0, 0, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
        DeleteDC(hdcMem);*/
        //Rectangle(hdc,40,cySource-25,cxSource-40,cySource-20);

        SetTextColor(hdc, RGB(176, 196, 222)); //设置字体颜色
        SetBkColor(hdc, RGB(122, 122, 122)); //设置背景色
        SetBkMode(hdc, OPAQUE); //非透明模式
        SetBkMode(hdc, TRANSPARENT); //透明模式
        HFONT hFont = CreateFont(
            -20/*高度*/, -9/*宽度*/, 0/*不用管*/, 0/*不用管*/, 400 /*一般这个值设为400*/,
            FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
            DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
            DEFAULT_QUALITY,  //默认输出质量
            FF_DONTCARE,  //不指定字体族*/
            TEXT("微软雅黑")  //字体名
        );
        //2.设置字体
        HGDIOBJ hOldGdiobj = SelectObject(hdc, (HGDIOBJ)hFont);
        GetClientRect(hwnd, &rect);
        DrawProgress(hdc, rect, g_progress);
        if (Draw) {
            //::SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);//指定颜色透明
            DrawText(hdc, TEXT("拖入或传参[.bmp] [0-255] [0-255] [500ms] [0/1]"), -1, &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            //DrawText(hdc, TEXT("正在加载 . . .   "), -1, &rect,DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);

            TEXTMETRIC tm;
            GetTextMetrics(hdc, &tm);
            int textHeight = tm.tmHeight + tm.tmExternalLeading;
            // Move the formatting rectangle up by one text line
            rect.bottom -= textHeight;
            // Draw the text
            DrawText(hdc, TEXT("Loading . . .     "), -1, &rect,
                DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
        }
        EndPaint(hwnd, &ps);
        //4.把旧的字体还回系统
        SelectObject(hdc, hOldGdiobj);
        //5.删除字体句柄
        DeleteObject(hOldGdiobj);
        return 0;
    }
    //case WM_SIZE:
    //{
    //    int cxClient = LOWORD(lParam); //窗口客户区宽度
    //    int cyClient = HIWORD(lParam); //窗口客户区高度
    //    int cyProgress = GetSystemMetrics(SM_CYVSCROLL); //进度条高度
    //    MoveWindow(hProgress, 0, cyClient - cyProgress, cxClient, cyProgress, TRUE); //移动进度条到窗口
    //    break;
    //}
    case WM_TIMER:
    {
        if (wParam == 1)
        {
            // Update the window title
            progress++; //增加进度
            /*
            if (progress == 200) {
                HGDIOBJ oldBitmap;
                BITMAP bitmap;
                hdc = BeginPaint(hwnd, &ps);
                hBitmap = (HBITMAP)LoadImage(NULL,
                    TEXT("C:\\Users\\Administrator\\Desktop\\399.bmp"),
                    IMAGE_BITMAP,
                    0,
                    0,
                    LR_LOADFROMFILE | LR_CREATEDIBSECTION);

                hdcMem = CreateCompatibleDC(hdc);
                oldBitmap = SelectObject(hdcMem, hBitmap);

                GetObject(hBitmap, sizeof(bitmap), &bitmap);
                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

                cxSource = bitmap.bmWidth;
                cySource = bitmap.bmHeight;

                SelectObject(hdcMem, oldBitmap);
                DeleteDC(hdcMem);
                EndPaint(hwnd, &ps);
                InvalidateRect(hwnd, NULL, TRUE);
                // 或者使窗口的一个矩形区域无效
                RECT rect;
                rect.left = 0;
                rect.top = 0;
                rect.right = cxSource;
                rect.bottom = cySource;
                InvalidateRect(hwnd, &rect, TRUE);
            }
            */
            //if (progress > wait_time) {
            //    KillTimer(hwnd, 1);
            //    PostQuitMessage(0);	//向系统表明有个线程有终止请求，用来响应WM_DESTROY消息
            //    break; //如果超过100，重置为0
            //}
            //SendMessage(hProgress, PBM_SETPOS, progress, 0); //更新进度条位置
        }
        break;
    }
    case WM_NCHITTEST:
    {
        UINT nHitTest;
        nHitTest = ::DefWindowProc(hwnd, message, wParam, lParam);
        //如果鼠标左键按下, GetAsyncKeyState 的返回值小于0
        if (progress > wait_time) {
            KillTimer(hwnd, 1);
            PostQuitMessage(0);	//向系统表明有个线程有终止请求，用来响应WM_DESTROY消息
            break; //如果超过100，重置为0
        }
        if (nHitTest == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
        {
            ::SetLayeredWindowAttributes(hwnd, 0, trans1, LWA_ALPHA);
            nHitTest = HTCAPTION;
        }
        else {
            ::SetLayeredWindowAttributes(hwnd, 0, trans, LWA_ALPHA);
        }
        return nHitTest;
    }
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);	//向系统表明有个线程有终止请求，用来响应WM_DESTROY消息
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);	//调用默认窗口过程为应用程序没有处理的窗口消息提供默认的处理
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
