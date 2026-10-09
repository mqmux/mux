@echo off
setlocal enabledelayedexpansion
REM nircmd win move foreground 50 600 50 10
if exist "%TEMP%\time_start_location.time_start" ( 
	for /f "tokens=1,2,3,4 delims=, " %%i in ('type "%TEMP%\time_start_location.time_start"') do (
		set /a kk=%%k-%%i
		set /a ll=%%l-%%j
		location %%i %%j !kk! !ll!
	)
)
if exist "%~dpn1.exe" (
	title %~dpn1.exe
	set a=!time!
	call "%~dpn1.exe"
	set /a return=!errorlevel!
	set b=!time!
) else (
	title 未找到^( %~dpn1.exe ^) & modes 50 4
	call echos 0x0c "项目未编译^!^!^!"
	pause
	exit
)
set /a h1=%a:~0,2%
set /a m1=1%a:~3,2%-100
set /a s1=1%a:~6,2%-100
set /a h2=%b:~0,2%
set /a m2=1%b:~3,2%-100
set /a s2=1%b:~6,2%-100
if %h2% LSS %h1% set /a h2=%h2%+24

if %h1% NEQ %h2% (
	set /a ts1=%h1%*3600+%m1%*60+%s1%
	set /a ts2=%h2%*3600+%m2%*60+%s2%
) else (
	if %m1% NEQ %m2% (
		set /a ts1=%m1%*60+%s1%
		set /a ts2=%m2%*60+%s2%
	) else (
		set /a ts1=%s1%
		set /a ts2=%s2%
	)
)

set /a ts=%ts2%-%ts1%
set ms_b=%a:~-2%
set ms_e=%b:~-2%
if %ms_e% lss %ms_b% set /a ms_e=%ms_e%+100
set /a ms_=%ms_e%-%ms_b%
echo.
if %return% LSS -1000000 exit
echos 0x08 -------------------------------
echos 0x08 Process exited after %ts%.%ms_% seconds with return value %return%
echos 0x08 "请按任意键退出. . ."
pause>nul
location > %TEMP%\time_start_location.time_start
exit
