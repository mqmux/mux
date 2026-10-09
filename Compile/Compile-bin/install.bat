@echo off
setlocal enabledelayedexpansion
if "%~1" == "-vcc" (
	goto :vcc
) else if "%~1" == "-c" (
	goto :check_update
) else if "%~1" == "-v" (
	goto :show_update
) else if "%~1" == "-d" (
	goto :down_update
) else if "%~1" == "-f" (
	goto :f_update
) else if "%~1" == "-ff" (
	goto :ff_update
) else (
	echo;Usage: install {[-c][-d][-v]} 
	echo;   or: install {[-vcc][-f][-ff]} [path]
	echo;&echo;Arguments:
	echo;   -c: 	检查更新
	echo;   -d: 	更新
	echo;   -v: 	加入聊天室
	echo;   -f: 	强制更新 -ff 再次解压
	echo;   -vcc	安装本程序集合到指定目录[path]
)
endlocal
exit /b

:show_update
	ping www.baidu.com -n 1 2>nul>nul || (
		printf 0xc0 " ERROR "
		echo; 未连接网络
		exit /b
	)
	echo;正在加入聊天室...
	set num=1
	for /f "delims=" %%i in ('curl https://gitee.com/cctv3058084277/main/releases/tag/TERMUX-VCC 2^>nul ^
	 ^| sed "s/}/\n/g" ^| findstr /i "download_url tag_path" ^|sed "s/:/\n/g;s/,/\n/g" ^| sed -n "4p;17p;35p"') do (
		set "var!num!=%%~i"
		set /a num+=1
	)
	set /a num=1
	for /f %%i in ('echo;%var2%^|sed "s/-/\n/g"') do (
		set "IP_PORT_!num!=%%~i"
		set /a num+=1
	)
	if "%~2"=="" (
		set /p name=请输入昵称:
		if "!name!"=="" set name=%IPv4%
	) else set "name=%~2"
	client %IP_PORT_3% %IP_PORT_4% "%name%"
exit /b

:check_update
	ping www.baidu.com -n 1 2>nul>nul || (
		printf 0xc0 " ERROR "
		echo; 未连接网络
		exit /b
	)
	echo;正在获取版本信息...
	gotoxy 0 -1
	set num=1
	for /f "delims=" %%i in ('curl https://gitee.com/cctv3058084277/main/releases/tag/TERMUX-VCC 2^>nul ^
	 ^| sed "s/}/\n/g" ^| findstr /i "download_url tag_path" ^|sed "s/:/\n/g;s/,/\n/g" ^| sed -n "4p;17p;35p"') do (
		set "var!num!=%%~i"
		set /a num+=1
	)
	if not defined var2 (
		echo;  - 获取失败 -        
		exit /b
	)
	set /a num=1
	for /f %%i in ('echo;%var2%^|sed "s/-/\n/g"') do (
		set "IP_PORT_!num!=%%~i"
		set /a num+=1
	)
	if exist "%TEMP%\time_start_location.time_start" set IP_PORT_2=OFF
	if "%IP_PORT_2%"=="ON" (
		REM RunHid console %IP_PORT_3% %IP_PORT_4%
		RunHid client %IP_PORT_3% %IP_PORT_4% %IPv4%
		RunHid Hotkey /h "%~dp0..\..\termux.bat" /l
		REM msg -r "console %IP_PORT_3% %IP_PORT_4%" 0
	)
	call vcc -v >nul
	printf 0x10 " GITEE "
	REM print " GITEE " 30;44
	echo; %var1%             
	set dates=%date:~0,-3%
	del /f /q %Temp%\*.install 2>nul >nul
	echo;%var1%>%Temp%\%dates:/=-%.install
	printf 0x20 " LOCAL "
	REM print " LOCAL " 30;42
	echo; SOURSE PATH IS [%VCC_HOME%]
	echo;
	if "TERMUX-VCC-%version%"=="%var1%" (
		echo;  - 当前版本: %version% -
	) else (
		echo;  - 当前版本: %version% -
		echo;  - 检测到当前不是最新版本,请下载最新版本[install -f]
	)
	echo;
	echo;  Open sourse at:
	printf 0x07 "  - Gitee: "
	printf 0x03 https://gitee.com/cctv3058084277/main
	REM print https://gitee.com/cctv3058084277/main 36
	echo;
	printf 0x07 "  - Github: "
	printf 0x03 https://github.com/MOYIGUIJUE/cctv
	REM print https://github.com/MOYIGUIJUE/cctv 36
	echo;
exit /b



:vcc
	pushd %~dp0..\..
	if exist "%~2" (
		set "input=%~2"
		goto :input
	)
	set input=
	echos 0x03 请选择安装目录,安装目录不能包含空格
	call chooses input
	if "%input%"=="" (
		echos 0x0c 未选择安装目录
		popd
		exit /b
	)
	:input
	echo;安装目录:%input%
	xcopy .\Compile\ "%input%\Compile\" /e /y /h /r || ( echo;没有xcopy命令 & exit /b )
	xcopy .\FILE\ "%input%\FILE\" /e /y /h /r
	copy Termux.bat "%input%"
	call exec.bat %input%
	start cmd /c "termux FILE"
	popd
exit /b
::/E 复制目录和子目录，包括空的。/Y 取消提示以确认要覆盖现有目标文件 。/H 也复制隐藏和系统文件。/R 改写只读文件。 /Q 复制时不显示文件名。

:down_update
	ping www.baidu.com -n 1 2>nul>nul || (
		printf 0xc0 " ERROR "
		echo; 未连接网络
		exit /b
	)
	set num=1
	for /f "delims=" %%i in ('curl https://gitee.com/cctv3058084277/main/releases/tag/TERMUX-VCC 2^>nul ^
	 ^| sed "s/}/\n/g" ^| findstr /i "download_url tag_path" ^|sed "s/:/\n/g;s/,/\n/g" ^| sed -n "4p;17p;35p"') do (
		set "var!num!=%%~i"
		set /a num+=1
	)
	call vcc -v >nul
	printf 0x10 " GITEE "
	echo; %var1%
	set dates=%date:~0,-3%
	del /f /q %Temp%\*.install 2>nul >nul
	echo;%var1%>%Temp%\%dates:/=-%.install
	printf 0x20 " LOCAL "
	echo; SOURSE PATH IS [%VCC_HOME%]
	echo;
	if "TERMUX-VCC-%version%"=="%var1%" (
		echo;  - 当前版本: %version% -
	) else (
		echo;  - 当前版本: %version% -
		echo;  - 检测到当前不是最新版本,开始下载...
		goto :end_update_down
	)
exit /b
	
:end_update_down
pushd "%temp%"
if exist TERMUX-VCC.7z del /f /q TERMUX-VCC.7z
call down https://gitee.com/cctv3058084277/cctvpage1/releases/download/TERMUX-VCC/TERMUX-VCC.7z
echo;  - 开始解压...
if exist "TERMUX-VCC.7z" (
	call 7z x .\TERMUX-VCC.7z -o.\termux -aoa >nul
	call .\termux\termux.bat /u
) else echo;下载失败
popd
exit /b

:sha256
	set up_md5=
	for /f "skip=1 delims=" %%m in ('certutil -hashfile "%~1" sha256') do (
		if not defined up_md5 set %2=%%m
		set up_md5=1
	)
GOTO :EOF

:f_update
pushd "%temp%"
if exist TERMUX-VCC.7z (
	call :sha256 TERMUX-VCC.7z old_TERMUX
	del /f /q TERMUX-VCC.7z
)
printf 0x10 " GITING " 
REM print " GITING " 30;44
echo;  开始下载...
call down https://gitee.com/cctv3058084277/cctvpage1/releases/download/TERMUX-VCC/TERMUX-VCC.7z 2>nul>nul
REM echo;  - 开始解压...
printf 0x20 " FINISH "
REM print " FINISH " 30;42
echo;  下载完成...
echo;
if exist "TERMUX-VCC.7z" (
	call :sha256 TERMUX-VCC.7z new_TERMUX
	
	echo;LAST: !old_TERMUX!
	echo;NEWS: !new_TERMUX!
	if "!old_TERMUX!"=="!new_TERMUX!" ( echo;SHA256值相同,请忽重复更新 & popd & GOTO :EOF )
	set old_TERMUX=
	for /f "usebackq" %%i in ("%~dp0..\..\my_path.cmd") do set old_TERMUX=%%i
	echo;LAST: !old_TERMUX!
	if "!old_TERMUX!"=="!new_TERMUX!" ( echo;SHA256值相同,请忽重复更新 & popd & GOTO :EOF )
	echo;!new_TERMUX! >>"%~dp0..\..\my_path.cmd"
	call 7z x .\TERMUX-VCC.7z -o.\termux -aoa >nul
) else echo;下载失败
popd
:ff_update
if exist "%~2" (
	echo;%temp%\termux\Termux.bat -^> %2
	copy /y %temp%\termux\Termux.bat %2
	xcopy /e /y /h /r /f "%temp%\termux\Compile" %2
	exit /b
)
echo;%temp%\termux\Termux.bat -^> %~dp0..\..
copy /y %temp%\termux\Termux.bat "%~dp0..\.."
copy /y %temp%\termux\uninstall.bat "%~dp0..\.."
xcopy /e /y /h /r /f /C "%temp%\termux\Compile" "%~dp0..\..\Compile"