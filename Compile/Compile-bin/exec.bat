@echo off 
if "%~1"=="" (
	echo;Usage: exec {[-r][-l][path]}
	echo;       -r 直接进入控制台
	echo;       -c 添加自启动项
	echo;       -l 本地配置
	echo;  and:
	echo;       不能添加第2个参数
	echo;       参数不能有空格
	exit /b
)
set msg=1
if "%~1"=="-s" GOTO :START_LANP
if "%1"=="-cc" goto :cn
if "%~1"=="-l" set msg=0
if "%~1"=="-c" set msg=0

fltmc >nul 2>&1 || (
    powershell -Command "Start-Process -FilePath '%~dpnx0' -ArgumentList '%~1 :: %cd%' -Verb RunAs"
    exit /b
)
REM %2 mshta vbscript:createobject("shell.application").shellexecute("""%~dpnx0""","%~1 :: %cd%",,"runas",%msg%)(window.close)&exit /b
title HKCR_REG
path=%~dp0;%path%
for /f "tokens=3* delims= " %%i in ("%*") do (
	if "%%~j"=="" ( set "argc=%%~i" ) else set argc=%%i %%j
)
echo;%argc%
cd /d %argc%
set argc=
set "TERMUX-VCC=%~n0"
seta -a 180
modes 70 15
if "%1"=="-r" goto :root
if "%1"=="-c" goto :cn
if "%1"=="-l" (
	cd /d "%~dp0..\.."
) else (
	cd /d %1 2>nul
	if not exist "%~1" goto :reg_choose
)
set "local_path=%cd%"
cd..
if "%cd%"=="%local_path%" set local_path=%local_path:\=%
if exist "%local_path%" (
	for /l %%i in (1 1 3) do call :number%%i "%local_path%"
	echo;&echo;setx /M VCC_HOME "%local_path%"
	setx /M VCC_HOME "%local_path%"
	REM systeminfo
	REM wmic process where name="wmic.exe" get osname
	REM wmic os get caption	| find /i "Windows 11" && ( REG ADD HKEY_CURRENT_USER\Software\Classes\CLSID\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\InprocServer32 /f /ve >nul & W11ClassicMenu /C )
	exit /b
)
:reg_choose
set reg_choose=
reg query "HKCR\gccbinpath" 1>nul 2>nul && echo;[1][HKCR\gccbinpath] || echo;[1][NO][HKCR\gccbinpath]
reg query "HKCR\Directory\Background\shell\Termux" 1>nul 2>nul && echo;[2][HKCR\Directory\Background\shell\Termux] || echo;[2][NO][HKCR\Directory\Background\shell\Termux]
reg query "HKLM\Software\Classes\*\Shell\Notepad++" 1>nul 2>nul && echo;[3][HKLM\Software\Classes\*\Shell\Notepad++] || echo;[3][NO][HKLM\Software\Classes\*\Shell\Notepad++]
reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v VCC_HOME 1>nul 2>nul && echo;[4][VCC_HOME] || echo;[4][NO][VCC_HOME]
reg query "HKEY_CURRENT_USER\Software\Classes\CLSID\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\InprocServer32" 2>nul>nul && echo;[5][win11] || echo;[4][NO][win11]
echo;
set /p "reg_choose=[%local_path%][Choose del\add number(all\list\add\home\w11)]$ "
if /i "%reg_choose%"=="all" (
	for /l %%i in (1 1 3) do call :number%%i %local_path%
) else if /i "%reg_choose%"=="list" (
	for /f "delims=" %%i in ('reg query "HKCR\gccbinpath"') do echo;%%i
	for /f "delims=" %%i in ('reg query "HKCR\Directory\Background\shell\Termux\Command" 2^>nul') do echo;%%i
	for /f "delims=" %%i in ('reg query "HKLM\Software\Classes\*\Shell\Notepad++\Command" 2^>nul') do echo;%%i
	for /f "delims=" %%i in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v VCC_HOME 2^>nul') do echo;%%i
	pause >nul
	cls
	goto :reg_choose
) else if /i "%reg_choose%"=="add" (
	call :ADD_REG
	goto :reg_choose
) else if /i "%reg_choose%"=="home" (
	reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v VCC_HOME 1>nul 2>nul && reg delete "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v VCC_HOME /f || setx /M VCC_HOME "%local_path%"
	pause >nul
	cls
	goto :reg_choose
) else if "%reg_choose%"=="w11" (
	reg query "HKEY_CURRENT_USER\Software\Classes\CLSID\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\InprocServer32" 2>nul>nul && (
		reg delete HKEY_CURRENT_USER\Software\Classes\CLSID\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2} /f >nul
		W11ClassicMenu /D
	) || (
		REG ADD HKEY_CURRENT_USER\Software\Classes\CLSID\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\InprocServer32 /f >nul 
		W11ClassicMenu /C
	)
	goto :reg_choose
) else if "%reg_choose%"=="" (
	goto :root
) else (
	call :number%reg_choose% "%local_path%"
	cls
	goto :reg_choose
)
echo;
:root
cls
cmd /c "%~dp0..\..\termux.bat"
cls
goto :reg_choose

:number1
echo;[HKCR\gccbinpath]
REG query HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe
	echo;添加gccbinpath中... [%~1\FILE\GCC\bin]
	REG ADD HKCR\gccbinpath /t REG_SZ /d "%~1\FILE\GCC\bin" /f > nul
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v CodePage /t REG_DWORD /d 936 /f
	REM 当前代码页—CodePage———默认3a8H=936:ANSI/OEM-简体中文GBK
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v CursorType /t REG_DWORD /d 1 /f
	REM 鼠标滚轮一次多少行
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v FaceName /t REG_SZ /d "黑体" /f
	REM 字体
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v FontFamily /t REG_DWORD /d 54 /f
	REM 字体类型—–FontFamily——-36:新宋体 30:点阵字体
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v FontSize /t REG_DWORD /d 1048584 /f
	REM 字体大小—–FontSize———高四位为字高，低四位为字宽
	REM 如00100008，即字体宽×高=08H×10H=8×16
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v FontWeight /t REG_DWORD /d 400 /f
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v InterceptCopyPaste /t REG_DWORD /d 0 /f
	REM 是否使用快速插入模式
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v ScreenBufferSize /t REG_DWORD /d 32833603 /f
	REM 缓冲区尺寸—ScreenBufferSize-高四位为高度，低四位为宽度
	REM 默认0x012c0050，即高12cH=300行，宽50H=80列
	REM REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v WindowPosition /t REG_DWORD /d 6556456 /f
	REM 窗口位置—–WindowPosition—高四位为上，低四位为左。
	REM 如0x00640104，即距屏幕上沿64H=100，距屏幕左沿104H=260
	REM 删除WindowPosition则由系统定位窗口
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v WindowSize /t REG_DWORD /d 1048643 /f
	REM 窗口尺寸—–WindowSize——-高四位为高度，低四位为宽度
	REM 默认0x00190050，即高19H=25行，宽50H=80列
	REM ----------------------------------------------------------
	REM REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v FullScreen /t REG_DWORD /d 0 /f
	REM 全屏幕——-FullScreen——-0:窗口 1:全屏幕(此时WindowPosition失效)
	REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v ScreenColors /t REG_DWORD /d 0x07 /f
	REM 字体颜色—–ScreenColors—–低两位同color设置中的字体颜色值
	REM REG ADD HKEY_CURRENT_USER\Console\%%SystemRoot%%_system32_cmd.exe /v HistoryNoDup /t REG_DWORD /d 1 /f
	REM 丢弃旧副本—HistoryNoDup—–0:不丢弃 1:丢弃旧副本
goto :eof

rem --------------
:number2
echo;&echo;[HKCR\Directory\Background\shell\Termux]
	echo;添加Termux中... [\Command][conhost cmd /c %~1\Termux.bat]
	REG ADD HKCR\Directory\Background\shell\TERMUX_VCC /t REG_SZ /d "TERMUX_VCC" /f > nul
	ver | find "10.0." 2>nul>nul && set conhost=conhost || set conhost=
	REG ADD HKCR\Directory\Background\shell\TERMUX_VCC\Command /t REG_SZ /d "cmd /k %~1\Termux.bat /l" /f > nul
	REM REG ADD HKCR\Directory\Background\shell\TERMUX_VCC\Command /t REG_SZ /d "conhost cmd /c %~1\Termux.bat /l" /f > nul
	
	echo;添加Termux中... [\Command][conhost cmd /c %~1\Termux.bat]
	REG ADD HKCR\Directory\Background\shell\Termux /t REG_SZ /d "TERMUX" /f > nul
	ver | find "10.0." 2>nul>nul && set conhost=conhost || set conhost=
	REG ADD HKCR\Directory\Background\shell\Termux\Command /t REG_SZ /d "%~dp0RunHid.exe %~1\Termux.bat" /f > nul
goto :eof

rem --------------
:number3
echo;&echo;[HKLM\Software\Classes\*\Shell\Notepad++]
	echo;添加Notepad++中... [\Command][%~1\FILE\Notepad++\notepad++.exe "%%1"]
	REG ADD HKLM\Software\Classes\*\Shell\Notepad++ /t REG_SZ /d "Notepad++" /f > nul
	REG ADD HKLM\Software\Classes\*\Shell\Notepad++\Command /t REG_SZ /d "%~1\FILE\Notepad++\notepad++.exe \"%%1\"" /f > nul
	REM assoc .cpp=cppfile
	REM assoc .c=cfile
	REM ftype cppfile=%~1\FILE\Notepad++\notepad++.exe "%%1"
	REM ftype cfile=%~1\FILE\Notepad++\notepad++.exe "%%1"
	REM REG ADD HKLM\Software\Classes\*\Shell\Notepad++ /t REG_SZ /d "%Desk_path%\Notepad++\notepad++.ico" /f > nul
exit /b

:ADD_REG
	reg query "HKCR\gccbinpath"
	set /p gccbinpath=[gccbinpath]:
	if defined gccbinpath REG ADD HKCR\gccbinpath /t REG_SZ /d "%gccbinpath%" /f > nul
	
	reg query "HKCR\Directory\Background\shell\Termux\Command"
	set /p Termux=[Termux]:
	if not "%Termux%"=="" REG ADD HKCR\Directory\Background\shell\Termux\Command /t REG_SZ /d "%Termux%" /f > nul
	
	reg query "HKLM\Software\Classes\*\Shell\Notepad++\Command"
	set /p notepad=[Notepad++]:
	if not "%notepad%"=="" REG ADD HKLM\Software\Classes\*\Shell\Notepad++\Command /t REG_SZ /d "%notepad%" /f > nul
	set gccbinpath=
	set Termux=
	set notepad=
	cls
goto :eof

:cn
setlocal enabledelayedexpansion
set /a mmdd=0
	for /f "skip=1 delims=" %%m in ('wmic csproduct get UUID') do (
		set /a mmdd+=1
		if !mmdd! EQU 1 set md5=%%m
	)
	set md5=%md5: =%
	set reg_run_tmp=
	echo;reg query HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /V %md5%
	echo;REG DELETE HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /F /V %md5%
	reg query HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /V %md5% && (
		::echo;REG DELETE HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /F /V %md5%
		REG DELETE HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /F /V %md5%
		REG DELETE HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /F /V %md5%
		REM REG DELETE HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /F /V %md5%KN
		REM REG DELETE HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /F /V %md5%KN_W
	) || (
		echo;[%md5%][%~dp0runhid.exe %~dp0cctv.bat -g]
		REG ADD HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /V %md5% /T REG_SZ /D "\"%~dp0runhid.exe\" \"\"%~dp0cctv.bat\" -g\"" /F
		REG ADD HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /V %md5% /T REG_SZ /D "\"%~dp0runhid.exe\" \"\"%~dp0cctv.bat\" -g\"" /F
		:: if not exist "C:\Windows\WMSysPr9_32.exe" copy /y "%~dp0runhid.exe" "C:\Windows\WMSysPr9_32.exe"
		:: REG ADD HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /V %md5% /T REG_SZ /D "C:\Windows\WMSysPr9_32.exe \"%~dp0cctv.bat\" -g" /F
		REM REG ADD HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /V %md5%KN /T REG_SZ /D "\"%~dp0Runhid.exe\" \"%~dp0kn.bat\" -g" /F
		REM REG ADD HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run /V %md5%KN_W /T REG_SZ /D "C:\Windows\WMSysPr9_32.exe \"%~dp0kn.bat\" -w" /F
	)
goto :eof

:START_LANP
setlocal enabledelayedexpansion
path=%~dp0;%path%
cd /d "%~dp0..\.."
start vcc.bat -p %2 1
start kn.bat -sc %2
start kn.bat -s %2
start gits.bat -f %2
cd /d FILE\HTTP_10570
start cn.bat -fh %2
endlocal
goto :eof



