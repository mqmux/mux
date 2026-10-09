@echo off
setlocal enabledelayedexpansion
set num=0
if "%~1"=="-" call :sour &exit /b
if "%~1"=="-s" call :sourse_s
if "%~1"=="-u" call :sourse_u
if "%~1"=="-a" call :sourse

:paths
if "%paths%"=="" set "paths=%path%"

REM 外部实现
REM echo;%paths% | sed "s/;/\n/g" | sed "/^$/d" | nl

set num=0

set "paths=%paths:;=" "%"
call :loop "%paths%"
exit /b

:loop
	set tmps=%1
	if not defined tmps exit /b
	if "%~1"==" " (
		shift
		goto :loop
	)
	if "%~1"=="" (
		shift
		goto :loop
	)
	set /a num+=1
	echo;  [%num%]	%~1
	if not exist "%~1" echo;  - %~1 [ERROR]
	shift
goto :loop

:sourse_u
for /f "tokens=3* delims= " %%i in ('reg query HKCU\Environment /v path') do (
	if "%%j"=="" ( set "paths=%%i" ) else set "paths=%%i %%j"
	exit /b
)

:sourse_s
for /f "tokens=3* delims= " %%i in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v path') do (
	if "%%j"=="" ( set "paths=%%i" ) else set "paths=%%i %%j"
	exit /b
)

:sourse
for /f "tokens=3* delims= " %%i in ('reg query HKCU\Environment /v path') do (
	for /f "tokens=3* delims= " %%a in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v path') do (
		if "%%j"=="" ( set "path_user=%%i" ) else set "path_user=%%i %%j"
		if "%%b"=="" ( set "path_sys=%%a" ) else set "path_sys=%%a %%b"
		set "paths=!path_user!!path_sys!"
		exit /b
	)
)

:sour
choice /C us /N /M "[u:用户环境变量 s:系统环境变量]"
if %errorlevel% equ 1 call :environment_u
if %errorlevel% equ 2 call :environment_s
:loops
set choose=
set /p "choose=[输入序号查看内容 +/-]: "
if "%choose%"=="" exit /b
if "%choose:~0,1%"=="-" ( set "chooses=%choose:~1%" 
) else if "%choose:~0,1%"=="+" ( set "chooses=%choose:~1%" 
) else set chooses=%choose%

if "!environment%chooses%!"=="" (
	echo;  - 输入格式错误,或序号不存在
	goto :loops
)
set "paths=!paths%chooses%!"
if "!paths!"=="" ( call :loop_p "" ) else call :loop_p "%paths:;=" "%"
if not "%chooses%"=="%choose%" call :environment%choose:~0,1%
set num=0
set chooses=0
set choose=0
set environment=
exit /b

:loop_p
	set a=%1
	if not defined a goto :eof
	if "%~1"=="" (
		shift
		goto :loop_p
	)
	set /a num+=1
	echo;  - %~1 [%num%]
	shift
goto :loop_p

:environment_u
set nums=0
set "environment=HKCU\Environment"
for /f "tokens=1,2,3* delims= " %%i in ('reg query "%environment%"') do (
	if NOT !nums! equ 0 (
		if "%%l"=="" ( set "paths!nums!=%%k" ) else set "paths!nums!=%%k %%l"
		if "%%k"=="" ( echo;  [!nums!]	%%i	%%j	空 ) else call echo;  [!nums!]	%%i	%%j	%%paths!nums!%%
		set "environment!nums!=%%i"
	)
	set /a nums+=1
)
exit /b

:environment_s
set nums=0
set "environment=HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment"
for /f "tokens=1,2,3* delims= " %%i in ('reg query "%environment%"') do (
	if NOT !nums! equ 0 (
		if "%%l"=="" ( set "paths!nums!=%%k" ) else set "paths!nums!=%%k %%l"
		if "%%k"=="" ( echo;  [!nums!]	%%i	%%j	空 ) else call echo;  [!nums!]	%%i	%%j	%%paths!nums!%%
		set "environment!nums!=%%i"
	)
	set /a nums+=1
)
exit /b

:environment-
if "%chooses%"=="" (
	echo;  - REG DELETE "%environment%" /v "[环境变量名]" /f
	echo;  - REG ADD "%environment%" /v "[环境变量名]" /t [值类型] /d "[环境变量值]" /f
) else (
	echo;  - REG delete "%environment%" /v "!environment%choose:~1%!" /f
	REG delete "%environment%" /v "!environment%choose:~1%!" /f 2>nul>nul && echo;  - 删除成功 || echo;  - 删除此环境变量需要管理员权限
)
exit /b

:environment+
if "%chooses%"=="" (
	set add_environment_content=
	set add_environment=
	set /p "add_environment=--> 请输入新的环境变量: "
	if "!add_environment!"=="" exit /b
	if not "!add_environment: =!"=="!add_environment!" echo;环境变量名含有空格 & goto :environment+
	set /p "add_environment_content=--> 请输入新的环境变量的内容: "
	REG ADD "!environment!" /v "!add_environment!" /t REG_SZ /d "!add_environment_content!" /f 2>nul>nul && echo;  - 添加成功 || echo;  - 添加此环境变量需要管理员权限
) else (
	set revise_environment=
	set /p "revise_environment=--> 请输入新的内容: "
	echo;  - REG ADD "%environment%" /v !environment%choose:~1%! /t REG_SZ /d "!revise_environment!" /f
	REG ADD "%environment%" /v !environment%choose:~1%! /t REG_SZ /d "!revise_environment!" /f 2>nul>nul && echo;  - 修改成功 || echo;  - 修改此环境变量需要管理员权限
)
exit /b
