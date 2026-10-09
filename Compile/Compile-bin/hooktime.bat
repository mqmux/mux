@if (@a==@b) @end /*
@echo off
for /f "tokens=1,2" %%i in ('type %1') do set "mtime=%%i %%j"
echo;%mtime% - %date:~0,10% %time:~0,-3%
cscript //nologo //e:jscript "%~f0" %mtime% && echo;截图 || echo;SKIP
GOTO :EOF
*/
var dateStr = WScript.Arguments(0);
var timeStr = WScript.Arguments(1);

var dateParts = dateStr.split('-');
var timeParts = timeStr.split(':');
var target = new Date(
    parseInt(dateParts[0], 10),
    parseInt(dateParts[1], 10) - 1,  // 月份从0开始
    parseInt(dateParts[2], 10),
    parseInt(timeParts[0], 10),
    parseInt(timeParts[1], 10),
    parseInt(timeParts[2], 10)
);

var now = new Date();
var diffMs = now - target;     // 毫秒差，正数表示已过
var diffSec = Math.abs(diffMs) / 1000;

var result;
if (diffMs >= 0 && diffSec <= 5) {
    result = "不足5秒";
	WScript.Quit(0);
} else if (diffMs < 0) {
    result = "尚未到达";
} else {
    result = "已超过5秒";
}
WScript.Quit(1);