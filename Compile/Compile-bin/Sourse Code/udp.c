#include "udp.h"

int main(int argc, char* argv[]){
    if (argc == 2) {
        serverbind(argv[1]);
        while (1) {
            printf("server recv...\n");
            recvmsg(buffer);
            Sleep(1000);
        }
    }
    else if(argc == 3){
        clientbind(argv[1], argv[2]);
        strcpy(buffer, "test");
        printf("cli sned...\n");
        sendmsg(buffer);
    }
    closesocket(soc);
    WSACleanup();
    return 0;
}