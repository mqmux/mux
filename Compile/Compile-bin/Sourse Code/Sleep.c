#include <windows.h>

BOOL Is64bitSystem()
{
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
        return TRUE;
    else
        return FALSE;
}

int main(int argc, char *argv[])
{
	if( argc == 2 )
		Sleep(atoi(argv[1]));
	
	if(__SIZEOF_POINTER__== 4)
        return 32;
    else if(__SIZEOF_POINTER__ == 8)
        return 64;
    else return 0;
	
	/* if (Is64bitSystem())
        return 64;
    else
        return 32; */
}
