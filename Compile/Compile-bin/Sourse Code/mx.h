#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <thread>
#include <direct.h>
#include "mux_rsa.h"
#pragma comment(lib, "ws2_32.lib")
#define MAX_SOCKETS 32

struct muxClient
{
    char name[1024];
    char passwdmd5[256];
    time_t current_time;
};

muxClient muxClients[MAX_SOCKETS];
WSAPOLLFD fdArray[MAX_SOCKETS];
int fdArray_size = 1;
char buffer[1024];
bool bl_client = true;
char mux_history[4096] = { 0 };
int mux_selected = 1;

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
    char* msg = _strdup(initial_msg);
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
    int pid = _getpid();
    char pid_str[20];
    snprintf(pid_str, 20, "%d", pid);
    char seed[40];
    snprintf(seed, 40, "%s%s", timestamp, pid_str);
    static char md5_hash[33];
    md5(seed, md5_hash);
    return md5_hash;
}

void mux_rsa(uint64_t e, uint64_t n, size_t length, const char* plaintext, char* buf) {
    uint64_t* encrypted = (uint64_t*)malloc(length * sizeof(uint64_t));
    encrypt_string(plaintext, e, n, encrypted, length);
    char tmp_encrypted[16];
    buf[0] = 0;
    for (size_t i = 0; i < length; i++) {
        sprintf(tmp_encrypted, "%llx ", encrypted[i]);
        strcat(buf, tmp_encrypted);
    }
    free(encrypted);
}

void mux_dexc(uint64_t d, uint64_t n, int length, char* decrypted, char* buf) {
    uint64_t* encrypted = (uint64_t*)malloc(length * sizeof(uint64_t));
    char* token = strtok(buf, " ");
    size_t count = 0;
    while (token != NULL && count < length) {
        sscanf(token, "%llx", (unsigned long long*) & encrypted[count]); // 将十六进制字符串转换为uint64_t
        token = strtok(NULL, " "); count++;
    }
    decrypt_string(encrypted, length, d, n, decrypted);
    free(encrypted);
}

void remove_socket(int i) {
    time_t remove_socket_t = time(nullptr);
    struct tm* local_time = localtime(&remove_socket_t);
    char time_string[100];
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", local_time);
    sprintf(buffer, "[%s] %-3d - #%d %s", time_string, fdArray[i].fd, i, muxClients[i].name);
    closesocket(fdArray[i].fd);
    while ((strlen(mux_history) + strlen(buffer) + 2) > 4096) {
        char* pos = strchr(mux_history, '\n');
        if (pos != nullptr) {
            int index = pos - mux_history + 1; // +1 to remove the '\n'
            strcpy(mux_history, mux_history + index);
        }
    }
    strcat(mux_history, buffer);
    strcat(mux_history, "\n\0");
    printf("%s\n", buffer);

    if (i != fdArray_size - 1) {
        fdArray[i] = fdArray[fdArray_size - 1];
        strcpy(muxClients[i].name, muxClients[fdArray_size - 1].name); muxClients[fdArray_size - 1].name[0] = 0;
        strcpy(muxClients[i].passwdmd5, muxClients[fdArray_size - 1].passwdmd5); muxClients[fdArray_size - 1].passwdmd5[0] = 0;
        muxClients[i].current_time = muxClients[fdArray_size - 1].current_time;
    } --fdArray_size;
}

