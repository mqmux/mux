import socket
import threading
import time
import sys
import os

# 定义一个全局变量，用来控制子线程的循环
running = True


def send_space(client_socket):
    while True:
        msg = " ".encode("GBK")
        client_socket.send(msg)
        time.sleep(200)

def handle_serverResponse(client_socket):
    try:
        global running
        while running:
            rec_data = client_socket.recv(1024)
            rec_content = rec_data.decode("gbk", errors="ignore")
            print(rec_content,end='')
            
            if len(rec_content) == 0:
                client_socket.close()
                print("服务端关闭了连接")
                break
    except Exception as err:
        print(err)
        exit
        
if __name__ == '__main__':
    try:
        ip = 'YOUR_SERVER_IP'
        port = 10317
        if len(sys.argv) == 2:
            ip = sys.argv[1]
        if len(sys.argv) == 3:
            port = int(sys.argv[2])
            
        tcp_client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tcp_client_socket.connect((ip, port))
        
        rec_data = tcp_client_socket.recv(8)
        rec_content = rec_data.decode("GBK", errors="ignore")
        print(f"----> [{rec_content}]\n",end='')
        
        sub_thread = threading.Thread(target = handle_serverResponse, args=(tcp_client_socket,))
        sub_thread.daemon = True
        sub_thread.start()
        
        space_thread = threading.Thread(target = send_space, args=(tcp_client_socket,))
        space_thread.start()
        
        print('$',end=' ')
        while True:
            msg = input()
            if msg == ';':
                running = False
                break
            if msg == 'cls':
                os.system('cls')
            msg = msg.encode("GBK")
            # ------------------------
            msg_len = struct.pack("i", len(msg))
            tcp_client_socket.send(msg_len)
            # ------------------------
            tcp_client_socket.send(msg)
            time.sleep(0.5)
        tcp_client_socket.close()
    except Exception as err:
        print(err)
        exit
        