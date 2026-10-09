//#define _CRT_SECURE_NO_WARNINGS
#define HELP "\
\x1b[  # call a CSI function\n\
0;1;34 # function arguments (0, 1, 34)\n\
m      # function name, f : 将光标向上移到到n行的行首\n\
\033[显示方式;前景色;背景色m\n\
\033[4;32m4表示下划线，32表示绿色，40表示黑色背景\033[0m\n\
\\033[0;35m4表示下划线，32表示绿色，40表示黑色背景\\033[0m\n\
\n\
显示方式\n\
默认0 加粗1 弱化2 斜体3 下划线4 缓慢闪烁5 快速闪烁6 反显7 隐藏8 划除9\n\
主要（默认）替代字体11-19 尖角体20\n\ 关闭粗体或双下划线21 正常颜色或强度22\n\
非斜体、非尖角体23 关闭下划线24 关闭闪烁25 关闭反显27 关闭隐藏28 关闭划除29\n\
上划线53 关闭上划线55 表意文字60-64\n\ 设置明亮的前景色90-97 明亮的背景色100-107\n\
\n\
前景色    背景色\n\
黑色   30 黑色   40\n\
红色   31 红色   41\n\
绿色   32 绿色   42\n\
黄色   33 黄色   43\n\
蓝色   34 蓝色   44\n\
品红色 35 品红色 45\n\
青色   36 青色   46\n\
白色   37 白色   47\n\
"
#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	switch (argc)
	{
	case 1:
		printf(HELP); fflush(stdout);
		break;
	case 2:
		printf("%s", argv[1]);
		break;
	case 3:
		printf("\033[%sm%s\033[0m", argv[2], argv[1]);
		break;
	case 4:
		printf("\033[%sm%s\033[%sm", argv[2], argv[1], argv[3]);
		break;
	default:
		printf("\033[0;41;30mERROR\033[0m : \033[4m参数数量有误\033[0m");
		break;
	}
	return 0;
}