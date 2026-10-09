@echo off
setlocal enabledelayedexpansion
if "%~1"=="" (
	echo;Usage: %~n0 [-r] 正则表达式
	echo;   or: %~n0 [-d] 下载
	echo;   or: %~n0 [-v] [path name]查看版本
	echo;   or: %~n0 [-rv] [run name]查看版本
	exit /b
) else if "%~1"=="-v" (
	if exist "%~2" ( set "get_version=%~2" ) else set "get_version=%~dp0..\..\FILE\es.exe"
	call :get_version "%~dp0..\..\FILE\es.exe" ESV
	echo;!ESV!
) else if "%~1"=="-rv" (
	if "%~2"=="" ( set "RUNFILE=Everything.exe" ) else set RUNFILE=%~2
	tasklist | find /i "!RUNFILE!" 2>nul>nul && (
		call :get_run_version !RUNFILE! EVV
		echo;!EVV!
	)
) else (
	if exist "%~dp0..\..\FILE\es.exe" (
		"%~dp0..\..\FILE\es.exe" %* 2>nul
		if !errorlevel! EQU 8 goto :run_Everything
	) else (
		if "%~1"=="-d" ( goto :downloads ) else (
			echo;Not installed,Please run [ %~n0 -d ]
			exit /b
		)
	)
)
endlocal
exit /b

:downloads
cd /d %~dp0..\..\FILE
if exist "%~dp0down.exe" (
	down https://www.voidtools.com/Everything-1.4.1.1024.x86.zip
	down https://www.voidtools.com/ES-1.1.0.26.zip
) else (
	bitsadmin /transfer ESEverythingDownlaod https://www.voidtools.com/Everything-1.4.1.1024.x86.zip "%cd%\Everything-1.4.1.1024.x86.zip"
	bitsadmin /transfer ESDownlaod https://www.voidtools.com/ES-1.1.0.26.zip "%cd%\ES-1.1.0.26.zip"
)
7z x Everything-1.4.1.1024.x86.zip -o.
7z x ES-1.1.0.26.zip -o.
exit /b

:get_run_version <runing name> <return name>
set /a Line_Num=1
for /f "skip=1 delims=" %%i in ('wmic process WHERE NAME^="%~1" get executablepath') do (
	if !Line_Num! EQU 1 set ExecutablePath=%%i
	set /a Line_Num+=1
)
set /a Line_Num=256
:get_version <path name> <return name>
if "%Line_Num%" NEQ "256" set ExecutablePath=%~1
set /a Line_Num=1
for /f "skip=1" %%j in ('wmic datafile where name^="!ExecutablePath:\=\\!" get version') do (
	if !Line_Num! EQU 1 set %2=%%j
	set /a Line_Num+=1
)
exit /b

:run_Everything
start /min "" "%~dp0..\..\FILE\Everything.exe"
:Waiting
ping -n 1 127.1 >nul
"%~dp0..\..\FILE\es.exe" -r ^Everything.exe$ 2>nul>nul
if %errorlevel% EQU 8 goto :Waiting
"%~dp0..\..\FILE\es.exe" %*
exit /b

REM es -r "^Everything.exe$" 2>nul>nul || call :run_Everything
REM call :get_run_version Everything.exe EVV
REM call :get_version %cd%\es.exe ESV

es -r ^Everything.exe$
es -r ^java.exe$
ES 能返回以下任一错误级别代码：
错误级别	说明
0	无已知错误，搜索成功。
1	注册窗口类失败。
2	创建监听窗口失败。
3	内存溢出。
4	缺失额外的命令行选项参数。
5	创建导出文件失败。
6	未知参数。
7	发送查询到 Everything IPC 失败。
8	未找到 Everything IPC 窗口。请确认 Everything 客户端已运行。
