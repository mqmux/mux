<!-- :
@echo off
for /f "delims=" %%a in ('mshta "%~f0"') do (
	set %1=%%a
)
exit /b
-->

<input type=file id=f>
<script>
f.click();
new ActiveXObject('Scripting.FileSystemObject').GetStandardStream(1).Write(f.value);
close();
</script>
