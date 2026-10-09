#include <windows.h>
int main(int argc, char* argv[])
{
    char* p;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), strtol(argv[1], &p, 16));
    return 0;
}