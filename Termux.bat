@echo off & title 
set location=
if not "%~1" == "" (
	if /i "%~1" == "/c" goto :termux
	if /i "%~1" == "/u" goto :update
	if /i "%~1" == "/l" ( set location=1 & goto :termux )
	:loop
	if "%~1"=="" exit /b
	if not exist "%~1" shift&goto :loop
	echo;%~a1|findstr /i "hs">nul && (
		attrib -s -h %1
	) || attrib +s +h %1
	shift
	goto :loop
)
:termux
REM echo;termux
setlocal enabledelayedexpansion
REM for /f "tokens=15 delims=: " %%i in ('ipconfig ^|find "IPv4"') do set IPv4=%%i
set "path=%~dp0Compile\Compile-bin;%path%"
:: if exist "%TEMP%\time_start_location.time_start" (
	if not defined location (
		GSmouse
		set /a "ret=!errorlevel!,gx=ret>>16,gy=ret&65535"
		set /a gx-=300
		set /a gy-=218
		location 0 !gx! !gy!
		modes 70 15 71 200
		set gx=&set gy=&set ret=
		sico "%~dp0Compile\Home\empty.ico" 2>nul>nul
		seta -a 220 2>nul>nul
		color 07
	) else sico "%~dp0Compile\Home\empty.ico" 2>nul>nul
:: ) else modes 110 25 111 200
:history
REM echo;history
for /f "tokens=2,*" %%i in ('reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" /v "Desktop"') do set "Desk=%%j"
set LANG=zh_CN
doskey fe=explorer "%%cd%%"
doskey fee=explorer $*
doskey tn="%~dp0FILE\Notepad++\notepad++.exe" "%~f0"
doskey bn="%~dp0FILE\Notepad++\notepad++.exe" "%~dp0Compile\Compile-bin\$*.bat"
doskey sn="%~dp0FILE\Notepad++\notepad++.exe" "%~dp0Compile\Compile-bin\Sourse Code\$*"
doskey nes=new.bat $* ^& "%~dp0FILE\Notepad++\notepad++.exe" $*
doskey ee=echo;%%errorlevel%%
doskey kill=taskkill -f -im $*.exe /t
doskey cat=type $*
doskey cp=copy /y $*
doskey cdd=cd /d $*
doskey mv=move /y $*
doskey clear=Title ^& cls ^& chcp 936^>nul ^& color 07 ^& echo; ^& fortune
doskey while=for /l %%i in (1 1 $1) do $2 $3 $4 $5 $6 $7 $8 $9
REM doskey ip=printf 0x07 "%IPv4%" ^| clip ^& echos 0x03 [%IPv4%]ÒÑ¸´ÖÆÖÁ¼ôÇÐ°å
doskey wa=run /t $* ^& kn v 5000 ^& start /min gplay "%~dp0image\Connected Sky.mp3" ^& run windows "%~dp0image\Connected Sky.bmp" 255 230 7900 0
doskey HT=hotkey.exe "%VCC_HOME%\TERMUX.BAT" /l ^>"%~dp0h.log"
doskey hdd=hd ^& if "%%hdd%%" EQU "hide" (set hdd=show^&nircmd win show class Shell_TrayWnd) else (set hdd=hide^&nircmd win hide class Shell_TrayWnd)
doskey W=WMIC /NAMESPACE:\\root\wmi PATH WmiMonitorBrightnessMethods WHERE "Active=TRUE" CALL WmiSetBrightness Brightness=$1 Timeout=0
set dates=%date:~0,-3%
if not defined location (
	REM echo;printf
	printf
	REM echo;Activate
	Activate
	REM echo;insert
	insert
)
if not exist "%TEMP%\time_start_location.time_start" (
	if not exist "%Temp%\%dates:/=-%.install" (
		cd.>%Temp%\%dates:/=-%.install
		tasklist | find /i "hotkey.exe" >nul || runhid hotkey.exe "%~f0" /l
	)
)
set dates=
REM echo;prompt
prompt [$P]$+$$$S
REM echo;ver
ver|findstr 10. 2>nul>nul && Set /P=[3 q< Nul
for /f "tokens=2* delims= " %%i in ("%*") do (
	if "%%~j"=="" ( set "argc=%%~i" ) else set argc=%%i %%j
)
REM echo;cmd
cmd /k %argc%
set used=%USERNAME%
for %%i in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do call set used=%%used:%%i=%%i%%
call :showcmd
exit /b
:showcmd
printf 0x0a %used%@%COMPUTERNAME%
printf 0x09 [%cd%]
printf 0x07 "$ "
set /p.=
call %.%
set.=
goto :showcmd

:update
cd /d %~dp0
path=%~dp0Compile\Compile-bin;%path%
if not exist "%VCC_HOME%" (
	echo;  - Î´°²×°
	exit /b
)
call "%VCC_HOME%\Compile\Compile-bin\vcc.bat" -v >nul
set /p new_version=<%Temp%\%dates:/=-%.install
set new_version=%new_version:~11%
echo;  - ¿ªÊ¼¸üÐÂÎÄ¼þ...
:version_loop
if "%new_version:.=%" GTR "%version:.=%" (
	echo;  [%version%] -^> [%new_version%]
	call RUN-%version%.cmd 2>nul || RUN-%version% NO FIND 
	call :vcc_version_add %version%
	goto :version_loop
) else echo;  -------- ENDING -------- [ ¸üÐÂÊ§°Ü¿ÉÒÔÊ¹ÓÃ install -f Ç¿ÖÆ¸üÐÂ ]
exit /b

:vcc_version_add
for /f "tokens=1,2,3 delims=." %%i in ("%~1") do (
	set tal=%%i
	set mid=%%j
	set low=%%k
)

set /a low+=1
if %low% GTR 9 set /a mid+=1 & set low=0
if %mid% GTR 9 set /a tal+=1 & set mid=0
set version=%tal%.%mid%.%low%
goto :eof