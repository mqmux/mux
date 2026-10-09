@echo off
cd /d %~dp1
echos 0x03 "’˝‘⁄±‡“Î[%~nx1]«Î…‘∫Ú..."
if "%~x1" == ".cpp" (
	echos 0x0e "g++ \"%~nx1\" -o \"%~n1\" %lwsock32%"
	g++ "%~nx1" -o "%~n1" %lwsock32% && ( echos 0x0a ====±‡“Î≥…π¶==== & exit /b 1 ) || ( echos 0x0c ====±‡“Î ß∞‹==== & exit /b 0 )
) Else if /i "%~x1" == ".c" (
	echos 0x0e "gcc \"%~nx1\" -o \"%~n1\" %lwsock32%"
	call gcc "%~nx1" -o "%~n1" %lwsock32% && ( echos 0x0a ====±‡“Î≥…π¶==== & exit /b 1 ) || ( echos 0x0c ====±‡“Î ß∞‹==== & exit /b 0 )
) Else ( exit /b 404 )
