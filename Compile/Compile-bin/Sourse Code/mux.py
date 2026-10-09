import socket
import threading
import time
import sys
import os
import struct

running = True

def sendmsg(tcp_client_socket,msg):
    msg = msg.encode("GBK")
    msg_len = struct.pack("i", len(msg))
    tcp_client_socket.send(msg_len)
    tcp_client_socket.send(msg)
    
def recvmsg(tcp_client_socket):
    msg_len_data = tcp_client_socket.recv(4)
    if not msg_len_data:
        return None
    msg_len = struct.unpack("i", msg_len_data)[0]
    msg = tcp_client_socket.recv(msg_len)
    try:
        return msg.decode("GBK")
    except UnicodeDecodeError:
        pass
    encodings_to_try = ["utf-8", "latin-1"]
    for encoding in encodings_to_try:
        try:
            return msg.decode(encoding)
        except UnicodeDecodeError:
            pass
    return msg

def send_space(client_socket):
    msg = ' '
    while True:
        time.sleep(60)
        sendmsg(client_socket,msg)
        
def send_space_cli(client_socket):
    msg = ' '
    msg = msg.encode("GBK")
    while True:
        time.sleep(60)
        client_socket.send(msg)

def handle_serverResponse(client_socket):
    try:
        global running
        while running:
            try:
                rec_data = recvmsg(client_socket)
                if not (rec_data[:len(" ")] == " " and len(rec_data) == 2):
                    print(rec_data,end='')
                if len(rec_data) == 0:
                    client_socket.close()
                    print("服务端关闭了连接")
                    break
            except UnicodeDecodeError:
                pass
    except Exception as err:
        print(err)
        exit

def handle_mux_control(sock, password):
    client_mux_control = 0
    sendmsg(sock, password)
    buffer = recvmsg(sock) # 接收客户端类型
    if buffer is None:
        return
    print(f"----> [{buffer}]\n", end='')
    if buffer[0] == '$': 
        if buffer[1] == '$':
            client_mux_control = 1
            space_thread = threading.Thread(target = send_space_cli, args=(tcp_client_socket,))
            space_thread.daemon = True
            space_thread.start()
        else:
            space_thread = threading.Thread(target = send_space, args=(tcp_client_socket,))
            space_thread.daemon = True
            space_thread.start()
        buffer = recvmsg(sock) # 接收密码
        print(f"----> [{buffer}]\n", end='')
        if len(buffer) >= len("PASSWORD ERROR") and buffer[:len("PASSWORD ERROR")] == "PASSWORD ERROR":
            return
        print("$ ", end='')
        sub_thread = threading.Thread(target = handle_serverResponse, args=(tcp_client_socket,))
        sub_thread.daemon = True
        sub_thread.start()
        
        while True:
            msg = input()
            if msg == ';':
                running = False
                break
            if msg == '':
                print("$ ", end='')
                continue
            if msg == 'cls':
                os.system('cls')
            msg = msg.encode("GBK")
            if client_mux_control == 1:
                tcp_client_socket.send(msg)
            else:
                msg_len = struct.pack("i", len(msg))
                tcp_client_socket.send(msg_len)
                tcp_client_socket.send(msg)
            time.sleep(0.5)

if __name__ == '__main__':
    try:
        ip = 'YOUR_SERVER_IP'
        port = 10561 # -cc
        # port = 10560
        password = 'uuu'
        if len(sys.argv) == 2:
            password = sys.argv[1]
            if password[:len("mux:")] == "mux:":
                port = 10560
        tcp_client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tcp_client_socket.connect((ip, port))
        handle_mux_control(tcp_client_socket, password)
        tcp_client_socket.close()
    except Exception as err:
        print(err)
        exit