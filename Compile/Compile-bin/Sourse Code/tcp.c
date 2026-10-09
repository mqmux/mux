#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
bool isIPAddressValid(const char* pszIPAddr)
{
	if (!pszIPAddr) return false; //若pszIPAddr为空  
	char IP1[100], cIP[4];
	int len = strlen(pszIPAddr);
	int i = 0, j = len - 1;
	int k, m = 0, n = 0, num = 0;
	//去除首尾空格(取出从i-1到j+1之间的字符):  
	while (pszIPAddr[i++] == ' ');
	while (pszIPAddr[j--] == ' ');

	for (k = i - 1; k <= j + 1; k++)
	{
		IP1[m++] = *(pszIPAddr + k);
	}
	IP1[m] = '\0';

	char* p = IP1;

	while (*p != '\0')
	{
		if (*p == ' ' || *p < '0' || *p>'9') return false;
		cIP[n++] = *p; //保存每个子段的第一个字符，用于之后判断该子段是否为0开头  

		int sum = 0;  //sum为每一子段的数值，应在0到255之间  
		while (*p != '.' && *p != '\0')
		{
			if (*p == ' ' || *p < '0' || *p>'9') return false;
			sum = sum * 10 + *p - 48;  //每一子段字符串转化为整数  
			p++;
		}
		if (*p == '.') {
			if ((*(p - 1) >= '0' && *(p - 1) <= '9') && (*(p + 1) >= '0' && *(p + 1) <= '9'))//判断"."前后是否有数字，若无，则为无效IP，如“1.1.127.”  
				num++;  //记录“.”出现的次数，不能大于3  
			else
				return false;
		};
		if ((sum > 255) || (sum > 0 && cIP[0] == '0') || num > 3) return false;//若子段的值>255或为0开头的非0子段或“.”的数目>3，则为无效IP  

		if (*p != '\0') p++;
		n = 0;
	}
	if (num != 3) return false;
	return true;
}

int main(int argc, char* argv[])
{
	if (isIPAddressValid(argv[1])) {
		char ServIP[50];//服务器IP地址
		strcpy(ServIP, argv[1]);
		int port = atoi(argv[2]);
		//加载套接字
		WSADATA wsaData;
		char buff[1024];

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			//printf("Failed to load Winsock");
			return 0;
		}

		SOCKADDR_IN addrSrv;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		addrSrv.sin_addr.S_un.S_addr = inet_addr(ServIP);

		//创建套接字
		SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
		if (SOCKET_ERROR == sockClient) {
			//printf("Socket() error:%d", WSAGetLastError());
			return 0;
		}

		//向服务器发出连接请求
		if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET) {
			//printf("Connect failed:%d", WSAGetLastError());
			return 0;
		}
		else
		{
			//发送数据
			memset(buff, 0, sizeof(buff));
			for (int i = 3; i < argc; i++) {
				strcat(buff, argv[i]);
				if (i != argc - 1) strcat(buff, " ");
			}
			//strcpy(buff, argv[3]);
			send(sockClient, buff, strlen(buff) + 1, 0);
			memset(buff, 0, sizeof(buff));
		}
		//接收数据
		recv(sockClient, buff, sizeof(buff), 0);
		printf("%s", buff);


		//关闭套接字
		closesocket(sockClient);
		WSACleanup();
	}
	else if (argc > 1 && argv[1][0] > 47 && argv[1][0] < 58) {
		WSADATA wsaData;
		int port = atoi(argv[1]);
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		for (int i = 2; i < argc; i++) {
			strcat(buf, argv[i]);
			if (i != argc - 1) strcat(buf, " ");
		}
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			//printf("Failed to load Winsock");
			return 0;
		}

		//创建用于监听的套接字
		SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN addrSrv;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port); //1024以上的端口号
		addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
		if (retVal == SOCKET_ERROR) {
			//printf("Failed bind:%d\n", WSAGetLastError());
			return 0;
		}

		if (listen(sockSrv, 10) == SOCKET_ERROR) {
			//printf("Listen failed:%d", WSAGetLastError());
			return 0;
		}

		SOCKADDR_IN addrClient;
		int len = sizeof(SOCKADDR);

		//等待客户请求到来    
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		if (sockConn == SOCKET_ERROR) {
			//printf("Accept failed:%d", WSAGetLastError());
			//break;
		}

		//printf("Accept:[%s]\n", inet_ntoa(addrClient.sin_addr));
		char recvBuf[100];
		memset(recvBuf, 0, sizeof(recvBuf));
		//         //接收数据
		recv(sockConn, recvBuf, sizeof(recvBuf), 0);
		printf("%s", recvBuf);

		//发送数据
		int iSend = send(sockConn, buf, sizeof(buf), 0);
		if (iSend == SOCKET_ERROR) {
			//printf("send failed");
			// break;
		}

		closesocket(sockConn);
		closesocket(sockSrv);
		WSACleanup();
	}
	else {
		printf("Usage: %s [htons] [msg]服务器 \n", argv[0]);
		printf("   or: %s [ip] [htons] [msg]客户端\n", argv[0]);
	}
	return 0;
}
