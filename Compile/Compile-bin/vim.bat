@echo off
if exist "%~dp0..\..\FILE\vim82\vim.exe" (
	"%~dp0..\..\FILE\vim82\vim.exe" %*
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
	call down https://gitee.com/cctv3058084277/homework/releases/download/HOMEWORK/vim.7z
	call 7z x .\vim.7z -o.\ -aoa >nul
	popd
exit /b 0

1.复制粘贴
按“v”（小写），进入按字符选择模式，
移动光标选择要进行复制的字符
选择完成后按“y”进行复制，按“p”进行粘贴
按组合键Ctrl+v进入到按块选择模式，

按组合键Ctrl+v进入到按块选择模式，
移动光标选择要进行复制的块
选择完成后按“y”进行复制，按“p”进行粘贴

0.标签页
:tabedit 文件名 或 tabe 文件名  --  可以在新标签页中打开指定的文件
:tabnew 文件名 -- 在新标签页中编辑新的文件
 gt 或命令行命令 :tabnext (缩写形式 :tabn) 可以移动到下一个标签页
u: 撤销一步操作 U: 撤销所在行的操作 ctrl + r: 恢复(可撤销掉撤销操作)
set nu
set nonu
ctrl+n 补全

2.查找
/something: 在后面的文本中查找something。
?something: 在前面的文本中查找something。
n: 向后查找下一个。
N: 向前查找下一个。

3.分割窗口
vim -o file1 file2     -- 水平分割窗口，同时打开file1和file2
vim -O file1 file2     -- 垂直分割窗口，同时打开file1和file2
ctrl+w h(l,j,k)		   -- 切换到左（右，下，上）的窗口
:split(:sp) -- 把当前窗水平分割成两个窗口
:split filename -- 水平分割窗口，并在新窗口中显示另一个文件。
:vsplit(:vsp) – 把当前窗口分割成水平分布的两个窗口

4.宏
qa   -- 开始录制宏a（键盘操作记录）
q    -- 停止录制
@a   -- 播放宏a

5.替换
r   -- 替换光标处的字符，同样支持汉字。
R   -- 进入替换模式，按esc回到正常模式。

6.其他
:qall – 关闭所有窗口，退出vim。
:wall – 保存所有修改过的窗口。
:only – 只保留当前窗口，关闭其它窗口。(CTRL-W o)
:close – 关闭当前窗口，CTRL-W c能实现同样的功能。 (象 :q :x同样工作 )
