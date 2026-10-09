/*
    Compile:
        MinGW: g++ main.cpp -municode
        MSVC : cl  main.cpp /link shell32.lib
    Usage:
        Example:
            main.exe D:\FA\* F:\Spare
            main.exe D:\FA\* D:\FB\name.iso F:\Spare
            main.exe "D:\FA\Pro?e.rar" "F:\Spare"
        Note:
            Last path is the destination

    Support: Copy from different folder, Unicode String
*/

#include <cstdio>
#include <cstdlib>
#include <io.h>
#include <windows.h>


void ShellCopy(wchar_t* SRC, wchar_t* DST);

void connect_wcs_array(
    wchar_t* buff,
    wchar_t* array[],
    int      begin,
    int      end
);

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 3)
    {
        printf("Arguments not enough\n");
        return -1;
    }

    //argv[argc-1] - Destination

    int length = 0;
    for (int i = 1; i <= argc - 2; i++)
    {
        length += wcslen(argv[i]) + 1;
    }
    length++; // 0x00 0x00

    wchar_t* fwaits = (wchar_t*)malloc(
        length * sizeof(wchar_t));

    connect_wcs_array(fwaits, argv, 1, argc - 2);
    ShellCopy(fwaits, argv[argc - 1]);
    free(fwaits);
    return 0;
}

void ShellCopy(wchar_t* SRC, wchar_t* DST)
{
    int sherr;
    SHFILEOPSTRUCTW op;
    ZeroMemory(&op, sizeof(op));
    op.hwnd = NULL;
    op.wFunc = FO_COPY;
    op.pFrom = SRC;
    op.pTo = DST;
    op.fFlags = 0;
    sherr = SHFileOperationW(&op);
    printf("%x", sherr);
}

void connect_wcs_array(
    wchar_t* buff,
    wchar_t* array[],
    int      begin,
    int      end
)
{
    int i;
    wchar_t* pt = buff;

    for (i = begin; i <= end; i++)
    {
        wcsncpy(pt, array[i], wcslen(array[i]) + 1);
        pt += wcslen(array[i]) + 1;
    }
    *(pt) = L'\0';   // append 0x00
}