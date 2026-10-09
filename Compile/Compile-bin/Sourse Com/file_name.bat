if /i "%~x1" == ".cpp" (
	echo;#include ^<iostream^>>%1
	echo;using namespace std;>>%1
	echo;int main^(int argc,char *argv[]^)>>%1
	echo;{>>%1
	echo;	cout^<^<"hello world!"^<^<endl;>>%~1
	echo;	system^("pause"^);>>%1
	echo;	return 0;>>%1
	echo;}>>%1
) Else if /i "%~x1" == ".c" (
		echo;#include ^<stdio.h^>>%1
		echo;#include ^<stdlib.h^>>>%1
		echo;int main^(^)>>%1
		echo;{>>%1
		echo;	printf^("hello world! \n"^);>>%~1
		echo;	system^("pause"^);>>%1
		echo;	return 0;>>%1
		echo;}>>%1
) Else ( exit /b 404 )