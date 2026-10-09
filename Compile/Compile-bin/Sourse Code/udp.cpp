#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
	int Ser_htons;
	WORD wVersionRequested;
	WSADATA wsaData;
	if( argc == 3 ){
		Ser_htons = atoi(argv[1]);
		//加载套接字库
		int err;
		wVersionRequested = MAKEWORD(1,1);
		err = WSAStartup(wVersionRequested, &wsaData);//错误会返回WSASYSNOTREADY
		if(err != 0)
		{
			return 0;
		}
		if(LOBYTE(wsaData.wVersion) != 1 ||     //低字节为主版本
			HIBYTE(wsaData.wVersion) != 1)      //高字节为副版本
		{
			WSACleanup();
			return 0;
		}
		//创建用于监听的套接字
		SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);//失败会返回 INVALID_SOCKET
		//printf("Failed. Error Code : %d",WSAGetLastError())//显示错误信息
	 
		SOCKADDR_IN addrSrv;     //定义sockSrv发送和接收数据包的地址
		addrSrv.sin_addr.S_un.S_addr = INADDR_ANY;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(Ser_htons);
		
		//绑定套接字, 绑定到端口
		bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//会返回一个SOCKET_ERROR
		//将套接字设为监听模式， 准备接收客户请求
		
		SOCKADDR_IN addrClient;   //用来接收客户端的地址信息
		int len = sizeof(SOCKADDR);
		char recvBuf[100];    //收
		char sendBuf[100];    //发
				
		recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&len);
		printf("%s",recvBuf);
	 
		//发送数据
		strcpy(sendBuf, argv[2]);
		sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addrClient,len);
		
		closesocket(sockSrv);
		WSACleanup();
	} else if( argc == 4 ){
		char ServIP[50];//服务器IP地址
		strcpy(ServIP, argv[1]);
		Ser_htons = atoi(argv[2]);
		int err;
		wVersionRequested = MAKEWORD(1,1);
		err = WSAStartup(wVersionRequested, &wsaData);
		if(err != 0)
		{
			return 0;
		}
	 
		if(LOBYTE(wsaData.wVersion) != 1 ||     //低字节为主版本
			HIBYTE(wsaData.wVersion) != 1)      //高字节为副版本
		{
			WSACleanup();
			return 0;
		}
		//创建用于监听的套接字
		SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);
		
		sockaddr_in  addrSrv;
		addrSrv.sin_addr.S_un.S_addr = inet_addr(ServIP);//输入你想通信的她（此处是本机内部）
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(Ser_htons);
		
		
		int len = sizeof(SOCKADDR);
	 
		char recvBuf[100];    //收
		char sendBuf[100];    //发
		strcpy(sendBuf, argv[3]);
		sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addrSrv,len);
		
		//等待并数据
		recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrSrv,&len);
		printf("%s",recvBuf);
		
		closesocket(sockSrv);
		WSACleanup();
	} else {
		printf("Usage: %s [htons] [msg]服务器 \n", argv[0]);
        printf("   or: %s [ip] [htons] [msg]客户端\n", argv[0]);
	}
	return 0;
}
 
