@echo off
setlocal enabledelayedexpansion
pushd %~dp0..\..
set VCC_HOME=%CD%
popd
for %%i in (%VCC_HOME%) do set "home=%%~pnxi"
if "%VCC_HOME%"=="\" set "VCC_HOME=%VCC_HOME:~0,-1%"
call vcc -v >nul
REM 准备工作
set reduce=0
set add=0
set change=0
set REMSS=REM
if exist "%~2" (
	set REMSS=
	set "MD_PATH=%~2"
	if "!MD_PATH:~-1,1!"=="\" (
		echo;路径最后不能带\
		exit /b
	)
)
%REMSS% echo;@echo off >"%temp%\run_update.cmd"
%REMSS% FC "%VCC_HOME%\Termux.bat" "%~2\Termux.bat" 2>nul>nul || echo;copy /y "%VCC_HOME%\Termux.bat" "%~2\" >>"%temp%\run_update.cmd"

if "%~1"=="-c" (
	if not "%~3"=="" set version=%3
	goto :check
) else if "%~1"=="-f" (
	if not exist "%~2" (
		echo;[ERROR] 路径不存在
		exit /b
	)
	goto :CHECK_FILE
) else if "%~1"=="-s" (
	if not exist "%~2" (
		echo;[ERROR] 路径不存在
		exit /b
	)
	goto :CHECK_FILE_SIZE
) else if "%~1"=="-l" (
	goto :log
) else if "%~1"=="-r" (
	goto :run_update
) else if "%~1"=="-t" (
	for /f "delims=" %%i in ('dir /b %temp%\?.?.?.log %temp%\?.?.?.cmd') do echo;%%temp%%\%%i
	if exist "%temp%\run_update.log" echo;%%temp%%\run_update.log
	if exist "%temp%\run_update.cmd" echo;%%temp%%\run_update.cmd
) else if not "%~1"=="" (
	if "%~2"=="" ( set md5=md5 ) else set md5=%2
	set up_md5=
	for /f "skip=1 delims=" %%m in ('certutil -hashfile "%~1" !md5!') do (
		if not defined up_md5 echo;%%m
		set up_md5=1
	)
) else (
	echo;Usage: %~n0 [-c] [path] [version]
	echo;   or: %~n0 {[-f][-s]} [path]
	echo;   or: %~n0 {[-t][-l][-r][file]}
	echo;   or: %~n0 [file] 查看文件MD5值[MD2 MD4 MD5 SHA1 SHA256 SHA384 SHA512]
	echo;
	echo;Arguments:
	echo;   -l:  生成本版本文件目录信息
	echo;   -t:  查看临时目录下版本文件   
	echo;   -f:  比较[path]文件内容、生成更新脚本
	echo;   -s:  比较[path]文件大小、生成更新脚本
	echo;   -c:  比较[version]版本信息记录的文件md5值、生成更新脚本
	echo;   -r:  运行更新脚本
	echo;
	echo;Others: 
	echo;   更新脚本[%%temp%%\run_update.cmd]
	echo;   日志文件[%%temp%%\run_update.log]
)
exit /b

:log
echo;
cd.>%tmp%\%version%.log
cd.>%tmp%\%version%.cmd
set log_num=0
set "progess=|/-\"
set /a progess_int=0
set log_time=!time:~-2,-1!
for /r "%VCC_HOME%\Compile" %%i in (*) do (
	set /a log_num+=1
	if "!time:~-2,-1!" NEQ "!log_time!" (
		call :progess
		set log_time=!time:~-2,-1!
		set /a progess_int+=1
		if !progess_int! GTR 3 set /a progess_int=0
	)
	gotoxy -l 0 -1
	set "tm=%%~pnxi"
	if "%home%"=="\" (set "tmps=!tm!") else set "tmps=!tm:%home%=!"
	set /a mmdd=0
	for /f "skip=1 delims=" %%m in ('certutil -hashfile "%%~i" md5') do (
		set /a mmdd+=1
		if !mmdd! EQU 1 set md5=%%m
	)
	echo;set "Arr!tmps!=!md5!">>%tmp%\%version%.cmd
	>>%tmp%\%version%.log echo;!tmps!
)
printf 07 "  已为版本 %version% 生成 %log_num% 个文件信息 [!progess:~%progess_int%,1!]"
echo;
exit /b

:progess
printf 07 "  已为版本 %version% 生成 !log_num! 个文件信息 [!progess:~%progess_int%,1!]"
GOTO :EOF


:check
if not exist "%tmp%\%version%.cmd" echo;Not find %version%.cmd & exit /b
if not exist "%tmp%\%version%.log" echo;Not find %version%.log & exit /b

call %tmp%\%version%.cmd

echo;
for /f "delims=" %%i in (%tmp%\%version%.log) do (
	if not exist "%VCC_HOME%%%i" (
		echo;  - %VCC_HOME%%%i | tee -a %temp%\run_update.log
		set /a reduce+=1
		%REMSS% echo;del /f /q "%~2%%~i" >>"%temp%\run_update.cmd"
	)
)

%REMSS% for /f "delims=" %%i in ('dir /s /b /ad %VCC_HOME%\Compile') do (
%REMSS% 	set "tm=%%~pnxi"
%REMSS% 	if "%home%"=="\" (set "tmps=!tm!") else set "tmps=!tm:%home%=!"
%REMSS% 	if not exist "%~2!tmps!" (
%REMSS% 		echo;  # %~2!tmps! | tee -a %temp%\run_update.log
%REMSS% 		echo;md "%~2!tmps!" >>"%temp%\run_update.cmd"
%REMSS% 	)
%REMSS% )

for /r "%VCC_HOME%\Compile" %%i in (*) do (
	set "tm=%%~pnxi"
	if "%home%"=="\" (set "tmps=!tm!") else set "tmps=!tm:%home%=!"
	call set name=%%Arr!tmps!%%
	set /a mmdd=0
	for /f "skip=1 delims=" %%m in ('certutil -hashfile "%%~i" md5') do (
		set /a mmdd+=1
		if !mmdd! EQU 1 set md5=%%m
	)
	if not "!name!" == "!md5!" (
		if "!name!"=="" ( 
			echo;  + %%i | tee -a %temp%\run_update.log
			set /a add+=1 
		) else (
			echo;  * %%i | tee -a %temp%\run_update.log
			set /a change+=1
		)
		%REMSS% echo;copy /y "%%~i" "%~2!tmps!" >>"%temp%\run_update.cmd"
	)
)
if %reduce% EQU 0 if %change% EQU 0 if %add% EQU 0 exit /b
goto :show_result

:CHECK_FILE_SIZE
echo;  - 对比 %VCC_HOME% 和 %2 [ SIZE ]
echo;  - Press any key to continue ...
pause >nul
echo;
for /r "%2\Compile" %%i in (*) do (
	 set "tm=%%~pnxi"
	if "%~pnx2"=="\" ( set "tmps=!tm!" ) else set "tmps=!tm:%~pnx2=!"
	if not exist "%VCC_HOME%!tmps!" (
		echo;  - %VCC_HOME%!tmps! | tee -a %temp%\run_update.log
		set /a reduce+=1
		echo;del /f /q "%%~i" >>"%temp%\run_update.cmd"
	)
)

for /f "delims=" %%i in ('dir /s /b /ad %VCC_HOME%\Compile') do (
	set "tm=%%~pnxi"
	if "%home%"=="\" (set "tmps=!tm!") else set "tmps=!tm:%home%=!"
	if not exist "%~2!tmps!" (
		echo;  # %~2!tmps! | tee -a %temp%\run_update.log
		echo;md "%~2!tmps!" >>"%temp%\run_update.cmd"
	)
)

for /r "%VCC_HOME%\Compile" %%i in (*) do (
	set "tm=%%~pnxi"
	if "%home%"=="\" (set "tmps=!tm!") else set "tmps=!tm:%home%=!"
	call :size_fc "%%i" "%~2!tmps!" || (
		if !errorlevel! EQU 2 (
			echo;  + %~2!tmps! | tee -a %temp%\run_update.log
			set /a add+=1
		) else echo;  * %%i | tee -a %temp%\run_update.log & set /a change+=1 
		echo;copy /y "%%~i" "%~2!tmps!" >>"%temp%\run_update.cmd"
	)
)
goto :show_result

:CHECK_FILE
echo;  - 对比 %VCC_HOME% 和 %2 [ - ]
echo;  - Press any key to continue ...
pause >nul
echo;
for /r "%2\Compile" %%i in (*) do (
	 set "tm=%%~pnxi"
	if "%~pnx2"=="\" ( set "tmps=!tm!" ) else set "tmps=!tm:%~pnx2=!"
	if not exist "%VCC_HOME%!tmps!" (
		echo;  - %VCC_HOME%!tmps! | tee -a %temp%\run_update.log
		set /a reduce+=1
		echo;del /f /q "%%~i" >>"%temp%\run_update.cmd"
	)
)

for /f "delims=" %%i in ('dir /s /b /ad %VCC_HOME%\Compile') do (
	set "tm=%%~pnxi"
	if "%home%"=="\" (set "tmps=!tm!") else set "tmps=!tm:%home%=!"
	if not exist "%~2!tmps!" (
		echo;  # %~2!tmps! | tee -a %temp%\run_update.log
		echo;md "%~2!tmps!" >>"%temp%\run_update.cmd"
	)
)

for /r "%VCC_HOME%\Compile" %%i in (*) do (
	set "tm=%%~pnxi"
	if "%home%"=="\" (set "tmps=!tm!") else set "tmps=!tm:%home%=!"
	FC "%%i" "%~2!tmps!" 2>nul>nul || (
		if !errorlevel! EQU 2 (
			echo;  + %~2!tmps! | tee -a %temp%\run_update.log
			set /a add+=1
		) else echo;  * %%i | tee -a %temp%\run_update.log & set /a change+=1 
		echo;copy /y "%%~i" "%~2!tmps!" >>"%temp%\run_update.cmd"
	)
)

:show_result
echo;
echo;  -- [ %date:~0,-3% %time:~0,-3% ][ REDUCE %reduce% CHANGE %change% ADD %add% ] | tee -a %temp%\run_update.log
exit /b

:size_fc
if "%~2" == "" exit /b 2
if %~z1 EQU %~z2 exit /b 0
exit /b 1

:run_update
type "%temp%\run_update.cmd"
echo;
choice /M ". - 是否更新"
echo;
if %errorlevel% EQU 1 "%temp%\run_update.cmd"