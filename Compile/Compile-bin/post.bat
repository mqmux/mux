@if (@a==@b) @end /*
@echo off
if "%~1"=="" GOTO :HELP
for /f "delims=" %%I in ('cscript /nologo /e:jscript "%~f0" "%~1" "%~2"') do echo;%%I
goto :EOF
:HELP
	echo;Usage: %~n0 [file] 上传文件.md
goto :EOF
REM 如果采用GET，请将参数通过`urlencode`编码；
REM 如果采用 POST 方式，默认以 FORM 方式解码，
REM 如果要通过 JSON 格式传递，请在 Header 中指定 `Content-type` 为 `application/json`，比如：
REM curl -X "POST" "https://sctapi.ftqq.com/key.send" -H 'Content-Type: application/json;charset=utf-8' -d ...
JScript */
function Qmsg(title,desp,key) {
	var x=new ActiveXObject("MSXML2.XMLHTTP");
	x.open("POST", "https://sctapi.ftqq.com/"+key+".send", true); //小号
	//x.open("POST", "https://sctapi.ftqq.com/SCT238041TacEO50alyuAluVivHsCLPDme.send", true);
	x.setRequestHeader("Content-Type", "application/json; charset=utf-8");
	x.setRequestHeader('User-Agent','XMLHTTP/1.0');
	var data = '{ "title": "' + title + '", "desp": "' + desp + '" }';
	    //var data = 'msg=' + encodeURIComponent(msg);
	//WSH.Echo(data);
	x.send(data);
	while (x.readyState!=4) {WSH.Sleep(50)};
	WSH.Echo(x.responseText);
	var json = eval('(' + x.responseText + ')');
	var description = json["reason"];
	return description
}

switch(WSH.Arguments(0)) {
    case "Bearer":
		WSH.Echo("Bearer");
    default:
		var fso = new ActiveXObject("Scripting.FileSystemObject");
		var file = fso.OpenTextFile(WSH.Arguments(0), 1);
		var text = file.ReadAll();
		file.Close();
		text = text.replace(/\\/g, '\\\\');
		text = text.replace(/\r\n/g, '\\n');
		text = text.replace(/\	/g, '\    ');
		text = text.replace(/\"/g, '\\"');
		//WSH.Echo(text);
		var key='SCT240876TxkFEZ6soyci1RGmVMmKGgU71'
		if(WSH.Arguments(1) == '-'){key='SCT238041TacEO50alyuAluVivHsCLPDme'}
		var fso = new ActiveXObject("Scripting.FileSystemObject");
		var filename = fso.GetFileName(WSH.Arguments(0));
		//WSH.Echo(key);
		//SCT238041TacEO50alyuAluVivHsCLPDme
		WSH.Echo(Qmsg(filename,text,key));
}

