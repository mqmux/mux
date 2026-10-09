@echo off
REM type %~dp0TP.ts
more +17 "%~dpn0.bat"
echo;
printf 0x0c [1]编译并运行 
printf 0x0e [2]编译 
printf 0x09 [3]运行 
printf 0x02 [4]重选文件
printf 0x0b [5]添加ico图标 
echo;
printf 0x03 [Q]Console
printf 0x06 [W]-lwsock
printf 0x07 [E]Path
printf 0x0d [6]添加文件 
printf 0x0f "[7]中止程序 "
goto :eof

     ___                      _ _      
    / __\___  _ __ ___  _ __ (_) | ___ 
   / /  / _ \| '_ ` _ \| '_ \| | |/ _ \
  / /__| (_) | | | | | | |_) | | |  __/
  \____/\___/|_| |_| |_| .__/|_|_|\___|
                       |_|          