#include <windows.h>
#include <stdio.h>
#include <psapi.h>

bool values[] = { false, false, false, false, false, false, false, false };
char className[256];
char title[256];
char path[MAX_PATH];
int last_values = 0;

VOID HWND_SHOW(HWND hWnd, bool value) {
    if (value)
        printf(". ");
    else
        printf("  ");
    // 获取窗口句柄
    if (values[1]) {
        printf("%#-9x", hWnd);
        if (last_values == 1) printf("\n");
    }
    // 获取窗口进程线程id
    DWORD pid, tid;
    tid = GetWindowThreadProcessId(hWnd, &pid);
    if (values[2]) {
        printf(" %-6d", pid);
        if (last_values == 2) printf("\n");
    }
    if (values[3]) {
        printf("  %-6d", tid);
        if (last_values == 3) printf("\n");
    }
    // 获取窗口位置
    if (values[4]) {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        printf("  %-6d %-6d %-6d %-6d", rect.left, rect.top, rect.right, rect.bottom);
        if (last_values == 4) printf("\n");
    }
    // 获取窗口类名
    if (values[5]) {
        GetClassName(hWnd, className, sizeof(className));
        printf("  %s", className);
        if (last_values == 5) printf("\n");
    }
    // 获取窗口标题
    if (values[6]) {
        GetWindowText(hWnd, title, 256);
        printf("  %s", title);
        if (last_values == 6) printf("\n");
    }
    // 获取进程文件位置
    if (values[7]) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess)
        {
            GetModuleFileNameEx(hProcess, NULL, path, MAX_PATH);
            printf("  %s", path);
            CloseHandle(hProcess);
        }
        if (last_values == 7) printf("\n");
    }
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    // 判断是否可见
    if (IsWindowVisible(hWnd))
        HWND_SHOW(hWnd, true);
    else if (values[0]) HWND_SHOW(hWnd, false);
    return TRUE;
}

void GetWindowPositionWithDPI(HWND hwnd, RECT* rect) {
    // 获取窗口矩形
    GetWindowRect(hwnd, rect);

    // 获取窗口的 DPI
    UINT dpi = GetDpiForWindow(hwnd); // 适用于Windows 10及以上版本
    float scalingFactor = dpi / 96.0f; // 96是系统的基础DPI，计算缩放因子

    printf("DPI: %d, Scaling Factor: %f\n", dpi, scalingFactor);

    // 调整窗口位置坐标以考虑 DPI 缩放因子
    rect->left = static_cast<int>(rect->left * scalingFactor);
    rect->top = static_cast<int>(rect->top * scalingFactor);
    rect->right = static_cast<int>(rect->right * scalingFactor);
    rect->bottom = static_cast<int>(rect->bottom * scalingFactor);
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        if (argv[1][0] == '-' && (argv[1][1] == 'h' || argv[1][1] == 'H')) {
            printf("%s ALL HWND PID TID RECT CLASSNAME TITLE PATH\n", argv[0]);
            return 0;
        }
        const char* names[] = { "all", "hWnd", "pid", "tid", "rect", "className", "title", "path" };
        int len = sizeof(names) / sizeof(names[0]);
        bool empty = true;
        for (int i = 1; i < argc; i++)
        {
            for (int j = 0; j < len; j++)
            {
                if (_stricmp(argv[i], names[j]) == 0)
                {
                    empty = false;
                    values[j] = true;
                    break;
                }
            }
        }
        for (int j = 0; j < len; j++)
        {
            if (values[j] == true)
                last_values = j;
        }
        if (empty || last_values == 0) {
            values[1] = true;
            values[2] = true;
            values[6] = true;
            last_values = 6;
        }
        EnumWindows(EnumWindowsProc, NULL);
        return 0;
    }
    char title[256];
    char className[256];
    DWORD processId;
    RECT rect;
    HWND hwnd = GetForegroundWindow();
    if (hwnd == NULL) {
        printf("Error: %d\n", GetLastError());
        return 1;
    }
    GetWindowText(hwnd, title, sizeof(title));
    GetClassName(hwnd, className, sizeof(className));
    DWORD threadId = GetWindowThreadProcessId(hwnd, &processId);
    GetWindowRect(hwnd, &rect);
    GetWindowPositionWithDPI(hwnd, &rect);
    printf("Handle       : %#x %ld\n", hwnd, hwnd);
    printf("Title        : %s \n", title);
    printf("Class_Name   : %s\n", className);
    printf("Thread_ID    : %d\n", threadId);
    printf("Process_ID   : %d\n", processId);
    printf("Position     : %d %d %d %d\n", rect.left, rect.top, rect.right, rect.bottom);
    HANDLE processHandle = NULL;
    TCHAR filename[MAX_PATH] = "";
    processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (processHandle != NULL) {
        if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0) {
            printf("Program_Path : Failed to get module filename\n");
        }
        else {
            printf("Program_Path : %s\n", filename);
        }
        CloseHandle(processHandle);
    }
    else {
        printf("Program_Path : Failed to open process\n");
    }
    return 0;
}