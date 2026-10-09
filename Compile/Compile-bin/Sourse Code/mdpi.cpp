#include <windows.h>
#include <stdio.h>

void get_mouse_position(int x, int y, int dpi, int resolution_x, int resolution_y) {
    // 获取本机屏幕分辨率
    int local_resolution_x = GetSystemMetrics(SM_CXSCREEN);
    int local_resolution_y = GetSystemMetrics(SM_CYSCREEN);

    // 计算鼠标位置相对于其他电脑屏幕分辨率的比例
    double relative_x = (double)x / resolution_x;
    double relative_y = (double)y / resolution_y;

    // 根据本机屏幕分辨率计算出新的鼠标坐标
    int new_x = (int)(relative_x * local_resolution_x);
    int new_y = (int)(relative_y * local_resolution_y);

    // 输出鼠标坐标
    printf("%d %d\n",new_x, new_y);
}

int main(int argc, char* argv[]) {
    // 获取屏幕分辨率
    int resolution_x = GetSystemMetrics(SM_CXSCREEN);
    int resolution_y = GetSystemMetrics(SM_CYSCREEN);

    // 获取DPI
    int dpi = GetDeviceCaps(GetDC(0), LOGPIXELSX);

    if (argc == 6) {
        int x = atoi(argv[1]);
        int y = atoi(argv[2]);
        int dpi = atoi(argv[3]);
        int resolution_x = atoi(argv[4]);
        int resolution_y = atoi(argv[5]);
        get_mouse_position(x, y, dpi, resolution_x, resolution_y);
    }
    else {
        POINT p;
        GetCursorPos(&p);
        printf("%d %d %d %d %d\n", p.x, p.y, dpi, resolution_x, resolution_y);
    }

    return 0;
}