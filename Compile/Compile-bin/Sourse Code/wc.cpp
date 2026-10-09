#include "wc_cs.h"
int main(int argc, char* argv[])
{
    WSADATA wsaData;
    int ws = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ws != 0) return 0;
    if (argc == 1) { printCWD(); return 0; }
    if ((argc == 2 && argv[1][0] >= '0' && argv[1][0] <= '9') || (argc == 4 && (!strchr(argv[1], '.'))))
    {
        server_(argc, argv);
    }
    else if (argc == 4)
    {
        client_(argc, argv);
    }
    else {
        HELP(argv);
    }
    WSACleanup();
    return 0;
}