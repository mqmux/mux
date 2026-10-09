;nasm -f win32 tcp.asm -o tcp.obj
;GoLink /console tcp.obj kernel32.dll ws2_32.dll

;http 80 方便测试

global Start
extern _WSAStartup@8, _socket@12, _connect@12, _send@16, _closesocket@4, _WSACleanup@0, _ExitProcess@4

section .data
    wsa_data   times 400 db 0
    sock       dd 0
    server_addr:
        sin_family   dw 2          ; AF_INET
        sin_port     dw 0x5000     ; 端口 80 (0x5000 是小端，实际 0x0050 是 80)
        sin_addr     dd 0x0100007F ; 127.0.0.1
        sin_zero     times 8 db 0
    msg        db "GET / HTTP/1.1",13,10,13,10
    msglen     equ $ - msg

section .text
Start:
    push    wsa_data
    push    0x0202                ; 请求 Winsock 2.2
    call    _WSAStartup@8
    test    eax, eax
    jnz     quit

    push    0                     ; protocol (IPPROTO_TCP)
    push    1                     ; type (SOCK_STREAM)
    push    2                     ; af (AF_INET)
    call    _socket@12
    mov     [sock], eax
    cmp     eax, -1
    je      cleanup

    push    16                    ; addrlen
    push    dword server_addr     ; 指向 sockaddr 结构
    push    eax                   ; 套接字句柄
    call    _connect@12
    test    eax, eax
    jnz     close

    push    0
    push    msglen
    push    msg
    push    dword [sock]          ; 显式 dword
    call    _send@16

close:
    push    dword [sock]          ; 显式 dword
    call    _closesocket@4
cleanup:
    call    _WSACleanup@0
quit:
    push    0
    call    _ExitProcess@4