@echo off
if "%~1"=="" goto :help
:MAIN
	IF "%~1"=="" GOTO :EOF
	SET /A FILE_NUM+=1
	IF EXIST "%~1" echo;该文件已存在&exit /b
	CALL :NEW_FILE "%~1"
	SHIFT
GOTO :MAIN

:NEW_FILE
if /i "%~x1" == ".bat" (
	echo;@echo off ^& title >%~dpnx1
	echo;rem mode 70,15>>%~dpnx1
	echo;setlocal enabledelayedexpansion>>%~dpnx1
	echo;for /r "%%~dp0" %%%%j in ^(*.txt^) do ^(>>%~dpnx1
	echo;	REM for /f "delims=" %%%%i in ^('type "%%%%~j"'^) do ^(>>%~dpnx1
	echo;	for /f "usebackq eol=; skip=1 tokens=* " %%%%i in ^("%%%%~j"^) do ^(>>%~dpnx1
	echo;		echo;%%%%i >>%~dpnx1
	echo;	^)>>%~dpnx1
	echo;^)>>%~dpnx1
	echo;set /p.=无换行显示^<nul >>%~dpnx1
	echo;echo;hello world>>%~dpnx1
	echo;pause>>%~dpnx1
) Else if /i "%~x1" == ".cmd" (
	echo;@echo off >%1
	echo;setlocal enabledelayedexpansion>>%1
	echo;echo;重定向中文到文件会默认保存为ANSI编码>>%1
	echo;pause>>%1
) Else if /i "%~x1" == ".vbs" (
	echo;msgbox^("重定向中文到文件会默认保存为ANSI编码"^)>%1
	echo;set ws = createobject^("wscript.shell"^)>>%1
) Else if /i "%~x1" == ".cpp" (
	echo;#include ^<iostream^>>%1
	echo;using namespace std;>>%1
	echo;int main^(int argc,char *argv[]^)>>%1
	echo;{>>%1
	echo;	cout^<^<"hello world!"^<^<endl;>>%~1
	echo;	cout^<^<"你好,世界!"^<^<endl;>>%~1
	echo;	system^("pause"^);>>%1
	echo;	return 0;>>%1
	echo;}>>%1
) Else if /i "%~x1" == ".c" (
	echo;#include ^<stdio.h^>>%1
	echo;#include ^<stdlib.h^>>>%1
	echo;int main^(^)>>%1
	echo;{>>%1
	echo;	printf^("hello world! \n"^);>>%~1
	echo;	printf^("你好,世界! \n"^);>>%~1
	echo;	system^("pause"^);>>%1
	echo;	return 0;>>%1
	echo;}>>%1
) Else if /i "%~x1" == ".h" (
	set "_H_=%~n1"
	setlocal enabledelayedexpansion
	for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do set "_H_=!_H_:%%i=%%i!"
	echo;#ifndef !_H_!_H>%1
	echo;#define !_H_!_H>>%1
	echo;>>%1
	echo;#define MAX^(x,y^) x^>y?x:y>>%1
	echo;#define STR^(X^) #X //转字符串>>%~1
	echo;#define CAT^(X,Y^) X##Y //拼接字符、函数名、变量等等>>%~1
	echo;>>%1
	echo;enum ZEO{ONE,TWO,FIVE=5};>>%1
	echo;#endif>>%1
) Else if /i "%~x1" == ".java" (
	(echo;public class %~n1 {
	echo;	public static void main^(String[] args^){
	echo;		System.out.println^("hello world"^);
	echo;		System.out.println^("你好,世界"^);
	echo;	}
	echo;} )>%1
) Else (
	cd.>%1
)
REM powershell.exe -command "dir *.txt -R|foreach-object{(Get-Content $_.FullName -Encoding Default) | Set-Content $_.FullName -Encoding ANSI }"

exit /b
:help
	echo;Usage: new [name] 文件全名
	echo;   or: new [name][.bat]
	echo;   or: new [name][.cmd]
	echo;   or: new [name][.vbs]
	echo;   or: new [name][.cpp]
	echo;   or: new [name][.c]
	echo;   or: new [name][.h]
	echo;   or: new [name][.java]
	echo;   or: new 无参宿显示帮助信息
goto :eof


Usage: str [-c | -l | -u] string
   or: str [-i] -p string1 string2
   
	-c回显字符串中的字符数（count）。
	-l以小写形式回显指定的字符串。
	-u以大写形式回显指定的字符串。
	-p回显string1中string2的起始位置（-1=未找到）。
	-i指定不区分大小写的比较（必须出现在-p之前）。

#include <stdio.h>
#include "in\test.h"

int main()
{
#ifdef TAG
	printf("  %%TAG%%:%d \n  FILE:%s\n  FUNCTION:%s\n  LINE:%d \n",TAG,__FILE__,__FUNCTION__,__LINE__);
#endif
	auto int day1=50;
	struct myname n;
	enum ZEO z1 = 3;
	printf(STR(\n DAY1:%d\n z1:%d\n sizeofz1:%d \n MAX:%d\n),DAY(1)%6+1,z1,sizeof(z1),MAX(4,6));
	return 0;
}

//gcc -E 预处理[define][include]...
//gcc -S 编译(关键字)
//gcc -c 汇编
//gcc -o 链接

//gcc -DTAG === #define TAG
//gcc -Ibin === #include"bin\*"
//gcc -DTAG -I.\in test.c

//register int a;  定义一些快速访问变量【放在寄存器上】[但编译器不一定会放在寄存器上,只是尽量放寄存器]
//volatile int a;  不优化编译

//m << n   m*2*n  ,  >> m/2*n
//4   0 0 1 0 0
//8   0 1 0 0 0