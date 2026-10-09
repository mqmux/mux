@echo off & title 
For %%i in (- / \) Do (
	If /i "%1"=="%%ih" Goto :helps
)
if not "%~1"=="" (
	for /f "delims=:" %%a in ('findstr /n "helps_%1.*$" %~fs0') do (
		REM echo;sed -n "%%a,4p" "%~fs0"
		echo;[more +%%a %~f0]
		more +%%a "%~fs0"
	)
	exit /b
)
set used=%USERNAME%
for %%i in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do (
	call set used=%%used:%%i=%%i%%
)
cls
echo;
call vcc -v
echo;
printf 0x10 " INFO "
echo; To start : 820146120
printf 0x20 " DONE "
echo; On line  : 235227728
echo;
echo;  Open sourse at:
printf 0x07 "  - Gitee: "
echos 0x0b https://gitee.com/cctv3058084277/main
printf 0x07 "  - Github: "
echos 0x0b https://github.com/MOYIGUIJUE/cctv
echo;
echo;  - ÁÄÌìÊÒ     server 1111   cctv -gg  install -v
echo;  - Ô¶³ÌÃüÁîĞĞ kn -s [key]   kn -w     cctv -g
echo;  - ÎÄ¼ş·şÎñÆ÷ gits -f [key] gits [command] [key]
echo;
printf 0x07 "  Press any key to show or run "
Call :SetConsoleTextAttributeEx 4
set /p .=https://cctv.rth1.me<nul
Call :SetConsoleTextAttributeEx 5
echo; to accelerate.
GOTO :EOF
CmdMenuSel 0708 "  - CONTINUE" "  - HELP" "  - QUIT"
if %errorlevel% equ 2 goto :helps
if %errorlevel% equ 3 exit /b
cls
echo;  ^< Ä¿Â¼ ^>
echo;
CmdMenuSel 0708 "  - µÚÒ»ÕÂ °²×°ÓëÊ¹ÓÃ" "  - µÚ¶şÕÂ »ù´¡½Ì³Ì" "  - µÚÈıÕÂ ½ø½×½Ì³Ì" "  - µÚËÄÕÂ ÁË½â±¾Íâ²¿ÃüÁî¼¯ºÏ" "  - µÚÎåÕÂ Íâ²¿ÃüÁî" "  - µÚÁùÕÂ ±àĞ´Íâ²¿ÃüÁî" "  - ÍË³ö"
cls
goto :book_%errorlevel%

:book_7
exit /b

:book_1
gotoxy -l 0 0
echo;
echo;  ©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´
echo;  ©¦  - µÚÒ»ÕÂ °²×°ÓëÊ¹ÓÃ      ©¦
echo;  ©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼
echo; 				 
echo; 				 
echo;  - °´ÉÏÏÂ¼üÑ¡Ôñ,»Ø³µÈ·¶¨,³öÏÖÈ·¶¨°´Å¥,µã»÷ºó
echo;  - ÓÒÏÂ½ÇÍĞÅÌ´¦»áÓĞÒ»¸öÍ¼±ê,ÓÒ¼üµã»÷quitÍË³ö
call :fechos 2 1 32 3 F0

call :fechos 0 6 50 6 F0 REM
:words-key
words -key
if %errorlevel% EQU 128 (
	call :fechos 0 7 50 7 07 REM
	call :fechos 0 6 50 6 F0 REM
)
if %errorlevel% EQU 129 (
	call :fechos 0 6 50 6 07 REM
	call :fechos 0 7 50 7 F0 REM
)
if %errorlevel% EQU 13 goto :words-key-break
goto :words-key
:words-key-break
gotoxy -l 40 9 
echo; ©°©¤©¤©¤©¤©¤©¤©´
gotoxy -l 40 10
echo; ©¦ È·¶¨ ©¦
gotoxy -l 40 11
echo; ©¸©¤©¤©¤©¤©¤©¤©¼
call :fechos 40 9 51 11 F5
:mouse_loop
call :mouse 40 9 51 11 1
if %errorlevel% EQU 1 goto :TimeMain
goto :mouse_loop

:mouse <X> <Y> <EX> <EY> <RE>
set /a X=%1
set /a Y=%2
set /a EX=%3
set /a EY=%4
for /f "tokens= 1,2,3 delims= " %%i in ('Mouse.exe') do (
	set MX=%%k
	set MY=%%j
)
REM echo;[%X%,%Y%,%EX%,%EY%]
if %MX% GEQ %X% if %MX% LSS %EX% if %MY% GEQ %Y% if %MY% LEQ %EY% exit /b %5
exit /b 0

:fechos <sx> <sy> <ex> <ey> <color>
set /a length=%3 - %1
set /a lengths=%length%+%1
echos -f %5 %length% %1 %2
set /a two=%2+1
for /l %%i in (%two%,1,%4) do (
	echos -f %5 %length% %1 %%i
	echos -f 80 2 %lengths% %%i
)
set /a four=%4+1
set /a ex=%1+2
%6 echos -f 80 %length% %ex% %four%
exit /b

:book_2
SetLocal EnableExtensions EnableDelayedExpansion
echo;  - µÚ¶şÕÂ »ù´¡½Ì³Ì | lolcat
echo;
echo;  paths ÃüÁî          paths ÃüÁî
echo;  path  ÃüÁî          path  ÃüÁî
echo;  sed   ÃüÁî          sed   ÃüÁî
echo;  nl    ÃüÁî          nl    ÃüÁî
echo;
call :fechos 0 2 12 5 80 REM
set now_line=2
call :fechos 0 %now_line% 14 %now_line% F0 REM
:book_2_mouses_loop
for /f "tokens= 1,2,3 delims= " %%i in ('mouses.exe 0 2 12 5 10') do (
	gotoxy -l 0 -1
	set /a now_line=%%j+2
	call :fechos 0 !now_line! 14 !now_line! F0 REM
	if !now_line! NEQ %now_line% call :fechos 0 %now_line% 14 %now_line% 80 REM
	if %%k EQU 10 goto:book_2_words
)
goto:book_2_mouses_loop
:book_2_words
call :fechos 20 2 32 5 80 REM
set now_line=2
call :fechos 20 %now_line% 34 2 F0 REM
call :book_2_2
gotoxy -l 0 -1
:book_2_words-key
words -key
if %errorlevel% EQU 128 (
	if !now_line! GTR 2 (
		set /a now_line-=1
		set /a now_line_l=!now_line!+1
		call :fechos 20 !now_line_l! 34 !now_line_l! 80 REM
		call :fechos 20 !now_line! 34 !now_line! F0 REM
		call :book_2_!now_line!
		gotoxy -l 0 -1
	) else (
		set now_line=5
		call :fechos 20 2 34 2 80 REM
		call :fechos 20 !now_line! 34 !now_line! F0 REM
		call :book_2_!now_line!
		gotoxy -l 0 -1
	)
)
if %errorlevel% EQU 129 (
	if !now_line! LSS 5 (
		set /a now_line+=1
		set /a now_line_l=!now_line!-1
		call :fechos 20 !now_line_l! 34 !now_line_l! 80 REM
		call :fechos 20 !now_line! 34 !now_line! F0 REM
		call :book_2_!now_line!
		gotoxy -l 0 -1
	) else (
		set now_line=2
		call :fechos 20 5 34 5 80 REM
		call :fechos 20 !now_line! 34 !now_line! F0 REM
		call :book_2_!now_line!
		gotoxy -l 0 -1
	)
)
if %errorlevel% EQU 13 goto :book_2_words-key-break
goto :book_2_words-key
:book_2_words-key-break
gotoxy -l 0 7
if %now_line% EQU 2 (
	call :showcmd "paths"
	call paths
	exit /b
)
printf 0x0a %used%@%COMPUTERNAME%
printf 0x09 [%cd%]
printf 0x07 "$ "
timeout 1 >nul
printf -t 0x07 100 "path | sed \"s/;/\n/g\" | sed \"/^$/d\" | nl"
timeout 1 >nul
call :Type "  - ²é¿´»·¾³±äÁ¿"
path | sed "s/;/\n/g" | sed "/^$/d" | nl

for /f "tokens=1,2,3,4 delims=," %%i in ('location') do (
	set /a x=%%i
	set /a y=%%j+10
	set /a xx=%%k-30
	
)
	for /l %%a in (%x%,1,%xx%) do (
		GSmouse %%a %y% 0 0
		call :show_title !errorlevel!
	)
	GSmouse %xx% %y% 1 1

exit /b
	
:show_title
set /a "ret=%1,x=ret>>16,y=ret&65535"
title [%x%,%y%]
exit /b

:book_2_2
	printfs "  - Ô­ĞÍ: paths.bat Íâ²¿ÃüÁî  "       
exit /b
:book_2_3
	printfs "  - Ô­ĞÍ: path.exe ÄÚ²¿ÃüÁî    " 
exit /b
:book_2_4 
	printfs "  - Ô­ĞÍ: sed.exe Íâ²¿ÃüÁî      "      
exit /b
:book_2_5
	printfs "  - Ô­ĞÍ: nl.exe Íâ²¿ÃüÁî     "       
exit /b


:book_3
echo;  - µÚÈıÕÂ ½ø½×½Ì³Ì
call :showcmd "vcc -h"
call vcc -h
locations
set /a "ret=%errorlevel%,x=ret>>16,y=ret&65535"
echo;ÆÁÄ»·Ö±æÂÊ¡¾%x%,%y%¡¿
call :showcmd "tree -N -d compile"
tree -N -d compile
location -6 1
location -6 1 %x% %y%
title Î»ÔËËãÊ¾Àı   code by:cn-bathome-more
echo.
echo.Î»ÔËËãÃû´Ê½âÊÍ(¸öÈËÀí½â):
echo.set /a "ret=%%errorlevel%%,x=ret>>16,y=ret&65535"
echo.Î»ÔËËã: °´²Ù×÷ÊıÔÚ¼ÆËã»úÄÚ±íÊ¾µÄ¶ş½øÖÆÊıÖğÎ»½øĞĞÂß¼­ÔËËã»òÒÆÎ»ÔËËã.
echo.Óë:   È«²¿¶¼ÊÇ1²ÅÎª1(Ö¸·µ»ØÖµ,ÏÂÍ¬),·ñÔòÎª0.
echo.»ò:   Ö»ÒªÓĞ1ÔòÎª1,·ñÔòÎª0.
echo.Òì»ò: ²»Í¬Îª1,ÏàÍ¬Îª0.
echo.È¡·´: ÊÇ1Ôò0,ÊÇ0Ôò1(µ¥Ä¿ÔËËã·û).
echo.È¡¸º: ÏÈÈ¡·´ÔÙ¼Ó1(µ¥Ä¿ÔËËã·û).
echo.×óÒÆnÎ»: Ïàµ±ÓÚ³ËÒÔ2µÄn´Î·½(ÓÃ0²¹Î»).
echo.ÓÒÒÆnÎ»: Ïàµ±ÓÚ³ıÒÔ2µÄn´Î·½(¸ºÊıÓÃ1²¹Î»,ÕıÊıÓÃ0²¹Î»).
echo.
echo.ÏÂÃæµÄÊ¾ÀıÊÇÔÚÈıÊ®¶şÎ»ÏµÍ³(ÊıÖµ·¶Î§:-2147483648~2147483647)ÏÂÔËĞĞµÄ½á¹û.
echo.ÇëÊäÈëÅú´¦ÀíÄÜ´¦ÀíµÄÊıÖµ(ÕûÊı),·ñÔò½«²»ÄÜÕıÈ·´¦Àí.Èç·¢ÉúÒç³ö,½á¹ûÒ²²»»áÕıÈ·.
echo.
rem ½«Ê®½øÖÆ×ª»»Îª¶ş½øÖÆµÄº¯Êı.
set "fun_d2b=setlocal enabledelayedexpansion&(for /l %%a in (0 1 31) do (set /a "str=!#a#!^>^>%%a"&set /a "str^&=1"&set "str_d2b=!str!!str_d2b!"))&(for %%a in (!str_d2b!) do (endlocal&set #a#=%%a))"
setlocal enabledelayedexpansion
:agn
set in=&set /p in=ÇëÊäÈëÁ½¸öÊı(ÖĞ¼äÓÃ¿Õ¸ñ¸ô¿ª):
if not defined in goto SetConsoleColor
set n=0
for %%a in (%in%) do (
   set /a n+=1
   set /a num!n!=%%a 2>nul
)
cls
echo.ÊäÈëµÄÁ½¸öÊıµÄÓĞĞ§ÊıÖµÎª: "!num1!" ºÍ "!num2!", ÓÃ¶ş½øÖÆ±íÊ¾·Ö±ğÎª:
set /a str1=num1,str2=num2
%fun_d2b:#a#=str1%
%fun_d2b:#a#=str2%
echo.&echo.!str1!  ==^> !num1!
echo.!str2!  ==^> !num2!
echo.&echo.¸÷ÖÖÎ»ÔËËãµÄ½á¹ûÈçÏÂ:
set /a "num=!num1!&!num2!,t=num"
%fun_d2b:#a#=num%&echo.&echo.!num! ==^> °´Î»Óë:   !num1! ^& !num2! = !t!
set /a "num=!num1!|!num2!,t=num"
%fun_d2b:#a#=num%&echo.&echo.!num! ==^> °´Î»»ò:   !num1! ^| !num2! = !t!
set /a "num=!num1!^^!num2!,t=num"
%fun_d2b:#a#=num%&echo.&echo.!num! ==^> °´Î»Òì»ò: !num1! ^^^^ !num2! = !t!
set /a "num=~!num1!,t=num"
%fun_d2b:#a#=num%&echo.&echo.!num! ==^> °´Î»È¡·´:    ~(!num1!) = !t!
set /a "num=-!num1!,t=num"
%fun_d2b:#a#=num%&echo.&echo.!num! ==^> È¡¸º:        -(!num1!) = !t!
set /a "ran=%random%%%5+2,num=!num1!,num<<=ran,t=num"
%fun_d2b:#a#=num%&echo.&echo.!num! ==^> ×óÒÆ!ran!Î»:   !num1! ^<^< !ran! = !t!
set /a "ran=%random%%%5+2,num=!num1!,num>>=ran,t=num"
%fun_d2b:#a#=num%&echo.&echo.!num! ==^> ÓÒÒÆ!ran!Î»:   !num1! ^>^> !ran! = !t!
echo.&goto :agn
:SetConsoleColor
echo;ÊÖ¶¯µ÷Õû´°¿Ú´óĞ¡ÓĞ¾ªÏ²à¸£¡
modes 127 32
	For /L %%I in (0,16,255) Do (
		For /L %%J in (0,16,255) Do (
			For /L %%K in (0,16,255) Do (
				Call :SetConsoleTextAttribute %%I-%%J-%%K %%I-%%J-%%K
				Set /P=#< Nul
			)
		)
	)
	
exit /b

:book_4
call :game_loop
cls
echo;  - µÚËÄÕÂ ÁË½â±¾Íâ²¿ÃüÁî¼¯ºÏ
echo;
echo;[·­Òë] Âé·³Ã»À´ÕÒÄã£¬¾Í±ğÈ¥×ÔÕÒÂé·³¡£  - °´ESCÌø¹ı
echo;[ÌáÊ¾] µÚÒ»¡¢ËÄ¸ötroubleÊÇ¶¯´Ê£¬µÚ¶ş¡¢Èı¸ötroubleÊÇÃû´Ê¡£
words "[-] Never trouble trouble" "till" "trouble troubles you¡£"
echo;	                  ----
echo;[µ¥´ÊÊÍÒâ]till
echo;	v.¸û×÷£»ÀçµØ
echo;	prep.Ö±µ½¡­ÎªÖ¹£»Ö±µ½¡­²Å¡­£»ÔÚ¡­Ç°(²»¡­)
echo;	conj.Ö±µ½¡­ÎªÖ¹£»ÔÚ¡­Ö®Ç°
echo;	n.ÊÕÒøÌ¨£»£¨ÏÖ½ğ³öÄÉ»úµÄ£©·ÅÇ®µÄ³éÌë
echo;	ÍøÂçÖ±µ½¡­¡­ÎªÖ¹£»°®µÄÊÄÑÔ£»¸ûÖÖ
echo;	µÚÈıÈË³Æµ¥Êı£ºtills  ÏÖÔÚ·Ö´Ê£ºtilling  ¹ıÈ¥Ê½£ºtilled
exit /b

:book_5
echo;  - µÚÎåÕÂ Íâ²¿ÃüÁî
cmdow @ /TOP 2>nul>nul
call :showcmd "start cmatrix"
start cmatrix.exe
echo;  - Æô¶¯Êı×ÖÓê³ÌĞò
call :showcmd "zms matrix"
sleep 5000
zms.exe matrix
echo;  - ÉèÖÃÎªProgman×Ó´°¿Ú,Ç¶Èë×ÀÃæ,¹Ø±Õ£ºKill cmatrix
cmdow @ /NOT 2>nul>nul
exit /b

:book_6
title;µÚÁùÕÂ ±àĞ´Íâ²¿ÃüÁî
call :SetConsoleCursorInfo 2 4
echo;©°©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©´
echo;©¦         ÊÇ·ñ¿ªÊ¼         ©¦
echo;©¦                          ©¦
echo;©¦      ÊÇ          ·ñ      ©¦
echo;©¸©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¼
movePos 0 0 28 4 20 5
call :fechos 20 5 49 9 F0
REM call :fechos 26 8 30 8 A0 REM
REM call :fechos 38 8 42 8 40 REM
:book_6_mouses_loop
set judges=0
for /f "tokens= 1,2,3 delims= " %%i in ('mouses.exe 27 8 39 8 10') do (
	title %%i %%j %%k
	if %%i LSS 3 ( call :fechos 26 8 30 8 A0 REM & set judges=1 ) else call :fechos 26 8 30 8 F0 REM
	if %%i GTR 10 ( call :fechos 38 8 42 8 40 REM & set judges=0 ) else call :fechos 38 8 42 8 F0 REM
	if %%k EQU 10 goto:book_6_words
)
goto:book_6_mouses_loop
:book_6_words
if %judges% EQU 0 gotoxy 0 6 &exit /b
REM nircmd.exe win center foreground
REM Topmost
REM (for /f %%i in ('CWnd find /!') do (
	REM CWnd Disable %%i min
	REM CWnd Disable %%i max
	REM CWnd Disable %%i close
REM )) 2>nul
REM cmdow @ /DIS 2>nul
cls
modes 75 16
for /l %%i in (1,1,9) do echos 0x08 "                                           |"
echos 0x08 "___________________________________________|________________________________"
echo;
set /a wide=45
gotoxy -l %wide% 1
echo;©À©¤ %cd%
gotoxy -l %wide% 2
echo;©¦   ©¸©¤ test.c     
gotoxy -l %wide% 3
echo;©À©¤ server
gotoxy -l %wide% 4
echo;©¦   ©À©¤ callbacks.h     
gotoxy -l %wide% 5
echo;©¦   ©¸©¤ ini
gotoxy -l %wide% 6
echo;©¦       ©¸©¤ ini.h    
gotoxy -l %wide% 7
echo;©¸©¤ zombie 

gotoxy -l 0 0

set /a code_num=0
call :code_show
printf -t 0x08 10 "#include "
printf -t 0x06 10 "<iostream>"
call :code_show
printf -t 0x03 10 "using namespace "
printf -t 0x07 10 "std;"
call :code_show
printf -t 0x03 10 "int "
printf -t 0x07 10 "main("
printf -t 0x03 10 "int "
printf -t 0x07 10 "argc,"
printf -t 0x03 10 "char *"
printf -t 0x07 10 "argv[])"
call :code_show
printf -t 0x07 10 {
call :code_show
printf -t 0x07 10 "	cout<<"
printf -t 0x06 10 "\"hello world\""
printf -t 0x07 10 ");"
call :code_show
printf -t 0x03 10 "	return "
printf -t 0x02 10 0
printf -t 0x07 10 ;
call :code_show
printf -t 0x07 10 }
gotoxy -l %wide% 2
echo;©¦   ©À©¤ test.c   
gotoxy -l %wide% 3         
echo;©¦   ©¸©¤ test.exe       
gotoxy -l %wide% 4        
echo;©À©¤ server             
gotoxy -l %wide% 5
echo;©¦   ©À©¤ callbacks.h       
gotoxy -l %wide% 6
echo;©¦   ©¸©¤ ini         
gotoxy -l %wide% 7
echo;©¦       ©¸©¤ ini.h      
gotoxy -l %wide% 8
echo;©¸©¤ zombie    

gotoxy -l 0 11
call fortune

REM cmdow @ /ENA 2>nul
REM cmdow @ /NOT 2>nul>nul
REM (for /f %%i in ('CWnd find /!') do (
	REM CWnd enable %%i min
	REM CWnd enable %%i max
	REM CWnd enable %%i close
REM )) 2>nul>nul
REM taskkill -f -im cmatrix.exe 2>nul>nul

call :SetConsoleCursorInfo 1 3
exit /b


:code_show
set /a code_num+=1
echo;
if %code_num% GTR 9 ( printf 0x08 " %code_num% " ) else printf 0x08 "  %code_num% "
exit /b

:command
	echos 0x06 [ÊäÈëÃüÁî]
	printf 0x0a %used%@%COMPUTERNAME%
	printf 0x09 [%cd%]
	set /p "comd=$ "
	if not defined comd exit /b 404
	call %comd%
	set comd=
exit /b


:showcmd
printf 0x07 "%used%@%COMPUTERNAME%[%cd%]$ "
timeout 1 >nul
printf -t 0x07 100 "%~1"
timeout 1 >nul
echo;
exit /b

:TimeMain
tasklist | findstr /i tray.exe || start tray "cmd /c %~dp0..\..\Termux.bat" %~dp0..\home\console.ico %~dp0..\home\console-off.ico
@Mode Con: Cols=128 Lines=9 & Chcp 437 > Nul & Title Clock & @SetLocal EnableExtensions EnableDelayedExpansion
	@Call :SetConsoleTextAttribute 0-0-0 58-150-221
	@Cls
	@Call :SetConsoleCursorInfo 2 0
	@Call :ShowTime
@Exit /B 0

:ShowTime
	@Call :GotoXY 61 2
	@Call :SetConsoleTextAttribute 0-0-0 58-150-221
	Set _Temp_Time_=%Time:~0,8%
	Set /P=#%_Temp_Time_%< Nul
	Set _Temp_Time_=%_Temp_Time_: =0+%
	Set _Temp_Time_=%_Temp_Time_::0=:0+%

	Set "Block=ÛÛ"
	Set "Block=%Block:~0,1%"

	For /F "Delims=:, Tokens=1,2,3" %%i in ("%_Temp_Time_%") Do (
		Set /A _Temp_H_=%%i
		Set /A _Temp_M_=%%j
		Set /A _Temp_S_=%%k
	)

	@Call :SetConsoleTextAttribute 255-255-255 58-150-221

	@Call :GotoXY 5 4
	For /L %%i in (0,1,23) Do (
		For /L %%j in (0,1,4) Do (
			If %%i Lss !_Temp_H_! (
				Set /P=#%Block%< Nul
			) Else Set /P=# < Nul
		)
	)

	@Call :GotoXY 5 6
	For /L %%i in (0,1,59) Do (
		For /L %%j in (0,1,1) Do (
			If %%i Lss !_Temp_M_! (
				Set /P=#%Block%< Nul
			) Else Set /P=# < Nul
		)
	)

	@Call :GotoXY 5 8
	For /L %%i in (0,1,59) Do (
		For /L %%j in (0,1,1) Do (
			If %%i Lss !_Temp_S_! (
				Set /P=#%Block%< Nul
			) Else Set /P=# < Nul
		)
	)

@Goto :ShowTime





%================================================================%
%= ÈÕÆÚ: 2022 Äê 9 ÔÂ 10 ÈÕ =%
%= Ãû³Æ: Type =%
%= ×÷ÓÃ: Ä£Äâ´ò×Ö =%
%= ²ÎÊı: String ÒªÏÔÊ¾µÄ×Ö·û´® =%
%= ±¸×¢: ´Ëº¯ÊıÒª·ÃÎÊ»·¾³±äÁ¿ _._ =%
%================================================================%
:Type <String>
	If "%~1"=="" @Goto :Eof
	Set "_._=%~1"
	Set /P=#%_._:~0,1%< Nul
	Set "_._=%_._:~1%"
	TimeOut /T 0 > Nul 2> Nul
	If Defined _._ @Call :Type "%_._%"
@Goto :Eof

%================================================================================================================================%
%= VT100 º¯Êı =%
%================================================================================================================================%

%================================================================%
%= ÈÕÆÚ: 2022 Äê 9 ÔÂ 9 ÈÕ =%
%= Ãû³Æ: InitVT100 =%
%= ×÷ÓÃ: ³õÊ¼»¯ Windows 10 µÄ ĞéÄâÖÕ¶ËĞòÁĞ =%
%================================================================%
:InitVT100
%= { =%

	%= »ñÈ¡ ESC ×Ö·û =%

	Set "ESC="

	%= Èç¹ûÃ»ÓĞ¶¨Òå ESC ×Ö·û£¬Õâ¸ö¹ı³ÌÊÇÎªÁË·ÀÖ¹ÂÛÌ³ÎóÍÌÉÏÎÄµÄ ESC ×Ö·û =%
	If Not Defined ESC (

		For /F "Delims=#" %%_ in ('Prompt #$E# ^& Echo On ^& For %%$ in ^(1^) Do DosKey') Do Set "ESC=%%_"
		REM for /F %%a in ('echo prompt $E^| cmd') do set "ESC=%%a"
	)

%= } =%
@Goto :Eof

%================================================================%
%= ÈÕÆÚ: 2022 Äê 9 ÔÂ 9 ÈÕ =%
%= Ãû³Æ: GotoXY =%
%= ×÷ÓÃ: ¶¨Î»¹â±êµ½Ö¸¶¨Î»ÖÃ =%
%= ²ÎÊı: X ¹â±êµÄ X Öá×ø±ê£¬Èç¹ûÊ¡ÂÔ»òÉèÎª 0£¬ÔòÉèÆäÎª1 =%
%= ²ÎÊı: Y ¹â±êµÄ Y Öá×ø±ê£¬Èç¹ûÊ¡ÂÔ»òÉèÎª 0£¬ÔòÉèÆäÎª1 =%
%================================================================%
:GotoXY <X> <Y>
%= { =%

	%= Èç¹ûÃ»ÓĞ¶¨Òå ESC ×Ö·û =%
	If Not Defined ESC (

		@Call :InitVT100

	)

	%= ¶¨Î»¹â±ê =%
	Set /P=%ESC%[%2;%1H< Nul

%= } =%
@Goto :Eof

%================================================================%
%= ÈÕÆÚ: 2022 Äê 9 ÔÂ 9 ÈÕ =%
%= Ãû³Æ: SetConsoleTextAttribute =%
%= ×÷ÓÃ: ÉèÖÃ¿ØÖÆÌ¨ÎÄ±¾ÑÕÉ« =%
%= ²ÎÊı: BGCOL ±³¾° RGB ÑÕÉ«Öµ£¬¸ñÊ½ÎªR-G-B =%
%= ²ÎÊı: FGCOL Ç°¾° RGB ÑÕÉ«Öµ£¬¸ñÊ½ÎªR-G-B =%
%================================================================%
:SetConsoleTextAttribute <BGCOL> <FGCOL>
%= { =%

	%= Èç¹ûÃ»ÓĞ¶¨Òå ESC ×Ö·û =%
	If Not Defined ESC (

		@Call :InitVT100

	)

	%= ÉèÖÃ±³¾°É« =%
	For /F "Delims=-, Tokens=1,2,3" %%I in ("%1") Do (
		Set /P=%ESC%[48;2;%%I;%%J;%%Km< Nul
	)

	%= ÉèÖÃÇ°¾°É« =%
	For /F "Delims=-, Tokens=1,2,3" %%I in ("%2") Do (
		Set /P=%ESC%[38;2;%%I;%%J;%%Km< Nul
	)

%= } =%
@Goto :Eof

%================================================================%
%= ÈÕÆÚ: 2022 Äê 9 ÔÂ 9 ÈÕ =%
%= Ãû³Æ: SetConsoleCursorInfo =%
%= ×÷ÓÃ: ÉèÖÃ¿ØÖÆÌ¨¹â±êÊôĞÔ =%
%= ²ÎÊı: CursorVisibility ¹â±ê¿É¼ûºÍÉÁË¸×´Ì¬£¬È¡Öµ1~4£¬·Ö±ğÎª£º =%
%=       1 ÏÔÊ¾¹â±ê =%
%=       2 Òş²Ø¹â±ê =%
%=       3 ¿ªÊ¼¹â±êÉÁË¸ =%
%=       4 Í£Ö¹ÉÁË¸¹â±ê =%
%= ²ÎÊı: CursorShape      ×Ô¶¨Òå¹â±êĞÎ×´£¬È¡Öµ0~6£¬·Ö±ğÎª£º =%
%=       0 ÓÃ»§ÅäÖÃµÄÄ¬ÈÏ¹â±êĞÎ×´ =%
%=       1 ÉÁË¸¿é¹â±êĞÎ×´ =%
%=       2 ÎÈ¶¨¿é¹â±êĞÎ×´ =%
%=       3 ÉÁË¸ÏÂ»®Ïß¹â±êĞÎ×´ =%
%=       4 ÎÈ¶¨ÏÂ»®Ïß¹â±êĞÎ×´ =%
%=       5 ÉÁË¸ÌõĞÎ¹â±êĞÎ×´ =%
%=       6 ÎÈ¶¨ÌõĞÎ¹â±êĞÎ×´ =%
%================================================================%
:SetConsoleCursorInfo <CursorVisibility> <CursorShape>
%= { =%

	%= Èç¹ûÃ»ÓĞ¶¨Òå ESC ×Ö·û =%
	If Not Defined ESC (

		@Call :InitVT100

	)

	%= ÉèÖÃ¹â±ê¿É¼ûºÍÉÁË¸×´Ì¬ =%
	For /F "Delims=-, Tokens=%1" %%I in ("25h-25l-12h-12l") Do (
		Set /P=%ESC%[?%%I< Nul
	)

	Set /P=%ESC%[%2 q< Nul

%= } =%
@Goto :Eof

%================================================================%
%= ÈÕÆÚ: 2022 Äê 9 ÔÂ 9 ÈÕ =%
%= Ãû³Æ: SetConsoleTextAttributeEx =%
%= ×÷ÓÃ: ¸ñÊ½»¯¿ØÖÆÌ¨ÎÄ±¾ =%
%= ²ÎÊı: Code ²Ù×÷´úÂë£¬È¡Öµ1~6£¬·Ö±ğÎª£º =%
%=       1 Ä¬ÈÏ£¬Í¨³£Ïàµ±ÓÚColor 07 =%
%=       2 ´ÖÌå/ÁÁ =%
%=       3 ÎŞ´ÖÌå/ÁÁ =%
%=       4 ÏÂ»®Ïß =%
%=       5 ÎŞÏÂ»®Ïß =%
%=       6 ½»»»Ç°¾°É«ºÍ±³¾°É« =%
%================================================================%
:SetConsoleTextAttributeEx <Code>
%= { =%

	%= Èç¹ûÃ»ÓĞ¶¨Òå ESC ×Ö·û =%
	If Not Defined ESC (

		@Call :InitVT100

	)

	For /F "Delims=-, Tokens=%1" %%I in ("0-1-22-4-24-7") Do (
		Set /P=%ESC%[%%Im< Nul
	)

%= } =%
@Goto :Eof


:game_loop
call :SetConsoleCursorInfo 2 0
mode 70,16
set MAX_X=20
set MAX_Y=13
for /l %%i in (1,1,15) do (
echo;                                          ^|
)
echo;------------------------------------------+ [20,13] wasd ESCÍË³ö
set X=2
set Y=2
title [%X%,%Y%]
gotoxy -l 4 2
set /p.=ÎÒ<nul
:loop_game
title [%X%,%Y%]
words -key
if %errorlevel% EQU 119 (
	if %Y% GEQ 1 (
		set /a Y-=1
		printfs "  "
		gotoxy -2 -1
		set /p.=ÎÒ<nul
	)
) else if %errorlevel% EQU 97 (
	if %X% GEQ 1 (
		set /a X-=1
		REM gotoxy -4 0
		printfs "  "
		set /p.=ÎÒ<nul
	)
) else if %errorlevel% EQU 115 (
	if %Y% LSS %MAX_Y% (
		set /a Y+=1
		printfs "  "
		gotoxy -2 1
		set /p.=ÎÒ<nul
	)
) else if %errorlevel% EQU 100 (
	if %X% LSS %MAX_X% (
		set /a X+=1
		set /p.=  ÎÒ<nul
	)
) else if %errorlevel% EQU 27 (
	call :SetConsoleCursorInfo 1 3
	exit /b
) 
goto :loop_game


for /F %%a in ('"prompt $h&for %%b in (1) do rem"')do Set "bs=%%a" &REM ÍË¸ñ¼ü»ñÈ¡
REM ----------------------------------------------help command----------------------------------------------

::::::::::::::::::::helps_cmdow.exe::::::::::::::::::::::

Ö´ĞĞcmdow @£¬ÏÔÊ¾Îª
Handle Lev Pid -Window status- Image Caption
0x230088 1 3356 Res Act Ena Vis cmd C:\WINDOWS\system32\cmd.exe - cmd - cm
window Ö¸¶¨ÒªÁĞ³öµÄ´°¿Ú£¬Ê¹ÓÃ´°¿Ú±êÌâ»ò¾ä±úÀ´Ö¸¶¨£¬Ã»ÓĞ±êÌâµÄÓÃ´°¿ÚÀà´úÌæ¡£Èç¹ûÃ»ÓĞÖ¸¶¨ÔòÁĞ³öËùÓĞ´°¿Ú¡£¡°@¡±±íÊ¾´Ë´°¿Ú£¬¼´ÃüÁîĞĞ´°¿Ú¡£
/T Ö»ÁĞ³öÈÎÎñÀ¸µÄ´°¿Ú
/B ÈçÉÏÃæµÄÀı×Ó£¬¼ÓÕâ¸ö²ÎÊıÔò²»ÏÔÊ¾µÚÒ»ĞĞ
/F ÏÔÊ¾ÍêÕû±êÌâ
/P ÁĞ³ö´°¿ÚÎ»ÖÃ¡¢¿íÓë¸ß
ÁíÍâ
Handle ¾ä±ú£¬16½øÖÆ¸ñÊ½
Window status ´°¿Ú×´Ì¬£¨Minimized, Maximized, Restored, Active, Inactive, Enabled, Disabled, Visible or Hidden£©

CMDOW /TH | /TV | /CW | /MA | /UW | /AT | /FS | /WM/TH ºáÏòÆ½ÆÌ´°¿Ú
/TV ×İÏòÆ½ÆÌ´°¿Ú
/CW ²ãµş´°¿Ú
/MA ×îĞ¡»¯ËùÓĞ´°¿Ú
/UW È¡Ïû/TH, /TV, /CW , /MA
/AT Ïàµ±ÓëAlt+Esc
/FS È«ÆÁÃüÁîĞĞ´°¿Ú£¬Ïàµ±ÓÚAlt+Enter
/WM È¡ÏûÈ«ÆÁ£¬¼´ÔÙ°´Ò»´ÎAlt+Enter

CMDOW window {[/ACT] [/INA] [/ENA] [/DIS] [/VIS] [/HID] [/MIN] [/MAX] [/RES] [/TOP] [/NOT] [/REN caption] [/MOV left top] [/SIZ width height] [/CLS] [/END]}/ACT ¼¤»îÖ¸¶¨´°¿Ú
/INA ²»ÖªÓĞÉ¶ÓÃ
/DIS °Ñ´°¿Ú±äÎª¡°Ö»Ğí¿´²»×¼Ãş¡±£ºÎŞ·¨ÒÆ¶¯¡¢Êó±ê×óÓÒ¼üÎŞĞ§£¬¸ü±ğËµÊäÈëÁË¡£µ«¿ÉÒÔÓÃ¿ì½İ¼ü¹Ø±Õ¡£
/ENA ½â³ı/DIS
/HID Òş²Ø´°¿Ú£¬ÈÎÎñÀ¸ºÍÍĞÅÌ½Ô¿´²»µ½
/VIS ÏÔÊ¾±»Òş²ØµÄ´°¿Ú
/MIN ×îĞ¡»¯Ö¸¶¨´°¿Ú
/MAX ×î´ó»¯Ö¸¶¨´°¿Ú
/RES ³·Ïú¡£±ÈÈç³·Ïúµ½´°¿ÚÔ­À´µÄ´óĞ¡ºÍÎ»ÖÃ¡£²»ÄÜ³·ÏúÁ½´Î£¬³·Ïúºó²»¿ÉÒÔ·µ»Ø³·ÏúÇ°µÄ×´Ì¬¡£
/TOP ÖÃ¶¥´°¿Ú¡£ÈçÖÃ¶¥ÁË¶à¸ö£¬ÔòµÚÒ»¸öÖÃ¶¥ÔÚ×îÉÏÃæ£¬ÖÃ¶¥ÁËµÄ¶¼»á±£ÁôÖÃ¶¥×´Ì¬¡£
/NOT È¡Ïû´°¿ÚÖÃ¶¥
/REN caption ÖØÃüÃû´°¿Ú±êÌâ
/MOV left top ÒÆ¶¯´°¿Ú ¾à×ó±ß¾àÀë ¾àÉÏ±ß¾àÀë
/SIZ width height ¸Ä±ä´°¿Ú¿í¡¢¸ß
/CLS ¹Ø±Õ´°¿Ú
/END ½áÊø½ø³Ì¡£¿Ö²ÀµÄÊÇÏàÍ¬PIDµÄ»áÒ»ÆğKillµô£¬Ğ¡ĞÄÊ¹ÓÃ£¡

cmdow @ /siz 700 350

CMDOW /RUN [state] file [args]state ÔËĞĞ·½Ê½£¨/MIN /MAX /HID£©

::::::::::::::::::::helps_png2ico.exe::::::::::::::::::::::
png2ico -i "png" -o "ico" -s 16 32bpp -s 24 32bpp -s 32 32bpp -s 48 32bpp -s 64 32bpp -s 72 32bpp -s 96 32bpp -s 128 32bpp -noconfirm
           "png"    "ico" Ö»ÄÜÊÇÎÄ¼ş¼ĞÃû³Æ
		   
::::::::::::::::::::helps_jpeg.exe::::::::::::::::::::::

-te <ÆäËûjpegÎÄ¼ş>
½«ÆäËûjpegÎÄ¼şµÄeixfµ¼ÈëÄ¿±êjpeg¡£ÀıÈç"jhead ¨Cte D:\My Pictures\B.jpg D:\My Pictures\A.jpg"

-dc
É¾³ıjpegĞÅÏ¢ÖĞµÄ±¸×¢¡£×¢Òâ£¬jpegÎÄ¼şÓĞÁ½¸ö±¸×¢£¬Ò»ÊÇºÍÆäËûÎÄ¼şÒ»ÑùµÄ±¸×¢£¬ÁíÒ»¸öÊÇexifĞÅÏ¢ÖĞµÄ±¸×¢¡£jhead½ö¶ÔexifĞÅÏ¢ÓĞĞ§¡£
-de
ÍêÈ«É¾³ıexifĞÅÏ¢¡£
-du
É¾³ı·ÇÔ­Ê¼exifĞÅÏ¢£¬ÀıÈçPhotoshop¡¢TurbophotoÖ®Àà±à¼­ºóĞŞ¸ÄexifÁôÏÂµÄĞÅÏ¢¡£
-purejpg
É¾³ıËùÓĞjpegÎÄ¼ş·Ç±ØĞëµÄĞÅÏ¢¡£Ïàµ±ÓÚ-de¡¢-dcºÍ-duµÄ¼¯ºÏ£¬¿ÉÒÔ½«ÎÄ¼ş¼õĞ¡Êık¡£
-ce
ĞŞ¸ÄÎÄ¼şµÄjpegÎÄ¼şÍ·²¿·Ö±¸×¢£¨´Ë±¸×¢²¢·ÇexifĞÅÏ¢£©¡£¸ÃÖ¸Áî»á´ò¿ªÎÄ±¾±à¼­Æ÷£¬²¢ÔÚ±à¼­Æ÷¹Ø±ÕÊ±½«±¸×¢ĞÅÏ¢´æÈëÎÄ¼ş¡£
-cs <ÎÄ±¾ÎÄ¼ş>
µ¼³ö±¸×¢¡£ÀıÈç"jhead ¨Ccs D:\1.txt D:\My Pictures\A.jpg"
-ci <ÎÄ±¾ÎÄ¼ş>
µ¼Èë±¸×¢¡£ÀıÈç"jhead ¨Cci D:\1.txt D:\My Pictures\A.jpg"
-cl <±¸×¢ÎÄ×Ö>
Ö±½ÓÊäÈë±¸×¢¡£¡£ÀıÈç"jhead ¨Ccl ÎÒµÄ±¸×¢ D:\My Pictures\A.jpg"
¶ş¡¢ÆäËûÖ¸Áî
Ê±¼äÈÕÆÚ
-ft ½«jpegÎÄ¼şµÄ¡°ĞŞ¸ÄÊ±¼ä¡±ĞŞ¸ÄÎªexifĞÅÏ¢ÖĞ¼ÇÂ¼µÄÊ±¼ä¡£
-n[<¸ñÊ½-Ë³Ğò>] ¸ÃÖ¸Áî»á½«ÎÄ¼şÃûĞŞ¸ÄÎªexifĞÅÏ¢ÖĞ¼ÇÂ¼µÄ¡°´´½¨Ê±¼ä¡±£»Èç¹ûjpegÎÄ¼şÃ»ÓĞexif»òÕßexifÖĞµÄ´´½¨Ê±¼ä²»¿ÉÓÃ£¬Ôò½«ÎÄ¼şÃûĞŞ¸ÄÎªÎÄ¼şµÄ¡°ĞŞ¸ÄÊ±¼ä¡±¡£
Ä¬ÈÏµÄ¸ñÊ½-Ë³ĞòÎªMMDD-HHMMSS
¸ñÊ½-Ë³Ğò²ÎÊıÈçÏÂ£º
%d-ÈÕ(01-31)
%H-Ğ¡Ê±(00-23)
%j-Ò»ÄêÖĞµÄµÚ¼¸Ìì(001-366)
%m-ÔÂ(01-12)
%M-·ÖÖÓ(00-59)
%S-Ãë(00-59)
%U-Ò»ÄêÖĞµÄµÚ¼¸ÖÜ(00-53)
%w-ĞÇÆÚ¼¸(0-6£¬ÖÜÈÕÎª0)
%y-Á½Î»Êı¼ÍÄê(00-99)
%Y-ËÄÎ»Êı¼¸Äê
%i-Ìí¼ÓÊı×ÖĞòºÅ
ÀıÈç£º
jhead -n%Y%m%d-%H%M%S d:\*.jpg
½«ËùÓĞjpgÎÄ¼şĞŞ¸ÄÎªYYYYMMDD-HHMMSS.jpgµÄ¸ñÊ½¡£
-nf Óë¡°-n¡±ÏàÍ¬¹¦ÄÜÏàÍ¬£¬²»±£ÁôÔ­ÎÄ¼şÃû¡£
-a ĞŞ¸Ä²»Í¬À©Õ¹ÃûµÄÍ¬ÃûÎÄ¼şÃû£¬Ïà»úÅÄÉãµÄavi¶ÌÆ¬exifĞÅÏ¢´æ´¢ÔÚÓëÆäÍ¬ÃûµÄthmÎÄ¼şÖĞ£¬¿ÉÓÃ´ËÖ¸Áî¸øaviÎÄ¼ş¸üÃû¡£Ò»°ãÓë¡°-n¡±Ö¸Áî¹²Í¬Ê¹ÓÃ¡£
-ta<Ê±²î> ĞŞÕıÊ±²î£¬ÀıÈçÊ±²î¸ù¾İÊ±ÇøÈ·¶¨£¬ÀıÈç+1:00»òÕß-1:00
-da<ÈÕÆÚ>-<ÈÕÆÚ> ĞŞÕıÈÕÆÚ¡£ÈÕÆÚ¸ñÊ½ÊÇyyyy:mm:dd¡¢yyyy:nn:dd+hh:mm»òÕß yyyy:mm:dd+hh:mm:ss¡£¸ù¾İÇ°ºó²ÎÊıÊ±¼ä²îµ÷ÕûexifµÄÊ±¼ä¡£
-ts<ÈÕÆÚ-Ê±¼ä> Ö±½ÓĞŞ¸ÄexifÖĞµÄÅÄÉãÊ±¼ä£¬ÈÕÆÚ-Ê±¼ä¸ñÊ½Îªyyyy:mm:dd-hh:mm:ss
ËõÂÔÍ¼

-dt É¾³ıexifÖĞµÄËõÂÔÍ¼¡£Õâ¸öËõÂÔÍ¼Ò»°ãÎª240x160ÏñËØ£¬10k´óĞ¡£¬ÓÃÓÚÊıÂëÏà»ú¡¢Windows XP²é¿´ÕÕÆ¬£¬É¾³ıËü²»»áÓ°Ïì¹¤×÷¡£
-st <ÎÄ¼şÃû> ½«exifÖĞµÄËõÂÔÍ¼¸´ÖÆÎªÁíÒ»¸öjpegÎÄ¼ş
-rt <ÎÄ¼şÃû> ÓÃÁíÒ»¸öjpegÎÄ¼şÌæ»»exifÖĞµÄËõÂÔÍ¼
-rgt[´óĞ¡] Ë¢ĞÂexifËõÂÔÍ¼£¬ÆäÖĞ´óĞ¡ÎªËõÂÔÍ¼µÄ×î´ó±ß³¤¡£
Ğı×ª
-autorot ¸ù¾İexifÖĞ¼ÇÂ¼µÄË®Æ½·½ÏòĞÅÏ¢×ª¶¯jpegÕÕÆ¬¡£
-norot Çå³ıexifÖĞµÄË®Æ½·½ÏòĞÅÏ¢¡£


::::::::::::::::::::helps_nircmd.exe::::::::::::::::::::::

µ¯³öÅÌ·ûÎªJµÄ¹âÇı

nircmd.exe cdrom open j:

µ¯ÈëÅÌ·ûÎªYµÄ¹âÇı

nircmd.exe cdrom close y:

¶Á³ö¼ôÌù°åÎÄ±¾ÄÚÈİ

speak text ~$clipboard$

Ôö¼Ó2000¸öµ¥Î»µÄÏµÍ³ÒôÁ¿(×î´ó65535)

nircmd.exe changesysvolume 2000

½µµÍ5000¸öµ¥Î»µÄÏµÍ³ÒôÁ¿(×î´ó65535)

nircmd.exe changesysvolume -5000

ÉèÖÃÎª×î¸ßÒôÁ¿

nircmd.exe setsysvolume 65535

ÉèÖÃÎª¾²Òô

nircmd.exe mutesysvolume 1

È¡Ïû¾²Òô

nircmd.exe mutesysvolume 0

ÔÚ¾²ÒôÓëÕı³£×´Ì¬Ö®¼äÇĞ»»

nircmd.exe mutesysvolume 2

ÔÚ×ÀÃæ´´½¨¾²ÒôÓëÕı³£×´Ì¬ÇĞ»»ÃüÁîµÄ¿ì½İ·½Ê½

nircmd.exe cmdshortcut "~$folder.desktop$" "Switch Volume" mutesysvolume 2

¹Ø±ÕÏÔÊ¾Æ÷

nircmd.exe monitor off

Æô¶¯Ä¬ÈÏÆÁ±£

nircmd.exe screensaver

½øÈë´ı»ú×´Ì¬

nircmd.exe standby

×¢Ïúµ±Ç°ÓÃ»§

nircmd.exe exitwin logoff

µ¯³ö¶Ô»°¿ò£¬Ñ¯ÎÊÊÇ·ñÖØÆô¡£Ñ¡ÔñÊÇÔòÖØÆôµçÄÔ

nircmd.exe qboxcom "Do you want to reboot ?" "question" exitwin reboot

¹Ø±Õ¼ÆËã»ú

nircmd.exe exitwin poweroff

¹Ø±Õcomputers.txtÖĞÖ¸¶¨µÄËùÓĞ¼ÆËã»ú

multiremote copy "c:\temp\computers.txt" exitwin poweroff force

²¦ºÅ¿í´øÁ¬½Ó

nircmd.exe rasdial "My Internet"

¶Ï¿ª¿í´øÁ¬½Ó

nircmd.exe rashangup "My Internet"

ÉèÖÃIE´°¿ÚÎª75%Í¸Ã÷¶È(192/256)

nircmd.exe win trans ititle "internet explorer" 192

×îĞ¡»¯ËùÓĞµÄIE´°¿Ú

nircmd.exe win min class "IEFrame"

¹Ø±ÕËùÓĞµÄIE´°¿Ú

nircmd.exe win close class "IEFrame"

¹Ø±ÕËùÓĞµÄ×ÊÔ´¹ÜÀíÆ÷´°¿Ú(ÎÒµÄµçÄÔ¡¢ÎÄ¼ş¼ĞµÈ)

nircmd.exe win close class "CabinetWClass"

Òş²ØËùÓĞµÄIE´°¿Ú

nircmd.exe win hide class "IEFrame"

ÏÔÊ¾ËùÓĞµÄIE´°¿Ú(ÔÚÓÃÉÏÃæÃüÁîÒş²Øºó)

nircmd.exe win show class "IEFrame"

°ÑËùÓĞµÄ¶¥¼¶´°¿Ú¾ÓÖĞ

nircmd.exe win center alltop

È¥µôÎÒµÄµçÄÔ´°¿ÚµÄ±êÌâÀ¸

nircmd.exe win -style title "my computer" 0x00C00000

»Ö¸´ÎÒµÄµçÄÔ´°¿ÚµÄ±êÌâÀ¸(µ±ÓÃÉÏÃæÃüÁîÈ¥³ıºó)

nircmd.exe win +style title "my computer" 0x00C00000

ÉèÖÃÎÒµÄµçÄÔ´°¿ÚÎª´ÓÓÒÖÁ×óµÄË³Ğò(ÊÊÓÃÓÚÏ£²®À´ÓïºÍ°¢À­²®Óï)

nircmd win +exstyle title "my computer" 0x00400000

ÉèÖÃÎÒµÄµçÄÔ´°¿ÚµÄËùÓĞ×Ó´°¿ÚÎª´ÓÓÒÖÁ×óµÄË³Ğò(ÊÊÓÃÍ¬ÉÏ)

nircmd win child title "my computer" +exstyle all 0x00400000

ÔÚ×ÀÃæ´´½¨¿ÉÒÔ¹Ø±ÕËùÓĞIE´°¿ÚµÄ¿ì½İ·½Ê½

nircmd.exe cmdshortcut " "~$folder.desktop$ "Close All IE" win close class "IEFrame"

ÔÚ×ÀÃæ´´½¨¿ÉÒÔÒş²ØËùÓĞIE´°¿ÚµÄ¿ì½İ·½Ê½

nircmd.exe cmdshortcut " "~$folder.desktop$ "Hide All IE" win hide class "IEFrame"

ÔÚ×ÀÃæ´´½¨¿ÉÒÔÔÙ´ÎÏÔÊ¾ËùÓĞIE´°¿ÚµÄ¿ì½İ·½Ê½

nircmd.exe cmdshortcut " "~$folder.desktop$ "Show All IE" win show class "IEFrame"

°Ñcalculator´°¿ÚÉèÖÃÎª×îÉÏ²ã´°¿Ú(ÔÚÆäËû´°¿ÚÖ®ÉÏ)

nircmd.exe win settopmost title "Calculator" 1

»Ö¸´calculator´°¿ÚÎªÔ­Ê¼´°¿Ú×´Ì¬(²»´¦ÓÚ×îÉÏ²ã)

nircmd.exe win settopmost title "Calculator" 0

´´½¨calc.exe¿ì½İ·½Ê½µ½¿ªÊ¼²Ëµ¥->³ÌĞò->Calculators(¼ÆËãÆ÷)

nircmd.exe shortcut "f:\winnt\system32\calc.exe" "~$folder.programs$\Calculators" "Windows Calculator"

Òş²Ø×ÀÃæËùÓĞÍ¼±ê

nircmd.exe win hide class progman

ÏÔÊ¾×ÀÃæËùÓĞÍ¼±ê(ÓÃÒÔÉÏÃüÁîÒş²Ø¹ıºó)

nircmd.exe win show class progman

Òş²ØÏµÍ³¿ªÊ¼°´Å¥

nircmd.exe win child class "Shell_TrayWnd" hide class "button"

ÏÔÊ¾ÏµÍ³¿ªÊ¼°´Å¥

nircmd.exe win child class "Shell_TrayWnd" show class "button"

Òş²ØÏµÍ³Ê±¼ä

nircmd.exe win child class "Shell_TrayWnd" hide class "TrayClockWClass"

ÏÔÊ¾ÏµÍ³Ê±¼ä

nircmd.exe win child class "Shell_TrayWnd" show class "TrayClockWClass"

ÖÕÖ¹ËùÓĞIEÊµÀı

nircmd.exe killprocess iexplore.exe

´´½¨×ÀÃæ¿ì½İ·½Ê½£¬ÔËĞĞ¼´¿Éµ¯³ö¹âÇı(KÅÌ)

nircmd.exe cmdshortcut "~$folder.desktop$" "Open CDROM" cdrom open k:

½«nirsoftÍøÖ·´´½¨µ½×ÀÃæ¿ì½İ·½Ê½

nircmd.exe urlshortcut "http://www.nirsoft.net" "~$folder.desktop$" "NirSoft"

Ìí¼ÓnirsoftÍøÖ·µ½ÊÕ²Ø¼ĞÁ´½ÓÏÂ

nircmd.exe urlshortcut "http://www.nirsoft.net" "~$folder.favorites$\Links" "NirSoft"

½«nirsoftÍøÖ·¿ì½İ·½Ê½´´½¨µ½computers.txtÖĞÁĞ³öµÄËùÓĞ¼ÆËã»úµÄ×ÀÃæ

nircmd.exe multiremote copy "c:\temp\computers.txt" urlshortcut "http://www.nirsoft.net" "~$folder.common_desktop$" "NirSoft"

ÉèÖÃÏÔÊ¾Ä£Ê½Îª800X600£¬24Î»É«

nircmd.exe setdisplay 800 600 24

½«ÉèÖÃÏÔÊ¾Ä£Ê½Îª800X600£¬24Î»É«ÃüÁî´´½¨Îª×ÀÃæ¿ì½İ·½Ê½

nircmd.exe cmdshortcut "~$folder.desktop$" "800x600x24" setdisplay 800 600 24

¸´ÖÆËùÓĞ×ÀÃæ¿ì½İ·½Ê½µ½ÆäËûÎÄ¼ş¼Ğ(f:tempdesktop)

nircmd.exe execmd copy "~$folder.desktop$\*.lnk" f:\temp\desktop

ÖØÆôµÄApache server·şÎñÆ÷(windows NT/2000/XP/2003)

nircmd.exe service restart apache

½«ÖØÆôApache server·şÎñÆ÷ÃüÁî´´½¨Îª×ÀÃæ¿ì½İ·½Ê½

nircmd.exe cmdshortcut "~$folder.desktop$" "Restart Apache" service restart apache

ÖØÆôIIS(»¥ÁªÍøĞÅÏ¢·şÎñ)

nircmd.exe service restart w3svc

ÖØÆôMySql

nircmd.exe service restart MySql

´ò¿ªÖ¸¶¨×¢²á±íÏî/Öµ

nircmd.exe regedit "HKLM\Software\Microsoft\Windows\CurrentVersion" "CommonFilesDir"

´ò¿ªÔÚ×¢²á±í±à¼­Æ÷ÖĞ¸´ÖÆµ½¼ôÇĞ°åµÄ×¢²á±íÏî

nircmd regedit "~$clipboard$"

½ûÓÃÆÁ±£

nircmd.exe regsetval sz "HKCU\control panel\desktop" "ScreenSaveActive" 0

È¡Ïû½ûÓÃÆÁ±£

nircmd.exe regsetval sz "HKCU\control panel\desktop" "ScreenSaveActive" 1

¸Ä±äÖ¸¶¨ÎÄ¼şµÄÈÕÆÚ/Ê±¼ä(´´½¨Ê±¼äºÍĞŞ¸ÄÊ±¼ä)

nircmd.exe setfiletime "c:\temp\myfile.txt" "24-06-2003 17:57:11" "22-11-2005 10:21:56"

¸´ÖÆ×ÀÃæËùÔÚÂ·¾¶µ½¼ôÇĞ°å

nircmd.exe clipboard set ~$folder.desktop$

¸´ÖÆ¿ªÊ¼²Ëµ¥ÎÄ¼ş¼ĞÂ·¾¶µ½¼ôÇĞ°å

nircmd.exe clipboard set ~$folder.start_menu$

¸´ÖÆÎÄ±¾info1.txtÖĞµÄÄÚÈİµ½¼ôÇĞ°å

nircmd.exe clipboard readfile "c:\My Files\info1.txt"

½«¼ôÇĞ°åÖĞÄÚÈİÌí¼Óµ½ÎÄ±¾info1.txtÖĞ

nircmd.exe clipboard addfile "c:\My Files\info1.txt"

Çå¿Õ¼ôÇĞ°å

nircmd.exe clipboard clear

´´½¨ËùÓĞÔÚ"c:\temp\folders.txt"ÖĞÖ¸¶¨µÄÎÄ¼ş¼Ğ£¬ÎÄ¼ş¼ĞÂ·¾¶ÃûÓÃ»Ø³µ·û¸ô¿ª

nircmd.exe paramsfile "c:\temp\folders.txt" "" "" execmd md ~$fparam.1$

°²×°ËùÓĞÔÚÈ«¾Ö³ÌĞò¼¯»º´æ(like gacutil)ÖĞÖ¸¶¨µÄ.NET³ÌĞò¼¯

nircmd.exe gac install "C:\temp\MyAssembly\bin\MyAssembly.dll"

Çå¿ÕËùÓĞ´ÅÅÌÉÏµÄ»ØÊÕÕ¾

nircmd.exe emptybin

¶ÔÒ»¸ö±ê×¼µÄWindowsĞÅÏ¢¿ò»Ø¸´È·ÈÏ

nircmd.exe dlg "" "" click yes

ÑÓ³Ù2ÃëÖÓ£¬È»ºó±£´æµ±Ç°ÆÁÄ»½ØÍ¼µ½shot.png

nircmd.exe cmdwait 2000 savescreenshot "f:\temp\shot.png"

Ñ­»·±£´æ10¸ö½ØÍ¼£¬½ØÍ¼¼ä¸ô60Ãë£¬ÎÄ¼şÃû°üº¬Ê±¼äºÍÈÕÆÚ

nircmd.exe loop 10 60000 savescreenshot c:\temp\scr~$currdate.MM_dd_yyyy$-~$currtime.HH_mm_ss$.png

µÈ´ıFixfox¹Ø±Õ£¬¶Á³ö "Firefox was closed"£¨»ğºüÒÑ¾­¹Ø±Õ£©

waitprocess firefox.exe speak text "Firefox was closed"

ÏµÍ³ÒªÇó

Õâ¸ö¹¤¾ß¿ÉÒÔ¹¤×÷ÔÚËùÓĞ32Î»µÄÊÓ´°²Ù×÷ÏµÍ³µÄ Windows 9x/ME, Windows NT, Windows 2000, Windows XP, Windows Server 2003,ºÍ Windows Vista¡£²»¹ı£¬Ò»Ğ© NirCmd ÃüÁîÖ»ÄÜÓÃÔÚ Windows NT/2000/XP/2003/Vista µÄÃüÁîĞĞÖ´ĞĞ¡£

¡¡

Ğí¿ÉÖ¤

¡¡

ÕâÊÇÒ»¿îÃâ·ÑÈí¼ş¡£Ö»Òª²»´Ó´ËÖĞÊÕÈ¡·ÑÓÃ£¬¾Í¿ÉÒÔ×ÔÓÉµÄÍ¨¹ıÈíÅÌ£¬CD£¬ÍøÂç»òÕßÆäËû·½·¨·¢²¼¡£×¢ÒâÔÚ·¢²¼Ê±Ó¦°üº¬´Ë·¢²¼°üÄÚµÄËùÓĞÎÄ¼ş£¬²»µÃÓĞÈÎºÎĞŞ¸Ä¡£

¡¡

Ê¹ÓÃNirCmd

¡¡

Õâ¿îÈí¼şÊÇÒ»¸ö¶ÀÁ¢µÄ¿ÉÖ´ĞĞ³ÌĞò£¬²»ĞèÒªÈÎºÎµÄ°²×°¹ı³Ì»òÕß¶îÍâµÄdllÎÄ¼şÖ§³Ö¡£Ö»ĞèÒª°ÑexeÎÄ¼ş¸´ÖÆµ½ÈÎºÎÖ¸¶¨µÄÎÄ¼ş¼Ğ£¬²¢ÇÒ¸ù¾İÄãµÄĞèÒªÌí¼ÓĞèÒªµÄÃüÁîÑ¡ÏîÔËĞĞ¼´¿É¡£ÍÆ¼ö¸´ÖÆNirCmd.exeµ½WindowsÄ¿Â¼£¬»òÕß¸´ÖÆµ½ÈÎºÎÏµÍ³ÖĞpath»·¾³±äÁ¿ÖĞËùÁĞ³öµÄÂ·¾¶ÏÂ£¬ÕâÑù¾Í²»ĞèÒªÔÚÃ¿´ÎÔËĞĞÊ±ÊäÈëNirCmdµÄÍêÕûÂ·¾¶¡£
´Ó°æ±¾1.50¿ªÊ¼£¬NirCmdµÄÒ²¿ÉÒÔÔÚ¿ØÖÆÌ¨ÖĞÊ¹ÓÃ£¬ÔÚ¿ØÖÆÌ¨°æ±¾ÖĞ£¬ËùÓĞµÄ´íÎóĞÅÏ¢¶¼»áÏÔÊ¾ÔÚ¿ØÖÆÌ¨´°¿ÚÖĞ£¬¶ø²»ÊÇ³öÏÖÔÚĞÅÏ¢¿òÖĞ
´ËÊµÓÃ¹¤¾ß¿ÉÒÔÔÚËùÓĞ32Î»Windows²Ù×÷ÏµÍ³£ºWindows 9x/ME£¬Windows NT £¬Windows 2000£¬ Windows XP£¬Windows Server 2003ºÍWindows Vista¡£²»¹ı£¬Ò»Ğ©NirCmdÃüÁîÖ»ÄÜÔÚ Windows NT/2000/XP/2003/Vista ÃüÁîĞĞÖ´ĞĞ¡£

¡¡

NirCmdµÄÒ»°ãÓï·¨

¡¡

nircmd.exe {showerror} [command] [command parameters]

showerrorÊÇ¿ÉÑ¡²ÎÊı¡£Èç¹ûÖ¸¶¨ÁË´Ë²ÎÊı£¬ÄÇÃ´ÔÚÃüÁîÔËĞĞ¹ı³ÌÖĞÈç¹û·¢Éú´íÎó£¬´íÎóĞÅÏ¢»áÏÔÊ¾ÔÚÆÁÄ»ÉÏ¡£·ñÔò£¬»áºöÂÔËùÓĞ´íÎó¡£
ÀıÈç£º
nircmd.exe showerror rasdial "dial1"
nircmd.exe rasdial "dial1"
ÔÚÉÏÃæµÄÁ½¸öÀı×ÓÖĞ£¬NirCmd»á³¢ÊÔÁ¬½ÓdiallÈë¿Ú£¬ÔÚµÚÒ»¸öÀı×ÓÖĞ£¬Èç¹ûÁ¬½ÓÊ§°Ü£¬½«»áÏÔÊ¾Ò»ÌõÓĞ¹Ø´ËÎÊÌâµÄ´íÎóĞÅÏ¢£»ÔÚµÚ¶ş¸öÀı×ÓÖĞ£¬Èç¹ûÁ¬½ÓÊ§°ÜÔò²»»áÏÔÊ¾´íÎóĞÅÏ¢¡£

¡¡

ÌØÊâ×Ö·û´®ĞòÁĞ

¡¡

´Ó°æ±¾1.20¿ªÊ¼£¬NirCmdÔÊĞíÔÚÃüÁî²ÎÊıÖĞ²åÈëÌØÊâµÄ×Ö·û¡£

ÏÂ±íÁĞ³öÁË¿ÉÒÔÊ¹ÓÃµÄÌØÊâ×Ö·û´®ĞòÁĞ£º

Ğò ÁĞ 

Ãè Êö

~n

»»ĞĞ(CR-LF)[°üº¬»»ĞĞ·ûºÍ»Ø³µ·û]

~q

Ë«ÒıºÅ£º"

~t

ÖÆ±í·û Tab¼ü

~xnn

ÓÃÒ»¸ö°üº¬Á½Î»Ê®Áù½øÖÆÊı×ÖµÄASCIIÀ´´ú±íÈÎºÎ×Ö·û¡£ÀıÈç£º~x41 (×Ö·û'A'), ~x27 (µ¥ÒıºÅ)

~$variable$

´ú±íÏµÍ³±äÁ¿£¬¸ü¶àĞÅÏ¢Çë¿´ÏÂÃæµÄÏµÍ³±äÁ¿¡£

~~

´ú±íµ¥¸ö~×Ö·û

ÀıÈç:
infobox "This is the first line~n~qThis is a second line, in quotes~q" "Example"
ÉÏÃæµÄÀı×Ó»áÏÔÊ¾Ò»¸ö°üº¬Á½ĞĞµÄĞÅÏ¢¿ò¡£

ÏµÍ³±äÁ¿

´Ó1.40°æ±¾¿ªÊ¼£¬NirCmdÌá¹©ÁËÌØÊâ×Ö·û´ú±íÎÄ¼ş¼ĞÒÔ¼°ÆäËûÏµÍ³±äÁ¿¡£Ã¿¸ö±äÁ¿¶¼ÒÔ×Ö·û~¿ªÍ·£¬ºóÃæµÄ±äÁ¿Ãû°üº¬ÔÚÁ½¸ö$ÄÚ¡£
Àı£º~$folder.desktop$¾Í´ú±íÁËÏµÍ³×ÀÃæÕâ¸öÎÄ¼ş¼ĞµÄÂ·¾¶¡£(¿ÉÒÔÓÃnircmd infobox "~$folder.desktop"À´²é¿´)¡£
ÏÂ±í¾ÍÖ¸¶¨ÁË¿ÉÒÔÊ¹ÓÃµÄÒ»Ğ©±äÁ¿£º

±äÁ¿Ãû 

Ãè Êö 

clipboard

´ú±í¸´ÖÆµ½¼ôÇĞ°åµÄÎÄ±¾ÄÚÈİ

param.paramname

µ±Ö¸¶¨Ò»¸öparam±äÁ¿Ê±£¬ÆÁÄ»ÉÏ»áÏÔÊ¾Ò»¸öÊäÈë´°¿Ú£¬ÒªÇóÎªÖ¸¶¨µÄ²ÎÊıÊäÈëÎÄ±¾(¿ÉÒÔÀí½âÎª¸ø²ÎÊı¸³Öµ),ËùÊäÈëµÄ ÎÄ±¾»á±»²åÈëµ½ÃüÁîĞĞÖĞ¡£Àı:ÔÚÏÂÃæµÄÃüÁîÖĞ£¬»áÒªÇóÊäÈë2¸ö²ÎÊı(1ºÍ2)£¬ËùÊäÈëµÄÎÄ±¾»áÏÔÊ¾ÔÚĞÅÏ¢¿òÖĞ¡£nircmd infobox ~$param.1$ ~$param.2$

fparam.number

´ÓÖ¸¶¨µÄÎÄ±¾ÖĞ¼ÓÔØÖ¸¶¨µÄ²ÎÊı£¬¸ü¶àĞÅÏ¢Çë²Î¿¼ÃüÁîparamsfile command

sys.varname

¿ÉÒÔÊ¹ÓÃ´ËÑ¡ÏîÀ´±íÊ¾ÏµÍ³ÉÏµÄÈÎºÎ»·¾³±äÁ¿¡£Àı: ~$sys.username$, ~$sys.windir$, ~$sys.computername$, ~$sys.logonserver$, ~$sys.temp$, µÈµÈ

nir.exefile

µ±Ç°ÔËĞĞµÄNirCmdµÄÍêÕûÂ·¾¶(Èç: c:\winnt\nircmd.exe)

loopcount

Ö´ĞĞÑ­»·ÃüÁîÊ±Ö¸¶¨Ñ­»·´ÎÊı

currtime.TimeFormat

¸ù¾İ¡°TimeFormat¡±Ñ¡ÔñµÄÊ±¼ä¸ñÊ½Ö¸¶¨µ±Ç°Ê±¼ä¡£
Èç:~%currtime.HH_mm_ss%£¬ÈçĞè¶ÔÊ±¼ä¸ñÊ½ÁË½â¸ü¶à²Î¿¼currtime(Ê±¼ä¸ñÊ½×Ö·û´®)

currdate.DateFormat

¸ù¾İ¡°DateFormat¡±Ñ¡ÔñµÄÈÕÆÚ¸ñÊ½Ö¸¶¨µ±Ç°ÈÕÆÚ¡£
Èç:~%currdate.MM_dd_yyyy% £¬ÈçĞè¶ÔÈÕÆÚ¸ñÊ½ÁË½â¸ü¶à²Î¿¼currdate(ÈÕÆÚ¸ñÊ½×Ö·û´®)

folder.nircmd

NirCmdËùÔÚµÄÎÄ¼ş¼Ğ(Â·¾¶)

folder.desktop

ÓÃ»§×ÀÃæ(Â·¾¶)

folder.start_menu

ÓÃ»§¿ªÊ¼²Ëµ¥ÎÄ¼ş¼Ğ(Â·¾¶)

folder.programs

ÓÃ»§¿ªÊ¼²Ëµ¥³ÌĞòÎÄ¼ş¼Ğ(Â·¾¶)

folder.startup

ÓÃ»§¿ªÊ¼²Ëµ¥³ÌĞòÆô¶¯ÎÄ¼ş¼Ğ(Â·¾¶)

folder.recent

ÓÃ»§¿ªÊ¼²Ëµ¥³ÌĞòÆô¶¯ÎÄ¼ş¼Ğ(Â·¾¶)

folder.favorites

ÓÃ»§ÊÕ²Ø¼Ğ(Â·¾¶)

folder.cookies

cookiesÎÄ¼ş¼Ğ(Â·¾¶)

folder.appdata

Application DataÎÄ¼ş¼Ğ(Â·¾¶)(ÀıÈç: C:\Documents and Settings\Administrator\Application Data)

folder.common_desktop

All user×ÀÃæÎÄ¼ş¼Ğ(Â·¾¶)

folder.common_start_menu

ALL user¿ªÊ¼²Ëµ¥ÎÄ¼ş¼Ğ(Â·¾¶)

folder.common_programs

ALL user¿ªÊ¼²Ëµ¥³ÌĞòÎÄ¼ş¼Ğ(Â·¾¶)

folder.common_startup

ALL user¿ªÊ¼²Ëµ¥³ÌĞòÆô¶¯ÎÄ¼ş¼Ğ(Â·¾¶)

folder.common_favorites

ALL userÊÕ²Ø¼Ğ(Â·¾¶)

folder.windows

WindowsÎÄ¼ş¼Ğ(Â·¾¶) (e.g.: C:\Windows)

folder.system

ÏµÍ³ÎÄ¼ş¼Ğ(Â·¾¶) (e.g.: C:\Windows\System32)

folder.programfiles

³ÌĞòÎÄ¼şµÄÎÄ¼ş¼Ğ(Â·¾¶) (e.g.: C:\Program Files)

folder.common_programfiles

ALL user³ÌĞòÎÄ¼şµÄÎÄ¼ş¼Ğ(Â·¾¶)(e.g.: C:\Program Files\Common)

folder.mydocuments

¡°My Documents¡±ÎÒµÄÎÄµµÎÄ¼ş¼Ğ(Â·¾¶)

ÀıÈç£º

1¡¢ ´´½¨¿ªÊ¼²Ëµ¥³ÌĞòÎÄ¼ş¼ĞµÄ¿ì½İ·½Ê½£º
    shortcut "c:\winnt\system32\calc.exe" "~$folder.programs$\My Programs" "Calculator"
2¡¢ É¾³ıÓÃ»§×ÀÃæµÄÄ³¸ö¿ì½İ·½Ê½£º
    execmd del "~$folder.desktop$\calc.lnk"
3¡¢ ÏÔÊ¾µ±Ç°ÒÑµÇÂ¼µÄÓÃ»§£º
    infobox "The current logged-on user is ~$sys.username$" "logged-on user"
4¡¢ ½ØÍ¼²¢ÓÃ°üº¬µ±Ç°Ê±¼äºÍÈÕÆÚµÄÎÄ¼şÃû½øĞĞ±£´æ
    savescreenshot c:\temp\scr~$currdate.MMddyyyy$-~$currtime.HHmmss$.png

currtimeÊ±¼ä¸ñÊ½×Ö·û´®

Ê±¼ä¸ñÊ½×Ö·û´®ÖĞ¿ÉÄÜ°üº¬ÒÔÏÂÄÚÈİ:(ÕâĞ©¸ñÊ½×Ö·û´®ĞèÒªÇø·Ö´óĞ¡Ğ´!)

h

µ¥Êı×ÖĞ¡Ê±£»Ê®¶şĞ¡Ê±ÖÆ

hh

Ë«Êı×ÖĞ¡Ê±£»Ê®¶şĞ¡Ê±ÖÆ

H

µ¥Êı×ÖĞ¡Ê±£»¶şÊ®ËÄĞ¡Ê±ÖÆ

HH

Ë«Êı×ÖĞ¡Ê±£»¶şÊ®ËÄĞ¡Ê±ÖÆ

m

µ¥Êı×Ö·ÖÖÓ

mm

Ë«Êı×Ö·ÖÖÓ

s

µ¥Êı×ÖÃë

ss

Ë«Êı×ÖÃë

t

±ê¼Ç×Ö·û´®µÄµ¥¸öÊ±¼ä×Ö·û, Èç A »òÕß P

tt

±ê¼Ç×Ö·û´®µÄ¶àÊ±¼ä×Ö·û, ÈçAM»òÕßPM

¡¡

currdateÈÕÆÚ¸ñÊ½×Ö·û´®

¡¡

ÈÕÆÚ¸ñÊ½×Ö·û´®ÖĞ¿ÉÄÜ°üº¬ÒÔÏÂÄÚÈİ:(ÕâĞ©¸ñÊ½×Ö·û´®ĞèÒªÇø·Ö´óĞ¡Ğ´!)

¡¡

d

ÔÂ·İÖĞµÄµ¥Êı×ÖÈÕÆÚ

dd

ÔÂ·İÖĞµÄË«Êı×ÖÈÕÆÚ

ddd

ĞÇÆÚÖĞµÄÈıÎ»ÊıÈÕÆÚËõĞ´

dddd

ĞÇÆÚÖĞµÄÈÕÆÚÈ«³Æ

M

µ¥Êı×ÖÔÂ·İ

MM

Ë«Êı×ÖÔÂ·İ

MMM

ÈıÎ»ÊıËõĞ´ÔÂ·İ

MMMM

ÔÂ·İÈ«³Æ

y

Äê·İÄ©Î²Êı

yy

Äê·İÄ©Î²Á½Î»Êı

yyyy

Äê·İÈ«³Æ

¡¡

ÔÚÔ¶³Ì¼ÆËã»úÉÏÖ´ĞĞNirCmdÃüÁî

¡¡

´Ó1.40°æ±¾¿ªÊ¼£¬¿ÉÒÔÔÚÔ¶³Ì¼ÆËã»úÉÏÖ´ĞĞNirCmdÃüÁî¡£µ«ÊÇ£¬Ó¦¸Ã×¢ÒâÒÔÏÂµÄÒ»Ğ©ÏŞÖÆºÍÎÊÌâ£º
1¡¢´ËÑ¡ÏîÖ»¿ÉÓÃÓÚWindows NT£¬Windows 2000ºÍWindows XP¡£²»¿ÉÒÔÔÚWindows 9XÏÂÊ¹ÓÃ¡£
2¡¢±ØĞëÒÔ¹ÜÀíÔ±Éí·İµÇÂ½Ô¶³Ì¼ÆËã»ú¡£
3¡¢Ô¶³Ì¼ÆËã»úÉÏµÄÈÎÎñ¼Æ»®(Task Schedule)·şÎñ±ØĞë¿ªÆô¡£(Ä¬ÈÏËùÓĞµÄNT/2000/XP¼ÆËã»ú¶¼×Ô¶¯¿ªÆô´Ë·şÎñ£¬ËùÒÔ²»ÓÃµ£ĞÄÕâµã¡£)
4¡¢ÓÉÓÚÈÎÎñ¼Æ»®·şÎñµÄÒ»Ğ©ÏŞÖÆ£¬±ØĞëµÈ´ı60ÃëÃüÁî²Å¿ÉÒÔÔÚÔ¶³Ì¼ÆËã»úÉÏÖ´ĞĞ¡£
5¡¢Ô¶³Ì¼ÆËã»úÉÏµÄÃüÁîÊÇÔÚsystemÕË»§ÏÂÖ´ĞĞ£¬Òò´ËÄ³Ğ©Í¬ÑùµÄÃüÁîÔÚ±¾µØÖ´ĞĞ¿ÉÄÜ»áÓĞËùÇø±ğ¡£

¡¡

ÕâÊÇÁ½ÌõÔÚÓÃÓÚÔ¶³Ì¼ÆËã»úÉÏÖ´ĞĞµÄNirCmdÃüÁî£º

¡¡

remote - ÓÃÓÚÔÚÒ»Ì¨Ô¶³Ì¼ÆËã»úÉÏÖ´ĞĞÃüÁî
multiremote - ÓÃÓÚÔÚ¶àÌ¨Ô¶³Ì¼ÆËã»úÉÏÖ´ĞĞÃüÁî
°ïÖú

¡¡

Èç¹û²»¼ÇµÃÈ·ÇĞµÄÓï·¨ÃüÁî£¬ËæÊ±¿ÉÒÔ²é¿´°ïÖúÎÄ¼ş NirCmd (NirCmd.chm) »òÕß¿ÉÒÔÊ¹ÓÃ°ïÖúÃüÁî£¨help command£©ÖĞÌáµ½µÄ http://nircmd.nirsoft.net/,ÀıÈç£º nircmd.exe help multiremote

NirCmdÃüÁî

remote {copy} [\\computer] [NirCmd command]

ÔÚÔ¶³Ì¼ÆËã»úÉÏÖ´ĞĞNirCmdÃüÁî¡£Èç¹ûÖ¸¶¨ÁË{copy}²ÎÊı£¬NirCmd½«»á¸´ÖÆ×ÔÉíµ½Ô¶³Ì¼ÆËã»úµÄ WindowsÄ¿Â¼ÏÂ¡£Èç¹ûÔ¶³Ì¼ÆËã»úµÄWindowsÄ¿Â¼ÏÂ²»´æÔÚNirCmd.exe£¬Ôò±ØĞëÖ¸¶¨{copy}²ÎÊı¡£ 
ÀıÈç:
remote copy \\comp1 exitwin poweroff
remote \\comp2 infobox "This is a message from NirCmd" "Hello !"
remote \\comp3 lockws

multiremote {copy} [computers file] [NirCmd command]

ÔÚ¶àÌ¨Ô¶³Ì¼ÆËã»úÉÏÖ´ĞĞ NirCmdÃüÁî¡£Èç¹ûÖ¸¶¨ÁË {copy} ²ÎÊı£¬NirCmd½«»á¸´ÖÆ×ÔÉíµ½Ô¶³Ì¼ÆËã»úµÄ WindowsÄ¿Â¼ÏÂ¡£Èç¹ûÔ¶³Ì¼ÆËã»úµÄWindowsÄ¿Â¼ÏÂ²»´æÔÚNirCmd.exe£¬Ôò±ØĞëÖ¸¶¨ {copy} ²ÎÊı¡£²ÎÊı [computers file] Ö¸¶¨Ò»¸öÎÄ¼ş£¬´ËÎÄ¼ş°üº¬ÁËÒÔ»Ø³µ·û·Ö¸ô(CRLF)µÄ¼ÆËã»úÁĞ±í¡£


ÎÄ¼ş[computers file] ¾ÙÀı:
\\comp1
\\comp2
\\winxp1
\\winxp2
\\nirsoft

ÔÚ²ÎÊı [NirCmd command] ÖĞÖ¸¶¨µÄÃüÁî»áÔÚËùÓĞ[computers file]Ö¸¶¨µÄ¼ÆËã»úÉÏÔËĞĞ¡£

ÀıÈç:

¹Ø±ÕËùÓĞÔÚcomputers.txtÖĞÁĞ³öµÄ¼ÆËã»ú¡£
multiremote "c:\temp\computers.txt" exitwin poweroff force
Ëø¶¨ËùÓĞÔÚcomputers.txtÖĞÁĞ³öµÄ¼ÆËã»ú¡£
multiremote "c:\temp\computers.txt" lockws
¸´ÖÆNirCmd.exeµ½ËùÓĞcomputers.txtÖĞÁĞ³öµÄ¼ÆËã»úÉÏ£¬²¢ÇÒ¹Ø±ÕÆäÏÔÊ¾Æ÷¡£
multiremote copy "c:\temp\computers.txt" monitor off
¸´ÖÆNirCmd.exeµ½ËùÓĞcomputers.txtÖĞÁĞ³öµÄ¼ÆËã»úÉÏ£¬ÔËĞĞwul.exe (WinUpdatesList)£¬ÒÔ±ã±£´æËùÓĞ¼ÆËã»úÉÏµÄWindows¸üĞÂĞÅÏ¢¡£
multiremote copy "c:\temp\computers.txt" exec hide c:\temp\wul.exe /shtml "c:\temp\wul_~$sys.computername$.html"
cmdwait [Number of Milliseconds] [NirCmd Command]

µÈ´ıÖ¸¶¨µÄºÁÃëÊı£¬È»ºóÖ´ĞĞÖ¸¶¨µÄNirCmdÃüÁî¡£

ÀıÈç:
cmdwait 1000 monitor off

loop [Number of Times] [Number of Milliseconds] [NirCmd Command]

Ö¸¶¨ÃüÁîÖ´ĞĞÑ­»·´ÎÊı£¬ ÃüÁîÖ´ĞĞÆÚ¼ä£¬NirCmd µÈ´ıµÚ¶ş¸ö²ÎÊıÖ¸¶¨µÄºÁÃëÊı£¬ Í¨¹ı~%loopcount% ±äÁ¿Ö¸¶¨Ñ­»·ÖĞµÄºô½ĞºÅÂë(´Ó1 µ½ n)

ÀıÈç:
loop 3 2000 speak text "Call Number ~$loopcount$"
loop 10 60000 savescreenshot c:\temp\scr~$loopcount$.png

paramsfile [Parameters File] [Delimiters] [Quote Character] [NirCmd Command]

ÕâÊÇÒ»¸öºÜÇ¿´óµÄÃüÁî£¬ËüÔÊĞí´ÓÒ»¸öÎÄ±¾ÎÄ¼şÖĞ¼ÓÔØÒ»¸ö»ò¶à¸ö²ÎÊı£¬´Ó¶ø¿ÉÒÔ³É±¶µÄÖ´ĞĞNirCmdÃüÁî¡£´ËÎÄ±¾ÎÄ¼şÄÚÈİ¿ÉÒÔÒÔ¶ººÅ¡¢Tab¡¢·ÖºÅ»òÕß ÈÎºÎÔÚ²ÎÊı[Delimiters]ÖĞÖ¸¶¨µÄ·ûºÅ×÷Îª·Ö¸ô·û¡£
¾ÙÒ»¸öÀı×Ó£º¼ÙÉèÓĞÈçÏÂÎÄ±¾ÎÄ¼ş£¬°üº¬ÁËÒ»Ğ©ÓÃ»§ÃûºÍÃÜÂë£¬ÒÔ','¸ô¿ª£º

user01, 123456

user02, abcfg

user03, 5fr23

user04, 33333

ÏÂÃæµÄNirCmdÃüÁî½«»á´´½¨ÔÚÒÔÉÏÎÄ±¾ÎÄ¼şÖĞÖ¸¶¨µÄ4¸öÓÃ»§ÃûºÍÃÜÂë¡£(Í¨¹ıÊ¹ÓÃWindows 2000/XP²Ù×÷ÏµÍ³Ìá¹©µÄnet userÃüÁî)
nircmd.exe paramsfile "c:\temp\users.txt" "," "" execmd net user ~$fparam.1$ ~$fparam.2$ /add

~$fparam.1$Ö¸Ã¿ĞĞµÄµÚÒ»¸ö´ÊÌõ£¬¼´ÓÃ»§Ãû¡£~$fparam.2$Ö¸Ã¿ĞĞµÄµÚ¶ş¸ö´ÊÌõ£¬¼´ÃÜÂë¡£

ÀıÈç:
paramsfile "c:\temp\folders.txt" "" "" execmd md ~$fparam.1$
paramsfile "c:\temp\shortcuts.txt" "," "~q" shortcut ~$fparam.1$ "~$folder.desktop$" ~$fparam.2$

infobox [message text] [title]

ÔÚÆÁÄ»ÉÏÏÔÊ¾Ò»¸ö¼òµ¥µÄĞÅÏ¢¿ò


ÀıÈç:
infobox "This is a message box !" "message"

qbox [message text] [title] [program to run]

ÔÚÆÁÄ»ÉÏÏÔÊ¾Ò»¸öÑ¯ÎÊ¶Ô»°¿ò¡£Èç¹ûÓÃ»§»Ø´ğÊÇ£¬ÔòÔËĞĞ³ÌĞò


ÀıÈç:
qbox "Do you want to run the calculator ?" "question" "calc.exe"

qboxtop [message text] [title] [program to run]

Í¬qboxÃüÁîÀàËÆ£¬µ«ÏÔÊ¾µÄ¶Ô»°¿òÎª×îÉÏ²ã´°¿Ú

qboxcom [message text] [title] [NirCmd Command]

ÔÚÆÁÄ»ÉÏÏÔÊ¾Ò»¸öÑ¯ÎÊ¶Ô»°¿ò¡£Èç¹ûÓÃ»§»Ø´ğÊÇ£¬ÔòÖ´ĞĞ²ÎÊı[NirCmd Command]ÖĞÖ¸¶¨µÄNirCmdÃüÁî

ÀıÈç:
qboxcom "Do you want to lock your workstation ?" "question" lockws
qboxcom "Do you want to turn off the monitor ?" "monitor" monitor off

qboxcomtop [message text] [title] [NirCmd Command]

Í¬qboxcomÃüÁîÀàËÆ£¬µ«ÏÖÊµµÄ¶Ô»°¿òÎª×îÉÏ²ã´°¿Ú

script [Script File]

´ËÃüÁîÔÊĞíÔËĞĞ°üº¬ÔÚÒ»¸ö½Å±¾ÎÄ¼şÖĞµÄÒ»ÏµÁĞÃüÁî£¬´Ë½Å±¾ÎÄ¼şÓÉ²ÎÊı[Script File]Ö¸¶¨

ÀıÈç:

script "c:\temp\msg.ncl"

½Å±¾ÎÄ¼şÑùÀı:

infobox "Hello !" "This is the first message"

infobox "Hello !" "This is the second message"

infobox "Hello !" "This is the third message"

regedit [key] [value name]

´ò¿ª×¢²á±í±à¼­Æ÷µÄÖ¸¶¨Ïî/Öµ

ÀıÈç:
regedit "HKLM\Software\Microsoft\Windows\CurrentVersion" "CommonFilesDir"
regedit "HKEY_CURRENT_USER\Control Panel\Desktop" "Wallpaper"

regsetval [type] [key] [value name] [value]

°ÑÖµĞ´Èë×¢²á±í¡£²ÎÊı[type]¿ÉÒÔÎª£º"sz"»ò"expand_sz" Ö¸×Ö·û´®£»"dword" Ö¸32Î»Êı×Ö£»"binary" Ö¸¶ş½øÖÆÊı¾İ¡£Èç¹ûÔÚ²ÎÊı[key]ÖĞÖ¸¶¨µÄÏîÔÚ×¢²á±íÖĞ²»´æÔÚ£¬Ôò»á×Ô¶¯´´½¨´ËÏî¡£
ÀıÈç:
regsetval dword "HKEY_CURRENT_USER\Software\Test" "Value1" "0xaabbccdd"
regsetval dword "HKCU\Software\Test" "Value2" "10561"
regsetval sz "HKCU\Software\Test" "Value3" "string value"
regsetval binary "HKLM\Software\Test" "Value4" "6a cb ab dd 80 86 95"

regdelval [key] [value]

É¾³ı×¢²á±íÖĞµÄÄ³¸öÖµ
ÀıÈç:
regdelval "HKEY_CURRENT_USER\Software\Test" "Value1"

regdelkey [key]

É¾³ı×¢²á±íÖĞµÄÕûÏî¡£Èç¹ûÖ¸¶¨µÄÏîÓĞÒ»¸ö»ò¶à¸ö×ÓÏîÊ±£¬´ËÃüÁîÊ§Ğ§
ÀıÈç:
regdelkey "HKEY_CURRENT_USER\Software\Test"

inisetval [INI filename] [section] [key] [value]

°ÑÖµĞ´ÈëiniÎÄ¼ş¡£Èç¹ûÎÄ¼ş²»´æÔÚ£¬Ôò×Ô¶¯´´½¨
ÀıÈç:
inisetval "c:\windows\test.ini" "section1" "TestValue" "1"

inidelval [INI filename] [section] [key]

É¾³ıiniÎÄ¼şÖĞµÄÒ»¶ÔÏî-Öµ
ÀıÈç:
inidelval "c:\windows\test.ini" "section1" "TestValue"

inidelval [INI filename] [section]

É¾³ıiniÎÄ¼şµÄÕû¶Î(ÒÔ[]·Ö¶Î)£¬ÇÒÉ¾³ı´Ë¶ÎÖĞ°üº¬µÄËùÓĞÖµ
ÀıÈç:
inidelsec "c:\windows\test.ini" "section1"

regsvr [reg or unreg] [filename or wildcard] {log file}

×¢²á»ò½â³ı×¢²áDLL/OCXÎÄ¼ş¡£
ÔÚµÚÒ»¸ö²ÎÊı[reg or unreg]ÖĞ£¬Ö¸¶¨"reg"±íÊ¾×¢²áÎÄ¼ş£¬Ö¸¶¨"unreg"±íÊ¾½â³ı×¢²áÎÄ¼ş¡£
ÔÚµÚ¶ş¸ö²ÎÊı[filename or wildcard]ÖĞ£¬¿ÉÒÔÖ¸¶¨Òª×¢²á»ò½â³ı×¢²áµÄÄ³¸öµ¥Ò»ÎÄ¼ş£¬¶à¸öÎÄ¼şÓÃÍ¨Åä·ûÖ¸¶¨¡£
µÚÈı¸öÎª¿ÉÑ¡²ÎÊı£¬Ö¸¶¨Ò»¸öÈÕÖ¾ÎÄ¼şÀ´±£´æ×¢²á²Ù×÷µÄ½á¹û
ÀıÈç:
regsvr reg "f:\winnt\system32\fv*.ocx" "c:\temp\log1.txt"
regsvr unreg "f:\winnt\system32\ab*.dll"

rasdial [Entry Name] {Phonebook File} {User Name} {Password} {Domain}

²»ÏÔÊ¾ÈÎºÎÓÃ»§½çÃæ£¬½¨Á¢²¦ºÅÁ¬½Óµ½RAS·şÎñ½Ó¿Ú(VPNÍøÂç»òµç»°Ïß)¡£³ıÁË²ÎÊı[Entry Name]Îª±ØĞè£¬ÆäËû²ÎÊı¶¼Îª¿ÉÑ¡²ÎÊı¡£Èç¹ûÃ»ÓĞÖ¸¶¨¿ÉÑ¡²ÎÊı£¬»òÕßÖ¸¶¨Îª¿Õ×Ö·û´®("")£¬½«»áÊ¹ÓÃÄ¬ÈÏÖµ
ÀıÈç:
rasdial "My Internet"
rasdial "VPN1" "" "user1" "abc1234" "domain1"

inetdial [Entry Name]

½¨Á¢²¦ºÅÁ¬½Óµ½RAS·şÎñ½Ó¿Ú(VPNÍøÂç»òµç»°Ïß)£¬ÏÔÊ¾²¦ºÅ½ø³ÌµÄĞÅÏ¢¶Ô»°¿ò¡£
´ËÃüÁîÖ»ÊÊÓÃÓÚIE°æ±¾4.0¼°ÆäÒÔÉÏ¡£×¢Òâ£º´ËÃüÁîÔÚ¾É°æ±¾µÄWindows 98ÉÏ¿ÉÄÜÎŞĞ§
ÀıÈç:
inetdial "VPN1"

rasdialdlg [Entry Name] {Phonebook File} {Phone Number or IP Address}

ÏÔÊ¾ÓÃÓÚ²¦ºÅÁ¬½ÓµÄRAS·şÎñ½Ó¿Ú¡£²ÎÊı {Phonebook File} ºÍ {Phone Number} Îª¿ÉÑ¡²ÎÊı¡£Èç¹ûÃ»ÓĞÖ¸¶¨²ÎÊı {Phonebook File} »òÕßÖ¸¶¨Îª¿Õ×Ö·û´®("")£¬ÔòÊ¹ÓÃÄ¬ÈÏµç»°²¾¡£
´ËÃüÁîÖ»ÊÊÓÃÓÚWindows NT£¬Windows 2000ºÍWindows XP¡£²»Ö§³ÖWindows 98/ME
ÀıÈç:
rasdialdlg "Internet1" "" "9876543"
rasdialdlg "VPN1"

rashangup {Entry Name}

ÖĞ¶ÏRASÁ¬½Ó¡£¿ÉÒÔÓÃ´ËÃüÁîÀ´ÖĞÖ¹Ö®Ç°ÓÃÃüÁîinetdial»òÕßrasdial´´½¨µÄÁ¬½Ó¡£Èç¹ûÃ»ÓĞÖ¸¶¨²ÎÊı{Entry Name}£¬ËùÓĞ»î¶¯Á¬½Ó¶¼½«±»ÖÕÖ¹.
ÀıÈç:
rashangup "VPN1"
rashangup

setdialuplogon [Entry Name] [User Name] {Password} {Domain} {Phonebook File}

ĞŞ¸Ä²¦ºÅÁ¬½ÓµÄÓÃ»§Ãû¡¢ÃÜÂë¡¢ÓòÃû
²ÎÊı: 

[Entry Name]: ²¦ºÅÁ¬½ÓÃû
[User Name]: Éè¶¨²¦ºÅÁ¬½ÓµÄÓÃ»§Ãû
{Password}: Éè¶¨²¦ºÅÁ¬½ÓµÄÃÜÂë
{Domain}: Éè¶¨²¦ºÅÁ¬½ÓµÄÓòÃû
{Phonebook File}: µç»°²¾ÎÄ¼ş°üº¬ÁËÒªĞŞ¸ÄµÄ²¦ºÅÁ¬½ÓÌõÄ¿¡£Èç¹ûÃ»ÓĞÖ¸¶¨´Ë²ÎÊı£¬½«»áÊ¹ÓÃÄ¬ÈÏµç»°²¾
ÀıÈç:
setdialuplogon "VPN1" "user1" "asder443" "domain1"
setdialuplogon "Internet1" "nirs3" "q5r4df7yh5"

exitwin [shutdown type] {additional option}

´ËÃüÁîÓÃÀ´¹Ø±Õµ±Ç°»á»°´°¿Ú¡£
²ÎÊı [shutdown type] ¿ÉÒÔÊÇÒ»ÏÂÖµÖ®Ò»£º

logoff: ½áÊøËùÓĞ½ø³Ì£¬×¢Ïúµ±Ç°ÓÃ»§£¬ÏÔÊ¾µÇÂ¼¶Ô»°¿òÀ´ÇĞ»»ÓÃ»§
reboot: ¹Ø±ÕÏµÍ³£¬ÖØĞÂÆô¶¯
poweroff: ¹Ø±ÕÏµÍ³£¬¹Ø±ÕµçÔ´(Ö»¶ÔÖ§³Ö´ËĞÔÖÊµÄÏµÍ³ÓĞĞ§)¡£
shutdown: Ö»¹Ø±ÕÏµÍ³£¬²»ÖØÆô£¬²»¹ØµçÔ´
»¹¿ÉÒÔÊ¹ÓÃÒ»ÏÂµÄ¸½¼ÓÑ¡Ïî£º

force: Á¢¼´Ç¿ÖÆÖÕÖ¹ËùÓĞÓ¦ÓÃ³ÌĞò¡£Ê¹ÓÃ´ËÑ¡Ïî¿ÉÄÜ»áÊÇÕıÔÚÔËĞĞµÄ³ÌĞò¶ªÊ§Êı¾İ¡£½¨ÒéÖ»ÔÚÌØÊâÇé¿öÏÂÊ¹ÓÃ£¡
forceifhung: ÖÕÖ¹ÎŞÏìÓ¦µÄÓ¦ÓÃ³ÌĞò¡£ÊÊÓÃÓÚWindows 2000/XP

ÀıÈç:
exitwin reboot
exitwin poweroff force
exitwin logoff forceifhung

initshutdown {message} {timeout value in seconds} {force} {reboot}

ÏµÍ³¹Ø»ú¡£´ËÃüÁîÓëexitwinÃüÁîÓĞĞ©ÏàËÆ£¬µ«Ëü¾ß±¸Ò»Ğ©exitwin²»Ö§³ÖµÄ²Ù×÷¡£´ËÃüÁî²»Ö§³ÖWindows 95/98/ME¡£Èç¹ûÃ»ÓĞÖ¸¶¨ÈÎºÎ²ÎÊı£¬½«»áÔÚ²»ÏÔÊ¾ÈÎºÎĞÅÏ¢µÄÇé¿öÏÂÁ¢¼´¹Ø±Õ¼ÆËã»ú¡£

²ÎÊı£º 

{message}: Ö¸¶¨¹Ø»úÇ°ÒªÏÔÊ¾µÄĞÅÏ¢
{timeout value in seconds}: Ö¸¶¨¹Ø»úÇ°µÈ´ıµÄÃëÊı
{force}: ²»±£´æµ±Ç°Êı¾İ£¬Ç¿ÖÆÖÕÖ¹ËùÓĞÓ¦ÓÃ³ÌĞò
{reboot}: ¹Ø»úºóÖØÆô

ÀıÈç:
initshutdown "shutting down the system within 60 seconds" 60 reboot
initshutdown "" 0 force

abortshutdown

È¡ÏûÓÃinitshutdownÃüÁî½øĞĞµÄ¹Ø»ú²Ù×÷£¬²»½ÓÊÜÈÎºÎ²ÎÊı
ÀıÈç:
abortshutdown

lockws

Ëø¶¨¼ÆËã»ú¡£Ëø¶¨¼ÆËã»úºó£¬±ØĞëÊäÈëÃÜÂëÀ´½âËø¡£Ö»ÊÊÓÃÓÚWindows 2000/XP

monitor [action]

¸Ä±äÏÔÊ¾Æ÷×´Ì¬¡£²ÎÊı [action] ¿ÉÒÔ°üº¬ÏÂÁĞÖµ£º
 

off: ¹Ø±ÕÏÔÊ¾Æ÷
on: ´ò¿ªÏÔÊ¾Æ÷
low: ÉèÖÃÎªµÍºÄµç×´Ì¬

Õâ¸öÃüÁî½öÄÜ¹¤×÷ÔÚÖ§³Ö¸ÃÌØĞÔµÄÏµÍ³ÖĞ
ÀıÈç:
monitor off
monitor on
monitor async_off

screensaver

¿ªÆôÄ¬ÈÏÆÁ±£

standby {force}

¹Ø±Õ¼ÆËã»ú£¬²¢´¦ÓÚ´ı»úÄ£Ê½¡£Èç¹ûÖ¸¶¨ {force} ²ÎÊı£¬ÏµÍ³»á²»¾­¹ıËùÓĞÓ¦ÓÃ³ÌĞòĞí¿ÉÖ±½Ó½øÈë´ı»ú×´Ì¬£¬²»ÍÆ¼öÊ¹ÓÃ´Ë²ÎÊı¡£ÊÊÓÃÓÚÖ§³Ö´ı»úÄ£Ê½µÄÏµÍ³


ÀıÈç: 
standby force
standby

hibernate {force}

¹Ø±Õ¼ÆËã»ú£¬²¢´¦ÓÚĞİÃßÄ£Ê½¡£Èç¹ûÖ¸¶¨ {force} ²ÎÊı£¬ÏµÍ³»á²»¾­¹ıËùÓĞÓ¦ÓÃ³ÌĞòĞí¿ÉÖ±½Ó½øÈëĞİÃß×´Ì¬£¬²»ÍÆ¼öÊ¹ÓÃ´Ë²ÎÊı¡£ÊÊÓÃÓÚÖ§³ÖĞİÃßÄ£Ê½µÄÏµÍ³

ÀıÈç:
hibernate force
hibernate

shortcut [filename] [folder] [shortcut title] {arguments} {icon file} {icon resource number} {ShowCmd} {Start In Folder} {Hot Key}

´´½¨ÎÄ¼şµÄ¿ì½İ·½Ê½£º
²ÎÊı: 

[filename]: Ö¸¶¨Òª´´½¨¿ì½İ·½Ê½µÄÎÄ¼ş
[folder]: Ö¸¶¨¿ì½İ·½Ê½µÄ´´½¨Î»ÖÃ¡£¿ÉÒÔÖ¸¶¨ÈÎºÎÓĞĞ§µÄÎÄ¼ş¼Ğ£¬°üÀ¨Ò»Ğ©´ú±íÏµÍ³ÎÄ¼ş¼ĞµÄÌØÊâ±äÁ¿£¬Èç£º~$folder.programs$ (¿ªÊ¼/³ÌĞòÎÄ¼ş¼Ğ)µÈµÈ...
[shortcut title]: ´Ë¿ì½İ·½Ê½µÄÃû×Ö
{arguments}: ¿ÉÑ¡²ÎÊı - Ö´ĞĞ´ËÎÄ¼şµÄ¸½¼Ó²ÎÊı
{icon file}: ¿ÉÑ¡²ÎÊı - Èç¹ûÏëÊ¹ÓÃÆäËûÍ¼±êÀ´´úÌæÄ¬ÈÏÍ¼±êÔòÊ¹ÓÃ´Ë²ÎÊı
{icon resource number}: ¿ÉÑ¡²ÎÊı - Í¼±êÎÄ¼şÖĞµÄÍ¼±êºÅÂë
{ShowCmd}: ¿ÉÑ¡²ÎÊı - ×î´ó»¯»ò×îĞ¡»¯³ÌĞòÔËĞĞ´°¿Ú¡£Ö¸¶¨"max"×î´ó»¯´°¿Ú£¬"min"×îĞ¡»¯´°¿Ú
{Start In Folder}: ¿ÉÑ¡²ÎÊı - Ö¸¶¨¿ªÊ¼Î»ÖÃ¡£Èç¹ûÃ»ÓĞÖÆ¶¨´Ë²ÎÊı£¬ÆğÊ¼Î»ÖÃ¾Í×Ô¶¯ÉèÖÃÎªÔÚ²ÎÊı [filename] ÖĞÖ¸¶¨µÄÎÄ¼ş¼Ğ
{Hot Key}: ¿ÉÑ¡²ÎÊı - Ö¸¶¨¿ì½İ¼ü(ÈÈ¼ü)¡£Àı£º Alt+Ctrl+A, Alt+Shift+F8, Alt+Ctrl+Shift+Y

ÀıÈç:
shortcut "f:\winnt\system32\calc.exe" "~$folder.desktop$" "Windows Calculator"
shortcut "f:\winnt\system32\calc.exe" "~$folder.programs$\Calculators" "Windows Calculator"
shortcut "f:\Program Files\KaZaA\Kazaa.exe" "c:\temp\MyShortcuts" "Kazaa"
shortcut "f:\Program Files" "c:\temp\MyShortcuts" "Program Files Folder" "" "f:\winnt\system32\shell32.dll" 45
shortcut "f:\Program Files" "c:\temp\MyShortcuts" "Program Files Folder" "" "" "" "max"

cmdshortcut [folder] [shortcut title] [command]

´´½¨Ò»¸öNirCmdÃüÁî¿ì½İ·½Ê½¡£ÔËĞĞ¿ì½İ·½Ê½Ê±£¨Ë«»÷£©£¬ÃüÁîÖĞÖ¸¶¨µÄ [command] ²ÎÊı½«±»Ö´ĞĞ
ÀıÈç:
cmdshortcut "~$folder.desktop$" "Open CDROM" cdrom open k:
cmdshortcut "c:\temp" "Turn Monitor Off" monitor off

cmdshortcutkey [folder] [shortcut title] [hot key] [command]

´´½¨Ò»¸öÖ¸¶¨¿ì½İ¼üµÄNirCmdÃüÁî¿ì½İ·½Ê½¡£µ±ÔËĞĞ´Ë¿ì½İ·½Ê½(Ë«»÷),²ÎÊı [command] Ö¸¶¨µÄÃüÁî»á±»Ö´ĞĞ
ÀıÈç:
cmdshortcutkey "~$folder.desktop$" "Open CDROM" "Ctrl+Shift+K" cdrom open k:
cmdshortcutkey "c:\temp" "Turn Monitor Off" "Ctrl+Shift+M" monitor off

urlshortcut [URL] [Folder] [Shortcut Title]

´´½¨ÍøÒ³¿ì½İ·½Ê½(ÀàËÆÊÕ²Ø¼ĞÖĞµÄÍøÒ³¿ì½İ·½Ê½)¡£²ÎÊı [Folder] Ö¸¶¨´Ë¿ì½İ·½Ê½µÄ´´½¨Î»ÖÃ£¬Èç¹ûÏë´´½¨¿ì½İ·½Ê½µ½ÊÕ²Ø¼Ğ£¬ÔòÊ¹ÓÃ£º~$ folder.favorites$ variable.

ÀıÈç:
urlshortcut "http://www.nirsoft.net" "~$folder.favorites$" "NirSoft Web Site"
urlshortcut "http://www.google.com" "~$folder.favorites$\Search Engines" "Google"
urlshortcut "http://www.yahoo.com" "c:\temp" "Yahoo"

exec [show/hide/min/max] [application + command-line]

ÔËĞĞÒ»¸öÓ¦ÓÃ³ÌĞò,¿ÉÑ¡ÔñĞÔµØÎª´Ë³ÌĞòÖ¸¶¨Ò»¸ö»ò¶à¸öÃüÁîĞĞ²ÎÊı [show/hide/min/max] (ÏÔÊ¾/Òş²Ø/×îĞ¡/×î´óÖµ)] ²ÎÊıÖ¸¶¨ÊÇ·ñÏÔÊ¾»òÒş²ØÔËĞĞµÄÓ¦ÓÃ³ÌĞòÇÒ´°¿Ú×î´ó»¯»ò×îĞ¡»¯¡£Èç¹ûÑ¡Ôñ¡°hide(Òş²Ø)¡±Ñ¡Ïî,ÔËĞĞµÄÓ¦ÓÃ³ÌĞòÔÚºóÌ¨ÔËĞĞ²»¿É¼û¡£Èç¹û¡°max(×î´óÖµ)¡±Ñ¡Ïî,ÔËĞĞµÄÓ¦ÓÃ³ÌĞò´°¿Ú×î´ó»¯¡£Èç¹ûÑ¡Ôñ¡°min(×îĞ¡)¡±Ñ¡Ïî,ÔËĞĞµÄÓ¦ÓÃ³ÌĞò´°¿Ú×îĞ¡»¯¡£
ÀıÈç:
exec show "f:\winnt\system32\calc.exe"
exec hide "c:\temp\wul.exe" /savelangfile
exec max "c:\temp\test.exe"

exec2 [show/hide/min/max] [working folder] [application + command-line]

ÀàËÆÓÚÖ´ĞĞexecÃüÁî,µ«ÊÇÒ²Ìá¹©ÁËÁíÒ»¸ö²ÎÊı,[working folder],Äã¿ÉÒÔÔËĞĞÖ¸¶¨ÎÄ¼ş¼ĞÄ¬ÈÏµÄ¹¤×÷Ä¿Â¼µÄÓ¦ÓÃ³ÌĞò¡£

 

ÀıÈç:
exec2 show "f:\winnt\system32" "f:\winnt\system32\calc.exe"
exec2 hide c:\temp "c:\temp\wul.exe" /savelangfile

execmd [command]

Ö´ĞĞÃüÁîÌáÊ¾·ûÏÂ(¿ØÖÆÌ¨)ÃüÁî,ÆÁÄ»ÉÏ²»ÏÔÊ¾ÈÎºÎĞÅÏ¢


ÀıÈç:
execmd del "~$folder.desktop$\calc.lnk"
execmd mkdir f:\temp\desktop1
execmd copy "~$folder.desktop$\*.lnk" f:\temp\desktop

shexec [operation] [filename]

¸ù¾İ×¢²á±íÖĞÎÄ¼şÀ©Õ¹ÃûµÄ¶¨Òå´ò¿ª»ò´òÓ¡ÎÄ¼ş¡£Èç£ºÀ©Õ¹ÃûÎª.docµÄÎÄ¼ş½«»áÒÔMS-Word·½Ê½´ò¿ª£¬.htmlÎÄ¼şÒÔÄ¬ÈÏä¯ÀÀÆ÷´ò¿ª¡£Èç¹ûÖ¸¶¨Ò»¸ö ÓĞĞ§µÄÍøÖ·£¬½«»áÔÚÄ¬ÈÏä¯ÀÀÆ÷ÖĞ´ò¿ª´ËÍøÒ³¡£²ÎÊı [operation] ¿ÉÒÔÎª"open(´ò¿ª)"»ò"print(´òÓ¡)"¡£

ÀıÈç:
shexec "open" "c:\my documents\abc.doc"
shexec "print" "d:\files\1.pdf"
shexec "open" "http://www.nirsoft.net"

setfiletime [filename or wildcard] [Created Date] {Modified Date} {Accessed Date}

ĞŞ¸ÄÒ»¸ö»ò¶à¸öÎÄ¼şµÄ¡°´´½¨/ĞŞ¸Ä/·ÃÎÊ¡±ÈÕÆÚ¡£µÚÒ»¸ö²ÎÊı¿ÉÒÔÊÇµ¥¸öÎÄ¼şÃû»òÕßÓÃÍ¨Åä·ûÖ¸¶¨¡£ÈÕÆÚ²ÎÊı±ØĞëÒÔÏÂÁĞ¸ñÊ½Ö¸¶¨£º"dd-mm-yyyy hh:nn:ss" ¡£Èç¹ûÃ»ÓĞÖ¸¶¨ÈÕÆÚ²ÎÊı»òÕßÖ¸¶¨Îª¿Õ´®(" ")ÔòÈÕÆÚ²»±ä¡£Èç¹ûÖ¸¶¨Ê±¼ä²ÎÊıÎª¡°now(µ±Ç°)¡±£¬ÔòĞŞ¸ÄÎªµ±Ç°ÈÕÆÚºÍÊ±¼ä

¡¡

ÀıÈç:
setfiletime "c:\temp\*.exe" "15-05-2004 11:22:12" "22-01-2005 21:22:55"
setfiletime "c:\temp\myfile.txt" "24-06-2003 17:57:11" "" "22-11-2005 10:21:56"
setfiletime "c:\temp\myfile.txt" now now

setfilefoldertime [filename, folder or wildcard] [Created Date] {Modified Date} {Accessed Date}

ĞŞ¸ÄÒ»¸ö»ò¶à¸öÎÄ¼şµÄ¡°´´½¨/ĞŞ¸Ä/·ÃÎÊ¡±ÈÕÆÚ¡£´ËÃüÁîÍ¬ setfiletime ÃüÁî·Ç³£ÏàËÆ¡£Î¨Ò»µÄ²»Í¬ÔÚÓÚ setfilefoldertime ÃüÁî»¹¿ÉÒÔĞŞ¸ÄÎÄ¼ş¼ĞµÄÈÕÆÚ - Ö»ÊÊÓÃÓÚWindows 2000/XP


ÀıÈç:
setfilefoldertime "c:\temp\myfolder" "10-01-2003 11:22:12" "12-11-2002 02:34:11"
setfilefoldertime "c:\temp\myfolder" now now

clonefiletime [source filename] [Wildcard]

ĞŞ¸ÄÒ»¸öÎÄ¼ş»ò¶à¸öÎÄ¼şµÄ¡°´´½¨/ĞŞ¸Ä/·ÃÎÊ¡±ÈÕÆÚ£¬¿ÉÓÃ[Í¨Åä·û]¡£

¡¡

¸ñÊ½£ºNirCmd clonefiletime [source filename] [Wildcard]
²ÎÊı£º[source filename]-[Ô´ÎÄ¼şÃû] [Wildcard]-[Í¨Åä·û]

ÀıÈç:
clonefiletime "x:\temp\file1.txt" "x:\files\*.*"
clonefiletime "x:\temp\file1.txt" "x:\files\*.txt"
clonefiletime "x:\temp\file1.txt" "x:\files\a*.txt"
clonefiletime "x:\temp\file1.txt" "x:\temp\file2.txt"

emptybin {drive}

Çå¿Õ»ØÊÕÕ¾¡£²ÎÊı{drive}Ö¸¶¨ÏëÒªÇå¿ÕµÄ»ØÊÕÕ¾ËùÔÚµÄ´ÅÅÌ¡£Èç¹ûÃ»ÓĞÖ¸¶¨´Ë²ÎÊı£¬ÔòÇå¿ÕËùÓĞ´ÅÅÌµÄ»ØÊÕÕ¾¡£
 
ÀıÈç:
emptybin f:
emptybin

setdisplay {monitor:index/name} [width] [height] [color bits] {refresh rate} {-updatereg} {-allusers}

ĞŞ¸ÄÏÔÊ¾ÉèÖÃ¡£²ÎÊı [width] ºÍ [height] ´ú±íµÄÆÁÄ»ÉÏµÄÏñËØµãÊı¡£²ÎÊı [color bits]±íÊ¾ÆÁÄ»µÄÑÕÉ«Î»Êı(8 - 256 color, 16 - 16bit color, 24 - 24bit colorµÈµÈ {refresh rate} ÊÇ¿ÉÑ¡²ÎÊı£¬Ö¸¶¨ÏÔÊ¾Æ÷µÄË¢ĞÂÆµÂÊ¡£Èç¹ûÖ¸¶¨ÁË {-updatereg} ²ÎÊı£¬ĞÂÉèÖÃ»á±£´æÔÚ×¢²á±íÖĞ¡£Èç¹ûÍ¬Ê±Ö¸¶¨ {-updatereg} ºÍ {-allusers} ²ÎÊı£¬ĞÂÉèÖÃ»áÓ¦ÓÃÓë¼ÆËã»úÉÏËùÓĞÓÃ»§£¬²¢ÇÒ±£´æÔÚ×¢²á±íÖĞ¡£

¡¡

ÀıÈç:
setdisplay 800 600 24 -updatereg
setdisplay 1024 768 24 90
setdisplay 1024 768 8
setdisplay monitor:1 1024 768 24 90
setdisplay monitor:name1 1024 768 24 90

setprimarydisplay [Monitor index/name]

ÎªÏµÍ³Óë¶à¸öÏÔÊ¾Æ÷ÉèÖÃµÄÖ÷Òª¼à¿Ø¡£²ÎÊı£º[¼à¿Ø Ö¸Êı/Ãû³Æ]

¡¡

ÀıÈç:
setprimarydisplay 0
setprimarydisplay 1

killprocess [process]

½áÊøÖ¸¶¨½ø³Ì¡£ÔÚ²ÎÊı [process] ÖĞ£¬¿ÉÒÔÖ¸¶¨½ø³ÌÃûµÄÍêÕûÂ·¾¶(Èç£ºC:\Program Files\Internet Explorer\iexplore.exe)»òÕßÖ»Ö¸¶¨½ø³ÌÃû(Èç: iexplore.exe)¡£»¹¿ÉÒÔÍ¨¹ıºó×º'/'Ö¸¶¨½ø³ÌµÄID(Èç£º /1120)¡£

ÀıÈç:
killprocess iexplore.exe
killprocess "c:\winnt\system32\calc.exe"
killprocess /1830

closeprocess [process]

Í¨¹ı¹Ø±Õ¶¥¼¶´°¿ÚÀ´½áÊøÖ¸¶¨½ø³Ì¡£ÓëkillprocessÃüÁîÏà·´£¬closeprocess²¢²»Ç¿ÖÆÁ¢¼´½áÊø½ø³Ì¡£Ïà·´£¬Ëü¸øÖ¸¶¨½ø³ÌµÄ¶¥¼¶´°¿Ú·¢ËÍ Ò»¸öWM_CLOSEÏûÏ¢¡£×¢Òâ£º´ËÃüÁî¶ÔÃ»ÓĞÓÃ»§½çÃæµÄ½ø³ÌÎŞĞ§¡£
ÔÚ²ÎÊı [process] ÖĞ£¬¿ÉÒÔÖ¸¶¨½ø³ÌÃûµÄÍêÕûÂ·¾¶(Èç£ºC:\Program Files\Internet Explorer\iexplore.exe)»òÕßÖ»ÊÇ½ø³ÌÃû(Èç£ºiexplorer.exe)¡£»¹¿ÉÒÔÍ¨¹ıºó×º'/'Ö¸¶¨½ø³ÌµÄID(Èç£º /1120)¡£
 
ÀıÈç:
closeprocess iexplore.exe
closeprocess "c:\winnt\system32\calc.exe"
closeprocess /1830

waitprocess [process] {Command}

µÈ´ıÖ±µ½Ö¸¶¨½ø³Ì½áÊø
ÔÚ²ÎÊı [process] ÖĞ£¬¿ÉÒÔÖ¸¶¨½ø³ÌÃûµÄÍêÕûÂ·¾¶(Èç£ºC:\Program Files\Internet Explorer\iexplore.exe)»òÕßÖ»ÊÇ½ø³ÌÃû(Èç£ºiexplorer.exe)¡£»¹¿ÉÒÔÍ¨¹ıºó×º'/'Ö¸¶¨½ø³ÌµÄID(Èç£º /1120)¡£
 
ÀıÈç:
waitprocess iexplore.exe
waitprocess firefox.exe speak text "Firefox was closed"
waitprocess "c:\winnt\system32\calc.exe"
waitprocess /1830

setprocesspriority [process] [priority]

ÎªÖ¸¶¨½ø³ÌÉèÖÃĞÂµÄÓÅÏÈ¼¶
ÔÚ²ÎÊı [priority] ÖĞ£¬¿ÉÒÔÖ¸¶¨ÒÔÏÂÖµ£º

normal, low, belownormal, abovenormal, high, realtime
ÔÚ²ÎÊı [process] ÖĞ£¬¿ÉÒÔÖ¸¶¨½ø³ÌÃûµÄÍêÕûÂ·¾¶(Èç£ºC:\Program Files\Internet Explorer\iexplore.exe)»òÕßÖ»ÊÇ½ø³ÌÃû(Èç£ºiexplorer.exe)¡£»¹¿ÉÒÔÍ¨¹ıºó×º'/'Ö¸¶¨½ø³ÌµÄID(Èç£º /1120)¡£

ÀıÈç:
setprocesspriority iexplore.exe high
setprocesspriority "c:\winnt\system32\calc.exe" low
setprocesspriority /1830 abovenormal

setprocessaffinity [process] [list of processor numbers]

ÉèÖÃ½ø³Ì¹ØÁª,
ÔÚ²ÎÊı [process] ÖĞ£¬Ö¸¶¨½ø³ÌÊ¹ÓÃµÄ´¦ÀíÆ÷£¨½ö¾ßÓĞ¶à¸ö´¦ÀíÆ÷µÄ¼ÆËã»ú£©£¬¿ÉÒÔÖ¸¶¨ÍêÕûÂ·¾¶ÃûµÄ½ø³Ì (Èç: 'C:Program Files\Internet Explorer\iexplore.exe') »òÕßÖ»ÊÇ½ø³ÌÃû(Èç: iexplore.exe). )¡£»¹¿ÉÒÔÍ¨¹ıºó×º'/'Ö¸¶¨½ø³ÌµÄID(Èç£º /1120)¡£ÔÚ²ÎÊı [list of processor numbers] ÖĞ£¬¿ÉÒÔÖ¸¶¨ĞèÒªµÄ´¦ÀíÆ÷Êı×Ö£¬0ÊÇµÚÒ»¸ö´¦ÀíÆ÷£¬1ÊÇµÚ¶ş¸ö£¬ÒÀ´ËÀàÍÆ.

¡¡

ÀıÈç:
setprocessaffinity iexplore.exe 0 1
setprocessaffinity "c:\winnt\system32\calc.exe" 0 1 2 3
setprocessaffinity /1830 0

memdump [process] [dump file] [bytes per line] {bytes to read} {start address} {nohex} {noascii}

¸ù¾İÖ¸¶¨µÄ²ÎÊı£¬¶ÁÈ¡½ø³ÌµÄÄÚ´æ£¬²¢Ğ´ÈëÎÄ¼ş¡£
²ÎÊı:

[process]: ÏëÒª¶ÁÈ¡ÆäÄÚ´æµÄ½ø³Ì¡£¿ÉÒÔÖ¸¶¨ÍêÕûÂ·¾¶£¬»ò½ö½ø³ÌÃû£¬»òÓÃºó×º'/'Ö¸¶¨½ø³ÌID¡£(ÀıÈç:/ 520)
[dump file]: ÓÃÀ´±£´æÖ¸¶¨½ø³ÌÄÚ´æµÄÄÚÈİµÄÎÄ¼şÃû
[bytes per line]: dump fileÖĞÃ¿ĞĞµÄÄÚ´æ×Ö½ÚÊı
{bytes to read}: ¶ÁÈ¡µÄ×Ö½ÚÊı
{start address}: Ö¸¶¨½ø³ÌµÄ¿ªÊ¼¶ÁÈ¡µØÖ·
{nohex}: ¿ÉÑ¡²ÎÊı¡£Èç¹ûÖ¸¶¨´Ë²ÎÊı£¬ÄÚ´æÊı¾İ²»»áÒÔÊ®Áù½øÖÆĞÎÊ½±£´æ
{noascii}: ¿ÉÑ¡²ÎÊı¡£Èç¹ûÖ¸¶¨´Ë²ÎÊı£¬ÄÚ´æÊı¾İ²»»áÒÔASCII¸ñÊ½±£´æ

ÀıÈç:
memdump iexplore.exe "c:\temp\dump.txt" "16" "0x10000" "0x400000"
memdump calc.exe "c:\temp\calc.txt" "32" "0x30000" "0x400000" nohex
memdump /525 "c:\temp\dump.txt" "16" "0x100000" "0x120000" noascii

service {\\Computer} [action] [service name]

¸Ä±äÇı¶¯ºÍ·şÎñµÄ×´Ì¬ºÍÆô¶¯ÀàĞÍ
²ÎÊı:
 

{\\Computer}: ¿ÉÑ¡²ÎÊı¡£Ö¸¶¨Ô¶³Ì¼ÆËã»úÃû¡£Èç¹ûºöÂÔ´Ë²ÎÊı£¬Ä¬ÈÏÊ¹ÓÃ±¾µØ¼ÆËã»ú.
[action]: Òª½øĞĞµÄ²Ù×÷¡£´Ë²ÎÊı°üº¬ÒÔÏÂÖµ:
start: ¿ªÆôÖ¸¶¨µÄ·şÎñ»òÇı¶¯³ÌĞò.
stop: Í£Ö¹Ö¸¶¨µÄ·şÎñ»òÇı¶¯³ÌĞò.
pause: ÔİÍ£Ö¸¶¨µÄ·şÎñ»òÇı¶¯³ÌĞò.
continue: ¼ÌĞøÖ¸¶¨µÄ·şÎñ»òÇı¶¯³ÌĞò.
restart: Í£Ö¹²¢¿ªÆôÖ¸¶¨µÄ·şÎñ»òÇı¶¯³ÌĞò.
auto: ĞŞ¸ÄÖ¸¶¨µÄ·şÎñ»òÇı¶¯³ÌĞòÎª×Ô¶¯.
manual: ĞŞ¸ÄÖ¸¶¨µÄ·şÎñ»òÇı¶¯³ÌĞòÎªÊÖ¶¯.
disabled: ½ûÓÃÖ¸¶¨µÄ·şÎñ»òÇı¶¯³ÌĞò.
boot: (Ö»ÊÊÓÃÓÚÇı¶¯³ÌĞò)ĞŞ¸ÄÖ¸¶¨µÄÇı¶¯³ÌĞòÎªÒıµ¼(boot).
system: (Ö»ÊÊÓÃÓÚÇı¶¯³ÌĞò)ĞŞ¸ÄÖ¸¶¨µÄÇı¶¯³ÌĞòÎªÏµÍ³(system).
[service name]: ·şÎñ»òÇı¶¯³ÌĞòÃû.

ÀıÈç:
service start schedule
service \\remote stop schedule
service restart w3svc

win [action] [find] [window to find] [Additional Parameters]

Õë¶Ô×Ó´°¿Ú:

win child [find] [top window to find] [action] [find] [child window to find] [Additional Parameters]

Í¨¹ı´ËÃüÁî£¬¿ÉÒÔ¹Ø±Õ¡¢Òş²Ø¡¢ÏÔÊ¾¡¢×î´ó»¯ºÍ×îĞ¡»¯Ö¸¶¨µÄ´°¿Ú¡£
²ÎÊı:

[action]: ¶ÔÖ¸¶¨µÄ´°¿ÚÒª½øĞĞµÄ²Ù×÷:
child: ¶ÔÖ¸¶¨¶¥¼¶´°¿ÚµÄ×Ó´°¿Ú½øĞĞ²Ù×÷.
close: ¹Ø±ÕÖ¸¶¨´°¿Ú.
hide: Òş²ØÖ¸¶¨´°¿Ú.
show: ÏÔÊ¾Ö¸¶¨´°¿Ú(ÔÚÓÃhideÃüÁîÒş²Øºó).
hideshow: Òş²ØÖ¸¶¨´°¿ÚÈ»ºóÔÙÏÔÊ¾´Ë´°¿Ú¡£¿ÉÓÃ´ËÃüÁîÖØĞÂÌîĞ´´°¿ÚÄÚÈİ.
activate: °ÑÖ¸¶¨´°¿Ú·Åµ½ÉÏ²ã.
flash: FlashÖ¸¶¨µÄ´°¿Ú¡£[Additional Parameters]µÄÖµÖ¸¶¨ÁËÖ¡µÄ´ÎÊı£¨Ä¬ÈÏÊÇ5£©ºÍÃ¿Ö¡µÄºÁÃëÊı.
max: ×î´ó»¯Ö¸¶¨´°¿Ú.
min: ×îĞ¡»¯Ö¸¶¨´°¿Ú.
normal: ×î´ó¡¢×îĞ¡»¯Ö¸¶¨´°¿Úºó£¬»Ö¸´µ½Õı³£×´Ì¬.
trans: Ê¹Ö¸¶¨´°¿ÚÍ¸Ã÷¡£²ÎÊı [Additional Parameters] Ö¸¶¨Í¸Ã÷³Ì¶È£¬ÖµÎ»ÓÚ0 - 255Ö®¼ä¡£0 = ÍêÈ«Í¸Ã÷¡£255 = ÍêÈ«²»Í¸Ã÷ 
setsize: Éè¶¨Ö¸¶¨´°¿ÚµÄ´óĞ¡¡£²ÎÊı [Additional Parameters] Ö¸¶¨´°¿ÚµÄ´óĞ¡£¬Öµ°üÀ¨x£¬y£¬width(¿í)£¬height(¸ß). 
move: ÒÆ¶¯/µ÷Õû´°¿Ú¡£²ÎÊı [Additional Parameters] Ö¸¶¨ÒªÒÆ¶¯/µ÷ÕûµÄÏñËØµãÊı£¬Öµ°üÀ¨£ºx£¬y£¬width(¿í)£¬height(¸ß).  
center: Ö¸¶¨´°¿Ú¾ÓÖĞ.
settopmost: ÉèÖÃÖ¸¶¨´°¿ÚµÄ×îÉÏ²ãÏÔÊ¾×´Ì¬¡£Èç¹û²ÎÊı [Additional Parameters] Îª1£¬Ö¸¶¨´°¿ÚÎª×îÉÏ²ãÏÔÊ¾£¬Èç¹ûÎª0£¬ÔòÈ¡Ïû×îÉÏ²ãÏÔÊ¾
redraw: Ë¢ĞÂÖ¸¶¨´°¿Ú.
settext: ĞŞ¸ÄÖ¸¶¨´°¿Ú±êÌâ/Ãû³Æ.
focus: ÔÚÖ¸¶¨´°¿ÚÉÏÉèÖÃ½¹µã.
disable: ½ûÓÃÖ¸¶¨´°¿Ú.
enable: ÆôÓÃÖ¸¶¨´°¿Ú.
+style: ¶ÔÖ¸¶¨´°¿ÚÔö¼ÓÑùÊ½³£Á¿¡£²ÎÊı [Additional Parameters] Ö¸¶¨ÁËÔö¼ÓµÄÑùÊ½³£Á¿.
-style: ÒÆ³ıÖ¸¶¨´°¿ÚµÄÑùÊ½³£Á¿¡£²ÎÊı [Additional Parameters] Ö¸¶¨ÒªÒÆ³ıµÄÑùÊ½³£Á¿.
+exstyle: ¶ÔÖ¸¶¨´°¿ÚÔö¼ÓÀ©Õ¹ÑùÊ½³£Á¿¡£²ÎÊı [Additional Parameters] Ö¸¶¨ÒªÔö¼ÓµÄÀ©Õ¹ÑùÊ½³£Á¿.
-exstyle: ÒÆ³ıÖ¸¶¨´°¿ÚµÄÀ©Õ¹ÑùÊ½³£Á¿¡£²ÎÊı [Additional Parameters] Ö¸¶¨ÒªÒÆ³ıµÄÀ©Õ¹ÑùÊ½³£Á¿.
postmsg: ´«µİĞÅÏ¢µ½Ö¸¶¨´°¿Ú¡£²ÎÊı [Additional Parameters] ÖĞµÄÈı¸öÖµÖ¸¶¨ÁËĞÅÏ¢²ÎÊı£ºMsg, wParam, lParam.
sendmsg: ·¢ËÍĞÅÏ¢µ½Ö¸¶¨´°¿Ú¡£²ÎÊı [Additional Parameters] ÖĞµÄÈı¸öÖµÖ¸¶¨ÁËĞÅÏ¢²ÎÊı£ºMsg, wParam, lParam.
dlgclick: ·¢ËÍÒ»¸öÊó±êµ¥»÷ÃüÁîµ½¶Ô»°¿òÄÚµÄ°´Å¥¡£²ÎÊı [Additional Parameters] Ö¸¶¨ÁË°´Å¥µÄ¿Ø¼şID£¬»òÕßÏÂÁĞÔ¤¶¨ÒåµÄ°´Å¥Ö®Ò»£º yes, no, ok, cancel, retry, ignore, close, help.
dlgsettext: ÉèÖÃÎÄ±¾µ½¶Ô»°¿òÄÚ²¿Ö¸¶¨¿Ø¼ş¡£²ÎÊı [Additional Parameters] Ö¸¶¨ÁË¿Ø¼şID.
dlgsetfocus: ÉèÖÃ½¹µãµ½¶Ô»°¿òÄÚ²¿Ö¸¶¨¿Ø¼ş¡£²ÎÊı [Additional Parameters] Ö¸¶¨ÁË°´Å¥µÄ¿Ø¼şID.
[find]: Ö¸¶¨ÁËÕÒµ½´°¿ÚµÄ·½Ê½:
handle: Í¨¹ıÔÚ²ÎÊı [window to find] ÖĞÖ¸¶¨´°¿Ú¾ä±úÀ´²éÕÒÖ¸¶¨´°¿Ú.
id: Í¨¹ıÖ¸¶¨×Ó´°¿ÚµÄID²éÕÒĞèÒªµÄ×Ó´°¿Ú¡£ÊÊÓÃÓÚ×Ó´°¿Ú.
class: Í¨¹ıÔÚ²ÎÊı[window to find]ÖĞÖ¸¶¨´°¿ÚÀàÃûÀ´²éÕÒÖ¸¶¨´°¿Ú.
title: Í¨¹ıÔÚ²ÎÊı[window to find]ÖĞÖ¸¶¨¾«È·µÄ´°¿ÚÃûÀ´²éÕÒÖ¸¶¨´°¿Ú.
stitle: Í¨¹ıÔÚ²ÎÊı[window to find]ÖĞÖ¸¶¨´°¿ÚµÄÇ°¼¸¸ö×Ö·ûÀ´²éÕÒÖ¸¶¨´°¿Ú.
ititle: Í¨¹ıÖ¸¶¨Ò»ÏµÁĞ´æÔÚÓÚ´°¿Ú±êÌâÖĞµÄ×Ö·ûÀ´²éÕÒÖ¸¶¨´°¿ÚÍ¨¹ıÖ¸¶¨Ò»ÏµÁĞ´æÔÚÓÚ´°¿Ú±êÌâÖĞµÄ×Ö·ûÀ´²éÕÒÖ¸¶¨´°¿Ú.
alltop: ËùÓĞ¶¥²ã´°¿Ú.
alltopnodesktop: ËùÓĞ¶¥²ã´°¿Ú£¬³ıÁË×ÀÃæÓëÅÌ·û´°¿Ú.
active: µ±Ç°»î¶¯´°¿Ú.
foreground: Ç°Ì¨´°¿Ú.
desktop: ×ÀÃæ´°¿Ú.
process: Í¨¹ıÖ¸¶¨½ø³ÌID(ÀıÈç: /3412)»ò½ø³ÌµÄÃû×Ö(ÀıÈç: firefox.exe)ÕÒµ½´°¿Ú.
[window to find]: ¸ù¾İÇ°Ãæ²ÎÊıµÄÖµ£¬Í¨¹ı¾ä±ú¡¢ÀàÃû»ò±êÌâÀ´Ö¸¶¨´°¿Ú.

ÀıÈç:
win close title "calculator"
win hide stitle "calculat"
win hide ititle "culat"
win show class "SciCalc"
win activate title "my computer"
win flash title "calculator" 10 500
win max handle 0x00009c01
win redraw alltop
win trans ititle "internet explorer" 192
win setsize class "IEFrame" 50 50 400 300
win settopmost class "SciCalc" 1
win child class "Shell_TrayWnd" hide class "button"
win child class "Shell_TrayWnd" show class "button"
win child title "calculator" hide id 128
win -style title "my computer" 0x00C00000
win child title "my computer" +exstyle all 0x00400000
win sendmsg title "calculator" 0x10 0 0
win dlgclick class "#32770" cancel
win max process firefox.exe
win max process /1275

cdrom [action] {drive:}

¸ÃÃüÁîÓÃÓÚ´ò¿ªºÍ¹Ø±ÕCD-ROM¹âÅÌÇı¶¯Æ÷ÍĞÅÌ¡£ [action] ²ÎÊı¿ÉÉèÖÃ¡°open(¿ªÆô)¡±»ò ¡°close(¹Ø±Õ)¡±¡£Èç¹ûÎ´Ö¸¶¨ {drive:} Çı¶¯Æ÷²ÎÊı£¬Ä¬ÈÏËùÓĞCD-ROM¹âÅÌÇı¶¯Æ÷.


ÀıÈç:
cdrom open
cdrom close
cdrom open J:
cdrom close R:

clipboard [Action] [Parameter]

ÔÊĞíĞ´ÈëÒÔ¼°Çå¿Õ¼ôÇĞ°å.

[Action] °üº¬ÏÂÁĞ²ÎÊıÖµÖ®Ò»£º

set - ´¢´æÖ¸¶¨ÎÄ±¾ÎÄ¼şÖÁ¼ôÇĞ°å.
readfile - ´¢´æÖ¸¶¨ÎÄ±¾ÄÚÈİÖÁ¼ôÇĞ°å.
clear - Çå¿Õ¼ôÇĞ°å.
writefile - °Ñ¼ôÇĞ°åÖĞÄÚÈİĞ´ÈëÎÄ¼ş¡£(Ö»ÊÊÓÃÓÚÎÄ±¾ÎÄ¼ş)
addfile - °Ñ¼ôÇĞ°åÖĞÄÚÈİÌí¼Óµ½ÎÄ¼ş¡£(Ö»ÊÊÓÃÓÚÎÄ±¾ÎÄ¼ş)
saveimage - ±£´æ¼ôÇĞ°åÖĞµÄÍ¼Ïñµ½ÎÄ¼ş.
copyimage - ¸´ÖÆÖ¸¶¨Í¼ÏñÎÄ¼şÖĞµÄÍ¼Ïñµ½¼ôÇĞ°å.
saveclp - ±£´æµ±Ç°¼ôÌù°åÊı¾İµ½ÏµÍ³¼ôÌù°åÎÄ¼ş.
loadclp - ¼ÓÔØÏµÍ³¼ôÌù°åÎÄ¼şµ½¼ôÌù°å.
¶ÔÓÚcopyimage ºÍ saveimage ²Ù×÷£¬Ö§³ÖÒÔÏÂ¸ñÊ½£º.

bmp, .gif, .jpg, .png, .tiff - Ö»ÔÚ°²×°ÓĞGDI+µÄ¼ÆËã»úÉÏÓĞĞ§(ÔÚXP/2003/VistaÉÏ£¬GDI+ÒÑ¾­×÷Îª²Ù×÷ÏµÍ³µÄÒ»²¿·Ö±»°²×°ÔÚ¼ÆËã»úÉÏ)¡£
 
ÀıÈç:
clipboard set "123456"
clipboard set ~$folder.desktop$
clipboard set ~$folder.start_menu$
clipboard readfile "c:\temp\info1.txt"
clipboard addfile "c:\temp\clip1.txt"
clipboard clear
clipboard saveimage "c:\temp\clip01.png"
clipboard saveimage "c:\temp\clip01.gif"
clipboard copyimage "c:\temp\img01.gif"
clipboard saveclp "c:\temp\1.clp"
clipboard loadclp "c:\temp\1.clp"

gac [Action] [Parameter]

ÔÊĞí°²×°È«¾Ö³ÌĞò¼¯»º´æ(GAC)ÖĞµÄÒ»¸ö³ÌĞò¼¯.
ÀıÈç:
gac install "C:\temp\MyAssembly\bin\Release\6\MyAssembly.dll"

beep [Frequency] [Duration]

·¢³öà½à½ÏìÁåÉù¡£²ÎÊı [Frequency] Ö¸¶¨·¢ÉùÆµÂÊ£¬µ¥Î»ºÕ×È¡£²ÎÊı [Duration] Ö¸¶¨·¢Éù³ÖĞøÊ±¼ä£¬µ¥Î»ºÁÃë¡£ÔÚWindows 95/98/MEÖĞ, ÒÔÉÏ²ÎÊı±»ºöÂÔ£¬Ö»»á·¢³öÒ»Éù±ê×¼µÄà½à½ÏìÁåÉù


ÀıÈç:
beep 500 2000

stdbeep

·¢³öWindows±ê×¼à½à½ÏìÁåÉù.
ÀıÈç:
stdbeep

setvolume [device] [left-volume] [right-volume]

ÉèÖÃÒôÁ¿¡£²ÎÊı [device] ´ú±íÉùÒôÉè±¸±êÊ¶·û¡£´ó¶àÊıÇé¿öÏÂ£¬´Ë²ÎÊıÉèÖÃÎª0¡£ÔÚWindows VistaÏÂ£¬Ä¬ÈÏÖØ·ÅÉè±¸Ö¸¶¨Îª1£¬Ä¬ÈÏÂ¼ÒôÉè±¸Ö¸¶¨Îª0¡£
²ÎÊı [left-volume] ºÍ [right-volume] ÖµÎ»ÓÚ0(¾²Òô)ºÍ65535(×î´óÒôÁ¿)Ö®¼ä£¬´ú±íÁËÒôÁ¿µÄ×óÓÒÉùµÀ.

¡¡

ÀıÈç:
setvolume 0 0 0
setvolume 0 32768 32768
setvolume 0 0xffff 0

setsysvolume [volume] {Component} {Device Index}

ÉèÖÃÏµÍ³ÒôÁ¿¡£ĞŞ¸Ä´ËÖµÒà»áÓ°Ïì¿ØÖÆÃæ°åÏÂµÄÒôÁ¿ÉèÖÃ¡£²ÎÊı [volume] µÄÖµÎ»ÓÚ0(¾²Òô)ºÍ65535(×î´óÒôÁ¿) Ö®¼ä¡£
{Component} ÊÇ¿ÉÑ¡²ÎÊı£¬Ö¸¶¨ÁËÏÂÁĞÉùÒô×é¼şÖ®Ò»£ºmaster(Ö÷ÒôÁ¿)¡¢waveout(²¨ĞÎÊä³ö)¡¢synth(ºÏ³ÉÆ÷)¡¢cd¡¢microphone(Âó¿Ë·ç)¡¢phone¡¢aux(ÒôÆµÊäÈë¿Ú)¡¢line(ÏßÂ·)¡¢headphones(¶ú»ú)¡¢wavein(²¨ĞÎÊäÈë)¡£Ä¬ÈÏÖµÊÇmaster(Ö÷ÒôÁ¿)¡£ÔÚWindows VistaÏÂ£¬ÉùÒô´°¿Ú(¿ØÖÆÃæ°å->ÉùÒô)ÏÂµÄÏÔÊ¾µÄÉè±¸±ØĞëÊ¶±ğ´Ë×é¼şÃû¡£Èç¹ûÉè±¸Ãû°üº¬¿Õ¸ñ£¬Á½±ß±ØĞë¼ÓÒıºÅ¡£
{Device Index} ÊÇ¿ÉÑ¡²ÎÊı£¬Ö¸¶¨ÁËÉùÒôÉè±¸Ë÷Òı¡£Èç¹ûÓĞÒ»¸öÒÔÉÏÉù¿¨£¬ÄÇÃ´¿ÉÒÔÊ¹ÓÃ´Ë²ÎÊı¡£Èç¹ûÃ»ÓĞÖÆ¶¨´Ë²ÎÊı£¬Ä¬ÈÏÊ¹ÓÃ¿ØÖÆÃæ°åÖĞÖ¸¶¨µÄÉù¿¨


ÀıÈç:
setsysvolume 0x8000
setsysvolume 0
setsysvolume 25000 waveout
setsysvolume 25000 master 1

Vista ÀıÈç:
setsysvolume 0x8000
setsysvolume 0 "front mic"
setsysvolume 25000 speakers
setsysvolume 25000 "line in"
setsysvolume 25000 default_record

setsysvolume2 [left volume] [right volume] {Component} {Device Index}

ÀàËÆÓÚsetsysvolumeÃüÁî£¬µ«´ËÃüÁî²»ÊÇÉèÖÃÕû¸öÒôÁ¿£¬¶øÊÇµ¥¶ÀÉèÖÃ×óÉùµÀºÍÓÒÉùµÀ.


ÀıÈç:
setsysvolume2 10000 20000
setsysvolume2 30000 0
setsysvolume2 25000 15000 waveout
setsysvolume2 30000 0 master 1

changesysvolume [volume change] {Component} {Device Index}

Ôö¼Ó»ò½µµÍÏµÍ³ÒôÁ¿¡£²ÎÊı [volume change] Ö¸¶¨Ôö¼Ó»ò½µµÍµÄµ¥Î»¡£(¸ºÊı½µµÍ£¬ÕıÊıÔö¼Ó)
{Component} ÊÇ¿ÉÑ¡²ÎÊı£¬Ö¸¶¨ÁËÏÂÁĞÉùÒô×é¼şÖ®Ò»£ºmaster(Ö÷ÒôÁ¿)¡¢waveout(²¨ĞÎÊä³ö)¡¢synth(ºÏ³ÉÆ÷)¡¢cd¡¢microphone(Âó¿Ë·ç)¡¢phone¡¢aux(ÒôÆµÊäÈë¿Ú)¡¢line(ÏßÂ·)¡¢headphones(¶ú»ú)¡¢wavein(²¨ĞÎÊäÈë)¡£Ä¬ÈÏÖµÊÇmaster(Ö÷ÒôÁ¿)¡£ÔÚWindows VistaÏÂ£¬ÉùÒô´°¿Ú(¿ØÖÆÃæ°å->ÉùÒô)ÏÂµÄÏÔÊ¾µÄÉè±¸±ØĞëÊ¶±ğ´Ë×é¼şÃû¡£Èç¹ûÉè±¸Ãû°üº¬¿Õ¸ñ£¬Á½±ß±ØĞë¼ÓÒıºÅ¡£
{Device Index} ÊÇ¿ÉÑ¡²ÎÊı£¬Ö¸¶¨ÁËÉùÒôÉè±¸Ë÷Òı¡£Èç¹ûÓĞÒ»¸öÒÔÉÏÉù¿¨£¬ÄÇÃ´¿ÉÒÔÊ¹ÓÃ´Ë²ÎÊı¡£Èç¹ûÃ»ÓĞÖÆ¶¨´Ë²ÎÊı£¬Ä¬ÈÏÊ¹ÓÃ¿ØÖÆÃæ°åÖĞÖ¸¶¨µÄÉù¿¨

¡¡

ÀıÈç:
changesysvolume 5000
changesysvolume -3000
changesysvolume 0x1000 waveout
changesysvolume 0x1000 master 1

Vista ÀıÈç:
changesysvolume 5000
changesysvolume -3000
changesysvolume 0x1000 "line in"
changesysvolume 0x1000 "speakers" 1
changesysvolume 0x1000 default_record

changesysvolume2 [left volume change] [right volume change] {Component} {Device Index}

ÀàËÆchangesysvolumeÃüÁî£¬µ«´ËÃüÁî²»ÊÇÉèÖÃÕû¸öÒôÁ¿£¬¶øÊÇµ¥¶ÀÉèÖÃ×óÉùµÀºÍÓÒÉùµÀ

¡¡

ÀıÈç:
changesysvolume2 1000 -1000
changesysvolume2 -3000 0
changesysvolume2 0 -5000 waveout
changesysvolume2 -3000 0 master 1

mutesysvolume [action] {Component} {Device Index}

¾²Òô/È¡Ïû¾²Òô
²ÎÊı [action] ÖĞ£¬Ö¸¶¨1±íÊ¾¾²Òô£¬0±íÊ¾È¡Ïû¾²Òô£¬2±íÊ¾ÔÚ¾²ÒôÓëÕı³£Ö®¼äÇĞ»»¡£
µÚ¶ş¸ö²ÎÊıÎª¿ÉÑ¡²ÎÊı£¬¿ÉÒÔÖ¸¶¨ÏÂÁĞÉùÒô×é¼şÖ®Ò»£ºmaster(Ö÷ÒôÁ¿)¡¢waveout(²¨ĞÎÊä³ö)¡¢synth(ºÏ³ÉÆ÷)¡¢cd¡¢ microphone(Âó¿Ë·ç)¡¢phone¡¢aux(ÒôÆµÊäÈë¿Ú)¡¢line(ÏßÂ·)¡¢headphones(¶ú»ú)¡¢wavein(²¨ĞÎÊäÈë)¡£Ä¬ÈÏ ÖµÊÇmaster(Ö÷ÒôÁ¿)¡£ÔÚWindows VistaÏÂ£¬ÉùÒô´°¿Ú(¿ØÖÆÃæ°å->ÉùÒô)ÏÂµÄÏÔÊ¾µÄÉè±¸±ØĞëÊ¶±ğ´Ë×é¼şÃû¡£Èç¹ûÉè±¸Ãû°üº¬¿Õ¸ñ£¬Á½±ß±ØĞë¼ÓÒıºÅ¡£
{Device Index} ÊÇ¿ÉÑ¡²ÎÊı£¬Ö¸¶¨ÁËÉùÒôÉè±¸Ë÷Òı¡£Èç¹ûÓĞÒ»¸öÒÔÉÏÉù¿¨£¬ÄÇÃ´¿ÉÒÔÊ¹ÓÃ´Ë²ÎÊı¡£Èç¹ûÃ»ÓĞÖÆ¶¨´Ë²ÎÊı£¬Ä¬ÈÏÊ¹ÓÃ¿ØÖÆÃæ°åÖĞÖ¸¶¨µÄÉù¿¨

¡¡

ÀıÈç:
mutesysvolume 1
mutesysvolume 0
mutesysvolume 2 waveout
mutesysvolume 1 master 1

Vista ÀıÈç:
mutesysvolume 1
mutesysvolume 0
mutesysvolume 2 "rear mic"
mutesysvolume 1 speakers 1
mutesysvolume 1 default_record

 

setappvolume [Process] [volume level] {Device Name/Index}

ÒôÁ¿µ÷ÕûÔÚ Windows 7/Vista/2008 µÄ¾ßÌåÓ¦ÓÃµÄ¾ßÌåÓ¦ÓÃ.
²ÎÊı [process] ¿ÉÒÔÖ¸¶¨¡°exe¡±ÎÄ¼şÃû(ÀıÈç: Firefox.exe, iexplore.exe, wmplayer.exe),»òÍêÕûÂ·¾¶ÎÄ¼şÃû(ÀıÈç:¡°F:\Program Files\Internet Explorer\iexplore.exe¡±)»ò½ø³ÌID(ÀıÈç: /3426)
Èç¹ûÏëÒªÉèÖÃÒôÁ¿ÆäÖĞÈÎÒâÖµ£¬¿ÉÒÔÖ¸¶¨¡°focused¡±Õâ¸ö²ÎÊı¡£
 [volume level] ÒôÁ¿µçÆ½²ÎÊıÔÚ0ºÍ1Ö®¼ä¡£¾ÙÀıÀ´Ëµ,Èç¹ûÄãÏë°ÑÒôÁ¿½µµÍµ½50%,ÄãÓ¦¸Ã°ÑÕâ¸ö²ÎÊıÉèÖÃÎª0.5
 {Device Name/Index} Éè±¸ Ãû³Æ/Ö¸Áî£¬ÕâÊÇÒ»¸ö¿ÉÑ¡²ÎÊı¡£Èç¹ûÄã²»Ö¸¶¨Ëü,Êä³öµ½Ä¬ÈÏÉè±¸¡£Äã¿ÉÒÔÖ¸¶¨Éè±¸Ö¸ÁîÎªÊı×ÖÖµ(0£½µÚÒ»¸öÉè±¸,1£½µÚ¶şÉè±¸,µÈµÈ),ÄãÒ²Ö¸¶¨ÍêÕûµÄÉè±¸µÄÃû³Æ,ÈçÑïÉùÆ÷»ò¶ú»ú¡£²éÕÒÏµÍ³ÖĞÒÑÓĞµÄÒôÆµÉè±¸,Ê¹ÓÃ showsounddevices ÃüÁî¡£

¡¡

ÀıÈç:
setappvolume iexplore.exe 0.2
setappvolume wmplayer.exe 0.75 Speakers
setappvolume /1275 0.25 1
setappvolume Firefox.exe 0.5
setappvolume focused 0.75

changeappvolume [Process] [volume level] {Device Name/Index}

ÒôÁ¿¿ØÖÆ Increase/decrease(Ôö¼Ó/¼õÉÙ)ÔÚWindows 7/Vista/2008ÏµÍ³µÄ¾ßÌåÓ¦ÓÃ¡£
 [process] ²ÎÊı£¬Äú¿ÉÒÔÖ¸¶¨µÄ.exeÎÄ¼şÃû(ÀıÈç£ºFirefox.exe, iexplore.exe, wmplayer.exe)£¬»òÍêÕûÂ·¾¶ÎÄ¼şÃû(ÀıÈç:¡°F:\Program Files\Internet Explorer\iexplore.exe¡±)»ò½ø³ÌID(ÀıÈç£º/3426)
Èç¹ûÄãÏëÒª¸ÄÒôÁ¿£¬¿ÉÒÔÉèÖÃ¡°focused¡±²ÎÊı¡£
 [volume level] ÒôÁ¿²ÎÊıÊÇÒ»¸ö½éÓÚ0ºÍ1µÄÕıÊı»ò¸ºÊı¡£ÕıÊıÔö¼ÓÒôÁ¿£¬¸ºÊı¼õĞ¡ÒôÁ¿¡£ÀıÈç£¬Èç¹ûÄãÏëÔö¼Ó´Ó20%(Ä¿Ç°ÊıÁ¿)µ½70%£¬ÄãÓ¦¸Ã½«´Ë²ÎÊıÉèÖÃÎª0.5
 {Device Name/Index} Éè±¸Ãû³Æ/Ö¸Áî}ÊÇ¿ÉÑ¡µÄ²ÎÊı¡£Èç¹ûÄã²»Ö¸¶¨Ëü£¬Ä¬ÈÏµÄÊä³öÉè±¸Ê¹ÓÃ¡£Äã¿ÉÒÔÉèÖÃÉè±¸Ö¸ÁîÎªÊı×ÖÖµ(0£½µÚÒ»¸öÉè±¸,1£½µÚ¶şÉè±¸,µÈµÈ)Äú»¹¿ÉÒÔÖ¸¶¨Ï²»¶µÄÀ®°È»ò¶ú»úÍêÕûµÄÉè±¸Ãû³Æ¡£ÎªÁËÕÒ³öÉùÒôÉè±¸ÔÚÄúµÄÏµÍ³£¬Ê¹ÓÃshowsounddevicesÃüÁî¡£

¡¡

ÀıÈç:
changeappvolume iexplore.exe -0.2
changeappvolume wmplayer.exe 0.55 Speakers
changeappvolume /1275 -0.25 1
changeappvolume Firefox.exe 0.5

muteappvolume [Process] [mute mode] {Device Name/Index}

ÔÚWindows 7/Vista/2008ÏµÍ³ÖĞ£¬Increase/decrease µÄ¾ßÌåÓ¦ÓÃ¡£
ÓÃ[process] ²ÎÊı¿ÉÒÔÖ¸¶¨¡°exe¡±ÎÄ¼şÃû(ÀıÈç: Firefox.exe, iexplore.exe, wmplayer.exe),»òÍêÕûÂ·¾¶ÎÄ¼şÃû(ÀıÈç:¡°F:\Program Files\Internet Explorer\iexplore.exe¡±)»ò½ø³ÌID(ÀıÈç: /3426)
Ñ¡ÔñÆäÖĞÈÎÒâ²ÎÊı£¬¿ÉÉèÖÃ¾²Òô»òÏû³ı¾²Òô¡£ mute/unmute ¾²Òô/Ïû³ı¾²Òô¡£
 [mute mode] ¾²ÒôÄ£Ê½°üº¬ÏÂÁĞ²ÎÊıÖµ:
0 =Ïû³ı¾²Òô
1 =¾²Òô
2 =¾²Òô/Ïû³ı¾²ÒôÖ®¼ä×´Ì¬ÇĞ»»¡£
 {Device Name/Index} Éè±¸Ãû³Æ/Ö¸Áî£¬¿ÉÑ¡ÔñµÄ²ÎÊı¡£Èç¹ûÄã²»Ö¸¶¨Ëü,Ä¬ÈÏÊä³öÉè±¸Ê¹ÓÃ¡£Äã¿ÉÒÔÖ¸¶¨Éè±¸Ö¸Áî×÷ÎªÊı×ÖÖµ(0£½µÚÒ»¸öÉè±¸,1£½µÚ¶şÉè±¸,µÈµÈ),Ò²¿ÉÒÔÖ¸¶¨ÍêÕûµÄÉè±¸µÄÃû³Æ,ÈçÑïÉùÆ÷»ò¶ú»ú¡£²éÕÒÏµÍ³ÖĞÒÑÓĞµÄÒôÆµÉè±¸,Ê¹ÓÃshowsounddevicesÃüÁî¡£

¡¡

ÀıÈç:
muteappvolume iexplore.exe 0
muteappvolume wmplayer.exe 1 Speakers
muteappvolume /1275 2 1
muteappvolume Firefox.exe 1

showsounddevices

ÔÚWindows Vista/7/2008ÏµÍ³ÖĞ£¬ÄÜÏÔÊ¾ËùÓĞÒôÆµÉè±¸µÄÏûÏ¢¿òÁĞ±í¡£
Õâ¸öÃüÁîÖ§³ÖWindows Vista/7/2008ÏµÍ³°²×°µÄÒôÆµÉè±¸¡£

wait [milliseconds]

µÈ´ıÖ¸¶¨µÄºÁÃëÊı.

ÀıÈç:
wait 1000
wait 500

shellrefresh

½øĞĞWindows×ÊÔ´¹ÜÀíÆ÷µÄÈ«ÃæË¢ĞÂ¡£Èç¹û¸ü¸ÄÁË×¢²á±íÖĞÏà¹ØµÄÍâ¿ÇÎÄ¼şÀàĞÍ£¬²¢ÇÒĞèÒªÁ¢¼´ÉúĞ§£¬´ËÃüÁî·Ç³£ÓĞĞ§¡£ (Àı£ºÈç¹û¸Ä±äÁËgifÎÄ¼şÍ¼±ê) 

sysrefresh {Refresh Type}

½øĞĞÈ«ÃæµÄÏµÍ³Ë¢ĞÂ¡£ÔÚ¸ü¸ÄÁË×¢²á±íÖĞµÄÏµÍ³ÉèÖÃºó¿ÉÒÔÊ¹ÓÃ´ËÃüÁîË¢ĞÂÏµÍ³(Àı£º¸ü¸ÄÁËÍ¼±ê´óĞ¡£¬»·¾³±äÁ¿µÈµÈ...)¡£{Refresh Type}Îª¿ÉÑ¡²ÎÊı¡£¿ÉÒÔÖ¸¶¨ÏÂÁĞÖµÖ®Ò»£º "environment" - ¸üĞÂÏµÍ³±äÁ¿£¬"policy" - ¸üĞÂ²ßÂÔÉèÖÃ£¬ "intl" - ¸üĞÂ±¾µØÉèÖÃ¡£

ÀıÈç:
sysrefresh
sysrefresh environment
sysrefresh policy

setcursor [X] [Y]

ÉèÖÃÊó±êÎ»ÖÃ¡£²ÎÊı [X] ºÍ [Y] Ö¸¶¨¹â±êÎ»ÖÃ


ÀıÈç:
setcursor 100 50

setcursorwin [X] [Y]

ÉèÖÃÊó±ê¹â±êµÄÎ»ÖÃ,Ïà¶ÔÓÚµ±Ç°´°¿ÚµÄ×óÉÏ½Ç¡£²ÎÊı [X] ºÍ [Y] Ö¸¶¨ËùĞèµÄ¹â±êÎ»ÖÃ¡£


ÀıÈç:
setcursor 100 50

movecursor [X] [Y]

ÒÆ¶¯Êó±ê¡£²ÎÊı [X] ºÍ [Y] Ö¸¶¨ÒÆ¶¯µÄÏñËØµãÊı

¡¡

ÀıÈç:
movecursor 10 10

dlg [Process Name] [Window Title] [Action] [Parameters]

ÔÊĞíÓëWindows±ê×¼¶Ô»°¿òºÍĞÅÏ¢¿ò½øĞĞ½»»¥¡£´ò¿ªÒ»¸ö¶Ô»°¿òÊ±£¬¿ÉÒÔÊ¹ÓÃ´ËÃüÁî"µã»÷"È·ÈÏ/È¡Ïû/ÊÇ/·ñ°´Å¥£¬»òÕß½«ÎÄ±¾¿òÌîÈë¶Ô»°¿ò¡£
ÀıÈç£¬ÏÂÃæµÄÃüÁî»á½«calc.exe½ø³ÌÌîÈëWindowsµÄ"ÔËĞĞ"¶Ô»°¿ò£¬È»ºóµã»÷"È·ÈÏ"°´Å¥¡£12298ÊÇ"ÔËĞĞ"¶Ô»°¿òÖĞÎÄ±¾¿òµÄ¿Ø¼şID¡£ ¿ÉÒÔÊ¹ÓÃÎ¢ÈíµÄSpy++»òÕßÊ¹ÓÃmy WinExplorer utilityÀ´²éÕÒ¿Ø¼şID¡£

dlg "explorer.exe" "run" settext 12298 "calc.exe"
dlg "explorer.exe" "run" click ok

¡¡

ÏÂÃæµÄÃüÁî»á¶ÔÈÎºÎ×ÊÔ´¹ÜÀíÆ÷µÄÑ¯ÎÊ¶Ô»°¿òµã»÷"ÊÇ"¡£
dlg "explorer.exe" "" click yes

¡¡

ÏÂÃæµÄÃüÁî»á¶ÔÈÎºÎ½ø³ÌµÄÑ¯ÎÊ¶Ô»°¿òµã»÷"·ñ"¡£
dlg "" "" click cancel

¡¡

²ÎÊıÃèÊö:

[Process Name]: Ö¸¶¨Òª²Ù×÷µÄ´°¿ÚËù´ÓÊôµÄ½ø³Ì¡£¿ÉÒÔ½öÖ¸¶¨½ø³ÌÃû»òÕß½ø³ÌµÄÍêÕûÂ·¾¶¡£Èç¹û´Ë²ÎÊıÎª¿Õ´®("")£¬ÃüÁî»áÔÚÈÎºÎ½ø³ÌÉÏÖ´ĞĞ.
[Window Title]: Ö¸¶¨Òª²Ù×÷µÄ´°¿ÚµÄ±êÌâ¡£Èç¹û´Ë²ÎÊıÎª¿Õ("")£¬ÃüÁî»á²»¹Ü±êÌâ¶øÔÚÈÎºÎ´°¿ÚÉÏÖ´ĞĞ.
[Action]: ¿ÉÒÔÖ¸¶¨ÏÂÁĞÑ¡ÏîÖ®Ò»:
click: µã»÷Ö¸¶¨°´Å¥¡£¿ÉÒÔÖ¸¶¨ÏÂÁĞÔ¤ÉèÖµÖ®Ò»(½öÊÊÓÃÓÚ±ê×¼Windows¶Ô»°¿ò)£º yes, no, ok, cancel, retry, ignore, close, help.»¹¿ÉÒÔÖ¸¶¨ÈÎºÎ¿Ø¼şID×÷ÎªÊı×ÖĞÍÖµ.
settext: ÉèÖÃÖ¸¶¨¿Ø¼şµÄÎÄ±¾¡£´Ë²Ù×÷µÄµÚÒ»¸ö²ÎÊıÖ¸¶¨¿Ø¼şID£¬µÚ¶ş¸ö²ÎÊıÖ¸¶¨ÎÄ±¾.
dlgany [Process Name] [Window Title] [Action] [Parameters]

ÀàËÆÓÚdlgÃüÁî£¬µ«»¹¿ÉÒÔÓÃÓÚÆäËûÈÎºÎ´°¿Ú£¬°üÀ¨·Ç±ê×¼Windows¶Ô»°¿ò.

returnval [Return Value]

´ËÃüÁîÔÊĞíÎªNirCmdÃüÁîÖ¸¶¨·µ»ØÖµ¡£ÔÚÍâ²¿³ÌĞòµ÷ÓÃNirCmd.exeÊÇ»áÓÃµ½·µ»ØÖµ¡£
ÀıÈç£¬ÔÚÏÂÃæÃüÁîÖĞ£¬Èç¹ûÓÃ»§Ñ¡Ôñ"ÊÇ"£¬ÔòNirCmd·µ»ØÖµÎª0x30¡£

qboxcom "Do you want to continue ?" "question" returnval 0x30

sendkeypress [Keys Combination 1] [Keys Combination 2] [Keys Combination 3] ...

·¢ËÍÒ»¸ö»ò¶à¸ö°´¼ü×éºÏ¡£²Ù×÷ÏµÍ³½«ÍêÈ«°´ÕÕÓÃ»§µÄĞĞÎªÕæµÄ°´ÏÂÖ¸¶¨µÄ×éºÏ×éºÏ¼ü¡£
 [Key Combination] ×éºÏ¼ü²ÎÊıÖ¸¶¨Ò»¸ö°´¼üÀ´·¢ËÍ»òÒ»¸ö×éºÏ¼ü(Shift/Ctrl/Alt/Windows¼ü)£¬ÓÃ¡°+¡±×éºÏ(ÀıÈç£ºShift+A£¬Ctrl+K£¬Alt+I£¬Ctrl+Alt+I£¬µÈµÈ)¡£

¡¾DownÏÂ·­Ò³£¬UpÉÏ·­Ò³£¬Left×ó·½Ïò£¬RightÓÒ·½Ïò£¬Homeµ½Ò³Ê×£¬Endµ½Ò³Î²£¬Insert¸ÄĞ´¼ü£¬DeleteÉ¾³ı¼ü£¬Plus¼ÓºÅ£¬Comma¶ººÅ£¬Minus¼õºÅ£¬Period¾äºÅ£¬Down·­Ò³¼ü(ÏÂ)£¬Up·­Ò³¼ü(ÉÏ)£¬Left·½Ïò¼ü(×ó)£¬Right·½Ïò¼ü(ÓÒ)£¬Homeµ½Ò³Ê×£¬Endµ½Ò³Î²£¬Insert¸ÄĞ´¼ü£¬DeleteÉ¾³ı¼ü£¬Plus¼ÓºÅ£¬Comma¶ººÅ£¬Minus¼õºÅ£¬Period¾äºÅ£¬Lwin£¬Rwin£¬Óë(Windows¼ü)×éºÏ¡¿£»

¡¾appsÓ¦ÓÃ³ÌĞò¡¿£¬¡¾pageup·­Ò³¼ü(ÉÏ)¡¿£¬¡¾pagedown·­Ò³¼ü(ÏÂ)¡¿£¬¡¾Tab Tab¼ü¡¿£¬¡¾multiply³ËºÅ¡¿£¬¡¾add¼ÓºÅ¡¿£¬¡¾subtract¼õºÅ¡¿£¬¡¾seperator³ıºÅ¡¿£¬¡¾divide·ÖºÅ½¡¡¿£¬¡¾backspaceÍË¸ñ½¡¡¿£¬¡¾pauseÔİÍ£¼ü¡¿£¬¡¾capslock´óĞ´¼ü¡¿£¬¡¾numlockĞ¡¼üÅÌ¿ª¹Ø¡¿£¬¡¾scroll¹ö¶¯¼ü¡¿£¬¡¾printscreenÆÁÄ»Ó²¿½±´¡¿£¬

¡¡

Ê¹ÓÃÒÔÉÏµÄ [Key Combination] ×éºÏ¼ü×é³ÉÒ»¸ö»ò¶à¸öÃüÁî¡£

 ÏÂÃæÊÇÒ»Ğ©Ê¹ÓÃÀı×Ó£º

ÏÔÊ¾ÈÎÎñ¹ÜÀíÆ÷´°¿Ú:
sendkeypress ctrl+shift+esc
ÏÔÊ¾ÈÎÎñ¹ÜÀíÆ÷µÄ´°»§,È»ºóÒÆ¶¯µ½ÏÂÒ»¸ö´°¿Ú¡®alt+tab¡¯±êÇ©:
sendkeypress ctrl+shift+esc alt+tab
´óĞ´Ğ¡Ğ´Ä£Ê½×ª»»:
sendkeypress capslock
´òÓ¡µ±Ç°µÄÎÄ±¾±à¼­Æ÷ÄÚÈİ(Shift¼ü×éºÏÊÇÓÃÀ´»ñÈ¡µÄ´óĞ´×ÖÄ¸):
sendkeypress Shift+n i r spc shift+s o f e r
±£´æµ±Ç°ÆÁÄ»Í¼Ïñµ½¼ôÌù°å(µÈÍ¬°´ÏÂCtrl+PrintScreen):
sendkeypress Ctrl+printscreen
Ö´ĞĞ'Ctrl+Shift+K'Óë'Ctrl+Shift+A'ºÍ'Alt+Shift+P'ÃüÁî:
sendkeypress ctrl+shift+k ctrl+shift+a alt+shift+p
sendkey [Key] [press | down | up]

ÏòÏµÍ³·¢ËÍ»÷¼üÃüÁî¡£²Ù×÷ÏµÍ³»á°´ÕÕÖ´ĞĞ£¬¾ÍºÃÏñÓÃ»§ÕæµÄ°´Ñ¹ÁË¼üÅÌÉÏÖ¸¶¨¼üÒ»Ñù¡£
²ÎÊı[key]Ö¸¶¨ĞéÄâ¼ü´úÂë(Èç£º0x2e´ú±ídelete¼ü)£¬»òÕß×÷ÎªÏÂÁĞÔ¤ÉèÖµÖ®Ò»£ºa - z ºÍ 0 - 9(¶ÔÓ¦×ÖÄ¸Êı×Ö¼ü)£¬F1 - F24 (¶ÔÓ¦Fxx¼ü), shift, ctrl, alt, enter, esc¡£
µÚ¶ş¸ö²ÎÊıÖ¸¶¨ÁËÖ¸¶¨¼üµÄ²Ù×÷£º'press', 'down',»òÕß 'up'¡£ ¶ÔÃ¿Ò»¸ödown²Ù×÷£¬»¹Ó¦¸ÃÌí¼Óup²Ù×÷¡£
ÔÚÏÂÃæµÄ½Å±¾Àı×ÓÖĞ£¬ÏµÍ³»áÖ´ĞĞ¾ÍºÃÏñÓÃ»§°´ÁËShift+Ctrl+EscÒ»Ñù (»áÏÔÊ¾ÈÎÎñ¹ÜÀíÆ÷):
 
sendkey shift down
sendkey ctrl down
sendkey esc down
sendkey shift up
sendkey ctrl up
sendkey esc up

¸ü¶àÀı×Ó:
sendkey a press
sendkey f10 press
sendkey enter press
sendkey 0x2e press

sendmouse [right | left | middle] [down | up | click | dblclick]

sendmouse [move] [x] [y]

sendmouse [wheel] [Wheel Value]

ÏòÏµÍ³·¢ËÍÖ¸¶¨µÄÊó±êÊÂ¼ş¡£²Ù×÷ÏµÍ³»áÖ´ĞĞ¾ÍºÃÏñÓÃ»§ÕæµÄ×ö³öÁËÖ¸¶¨µÄÊó±ê²Ù×÷¡£
Ò»Ğ©sendmouseÃüÁîµÄÀı×Ó:
·¢ËÍµã»÷Êó±êÓÒ¼üÊÂ¼ş(¶Ô´ó¶àÊıÓ¦ÓÃ³ÌĞò¶øÑÔ£¬»á´ò¿ªÒ»¸öÉÏÏÂÎÄ±¾Ä¿Â¼)£º

¡¡

·¢ËÍµã»÷Êó±êÓÒ¼üÊÂ¼ş(¶Ô´ó¶àÊıÓ¦ÓÃ³ÌĞò¶øÑÔ£¬»á´ò¿ªÒ»¸öÉÏÏÂÎÄ±¾Ä¿Â¼)£º
sendmouse right click
·¢ËÍÒ»¸öË«»÷Êó±ê×ó¼üÊÂ¼ş:
sendmouse left dblclick
°´ÏÂÊó±ê×ó¼ü£¬°ÑÊó±êÍù×óÒÆ¶¯20ÏñËØµã£¬ÏòÏÂÒÆ¶¯30ÏñËØµã£¬È»ºóËÉ¿ªÊó±ê×ó¼ü:
sendmouse left down
sendmouse move -30 20
sendmouse left up
¹ö¶¯10¸öµ¥Î»(±ê×¼»¬ÂÖÊó±ê)µÄÊó±ê»¬ÂÖ¡£(ÔÚ±ê×¼»¬ÂÖÊó±êÉÏ£¬»¬ÂÖÖµÊÇ120µÄ±¶Êı).
sendmouse wheel 1200
convertimage [source filename] [destionation filename]

°ÑÍ¼ÏñÎÄ¼ş×ª»¯ÎªÆäËû¸ñÊ½¡£Ö§³ÖÒ»ÏÂ¸ñÊ½£º.bmp, .gif, .png, .jpg, .tiff
ÊÊÓÃÓÚWindows XP/2003/Vista (ĞèÒªGDI+)

ÀıÈç:
convertimage "c:\temp\test1.gif" "c:\temp\test1.png"
convertimage "c:\temp\test1.jpg" "c:\temp\test1.gif"

convertimages [source wildcard] [extension]

°Ñ¶à¸öÍ¼ÏñÎÄ¼ş×ª»¯ÎªÆäËû¸ñÊ½¡£Ö§³ÖÒÔÏÂ¸ñÊ½£º.bmp, .gif, .png, .jpg, .tiff
ÊÊÓÃÓÚWindows XP/2003/Vista (ĞèÒªGDI+)

ÀıÈç:
convertimages "c:\temp\a*.gif" .png
convertimages "c:\temp\*.jpg" .gif

savescreenshot [filename]

±£´æµ±Ç°ÆÁÄ»½ØÍ¼µ½Ö¸¶¨µÄÍ¼ÏñÎÄ¼ş¡£Ö§³ÖÒÔÏÂ¸ñÊ½.bmp, .gif, .png, .jpg, .tiff
ÊÊÓÃÓÚWindows XP/2003/Vista (ĞèÒªGDI+)

ÀıÈç:
savescreenshot "c:\temp\shot.png"

savescreenshotfull [filename]

ÀàËÆsavescreenshot£¬µ«ÔÚ¾ßÓĞ¶à¸ö¼àÊÓÆ÷µÄ¼ÆËã»úÉÏ£¬¸ÃÃüÁî½«²É¼¯ËùÓĞ¼àÊÓÆ÷µÄÆÁÄ»½ØÍ¼£¬ savescreenshotÖ»²É¼¯Ö÷¼àÊÓÆ÷µÄÆÁÄ»½ØÍ¼.

savescreenshotwin [filename]

±£´æµ±Ç°»î¶¯´°¿Ú½ØÍ¼µ½Ö¸¶¨Í¼ÏñÎÄ¼şÖ§³ÖÒÔÏÂ¸ñÊ½ .bmp, .gif, .png, .jpg, .tiff
ÊÊÓÃÓÚWindows XP/2003/Vista (ĞèÒªGDI+)

ÀıÈç:
savescreenshotwin "c:\temp\win.png"

shellcopy [source file or wildcard] [destination] {Flags}

´ÓÒ»¸öÎÄ¼ş¼Ğ¸´ÖÆÒ»¸ö»ò¶à¸öÎÄ¼şµ½ÆäËûÎÄ¼ş¼Ğ£¬²¢ÇÒ¾ÍÏñ×ÊÔ´¹ÜÀíÆ÷ÏÂ¸´ÖÆÎÄ¼şÊ±Ò»ÑùÏÔÊ¾½ø³Ì¿ò¡£
¶ÔÓÚ²ÎÊı{Flags}£¬¿ÉÒÔÖ¸¶¨Ò»¸ö»ò¶à¸öÏÂÁĞÖµ£º

yestoall: ¶ÔËùÓĞÑ¯ÎÊ»Ø´ğ"ÊÇ".
noerrorui: ÔÚÓÃ»§½çÃæ²»ÏÔÊ¾´íÎóĞÅÏ¢.
silent: °²¾²Ä£Ê½ - ²»ÏÔÊ¾¸´ÖÆ½ø¶ÈĞÅÏ¢.
nosecattr: ²»¸´ÖÆÎÄ¼şµÄÏà¹ØÊôĞÔ.
ÀıÈç:
shellcopy "c:\temp\*.*" "d:\temp"
shellcopy "c:\temp\*.*" "d:\temp" yestoall noerrorui

filldelete [source file or wildcard]

Áã×Ö½ÚÌî³äÖ¸¶¨ÎÄ¼ş»òÍ¨Åä·ûÎÄ¼ş£¬È»ºóÉ¾³ıÎÄ¼ş¡£ÕâÒ²¾ÍÒâÎ¶×ÅÓÃ´ËÃüÁîÉ¾³ıµôµÄÎÄ¼ş²»¿ÉÔÙ±»»Ö¸´.

ÀıÈç:
filldelete "c:\temp\*.*"
filldelete "c:\temp\myfile.dat"

speak [type] [text/Filename] {rate} {volume}

Í¨¹ıÊ¹ÓÃWindows XPºÍWindows VistaÖĞµÄÓïÒô¿â(SAPI),ÀÊ¶ÁÖ¸¶¨µÄÎÄ±¾»òÕßÎÄ¼şµÄÄÚÈİ.
ÔÚ[type]²ÎÊı¿ÉÒÔÖ¸¶¨ÏÂÁĞÀàĞÍ£º 

text: ¼òµ¥µÄÎÄ±¾.
xml: ÓïÒôXMLÊı¾İÎÄ±¾.
file: ÎÄ±¾ÎÄ¼şµÄÎÄ¼şÃû.
{rate}ÊÇÒ»¸ö¿ÉÑ¡µÄÖ¸¶¨ÓïËÙµÄ²ÎÊı£¬ÊıÖµÔÚ-10£¨·Ç³£»ºÂı£©ºÍ10£¨·Ç³£¿ì£©Ö®¼ä.{volume}²ÎÊıÖ¸¶¨ÒôÁ¿£¬ÊıÖµÔÚ0ºÍ100Ö®¼ä.

ÀıÈç:
speak text ~$clipboard$
speak text "Please visit the Web site of NirSoft at http://www.nirsoft.net" 2 80
speak file "c:\temp\speak1.txt"

elevate [Program] {Command-Line Parameters}

¶ÔÓÚWindows Vista/7/2008Ö»ÓĞÒÔ¹ÜÀíÔ±È¨ÏŞÔËĞĞ³ÌĞò¡£µ±[Program]°üº¬Ò»¸ö»ò¶à¸ö¿Õ¸ñ×Ö·û,Äã±ØĞë°ÑËü·ÅÔÚÒıºÅÀï¡£

¡¡

ÀıÈç:
elevate notepad.exe
elevate notepad.exe C:\Windows\System32\Drivers\etc\HOSTS
elevate "c:\program files\my software\abc.exe"

elevatecmd [NirCmd command]

¶ÔÓÚWindows Vista/7/2008ÃüÁî:ÔËĞĞÒ»¸ö¹ÜÀíÔ±È¨ÏŞNirCmdÃüÁî¡£

ÀıÈç:
elevatecmd regsetval binary "HKLM\Software\Test" "Value1" "6a cb ab dd 80 86 95"

runas [Domain\User] [Password] [Process and Command-Line Parameters]

ÔËĞĞ¹ı³ÌÖĞÖ¸¶¨µÄ name/password ÓÃ»§Ãû/ÃÜÂë¡£µÚÒ»¸ö²ÎÊı,¿ÉÒÔ°´ÕÕÏÂÃæµÄ3ÖÖ¸ñÊ½: user@domainÓÃ»§Ãû  User\DomainÓÃ»§\Óò

ÀıÈç:
runas admin1 Password1 "F:\Program Files\Mozilla Firefox\firefox.exe"
runas Domain01\user01 myp234Jk c:\temp\myprogram.exe
runas user01@domain.com Nm1923a "c:\temp\myprogram.exe" -a -k

trayballoon [Title] [Balloon Text] [Icon File] [Timeout]

ÏÔÊ¾Ò»¸öÍĞÅÌµ¯³ö¿ò(Windows XP¼°ÒÔÉÏ°æ±¾)¡£×¢Òâ,µ±Ê¹ÓÃÕâ¸öÃüÁî,NirCmd½«¼ÌĞøÔËĞĞ,Ò»Ö±µ½µ¯³ö¿ò¹Ø±Õ»òÏûÊ§¡£
[Title]²ÎÊıµÄÊéÃûÊÇµ¯³ö¿òºÍ [Balloon Text] µ¯³ö¿òÎÄ×Ö²ÎÊı£¬Ö¸¶¨ÎÄ±¾ÏÔÊ¾ÔÚµ¯³ö¿òÄÚ¡£ÔÚ [Icon File]²ÎÊı,Äã¿ÉÒÔÖ¸¶¨.icoÎÄ¼ş(ÀıÈç:c:\icons\myicon.ico)»òexeÎÄ¼şÓëdllÍ¼±êÎÄ¼ş¡£(ÀıÈç: shell32.dll,-154)
[Timeout]²ÎÊıÖ¸¶¨ÁËµÄºÁÃëÊıÀ´µÈ´ıµ¯³ö¿ò¾ÍÏûÊ§¡£

ÀıÈç:
trayballoon "Hello" "This is a test..." "shell32.dll,22" 15000
trayballoon "Hello" "This is the text that will be appear inside the balloon !" "shell32.dll,-154" 10000
trayballoon "Hello" "This is the text that will be appear inside the balloon !" "c:\temp\icon1.ico" 10000

setconsolemode [mode]

ÉèÖÃ¿ØÖÆÌ¨ÏÔÊ¾Ä£Ê½¡£½ö¹©nircmdc.exeÓĞ¹Ø
ÉèÖÃ[mode]²ÎÊı,¿ÉÒÔÖ¸¶¨1ÎªÈ«ÆÁÄ£Ê½»ò2¿ØÖÆÌ¨´°¿ÚÄ£Ê½¡£

setconsolecolor [forecolor] [backcolor]

½ö¹©nircmdc.exe²Î¿¼
ÉèÖÃµ±Ç°µÄÇ°¾°ºÍ±³¾°ÑÕÉ«µÄÎÄ×ÖÔÚ¿ØÖÆÌ¨´°¿Ú¡£Õâ¸öÃüÁî²»»á¸Ä±äÏÖÓĞÎÄ±¾µÄÑÕÉ«,µ«ËùÓĞÎÄ±¾ËÍµ½stdout(ÃüÁî)consolewriteºóÕâ¸öÃüÁî½«ÏÔÊ¾Ö¸¶¨µÄÑÕÉ«¡£
ÀûÓÃ forecolor/backcolor ²ÎÊı,¿ÉÒÔÖ¸¶¨ÈÎºÎEGAÉ«ºÅ½éÓÚ0µ½15:

0 - ºÚÉ«
1 - À¶É«
2 - ÂÌÉ«
3 - ÇàÉ«
4 - ºìÉ«
5 - Æ·ºì
6 - ×ØÉ«
7 - °×É«
8 - Éî»ÒÉ«
9 - ÁÁÀ¶É«
10 - ÁÁÂÌÉ«
11 - ÁÁÇàÉ«
12 - ÏÊºìÉ«
13 - ÁÁÑóºì
14 - ÁÁ»ÆÉ«
15 - ÁÁ°×É« 
ÀıÈç:
setconsolecolor 10 0
setconsolecolor 1 15

consolewrite [text]

½«Ö¸¶¨ÎÄ±¾Êä³öµ½±ê×¼Êä³ö(stdout). 

debugwrite [text]

Ğ´ÌØ¶¨ÎÄ±¾½øÈëµ÷ÊÔÊä³ö¡£Äú¿ÉÒÔ²é¿´¸ÃÎÄ±¾·¢ËÍµÄÕâ¸öÃüÁîµÄĞ§¹û¡£ 

:helps
cls
SetLocal EnableExtensions EnableDelayedExpansion
echos -f F0 66 2 3
echos -f F0 66 2 5
echos -f F0 66 2 7
echos -f F0 66 2 9
echos -f F0 66 2 11
echos -f F0 66 2 13
for /l %%i in (0,1,66) do (
	echos -f b0 %%i 2 3
	wait 0.05
)
for /l %%i in (0,1,66) do echos -f c0 %%i 2 7 & wait 0.01
for /l %%i in (0,1,130) do (
	if %%i EQU !var1! (
		set /a var=%%i/2
		echos -f 81 !var! 2 9 & sleep 100
	) else (
		if %%i LSS 67 echos -f 21 %%i 2 5 & wait 0.1
		set /a var1=%%i+1
	)
)
REM for /l %%i in (0,1,66) do 
for /l %%i in (0,1,66) do echos -f 60 %%i 2 13
for /l %%i in (0,1,66) do echos -f 50 %%i 2 11 & wait 0.2
sleep 5000
call :SetConsoleCursorInfo 2 0
REM for /l %%i in (1,1,20) do (
	REM set /a var_X=!random! %% 1321 - 6
	REM set /a var_Y=!random! %% 743 + 1
	REM location !var_X! !var_Y!
	REM wait 0.5
REM )


sleep 1000
movePos 0 0 40 1  30 6
sleep 2000
movePos 40 0 70 0  0 7
pause>nul
call :SetConsoleCursorInfo 1 3
findstr "^::" "%~f0"|more
echo;
exit /b
::
::  - FILE: DIY¿ØÖÆÌ¨,gcc 3.4.0,notepad++ 8.1.0 ,GvimÌáÈ¡
::  - Compile: Tcc,sed,nircmd,sed,grep...µÈÍâ²¿ÃüÁîºÍ.bat½Å±¾¼¯ºÏ,
::    ²¿·ÖexeÓĞÔ´ÎÄ¼ş.cpp/.c
::  - É±¶¾Èí¼ş¿ÉÄÜ±¨¶¾ÊÇÕı³£µÄ,Íâ²¿ÃüÁîµ÷ÓÃÏµÍ³API²ÅÄÜ×ö¸ü¶àÊÂÇé,
::    ËæºóµÄÌí¼Ó×¢²á±í²Ù×÷ĞèÒª¹ÜÀíÔ±È¨ÏŞ
::

REM https://blog.csdn.net/weixin_48178135/article/details/121715810
::::::::::::::::::::helps_xconvert.exe::::::::::::::::::::::

/*&@echo off
set "file=%~1"
set "exefile=%~dp0\xconvert.exe"
for /f "delims=" %%a in ('dir /a-d-h-s-r/b/s "%file%\*.jp*";"%file%\*.png"') do (
    for /f "tokens=1,2 delims=x" %%b in ('cscript -nologo -e:jscript "%~f0" "%%~a"') do (
		echo;IF [%%~b ^< %%~c] DO %%a
		if %%~b lss %%~c (
		     "%exefile%" "%%a" +repage -rotate -90 "%%a"
			  echo;%%~a**%%~b*%%~c
		)
	)
)
pause
exit
*/
var wi=new ActiveXObject('WIA.ImageFile');
var w=0;
var h=0;
try{
    wi.LoadFile(WSH.Arguments(0));
    w=wi.Width;
    h=wi.Height;
}catch(e){}
WSH.echo(w.toString()+'x'+h.toString());
wi =null;
WSH.Quit();