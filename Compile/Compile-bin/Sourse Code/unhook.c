#include<Windows.h>
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
int main(int argc, char* argv[])
{
	HHOOK m_hHOOK1 = NULL, m_hHOOK2 = NULL, m_hHOOK3 = NULL, m_hHOOK4 = NULL;
	m_hHOOK1 = (HHOOK)SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
	m_hHOOK2 = (HHOOK)SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
	//m_hHOOK2 = (HHOOK)SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandle(NULL), 0);
	m_hHOOK3 = (HHOOK)SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
	m_hHOOK4 = (HHOOK)SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
	//m_hHOOK4 = (HHOOK)SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseProc, GetModuleHandle(NULL), 0);
	while (true)
	{
			UnhookWindowsHookEx(m_hHOOK1);
			UnhookWindowsHookEx(m_hHOOK3);
			m_hHOOK1 = (HHOOK)SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
			m_hHOOK3 = (HHOOK)SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
			UnhookWindowsHookEx(m_hHOOK2);
			UnhookWindowsHookEx(m_hHOOK4);
			m_hHOOK2 = (HHOOK)SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
			m_hHOOK4 = (HHOOK)SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
			//m_hHOOK2 = (HHOOK)SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandle(NULL), 0);
			//m_hHOOK4 = (HHOOK)SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseProc, GetModuleHandle(NULL), 0);
	}
	return 0;
}
