#include "mux_rsa.h"
#include "mux_aes.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define MAX_SOCKETS 10
#define PORT 6666

void mux_rsa() {
    uint64_t n, e, d;
    generate_keys(&n, &e, &d);
    printf("公钥: (n: %" PRIu64 ", e: %" PRIu64 ")\n", n, e);
    printf("私钥: (n: %" PRIu64 ", d: %" PRIu64 ")\n", n, d);
    const char* plaintext = "yyds";
    size_t length = strlen(plaintext);
    uint64_t* encrypted = (uint64_t*)malloc(length * sizeof(uint64_t));
    encrypt_string(plaintext, e, n, encrypted, length);
    printf("加密后: ");
    for (size_t i = 0; i < length; i++) {
        printf("%" PRIu64 " ", encrypted[i]);
    }
    printf("\n");
    char* decrypted = (char*)malloc(length + 1);
    /*encrypted[0] = 6440;
    encrypted[1] = 20482;
    encrypted[2] = 3876;
    n = 35239;
    d = 24593;*/
    decrypt_string(encrypted, length, d, n, decrypted);
    printf("解密后: %s\n", decrypted);
    free(encrypted);
    free(decrypted);
}

void mux_aes() {
    uint8_t pt[16] = { 0 }; // 明文
    uint8_t ct[16] = { 0 };     // 用于加密后的数据
    uint8_t plain[16] = { 0 };  // 用于解密后的数据
    uint8_t key[16] = { 0 }; // 密钥
    pt[0] = 'p';
    pt[1] = 't';
    pt[2] = '6';
    key[0] = 'k';
    key[1] = 'e';
    key[2] = 'y';

    aesEncrypt(key, 16, pt, ct, 16); // 加密
    //printHex(ct, 16);
    for (int i = 0; i < 16; i++)
        printf("%c", ct[i]);
    printf("\n");

    aesDecrypt(key, 16, ct, plain, 16);// 解密
    for (int i = 0; i < 16; i++)
        printf("%c",plain[i]);
    printf("\n");
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        printf("socket() 失败，错误码 %d\n", WSAGetLastError());
        return 1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("bind() 失败，错误码 %d\n", WSAGetLastError());
        closesocket(server);
        WSACleanup();
        return 1;
    }

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen() 失败，错误码 %d\n", WSAGetLastError());
        closesocket(server);
        WSACleanup();
        return 1;
    }

    SOCKET sockets[MAX_SOCKETS]; //存放sockets
    WSAPOLLFD fdArray[MAX_SOCKETS]; //fdArray[i].fd = server;
    for (int i = 0; i < MAX_SOCKETS; ++i) {
        sockets[i] = INVALID_SOCKET;
    }

    // 将监听套接字添加到 fdArray 中
    fdArray[0].fd = server;
    fdArray[0].events = POLLRDNORM;
    fdArray[0].revents = 0;
    int nfds = 1;  // 包括监听套接字

    while (1) {
        int ret = WSAPoll(fdArray, nfds, 5000);  // 最多等待 5000 毫秒
        printf("WSAPoll ret = %d\n", ret);
        if (ret == SOCKET_ERROR) {
            printf("WSAPoll 失败，错误码 %d\n", WSAGetLastError());
            break;
        }
        else if (ret == 0) {
            printf("WSAPoll 超时.\n");
        }
        else {
            // 处理事件
            if (fdArray[0].revents & POLLRDNORM) {
                // 处理新的客户端连接
                sockaddr_in clientAddr;
                int clientAddrLen = sizeof(clientAddr);
                SOCKET clientSocket = accept(server, (sockaddr*)&clientAddr, &clientAddrLen);
                if (clientSocket == INVALID_SOCKET) {
                    printf("accept() 失败，错误码 %d\n", WSAGetLastError());
                }
                else {
                    // 将新的客户端套接字添加到 sockets 和 fdArray 中
                    for (int j = 0; j < MAX_SOCKETS; ++j) {
                        if (sockets[j] == INVALID_SOCKET) {
                            sockets[j] = clientSocket;
                            fdArray[nfds].fd = clientSocket;
                            fdArray[nfds].events = POLLRDNORM;
                            fdArray[nfds].revents = 0;
                            ++nfds;
                            break;
                        }
                    }
                    printf("fdArray[%d].fd = %d,nfds = %d \n", nfds, clientSocket, nfds);
                }
            }

            for (int i = 1; i < nfds; ++i) {
                if (fdArray[i].revents & POLLRDNORM) {
                    char buffer[1024];
                    int bytesReceived = recv(fdArray[i].fd, buffer, sizeof(buffer), 0);
                    if (bytesReceived > 0) {
                        printf("bytesReceived = %d;接收到数据: %.*s\n", bytesReceived, bytesReceived, buffer);
                        //send(fdArray[i].fd, buffer, bytesReceived, 0);
                    }
                    else if (bytesReceived == 0 || WSAGetLastError() == WSAECONNRESET) {
                        // 客户端断开连接或出现错误
                        printf("客户端断开连接: 套接字 %d\n", fdArray[i].fd);
                        closesocket(fdArray[i].fd);
                        sockets[i] = INVALID_SOCKET;
                        // 从 fdArray 中移除该套接字
                        fdArray[i] = fdArray[nfds - 1];
                        --nfds;
                        --i;  // 调整索引，以正确处理下一个元素
                    }
                }
            }
        }
    }

    for (int i = 0; i < nfds; ++i) {
        if (fdArray[i].fd != INVALID_SOCKET) {
            closesocket(fdArray[i].fd);
        }
    }

    WSACleanup();
    return 0;
}

