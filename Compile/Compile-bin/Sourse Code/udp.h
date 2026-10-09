#ifndef UDP_H
#define UDP_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

WSADATA wsaData;
sockaddr_in serverAddr;
SOCKET soc;
char buffer[1024] = { 0 };

int serverbind(char* port) {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.");
        return 1;
    }
    soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (soc == INVALID_SOCKET) {
        printf("Failed to create socket.");
        WSACleanup();
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port)); // UDP端口
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(soc, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed.");
        closesocket(soc);
        WSACleanup();
    }
    u_long mode = 1;  // 1 to enable non-blocking socket
    ioctlsocket(soc, FIONBIO, &mode);
    return 0;
}

int clientbind(char* ip, char* port) {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.");
        return 1;
    }
    soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (soc == INVALID_SOCKET) {
        printf("Failed to create socket.");
        WSACleanup();
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port)); // 服务端UDP端口
    serverAddr.sin_addr.s_addr = inet_addr(ip); // 服务端IP地址
    u_long mode = 1;  // 1 to enable non-blocking socket
    ioctlsocket(soc, FIONBIO, &mode);
    return 0;
}

int sendmsg(char* buffer) {
    sendto(soc, buffer, strlen(buffer), 0, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    return 0;
}

int recvmsg(char* buffer) {
    int serverAddrSize = sizeof(serverAddr);
    int bytesReceived = recvfrom(soc, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&serverAddr), &serverAddrSize);
    if (bytesReceived == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            printf("Receive failed with error %d.", error);
        }
    }
    else {
        buffer[bytesReceived] = '\0'; // Add null terminator to received string
        printf("%s\n", buffer);
    }
    return 0;
}

#endif // !UDP_H

