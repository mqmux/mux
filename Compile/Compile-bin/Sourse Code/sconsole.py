import socket
import struct
import threading
import time
import sys

def handle_serverResponse(client_socket):
    while True:
        rec_data = client_socket.recv(1024)
        rec_content = rec_data.decode("GBK")
        print(rec_content,end='')
        
        if len(rec_content) == 0:
            client_socket.close()
            print("服务端关闭了连接")
            break

if __name__ == '__main__':
    try:
        tcp_client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tcp_client_socket.connect((sys.argv[1], int(sys.argv[2])))
        
        rec_data = tcp_client_socket.recv(8)
        rec_content = rec_data.decode("GBK")
        print(f"----> [{rec_content}]\n",end='')
        
        sub_thread = threading.Thread(target = handle_serverResponse, args=(tcp_client_socket,))
        sub_thread.start()
        print('$',end=' ')
        while True:
            msg = input()
            msg = msg.encode("GBK")
            tcp_client_socket.send(msg)
            time.sleep(2)
        tcp_client_socket.close()
    except Exception as err:
        print(err)
        exit
        