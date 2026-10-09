@echo off
if exist "%~dp0..\..\FILE\Notepad++\notepad++.exe" (
	cmd /c "start %~dp0..\..\FILE\Notepad++\notepad++.exe" %*
) else (
	if "%~1"=="-d" ( goto :lanp ) else (
		echo;Not installed,Please run [ %~n0 -d ]
		exit /b
	)
)
exit /b

:lanp
	if not exist "%~dp0..\..\FILE" md "%~dp0..\..\FILE"
	pushd %~dp0..\..\FILE
	call down https://gitee.com/cctv3058084277/homework/releases/download/HOMEWORK/notepad.7z
	call 7z x .\notepad.7z -o.\ -aoa >nul
	popd
exit /b 0
