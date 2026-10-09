
#include <iostream>
#include <stdio.h>
#include <cstring>       // void *memset(void *s, int ch, size_t n);
#include <sys/types.h>   // 数据类型定义
#include <sys/socket.h>  // 提供socket函数及数据结构sockaddr
#include <arpa/inet.h>   // 提供IP地址转换函数，htonl()、htons()...
#include <netinet/in.h>  // 定义数据结构sockaddr_in
#include <ctype.h>       // 小写转大写
#include <unistd.h>      // close()、read()、write()、recv()、send()...
#include <thread>		 // c++11 thread类
using namespace std;

void requestHandling(const int client_sockfd, const struct sockaddr_in& client_addr);

const int flag = 0; // 0表示读写处于阻塞模式
int port = 8080;
const int buffer_size = 255;

int main(int argc, const char* argv[]){
	if(argc != 2){
		printf("Usage: %s [htons] 服务器 \n", argv[0]);
	}
	port = atoi(argv[1]);
    // 创建服务器监听的套接字。Linux下socket被处理为一种特殊的文件，返回一个文件描述符。
    // int socket(int domain, int type, int protocol);
    // domain设置为AF_INET/PF_INET，即表示使用ipv4地址(32位)和端口号（16位）的组合。
    int server_sockfd = socket(PF_INET,SOCK_STREAM,0);  
    if(server_sockfd == -1){
        close(server_sockfd);
        perror("socket error!");
    }
    // /* Enable address reuse */
    // int on = 1;
    // int ret = setsockopt( server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

    // 此数据结构用做bind、connect、recvfrom、sendto等函数的参数，指明地址信息。
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr)); // 结构体清零
    server_addr.sin_family = AF_INET;  // 协议
    server_addr.sin_port = htons(port);  // 端口16位, 此处不用htons()或者错用成htonl()会连接拒绝!!
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 本地所有IP
    // 另一种写法, 假如是127.0.0.1
    // inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);


    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); 
    // bind()函数的主要作用是把ip地址和端口绑定到套接字(描述符)里面
    // struct sockaddr是通用的套接字地址，而struct sockaddr_in则是internet环境下套接字的地址形式，二者长度一样，都是16个字节。二者是并列结构，指向sockaddr_in结构的指针也可以指向sockaddr。
    // 一般情况下，需要把sockaddr_in结构强制转换成sockaddr结构再传入系统调用函数中。
    if(bind(server_sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
        close(server_sockfd);
        perror("bind error");
    }
    // 第二个参数为相应socket可以排队的准备道来的最大连接个数
    if(listen(server_sockfd, 5) == -1){
        close(server_sockfd);
        perror("listen error");
    }

    printf("Listen on port %d\n", port);
    //while(1){
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        // accept()函数从处于established状态的连接队列头部取出一个已经完成的连接，
        // 如果这个队列没有已经完成的连接，accept()函数就会阻塞当前线程，直到取出队列中已完成的客户端连接为止。
        int client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &client_len);

        
        // 为完成的连接新建立一个线程
        thread tcpThread(requestHandling, client_sockfd, client_addr);
        tcpThread.detach(); // 不阻塞主线程，执行tcp通讯的线程从线程对象分离
		char buf[buffer_size];
		strcpy(buf,"console");
        // 大写串发给客户端
        if(send(client_sockfd, buf, strlen(buf),flag) == -1){
            close(client_sockfd);
            // close(server_sockfd);
            perror("write error");
        }
		//memset(buf,'\0',strlen(buf)); // 清空buf
		printf("$ ");
		while(true){
			cin>>buf;
			if(!strcmp(buf,"")) continue;
			if(send(client_sockfd, buf, strlen(buf),flag) == -1){
				close(client_sockfd);
				// close(server_sockfd);
				perror("write error");
			}
			sleep(1);
		}
    //}
    close(server_sockfd);

    return 0;
}

void requestHandling(const int client_sockfd, const struct sockaddr_in& client_addr){
    char ipbuf[128];
    /* printf("Connect client iP: %s, port: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuf, 
        sizeof(ipbuf)), ntohs(client_addr.sin_port)); */

    // 实现客户端发送小写字符串给服务端，服务端将小写字符串转为大写返回给客户端
    char buf[buffer_size];
		/* strcpy(buf,"console");
        // 大写串发给客户端
        if(send(client_sockfd, buf, strlen(buf),flag) == -1){
            close(client_sockfd);
            // close(server_sockfd);
            perror("write error");
        }
		memset(buf,'\0',strlen(buf)); // 清空buf */
    while(1) {
        // read data, 阻塞读取
		int len = recv(client_sockfd, buf, sizeof(buf),flag);
        if (len == -1) {
            close(client_sockfd);
            // close(server_sockfd);
            perror("read error");
        } else if(len == 0) {  // 这里以len为0表示当前处理请求的客户端断开连接
            break;
        }
        printf("%s",buf);
        memset(buf,'\0',strlen(buf)); // 清空buf
    }
    close(client_sockfd);
    printf("Disconnect client iP: %s, port: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuf, 
        sizeof(ipbuf)), ntohs(client_addr.sin_port));
}
