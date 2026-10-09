#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "mux.h"
#define help "\
Usage: %s [port][key][path] server %s %s\n\
       %s [ip][port] [ll|pull|rm|push] [path|file] client\n"
char* filenames[1024];
char* add_filenames[FILE_SIZE];
constexpr unsigned int hash(const char* str, int h = 0) {
    return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
}
struct tm* sysTime_t = NULL;
void Now_TIME(time_t now) {
    sysTime_t = localtime(&now);
}
void _status_send(SOCKET client) { //服务端发送
    int file_cnt = 0;
    //std::ifstream file;
    uint32_t crc;
    LoadGitignore(); //读取 .gitignore
    if (ListFiles(".", ".", filenames, file_cnt, 0)) file_cnt = 1024;
    SEND(client, file_cnt);//发送文件数量
    //printf("$ git status %d\n", file_cnt);
    char buf[SEND_SIZE];
    for (int i = 0; i < file_cnt; i++) {
        std::ifstream file(filenames[i], std::ios::binary);
        uint32_t crc = crc32(file);
        //printf("%-8x %s\n", crc, filenames[i]); fflush(stdout);
        sprintf(buf, "%-8x %s", crc, filenames[i]);
        SEND(client, buf);
        delete[] filenames[i];
    }
}
void _pull_send(SOCKET client) {
    int recv_file_cnt = RECV(client);//接收文件数量
    //printf("$ git pull %d\n", recv_file_cnt);
    char buf[SEND_SIZE];
    printf("\n"); fflush(stdout);
    for (int i = 0; i < recv_file_cnt; i++) {
        RECV(client, buf);
        printf("%4d - %s\n", i + 1, buf); fflush(stdout);
        SendFile(client, buf);
    }
    printf("\n"); fflush(stdout);
}
int _status_recv(SOCKET client) { //客户端接收
    int file_cnt = RECV(client);//接收文件数量
    char recvbuf[RECV_SIZE];
    char recv_crc[16];
    int dif = 0;
    for (int i = 0; i < file_cnt; i++) {
        //memset(recvbuf,0,sizeof(recvbuf));
        int ret = RECV(client, recvbuf);//接收key
        std::ifstream file(recvbuf + 9, std::ios::binary);
        recvbuf[8] = '\0';
        if (file.is_open()) {
            uint32_t crc = crc32(file);
            sprintf(recv_crc, "%-8x", crc);
            if (strcmp(recvbuf, recv_crc)) {
                printf("%s %s ", recvbuf, recv_crc);
                printf("* ");
                filenames[dif] = new char[FILENAME_MAX];
                strcpy(filenames[dif], recvbuf + 9);
                dif++;
                printf("%s\n", recvbuf + 9); fflush(stdout);
            }
            else {
                //printf("%s %s ", recvbuf, recv_crc);
                //printf("  ");//相同
                //printf("%s\n", recvbuf + 9);
            }
        }
        else {
            filenames[dif] = new char[FILENAME_MAX];
            strcpy(filenames[dif], recvbuf + 9);
            dif++;
            printf("%s 00000000 + %s\n", recvbuf, recvbuf + 9); fflush(stdout);//不存在
        }
        fflush(stdout);
    }
    return dif;
}
void _pull_recv(SOCKET client, int dif) {
    printf("\n"); fflush(stdout);
    SEND(client, dif);
    for (int i = 0; i < dif; i++) {
        printf("%4d - %s\n", i + 1, filenames[i]); fflush(stdout);
        SEND(client, filenames[i]);
        RecvFile(client);
        delete[] filenames[i];
    }
}
BOOL key_cmp_S(SOCKET client, char* key) {
    char recvbuf[RECV_SIZE];
    RECV(client, recvbuf);//接收key
    //printf("RECV:%s == Key:%s\n", recvbuf,key);
    if (strcmp(recvbuf, key)) {  //对比key
        printf("Password error: %s\n", recvbuf); fflush(stdout);
        SEND(client, "0");
        //printf("0:%s\n", recvbuf);
        return 1;
    }
    else {
        SEND(client, "1");
        //printf("1:%s\n", recvbuf);
        return 0;
    }
}
BOOL key_cmp_C(SOCKET client, char* key) {
    SEND(client, key);//发送key
    RECV(client, key);//接收key
    if (key[0] == '0') {
        printf("ERROR 0 \n"); fflush(stdout);
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf(help, argv[0], __TIME__, __DATE__, argv[0]); fflush(stdout);
        return 0;
    }
    
    if (argc > 1 && argv[1][1] != '.' && argv[1][2] != '.' && argv[1][3] != '.' && argv[1][0] >= '0' && argv[1][0] <= '9') {
        if (argc == 2) {
            printf(help, argv[0], __TIME__, __DATE__, argv[0]); fflush(stdout);
            return 1;
        }
        SOCKET server = init_server(atoi(argv[1]));
        SOCKET client;
        char recvbuf[RECV_SIZE];
        if (argc == 4) SetCurrentDirectoryA(argv[3]);
        while (1) {
            Now_TIME(time(NULL));
            printf("[%d.%02d.%02d %02d:%02d:%02d.%03d]$ ",
                1900 + sysTime_t->tm_year, 1 + sysTime_t->tm_mon, sysTime_t->tm_mday,
                sysTime_t->tm_hour, sysTime_t->tm_min, sysTime_t->tm_sec, (int)(time(NULL) % 1000)
            ); fflush(stdout);
            listen(server, SOMAXCONN); //listen
            sockaddr_in remoteAddr;
            int nAddrlen = sizeof(remoteAddr);
            client = accept(server, (SOCKADDR*)&remoteAddr, &nAddrlen); //accept
            recvbuf[0] = '\0';
            RECV(client, recvbuf);//接收cmd
            printf("%s\n", recvbuf);
            switch (hash(recvbuf))
            {
            case hash("rm"):
            {
                if (key_cmp_S(client, argv[2])) continue;
                RECV(client, recvbuf);//接收rm
                DWORD dwAttrib = GetFileAttributes(recvbuf);
                if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
                    SEND(client, "Not found !");
                    DWORD dwError = GetLastError();
                }
                else if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
                    if (!RemoveDirectory(recvbuf)) {
                        SEND(client, "Failed to delete folder ! ");
                        DWORD dwError = GetLastError();
                    }
                    else SEND(client, "Delete folder successfully !");
                }
                else {
                    if (!DeleteFile(recvbuf)) {
                        SEND(client, "Failed to delete file ! ");
                        DWORD dwError = GetLastError();
                    }
                    else SEND(client, "Deleted file successfully ! ");
                }
                break;
            }
            case hash("pull"):
            {
                int pull_mode = RECV(client);
                if (pull_mode == 4) {
                    RECV(client, recvbuf);
                    int file_cnt = 0;
                    BOOL isSendFile = false;
                    LoadGitignore(); //读取 .gitignore
                    if (ListFiles(".", ".", filenames, file_cnt, 0)) file_cnt = 1024;
                    for (int i = 0; i < file_cnt; i++) {
                        //printf("%s,%s\n", recvbuf, filenames[i]);
                        if (!strcmp(recvbuf, filenames[i])) {
                            isSendFile = true;
                            break;
                        }
                        delete[] filenames[i];
                    }
                    if (isSendFile) {
                        printf("SendFile: %s\n", recvbuf);
                        SEND(client, 1);
                        SendFile(client, recvbuf);
                    }
                    else {
                        printf("Not Find %s\n", recvbuf);
                        SEND(client, 0);
                    }
                }
                else if (pull_mode == 3) {
                    _status_send(client);
                    _pull_send(client);
                }
                break;
            }
            case hash("push"):
            {
                if (key_cmp_S(client, argv[2])) continue;
                _RecvFile(client);
                break;
            }
            case hash("ll"):
            {
                int pull_mode = RECV(client);
                if (pull_mode == 4) {
                    RECV(client, recvbuf);
                    std::ifstream file(recvbuf, std::ios::binary | std::ios::ate);
                    std::streamsize size = file.tellg();
                    file.seekg(0, std::ios::beg);
                    uint32_t crc = crc32(file);
                    char buf[SEND_SIZE];
                    sprintf(buf, "%-8x %s", crc, recvbuf);
                    printf("%s [%lldB]\n", buf, static_cast<long long>(size)); fflush(stdout);
                    SEND(client, buf);
                    sprintf(buf, "%lld", static_cast<long long>(size));
                    SEND(client, buf);
                }
                else if (pull_mode == 3) {
                    _status_send(client);
                }
                break;
            }
            default:
                printf("Error parameter: %s\n", recvbuf);
                break;
            }
            closesocket(client);
        }
    }
    else { // client ip port [ll] [path/file]
        char recvbuf[RECV_SIZE];
        char path[256];

        SOCKET client = init_client(argv[1], atoi(argv[2]));

        SEND(client, argv[3]);//发送 cmd
        switch (hash(argv[3]))
        {
        case hash("rm"):
            if (argc != 6) break;
            if (key_cmp_C(client, argv[5])) return 1;
            SEND(client, argv[4]);
            char ttt[256];
            RECV(client, ttt);//接收rm
            printf("%s\n", ttt);
            break;
        case hash("pull"):
        {
            if (argc == 5) {
                SEND(client, 4);
                SEND(client, argv[4]);
                if (RECV(client)) {
                    printf("RecvFile: %s\n", argv[4]);
                    RecvFile(client);
                }
                else printf("Not Find %s\n", argv[4]);
            }
            else {
                SEND(client, 3);
                _pull_recv(client, _status_recv(client));
            }
            break;
        }
        case hash("push"):
        {
            if (argc != 6) break;
            if (key_cmp_C(client, argv[5])) return 1;
            _SendFile(client, argv[4]);
            break;
        }
        case hash("ll"):
        {
            if (argc == 5) {
                SEND(client, 4);
                SEND(client, argv[4]);
                int ret = RECV(client, recvbuf);//接收key
                //std::ifstream file(recvbuf + 9, std::ios::binary);
                std::ifstream file(recvbuf + 9, std::ios::binary | std::ios::ate);
                std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);
                recvbuf[8] = '\0';
                char recv_crc[16];
                if (file.is_open()) {
                    uint32_t crc = crc32(file);
                    sprintf(recv_crc, "%-8x", crc);
                    if (strcmp(recvbuf, recv_crc)) {
                        printf("%s %s ", recvbuf, recv_crc);
                        printf("* ");
                    }
                    else {
                        printf("%s %s ", recvbuf, recv_crc);
                        printf("# ");//相同
                    }
                }
                else {
                    printf("%s 00000000 + ", recvbuf); fflush(stdout);//不存在
                }
                printf("%s (%lldB) ", recvbuf + 9, static_cast<long long>(size));
                RECV(client, recvbuf);
                printf("[%sB]\n", recvbuf); fflush(stdout);
            }
            else {
                SEND(client, 3);
                _status_recv(client);
            }
            break;
        }
        default:
            printf(help, argv[0], __TIME__, __DATE__, argv[0]); fflush(stdout);
            break;
        }
    }
    return 0;
}