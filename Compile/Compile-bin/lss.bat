@echo off
setlocal enabledelayedexpansion
if "%~1"=="l" goto :LSS
if "%~1"=="" goto :LSS_HELP
set LSS_HELP=0
For %%i in (- / \) Do (
	If /i "%1"=="%%ih" set LSS_HELP=1
	If /i "%1"=="%%i?" set LSS_HELP=1
)
if %LSS_HELP% EQU 1 GOTO :LSS_HELP
dir "%~1" 2>nul>nul || GOTO :LSS_HELP
if "%~1"=="*" goto :LSS_BIN
pushd %1 2>nul || goto :LSS_BIN
if not "%~2"=="" (
	set "local_path=%~1"
	pushd "!local_path!" 2>nul
	cd..
	if "!cd!"=="!local_path!" (
		for /d %%i in (*) do cscript //nologo "%~dp0\tre.vbs" "%%~i" 0
		popd
		GOTO :EOF
	) else cd !local_path!
	popd
	cscript //nologo "%~dp0\tre.vbs" %1 %2
	popd
	GOTO :EOF
)
set deep=0
set cds=%cd%
set FILE_NAME=0
echo;
call :TREE
endlocal
popd
GOTO :EOF

:TREE
set "Line!deep!=│"
set FILE_NAME=0
for /f %%i in ('dir /b') do set /a FILE_NAME+=1
set /a end!deep!=!FILE_NAME!
set /a filenum!deep!=0
for /f "delims=" %%i in ('dir /b') do (
	set /a filenum!deep!+=1
	for %%a in (!deep!)	do if "!filenum%%a!" EQU "!end%%a!" (set ICO=└─&set "Line!deep!= ") else set ICO=├─
	cd %%i 2>nul && (
		for /l %%j in (1,1,!deep!) do (
			set /a tmp=%%j-1
			for %%a in (!tmp!) do set/p.=!Line%%a! <nul
		)
		echo;!ICO! %%i
		set /a deep+=1
		call :TREE
	) || (
		for /l %%j in (1,1,!deep!) do (
			set /a tmp=%%j-1
			for %%a in (!tmp!) do set/p.=!Line%%a! <nul
		)
		set /a kb=%%~zi/1024
		if %%~zi LSS 1024 (
			set filesize=%%~ziB
		) else if !kb! LSS 1024 (
			set filesize=!kb!KB
		) else (
			set /a mb=!kb!/1024
			set filesize=!mb!MB
		)
		echo;!ICO! %%i [!filesize!] [%%~ti]
	)
)
set /a deep-=1
if "%cd%"=="%cds%" GOTO :EOF
cd..
GOTO :EOF

REM call :cal_row <char**> <int> <int> <int>				
REM //为分栏算法提供要显示的行/列数
REM set filenames[100];	//暂存目录中文件名
REM set file_cnt = 0;//字符串数量
:LSS
set n=0
for /f "usebackq delims=" %%A in (`dir /b`) do (
	set filenames[!n!]=%%A
	set /a n+=1
)
set /a file_cnt=%n%

REM for /l %%a in (0,1,!n!) do (
		REM echo;filenames[%%a] : !filenames[%%a]!
REM )
set /a n=%n%-1
REM echo;file_cnt:%file_cnt%
REM echo;n:%n%
set col=0
set row=0
goto :cal_row
:begain
set /a col=%col%+1
REM echo;[col:%col% row:%row%]


	set /a shwo=0
	REM 文件名数组下标
	set /a colss=0
	REM 列数组下标
	set /a tt=0
	REM 计数
	
	for /l %%i in (0 1 %n%) do (
		REM call printf 0x07 "[!shwo! - %%col_max_arr[!tt!]%% - !tt!]"
		call :printfs "%%col_max_arr[!tt!]%%"  "%%filenames[!shwo!]%%"
		set /a shwo+=%row%
		set /a tt+=1
		if !tt! GEQ %col% (
			echo;
			set /a colss+=1
			set /a shwo=!colss!
			set /a tt=0
		)
		if !shwo! GEQ !file_cnt! (
			echo;
			set /a colss+=1
			set /a shwo=!colss!
			set /a tt=0
		)
	)
exit /b


:printfs
length %2
set /a tmp=%~1-%errorlevel%
if "%~x2"==".exe" (
	printf 0x0A %2
) else if "%~x2"==".bat" (
	printf 0x0B %2
) else if exist %2\ (
	printf 0x09 %2
) else printf 0x07 %2
printf -n 0x07 %tmp% " "
exit /b

:cal_row
setlocal enabledelayedexpansion
modes 0
set mode_x=%errorlevel%
REM echo;获得当前窗口的宽度:%mode_x%

set size=0
for /l %%a in (0,1,%n%) do (
	length "!filenames[%%a]!"
	set /a size+=!errorlevel!
)
REM echo;size:%size%
set /a row=%size%/%mode_x%
REM echo;行数row:%row%
:loop
set /a row_tmp=0
set /a row_one=%row%-1
set /a col_tmp=0
set /a col_max_arr[0]=0
set /a MAX=0

for /l %%b in (0,1,%n%) do (
	set /a col_max_arr[%%b]=0
)
set /a ttmp=0
for /l %%b in (0,1,%n%) do (
	length "!filenames[%%b]!"
	set /a ttmp=!errorlevel!+2
	if !ttmp! GTR !MAX! set /a MAX=!ttmp!
	REM call printf 0x0E "[!ttmp! - !MAX! - !col_tmp!]"
	set col_max_arr[!col_tmp!]=!MAX!
	set /a row_tmp+=1
	if !row_tmp! GTR %row_one% (
		REM echo;
		set /a row_tmp=0
		set /a col_tmp+=1
	)
)

set /a col=%col_tmp%
REM echo;[col:%col% , row:%row%]
set /a col_one=%col%

set /a n_size=0
set /a n_col=0
for /l %%b in (0,1,%col_one%) do (
	REM printf 0x0a "!col_max_arr[%%b]!  "
	set /a n_size+=!col_max_arr[%%b]!*%row% 2>nul>nul
)
REM echo;
set /a size=%mode_x%*%row%
REM echo;[n_size:%n_size% size:%size%]
if %n_size% LEQ %size% goto :begain
set /a row+=1
REM echo;row:%row%
goto :loop
exit /b

:end
echo end
exit /b


:col_max_arr
if %1 GTR %2 exit /b %1
exit /b 0

:LSS_BIN
if not "%~1"=="*" (
	if exist "%~1" (
		cscript //nologo %~dp0size.vbs %1
		GOTO :EOF
	)
)
set LSS_BIN_deep=5
if not "%~2"=="" set LSS_BIN_deep=%2
set INIT_LSS_LIE=1
set INIT=0
set semaphore=
:LSS_BIN_LOOP
set LSS_LIE_MAX=0
set LSS_LIE=0
set LSS_HANG=1
REM set LSS_LIE[%LSS_LIE%]=1
for /f "usebackq delims=" %%i in (`dir /b /o-d %1`) do (
	set /a LSS_LIE+=1
	if not defined semaphore (
		length "%%~nxi"
		set %%~nxi=!errorlevel!
	)
	set LSS_LIE[!LSS_HANG!][!LSS_LIE!]=!%%~nxi!
	set LSS_NAME[!LSS_HANG!][!LSS_LIE!]=%%~nxi
	REM set/p.=[!LSS_HANG!][!LSS_LIE!]=!errorlevel!	<nul
	if !LSS_LIE! GTR !LSS_LIE_MAX! set LSS_LIE_MAX=!LSS_LIE!
	if !LSS_LIE! GEQ !INIT_LSS_LIE! (
		set LSS_LIE=0
		set /a LSS_HANG+=1
		REM echo;
	)
)
REM echo;
set semaphore=0
REM echo;LSS_LIE_MAX:!LSS_LIE_MAX!
REM echo;LSS_LIE:!LSS_LIE!

for /l %%j in (1 1 !LSS_LIE_MAX!) do if defined LSS_LIE_MAX[%%j] set LSS_LIE_MAX[%%j]=0

REM set LSS_LIE_MAX[!LSS_LIE!]
for /l %%j in (1 1 !LSS_LIE_MAX!) do (
	for /l %%i in (1 1 !LSS_HANG!) do (
		if defined LSS_LIE[%%i][%%j] (
			REM set/p.=[!LSS_LIE[%%i][%%j]!]	<nul
			if !LSS_LIE[%%i][%%j]! GTR !LSS_LIE_MAX[%%j]! set LSS_LIE_MAX[%%j]=!LSS_LIE[%%i][%%j]!
		)
	)
	REM echo;
)
REM echo;
set AREA_LIE=0
for /l %%i in (1 1 !LSS_LIE_MAX!) do (
	REM set/p.=!LSS_LIE_MAX[%%i]!  <nul
	set /a AREA_LIE+=!LSS_LIE_MAX[%%i]!
)
REM echo;AREA_LIE:%AREA_LIE%
set /a AREA_LIE+=%INIT_LSS_LIE%*2
set /a AREA=%AREA_LIE%*%LSS_HANG%
REM echo;
REM echo;AREA_LIE:%AREA_LIE%+=%INIT_LSS_LIE%*2
REM echo;AREA:%AREA% = %AREA_LIE%*%LSS_HANG%
if %INIT% EQU 1 GOTO :BIN_PRINT
if not "%~2"=="" (
	set INIT=1
	set /a INIT_LSS_LIE=%2
	GOTO :LSS_BIN_LOOP
)
modes 0
set /a mode_x=%errorlevel%-1
if %errorlevel% GTR 5120 (
	set INIT=1
	set /a INIT_LSS_LIE=%LSS_BIN_deep%
	GOTO :LSS_BIN_LOOP
)
if %errorlevel% LSS 14 (
	set INIT=1
	set /a INIT_LSS_LIE=%LSS_BIN_deep%
	GOTO :LSS_BIN_LOOP
)
REM echo;mode_x:%mode_x%
set /a MODE_SIZE=%mode_x%*%LSS_HANG%
REM echo;MODE_SIZE:%MODE_SIZE% = %mode_x%*%LSS_HANG% ^>= %AREA% - !LSS_LIE_MAX! - !INIT_LSS_LIE! - INIT:%INIT%
REM PAUSE >NUL
if !INIT_LSS_LIE! GTR !LSS_LIE_MAX! GOTO :BIN_PRINT
if %AREA% GTR %MODE_SIZE% (
	set INIT=1
	set /a INIT_LSS_LIE-=1
	GOTO :LSS_BIN_LOOP
)
if %AREA% LSS %MODE_SIZE% (
	set /a INIT_LSS_LIE+=1
	GOTO :LSS_BIN_LOOP
)
:BIN_PRINT

REM echo;
REM echo;
for /l %%i in (1 1 !LSS_HANG!) do (
	if %%i EQU !LSS_HANG! set LSS_LIE_MAX=!LSS_LIE!
	for /l %%j in (1 1 !LSS_LIE_MAX!) do (
		set /a T=!LSS_LIE_MAX[%%j]!-!LSS_LIE[%%i][%%j]!+2
		REM set/p.=!LSS_LIE_MAX[%%j]!-!LSS_LIE[%%i][%%j]!=!T!	<nul
		call :PRINT_LSS_NAME "!LSS_NAME[%%i][%%j]!"
		REM set/p.=!LSS_NAME[%%i][%%j]!<nul
		printf -n 0x07 !T! " "
	)
	echo;
)
GOTO :EOF

:PRINT_LSS_NAME
if /i "%~x1"==".exe" (
	printf 0A "%~1"
) else if /i "%~x1"==".bat" (
	printf 0C "%~1"
) else if /i "%~x1"==".cmd" (
	printf 0C "%~1"
) else if /i "%~x1"==".h" (
	printf 2E "%~1"
) else if /i "%~x1"==".cpp" (
	printf 0B "%~1"
) else if /i "%~x1"==".py" (
	printf 02 "%~1"
) else if exist "%~1\" (
	printf 03 "%~1"
) else printf 0x07 "%~1"
GOTO :EOF

:LENGTH <String>
set count=0
set arg1=%~1
:LENGTH_loop
if not "!arg1:~%count%,1!" == "" (
    set /a count+=1
    goto LENGTH_loop
)
exit /b %count%


:LSS_MAIN
	set num=0
	for /f "delims=" %%i in ('dir /b /a-d /o-d *') do (
		set/a num+=1
		if !num! LSS 3 call :LSS_HELP_SIZE "%%~dpnxi"
	)
	echo;
	set num=0
	for /f "delims=" %%i in ('dir /b /a-s /o-s *') do (
		set/a num+=1
		if !num! LSS 3 call :LSS_HELP_SIZE "%%~dpnxi"
	)
	echo;
	::echo;[%date:~0,-3% %time:~0,-6%]
pushd "%~dp0"
	set num=0
	for /f "delims=" %%i in ('dir /b /a-d /o-d *') do (
		set/a num+=1
		if !num! LSS 3 call :LSS_HELP_SIZE "%%~dpnxi"
	)
popd
GOTO :EOF

:LSS_HELP_SIZE
	set /a kb=%~z1/1024
	if %~z1 LSS 1024 (
		set filesize=%~z1B
	) else if !kb! LSS 1024 (
		set filesize=!kb!KB
	) else (
		set /a mb=!kb!/1024
		set filesize=!mb!MB
	)
	echo;[%~t1]  [%filesize%]  	%~nx1
GOTO :EOF

:LSS_HELP
	echo;Usage: %~n0 打印当前目录所有文件/文件夹
	echo;   or: %~n0 [path] 打印路径目录树
	echo;   or: %~n0 [file^|*] 打印当前目录所有文件夹/文件 [l]
	echo;   or: %~n0 [path] [num] num:打印目录树的深度
	echo;   or: %~n0 [file^|*] [num] num:打印文件夹/文件列数
	echo;   or: %~n0 [-/\][hH?] 打印此信息
	echo;   
	echo;   dir /a-d *bat /d /o-s ^| /d /o-[s d e]
	echo;   xcopy /Q /S /E /Y /K [/C 即使出现错误也继续复制]
	echo;   robocopy /S /E [/MT[:n] n个线程复制默认为8]/MOV 移动文件
	echo;   rd /q /s ^| del /s /q /f ^| fsutil file createnew filename size字节
	echo;   clocks 150 100 30 11 255 255 255 0
	echo;  
	echo;   7z x -aoa：表示直接覆盖现有文件，而没有任何提示
	echo;        -aos：跳过现有文件，其不会被覆盖。
	echo;        -aou：如果相同文件名的文件以存在，将自动重命名被释放的文件
	echo;        -aot：如果相同文件名的文件以存在，将自动重命名现有的文件
GOTO :EOF