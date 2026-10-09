
# def out(fun):
    # def inn():
        # print('--- 2 ---')
        # fun()
    # return inn
# @out
# def sleep():
    # print('--- 1 ---')
# sleep()
# fn = out(sleep)
# fn()

# import time

# def process1(mag):
    # while True:
        # print(mag)
        # time.sleep(1)
 
# def process2():
    # while True:
        # print('2')
        # time.sleep(1)

# import threading

# p1 = threading.Thread(target = process1,args = ("--- 1 ---",))
# p2 = threading.Thread(target = process2)

# p1.start()
# p2.start()



# import socket
# socket_server = socket.socket()
# socket_server.bind(("localhost",8888))
# socket_server.listen(1)
# print('accepting...')
# conn,address = socket_server.accept()
# print("accept:{address}")
# conn.send(("console").encode("UTF-8"))
# while True:
    # msg = input().encode("UTF-8")
    # conn.send(msg)
    # data = conn.recv(1024).decode("UTF-8")
    # print(data)
# conn.close()
# socket_server.close()


import socket
import threading
import time
import sys

# 处理客户端请求的任务
def handel_clientRequest(accept_result):
    # 对元组进行拆包
    new_client_socket = accept_result[0]
    client_IPinfo = accept_result[1]
    # print(f"accept_result:{accept_result}\n客户端ip端口信息为：{client_IPinfo}")
    # print(f"connect:{client_IPinfo}")
    # tcp_server_socket.accept()只接收客户端的连接请求，不收发消息

    # 返回值为元组数据；首值为客户端套接字 二值为客户端的IP地址与端口号
    # 此步代码会堵塞等待客户端的到来

    # 5 接收客户端数据
    # 收发消息都使用新的套接字new_client_socket
    # print("已收取客户端信息")
    while True:
        rec_Data = new_client_socket.recv(256)
        # 进行解码
        rec_Content = rec_Data.decode("GBK")
        print(rec_Content,end='')

        if len(rec_Content) == 0:
            new_client_socket.close()
            print(f"{client_IPinfo}关闭了与服务端的连接")
            break

if __name__ == '__main__':
    try:
        # 1 创建tcp服务套接字
        # socket.AF_INET:ipv4 socket.AF_INET6:ipv6
        tcp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # 设置端口号复用；程序退出时端口号立即销毁
        # socket.SOL_SOCKET当前套接字
        #  socket.SO_REUSEADDR 调用端口复用
        # True 表示确认
        tcp_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)
        # 2 设定端口号
        # 第一个参数为IP地址，一般不指定，为本机IP即可；第二个参数为端口号
        tcp_server_socket.bind(("", int(sys.argv[1])))
        # 3 设置监听
        # 128 表示为等待连接的最大连接数
        tcp_server_socket.listen(1)
        # 4 等待接收客户端的连接请求
        #  设立循环等待客户端的连接请求
        accept_result = tcp_server_socket.accept()
        # 当客户端与服务端建立连接成功，创建子线程，让子线程专门负责接收客户端消息
        sub_thread = threading.Thread(target = handel_clientRequest, args=(accept_result,))
        # 设置主线程守护，主线程销毁，子线程也销毁
        # sub_thread.setDaemon(True)
        # 开启线程
        sub_thread.start()
        conn = accept_result[0]
        conn.send(("console").encode("GBK"))
        print('$',end=' ')
        while True:
            msg = input().encode("GBK")
            if msg == b'exit':
                break
            conn.send(msg)
            time.sleep(1)
        conn.close()
    except Exception as err:
        print(err)
        exit



