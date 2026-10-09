@echo off
setlocal enabledelayedexpansion
rem @file:auto_connect_wifi.bat
rem @author:cctv
rem @date:2023.02.23
rem @note:for auto connect wifi
if "%~1"=="-w" (
	goto :wifi
) else if "%~1"=="-p" (
	for /f "tokens=1*" %%i in ('netsh wlan show profiles ^| find /i "所有用户配置文件"') do (
		for /f "tokens=1* delims=:" %%a in ('netsh wlan show profile name^="%%j" key^=clear ^| find /i "关键内容"') do set /a num+=1 & echo;  %%j - %%b
	)
) else (
	echo;Usage: %~n0 [-w] wifi自动连接工具
	echo;   or: %~n0 [-p] 查看保存的wifi密码
)
exit /b

:wifi
set num=0
for /f "tokens=2* delims=:" %%i in ('netsh wlan show profiles ^| findstr "所有用户配置文件"') do (
	set /a num+=1
	set wifi_!num!=%%i
)
REM for /l %%i in (1,1,%num%) do echo;  - %%i !wifi_%%i:~1!
REM netsh wlan connect ssid=%wifiname% name=%wifiname%
REM echo;[当前连接]
REM netsh WLAN show interfaces | findstr SSID
set /a connect_num=0

:loop
	if %num% LSS %connect_num% set /a connect_num=0
	ping www.baidu.com -n 2 2>nul>nul || ( set /a connect_num+=1 & goto :connect )
	printfs "  # %time:~0,-3% online"
	gotoxy -l 0 -1
	sleep 5000
goto :loop

:connect
	echo;  connecting !wifi_%connect_num%:~1! ... [%connect_num%]
	netsh wlan connect ssid="!wifi_%connect_num%:~1!" name="!wifi_%connect_num%:~1!" 2>nul>nul || (
		echo;  - 不存在 !wifi_%connect_num%:~1!
		set /a connect_num+=1
		goto :loop
	)
	set continue_num=0
	:connecting
	sleep 5000
	(netsh WLAN show interfaces | findStr !wifi_%connect_num%:~1! >nul && ( 
		echo;  - already connected !wifi_%connect_num%:~1!
	)) || (
		echo;  - continue connecting !wifi_%connect_num%:~1!
		set /a continue_num+=1
		if !continue_num! LSS 3 goto :connecting
	)
	REM 给连接wifi预留的时间,根据自己电脑连接wifi速度而定
goto :loop

choice /t 5 /d y /n >nul

netsh wlan disconnect 断开连接

1、查看已经连接的wifi
netsh wlan show profiles

2、导出wifi名称为mywifi的配置文件(随便自己找个连接过的wifi就行了),key=clear表示密码用明文输出,folder=.路径
netsh wlan export profile name="hyb-5" folder=. key=clear

<SSID>
	<hex>6469646964696469</hex> 注:didididi 转换十六进制 [命令转换:printf -h didididi]
	<name>didididi</name>  
</SSID>

4、将添加wifi配置文件
netsh wlan add profile filename="WLAN.xml"
 
5、查看配置是否添加成功
netsh wlan show profiles | findStr mywifi2
 
6、连接wifi(记得先开启热点mywifi2)
netsh wlan connect ssid=mywifi2 name=mywifi2

附：删除配置
netsh wlan delete profile name="47621"
netsh wlan delete profile name="didididi"
