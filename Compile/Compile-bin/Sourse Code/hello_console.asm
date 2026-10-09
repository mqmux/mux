; nasm -f win32 hello.asm -o hello_console.obj
; GoLink /console hello_console.obj user32.dll kernel32.dll

global Start
extern _GetStdHandle@4
extern _WriteFile@20
extern _ExitProcess@4

section .data
    msg     db 'Hello, World!', 13, 10   ; 13=回车, 10=换行
    msglen  equ $ - msg

section .bss
    written resd 1

section .text
Start:
    ; 获取标准输出句柄
    push    -11                         ; STD_OUTPUT_HANDLE
    call    _GetStdHandle@4

    ; 调用 WriteFile
    push    0                           ; lpOverlapped
    push    written                     ; lpNumberOfBytesWritten
    push    msglen                      ; nNumberOfBytesToWrite
    push    msg                         ; lpBuffer
    push    eax                         ; hFile (刚才返回的句柄)
    call    _WriteFile@20

    ; 退出程序
    push    0
    call    _ExitProcess@4