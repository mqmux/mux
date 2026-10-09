#include<stdio.h>
#include<stdlib.h>
#include <windows.h>  
#include <string.h>  
#include <TlHelp32.h>  
HWND GetWindowHandleByPID(DWORD dwProcessID)//通过进程ID获取顶层窗口句柄
{
	HWND h = ::GetTopWindow(0);
	while (h)
	{
		DWORD pid = 0;
		DWORD dwTheardId = GetWindowThreadProcessId(h, &pid);
		if (dwTheardId != 0)
		{
			if (pid == dwProcessID)
			{
				// here h is the handle to the window
				while (::GetParent(h) != NULL)
					h = ::GetParent(h);
				return h;
			}
		}
		h = ::GetNextWindow(h, GW_HWNDNEXT);
	}
	return NULL;
}

int main()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL find = Process32First(hp, &pe32);
	HWND chuangkoujubing=0;
	HANDLE jinchengjubing1=0;
	while (find)
	{
		chuangkoujubing = GetWindowHandleByPID(pe32.th32ProcessID);
		jinchengjubing1 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		printf("%*s %*d %*X %*X\n",-28,pe32.szExeFile,-8,pe32.th32ProcessID,-8,chuangkoujubing,-8,jinchengjubing1);
		/* if (lstrcmp(pe32.szExeFile, TEXT("cmd.exe")) == 0)
		{
			DWORD jinchengid = pe32.th32ProcessID;
			printf("进程id %d\n", jinchengid);
			chuangkoujubing = GetWindowHandleByPID(jinchengid);
			printf("窗口句柄 %d\n", chuangkoujubing);
			jinchengjubing1 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, jinchengid);
			printf("进程句柄 %x\n", jinchengjubing1);
			break;
		} */
		/* if (pe32.th32ProcessID==2652)
		{
			break;
		} */
		find = Process32Next(hp, &pe32);
	}
	return 0;
}

