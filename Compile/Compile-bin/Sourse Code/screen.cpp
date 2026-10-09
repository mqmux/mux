#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <vector>

using namespace Gdiplus;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;  // number of image encoders
    UINT size = 0; // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;
    GetImageEncodersSize(&num, &size);
    if (size == 0) {
        return -1;  // Failure
    }
    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL) {
        return -1;  // Failure
    }

    // 避免读取无效数据
    if (GetImageEncoders(num, size, pImageCodecInfo) != Ok) {
        free(pImageCodecInfo);
        return -1;
    }

    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

// 获取全部显示器的句柄
std::vector<HMONITOR> GetAllMonitors() {
    std::vector<HMONITOR> monitors;
    EnumDisplayMonitors(
        NULL,  // ensure that every display is included
        NULL,  // use the whole desktop as the enumeration region
        [](HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
            std::vector<HMONITOR>* monitors = reinterpret_cast<std::vector<HMONITOR>*>(dwData);
    monitors->push_back(hMonitor);
    return TRUE;
        },
        reinterpret_cast<LPARAM>(&monitors));
    return monitors;
}

// 获取一个显示器的左上角和右下角坐标值
RECT GetMonitorBounds(HMONITOR hMonitor) {
    MONITORINFOEX monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);
    GetMonitorInfo(hMonitor, &monitorInfo);
    return monitorInfo.rcMonitor;
}

// 全屏截图函数
Bitmap* CaptureScreen() {
    std::vector<HMONITOR> monitors = GetAllMonitors();

    // 获取全部显示器的大小
    int totalWidth = 0;
    int maxHeight = 0;
    for (auto hMonitor : monitors) {
        RECT rect = GetMonitorBounds(hMonitor);
        totalWidth += (rect.right - rect.left);
        if (maxHeight < (rect.bottom - rect.top)) {
            maxHeight = (rect.bottom - rect.top);
        }
    }

    // 创建画布，准备绘制截图
    Bitmap* screen = new Bitmap(totalWidth, maxHeight, PixelFormat32bppARGB);
    Graphics graphics(screen);
    int currentX = 0;
    for (auto hMonitor : monitors) {
        RECT rect = GetMonitorBounds(hMonitor);
        // 创建每个矩形对应的 bitmap
        Bitmap bitmap(rect.right - rect.left, rect.bottom - rect.top, PixelFormat32bppARGB);
        Graphics bitmapGraphics(&bitmap);
        HDC hdc = bitmapGraphics.GetHDC();
        BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, GetDC(NULL), rect.left, rect.top, SRCCOPY);
        bitmapGraphics.ReleaseHDC(hdc);
        // 在画布上绘制 bitmap
        graphics.DrawImage(&bitmap, currentX, 0);
        currentX += (rect.right - rect.left);
    }

    return screen;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // 初始化 GDI+ 库
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // 截图
    Bitmap* bitmap = CaptureScreen();

    // 保存截图为文件
    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    bitmap->Save(L"screen.png", &pngClsid, nullptr);

    // 释放资源
    delete bitmap;
    GdiplusShutdown(gdiplusToken);

    return 0;
}