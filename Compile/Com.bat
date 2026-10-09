@echo off & setlocal enabledelayedexpansion
title Compile
path=%~dp0Compile-bin\Sourse Com;%path%
path=%~dp0Compile-bin;%path%
seta -a 180 2>nul>nul
sico "%~dp0Home\empty.ico" 2>nul>nul
printf
set LANG=zh_CN
set path_tmp=%path%
set "gcc-v=%~dp0..\..\FILE\GCC\bin"
path=%gcc-v%;%path%
set "local_path=%~dp0"
modes 55 9
if /i "%~x1" == ".exe" start time_start %1 &exit

if "%~x1" == ".ico" ( 
	call icon %2 %1 
	call :names %2 
	goto lip
) Else if "%~x2" == ".ico" (
	call icon %1 %2
	call :names %1
	goto lip
) 

if not "%~1" == "" (
	:loop
	if not exist "%~1" (
		echo;未找到指定文件,创建%1中...
		call file_name "%~1"
		if !errorlevel! equ 404 echos 0x0e [%~nx1]不是可编译文件 &exit /b
		echos 0x0e 创建成功
		call :names %1
		pause
		goto lip
	)
	call :names %1
	if !errorlevel! equ 404 echos 0x0e [%~nx1]不是可编译文件 &exit /b
	if "%~2"=="" goto lip
	call compile %1
	echo. & shift & goto loop
) else (
	cd /d %~dp0Home
	:input
	REM cls
	printf 0x0a vcc@COM-FIRST
	printf 0x09 [!cd!]
	echo;$ ll !cd!
	ls --color=auto !cd!
	echo;
	echos 0x0c 本程序只支持.c和.cpp两种文件的编译
	set input=
	set /p "input=请输入或拖入文件名:"
	if "!input!"=="" (
		call choose input
		if "!input!"=="" (
			echos 0x0c 未选择文件
			exit /b
		)
	)
	set "input=!input:"=!"
	if not exist "!input!" (
		echo;未找到指定文件,创建!input!中...
		call file_name "!input!"
		if !errorlevel! equ 404 pause&goto :input
		echos 0x0e 创建成功
		pause
	)
	call :names "!input!"
	if !errorlevel! equ 404 pause&goto :input
)

:lip 
cls & call menu
choice /C 1234567QWE /N
if %errorlevel%==1 cls & call compile "%input%" & if !errorlevel! equ 1 (
	start time_start "%input%" & pause & goto lip 
) else ( pause & goto lip ) 
if %errorlevel%==3 start time_start "%input%" & goto lip
if %errorlevel%==2 cls & call compile "%input%" & pause & goto lip
if %errorlevel%==4 set input=& goto input
if %errorlevel%==7 (
	taskkill /f /fi "WINDOWTITLE eq !name!.exe"
	Taskkill /f /im cmd.exe /fi "Windowtitle ne %name_com% - Compile"
	taskkill /f /im "!name!.exe"
	goto lip
)
if %errorlevel%==5 (
	cls
	set icon=
	set /p icon=请输入ico全名[可拖入]:
	if "!icon!"=="" call choose icon
	if "!icon!"=="" goto lip
	call icon "%input%" "!icon!" 
	goto lip
)
if %errorlevel%==6 (
	cls
	set /p new_file=请输入新建文件名: 
	if "!new_file!"=="" goto lip
	set /p=<nul>"!new_file!"
	goto lip
)
if %errorlevel%==8 start cmd /c "%local_path%..\Termux.bat" &goto lip
if %errorlevel%==9 (
	cls 
	echo;[ -lwsock32 -static -libgcc -lgdi32 -m64 ]
	echo;[ -fexec-charset=GBK -mwindows 不显示控制台 ]
	echo;[ gcc -shared -o export.dll export.c ]
	echo;[ gcc -o main.exe main.c -L./ -lexport ]
	echo;&echo;当前编译参数:%lwsock32% &echo;&set /p lwsock32=请输入新的编译参数:
	goto lip
)

if "%errorlevel%"=="10" (
	cls & echos 0x03 当前使用的gcc路径:
	echos 0x0e %gcc-v%
	echo;
	set /p gcc-v=请输入新的gcc路径:
	if "!gcc-v!" == "1" set gcc-v=!gcc-v-1!
	if "!gcc-v!" == "2" set gcc-v=!gcc-v-2!
	if "!gcc-v!" == "3" set gcc-v=!gcc-v-3!
	set path=!path_tmp!
	path=!gcc-v!;!path!
)
goto lip

:names
	if /i "%~x1" == ".cpp" (
		call :write "%~1"
	) Else if /i "%~x1" == ".c" (
		call :write "%~1"
	) Else (
		echos 0x0c "请输入正确文件名称" 
		exit /b 404 
	)
	exit /b 1

:write
	cd /d "%~dp1"
	set "input=%~1"
	title %~nx1 - Compile
	set "name_com=%~nx1"
	set "name=%~n1"
goto :eof