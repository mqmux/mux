#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

// 获取屏幕截图
HBITMAP CaptureScreen(int sw, int sh, int width, int height, bool Clipboard) {
    HDC hScreenDC = GetDC(NULL); // 获取屏幕设备上下文
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC); // 创建兼容设备上下文
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap); // 选择新创建的位图到兼容设备上下文中
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, sw, sh, SRCCOPY); // 将屏幕图像拷贝到位图中
    SelectObject(hMemoryDC, hOldBitmap); // 恢复旧的位图
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
    if (Clipboard) {
        // 将位图保存到剪贴板
        OpenClipboard(NULL);
        EmptyClipboard();
        SetClipboardData(CF_BITMAP, hBitmap);
        CloseClipboard();
    }
    return hBitmap;
}

// 计算简单的数字化指纹
int CalculateFingerprint(HBITMAP hBitmap) {
    // 这里简单地计算图像的平均像素值作为指纹
    // 可以根据实际需求使用更复杂的方法来生成指纹
    // 这个方法只是一个示例
    BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);
    int totalPixelValue = 0;
    int totalPixels = bm.bmWidth * bm.bmHeight;
    HDC hDC = CreateCompatibleDC(NULL);
    SelectObject(hDC, hBitmap);
    for (int i = 0; i < bm.bmWidth; ++i) {
        for (int j = 0; j < bm.bmHeight; ++j) {
            COLORREF pixelColor = GetPixel(hDC, i, j);
            totalPixelValue += GetRValue(pixelColor) + GetGValue(pixelColor) + GetBValue(pixelColor);
        }
    }
    DeleteDC(hDC);
    return totalPixelValue;
}

int main(int argc, char* argv[]) {
    //int previousFingerprint = -1;
    // 获取当前屏幕截
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    int x = 0;
    int y = 0;
    int Clipboard = 1;
    if (argc > 1) {
        int argvs = sscanf(argv[1], "%d.%d.%d.%d.%d", &x, &y, &width, &height, &Clipboard);
        if (argvs != 5) {
            width = GetSystemMetrics(SM_CXSCREEN);
            height = GetSystemMetrics(SM_CYSCREEN);
            Clipboard = 0;
        }
    }
    //printf("[%d,%d,%d,%d]:%d\n", x, y, width, height, Clipboard);
    //printf("[%d,%d]:%d\n", width, height, Clipboard);
    HBITMAP hBitmap = CaptureScreen(x, y, width, height, Clipboard);
    // 计算当前图像的数字化指纹
    int currentFingerprint = CalculateFingerprint(hBitmap);
    //int Fingerprint = abs(currentFingerprint - previousFingerprint); // 取绝对值
    //printf("[currentFingerprint-previousFingerprint]:[%d-%d=%d]\n", currentFingerprint, previousFingerprint, Fingerprint);
    printf("%d", currentFingerprint);
    // 比较变化
    //if (Fingerprint != 0) {
        // 如果变化超过阈值，则进行下一次截图
        //previousFingerprint = currentFingerprint;
        // 进行下一次操作...
        //printf("截图\n");
    //}
    // 释放资源
    DeleteObject(hBitmap);
    return currentFingerprint;
}
