#include <windows.h>
#include <shellapi.h>
#include <string>
#include <vector>

#define WM_SYSICON (WM_USER + 1)
#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT 1002
#define ID_TRAY_MESSAGE 1003

// Global variables
NOTIFYICONDATAA nid;
HWND hwnd;

// Convert wide character string to multi-byte string
std::string WideToMultiByte(const std::wstring& wideStr) {
    int size = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string multiByteStr(size, 0);
    WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &multiByteStr[0], size, NULL, NULL);
    return multiByteStr;
}

// Show notification
void ShowNotification(const char* title, const char* msg)
{
    // Fill the NOTIFYICONDATA structure
    nid.cbSize = sizeof(NOTIFYICONDATAA);
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uFlags = NIF_INFO;
    nid.dwInfoFlags = NIIF_INFO;
    strncpy_s(nid.szInfoTitle, title, sizeof(nid.szInfoTitle) - 1);
    strncpy_s(nid.szInfo, msg, sizeof(nid.szInfo) - 1);

    // Show the notification
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

std::vector<std::wstring> CommandLineToArgvW()
{
    std::vector<std::wstring> args;
    LPWSTR* argv;
    int argc;
    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv)
    {
        for (int i = 0; i < argc; ++i)
        {
            args.push_back(argv[i]);
        }
        LocalFree(argv);
    }
    return args;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        // Initialize NOTIFYICONDATA structure
        nid.cbSize = sizeof(NOTIFYICONDATAA);
        nid.hWnd = hwnd;
        nid.uID = ID_TRAY_APP_ICON;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_SYSICON;
        nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        strncpy_s(nid.szTip, "Tray Icon", sizeof(nid.szTip) - 1);

        Shell_NotifyIcon(NIM_ADD, &nid);

        SetTimer(hwnd, 1, 0, NULL);
        return 0;

    case WM_TIMER:
    {
        // Check command line parameters
        std::vector<std::wstring> argv = CommandLineToArgvW();
        if (argv.size() >= 3)
        {
            std::string title = WideToMultiByte(argv[1]);
            std::string message = WideToMultiByte(argv[2]);
            ShowNotification(title.c_str(), message.c_str());
        }
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char* CLASS_NAME = "TrayIconClass";
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClassA(&wc);
    hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Tray Icon",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) return 0;
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
