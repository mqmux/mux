#include <process.h>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int bWait = 0;
	DWORD exitcode = 0;
	WCHAR stopchar = L' ';
	WCHAR* lpszCmd = GetCommandLineW();
	if (lpszCmd[0] == L'"')
		stopchar = L'"';
	do {
		lpszCmd++;
	} while ((lpszCmd[0] != stopchar) && (lpszCmd[0] != 0));

	if (lpszCmd[0] != 0)
	{
		do {
			lpszCmd++;
		} while ((lpszCmd[0] != 0) && ((lpszCmd[0] == L' ')));
	};
	if (lpszCmd[0] == 0)
	{
		//MessageBoxW(0, HELP_INFO, L"Incorrect usage", 0);
		ExitProcess(0);
	};

	//search the "/w" option
	if ((lpszCmd[0] == L'/') && (lpszCmd[1] == L'w' || lpszCmd[1] == L'W') && (lpszCmd[2] == L' '))
	{
		bWait = 1;
		lpszCmd += 3;
	};

	//skip the space
	while ((lpszCmd[0] != 0) && (lpszCmd[0] == ' '))
		lpszCmd++;

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	if (CreateProcessW(NULL, lpszCmd,
		NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		if (bWait)
			WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
		exitcode = GetLastError();

	ExitProcess(exitcode);
}
