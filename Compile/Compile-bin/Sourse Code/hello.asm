; nasm -f win32 hello.asm -o hello.obj
; GoLink /console hello.obj user32.dll kernel32.dll

global Start          ; 改为 Start
extern _MessageBoxA@16
extern _ExitProcess@4

section .data
    msg     db 'Hello, World!', 0
    title   db 'Success', 0

section .text
Start:                ; 改为 Start
    push 0
    push title
    push msg
    push 0
    call _MessageBoxA@16
    push 0
    call _ExitProcess@4