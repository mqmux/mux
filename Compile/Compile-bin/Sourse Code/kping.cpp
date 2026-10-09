#define _CRT_SECURE_NO_WARNINGS
/*
	LINK GDI32 GDIPLUS SHLWAPI USER32
	UNICODE COMPILATION:
	--> G++ kping.cpp -lgdi32 -lgdiplus -D _UNICODE -D UNICODE -municode -O2 -static
	--> CL  kping.cpp /O2 /Oy- /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /MD

	ANSI COMPILATION:
	--> G++ kping.cpp -lgdi32 -lgdiplus -O2 -static
	--> CL  kping.cpp /O2 /Oy- /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /MD
*/
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <io.h>

#ifndef _FILE_EXIST
#define _FILE_EXIST 0
#endif

#if defined _MSC_VER
#include <gdiplus.h>
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "user32.lib")
#else
//兼容MINGW32
#include <gdiplus\gdiplus.h>
#endif

#ifndef _UNICODE
#define TCHARFORMAT CHAR
#else
#define TCHARFORMAT WCHAR
#endif

//GDI+命名空间
using namespace Gdiplus;

//定义帮助说明
#define HELP_INFORMATION "\
Usage: kping {[选项] [参数]} \n\
\n\
General options:\n\
  -n   截图个数\n\
  -o   输出图像名称\n\
  -b   图像位深度\n\
  -d   延时时间\n\
  -t   截图间隔时间\n\
  -x   截图起始点的x坐标\n\
  -y   截图起始点的y坐标\n\
  -w   截图宽度\n\
  -h   截图高度\n\
\n\
Example:\n\
  kping -x0 -y0 -w1920 -h1080 -o1.jpg\n\
  kping -n5 -o1.jpg -b[8/16/24/32] -d1000 -t1000 -x0 -y0 -w2560 -h1440\n"

#define FILE_EXIST 0

//定义位深度枚举
INT BITDEEP[65] = { 0,PixelFormat1bppIndexed,0,0,PixelFormat4bppIndexed,0,0,0,PixelFormat8bppIndexed,0,0,0,0,0,0,0,PixelFormat16bppRGB555,0,0,0,0,0,0,0,PixelFormat24bppRGB,0,0,0,0,0,0,0,PixelFormat32bppARGB,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,PixelFormat48bppRGB,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,PixelFormat64bppARGB };

//定义截图区域结构体
typedef struct
{
	int startX;
	int startY;
	int capWITH;
	int capHIGH;
} PAREA;

//开关解析
int OPTIND = 1, OPTOPT;
TCHAR* OPTARG;
int _tgetopt(int nargc, TCHAR* nargv[], TCHAR* ostr)
{
	static TCHAR* place = (TCHAR*)_T("");
	static TCHAR* lastostr = NULL;
	register TCHAR* oli = NULL;

	if (ostr != lastostr)
	{
		lastostr = ostr;
		place = (TCHAR*)_T("");
	}

	if (!*place)
	{
		if (
			(OPTIND >= nargc) ||
			(*(place = nargv[OPTIND]) != (TCHAR)_T('-')) ||
			(!*(++place))
			)
		{
			place = (TCHAR*)_T("");
			return _TEOF;
		}
		if (*place == (TCHAR)_T('-') && *(place + 1) == (TCHAR)_T('\0'))
		{
			++OPTIND;
			return _TEOF;
		}
	}

	if (
		(OPTOPT = *place++) == (TCHAR)_T(':') ||
		!(oli = (TCHAR*)_tcschr((TCHARFORMAT*)ostr, (TCHAR)OPTOPT))
		)
	{
		if (!*place)
		{
			++OPTIND;
		}
	}

	if (oli != NULL && *(++oli) != (TCHAR)_T(':'))
	{
		OPTARG = NULL;
		if (!*place)
		{
			++OPTIND;
		}
	}
	else
	{
		if (*place)
		{
			OPTARG = place;
		}
		else if (nargc <= ++OPTIND)
		{
			place = (TCHAR*)_T("");
		}
		else
		{
			OPTARG = nargv[OPTIND];
		}
		place = (TCHAR*)_T("");
		++OPTIND;
	}
	return OPTOPT;
}

//获取编码器CLSID
BOOL GetEncoderClsid(TCHAR* expNAME, CLSID* pClsid)
{
	UINT n = 0, s = 0;
	ImageCodecInfo* pInfo = NULL;
	GetImageEncodersSize(&n, &s);
	if (s == 0)
	{
		return FALSE;
	}
	pInfo = (ImageCodecInfo*)(malloc(s));
	if (pInfo == NULL)
	{
		return FALSE;
	}
	GetImageEncoders(n, s, pInfo);
	for (UINT i = 0; i < n; i++)
	{
		if (lstrlen(expNAME) < 2 || *expNAME == _T('\0'))
		{
			;
		}
		else if (
			*(pInfo[i].MimeType + 6) == *expNAME &&
			*(pInfo[i].MimeType + 7) == *(expNAME + 1)
			)
		{
			*pClsid = pInfo[i].Clsid;
			free(pInfo);
			return TRUE;
		}
	}
	free(pInfo);
	return FALSE;
}

//截图核心
int CaptureScreenToImage(TCHAR* srcNAME, PAREA* inpa, int imgBIT, int intervalTIME, int countNUM)
{
	//切分文件名
	TCHAR* imgNAME = NULL, * outDIR = NULL, * expNAME = NULL, * lpwstr = (TCHAR*)_tcsrchr((TCHARFORMAT*)srcNAME, _T('\\'));
	if (lpwstr == NULL)
	{
		outDIR = (TCHAR*)_T(".\\");
		imgNAME = srcNAME;
	}
	else
	{
		*lpwstr = _T('\0'), outDIR = srcNAME, imgNAME = ++lpwstr;
		if (_taccess((TCHARFORMAT*)outDIR, _FILE_EXIST) != 0)
		{
			_ftprintf(stderr, _T("The outdir don't exist\n"));
			exit(1);
		}
	}
	lpwstr = (TCHAR*)_tcsrchr((TCHARFORMAT*)imgNAME, _T('.'));
	if (lpwstr == NULL)
	{
		_ftprintf(stderr, _T("Wrong image output format\n"));
		exit(1);
	}
	else
	{
		*lpwstr = _T('\0'), expNAME = ++lpwstr;
	}

	//获取桌面HWND句柄
	HWND hDSK = GetDesktopWindow();
	//获取桌面DC
	HDC  hDC = GetDC(hDSK);
	//创建兼容桌面DC
	HDC  pDC = CreateCompatibleDC(hDC);
	//创建兼容位图
	HBITMAP pHBITMAP = CreateCompatibleBitmap(hDC, inpa->capWITH, inpa->capHIGH);
	//绑定兼容DC与位图
	HGDIOBJ preOBJ = SelectObject(pDC, pHBITMAP);

	//获取编码器
	CLSID clsid;
	if (!GetEncoderClsid(expNAME, &clsid))
	{
		_ftprintf(stderr, _T("Encode image failed\n"));
		exit(1);
	}

	//申请动态内存
	TCHAR* outNAME = new TCHAR[_MAX_PATH];

#ifndef UNICODE
	WCHAR* outNAMEW = new WCHAR[_MAX_PATH];
#endif

	//进入截图主循环
	for (int i = 1; i <= countNUM; i++)
	{
		//编号文件名
		if(i==1) 
			_stprintf((TCHARFORMAT*)outNAME, _T("%s\\%s.%s"), outDIR, imgNAME, expNAME);
		else 
			_stprintf((TCHARFORMAT*)outNAME, _T("%s\\%s[%d].%s"), outDIR, imgNAME, i, expNAME);

		//开始捕获屏幕
		BitBlt(pDC, 0, 0, inpa->capWITH, inpa->capHIGH, hDC, inpa->startX, inpa->startY, SRCCOPY);

		//HBITMAP转Bitmap
		Bitmap* bitIMG = new Bitmap(pHBITMAP, NULL);

		//位化深度
		Bitmap* covIMG = bitIMG->Clone(0, 0, inpa->capWITH, inpa->capHIGH, imgBIT);

		//保存截屏
#ifndef UNICODE
		int wLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, outNAME, -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, outNAME, -1, outNAMEW, wLen);
		outNAMEW[wLen] = '\0';
		if (covIMG->Save((LPCWSTR)outNAMEW, &clsid, NULL) != S_OK)
#else
		if (covIMG->Save((LPCWSTR)outNAME, &clsid, NULL) != S_OK)
#endif
		{
			_ftprintf(stderr, _T("Save image failed\n"));
			exit(1);
		}

		//局部释放
		delete bitIMG;
		delete covIMG;

		//间隔时间
		Sleep(intervalTIME);
	}

	//释放动态内存
	delete outNAME;

#ifndef UNICODE
	delete outNAMEW;
#endif

	//释放DC与绑定，恢复之前模式
	DeleteDC(pDC);
	DeleteDC(hDC);
	SelectObject(pDC, preOBJ);

	return 0;
}

#if defined _MSC_VER
#else
//兼容MINGW32
extern "C"
#endif

//主函数入口
int _tmain(int argc, TCHAR * *argv)
{
	if (argc < 2)
	{
		//无参数则退出
		fprintf(stdout, HELP_INFORMATION);
		exit(0);
	}

	//设置传入参数
	TCHAR* srcNAME = NULL;
	INT K = -1, countNUM = 1, imgBIT = PixelFormat32bppARGB, delayTIME = 0, intervalTIME = 0;
	PAREA pmi = { 0 }, * inpa = &pmi;

	//开关解析
	while ((K = _tgetopt(argc, argv, (TCHAR*)_T("n:b:t:d:r:o:x:y:w:h:N:B:T:D:R:O:X:Y:W:H:"))) != _TEOF)
	{
		switch (K)
		{
		case 'n':
		case 'N':
			if (OPTARG != NULL)
			{
				countNUM = _ttoi((TCHARFORMAT*)OPTARG);
				if (countNUM < 1)
				{
					_ftprintf(stderr, _T("The number of screenshots can't be less than 1\n"));
					exit(1);
				}
			}
			break;

		case 't':
		case 'T':
			if (OPTARG != NULL)
			{
				intervalTIME = _ttoi((TCHARFORMAT*)OPTARG);
				if (intervalTIME < 0)
				{
					_ftprintf(stderr, _T("The interval time can't be less than 0\n"));
					exit(1);
				}
			}
			break;

		case 'd':
		case 'D':
			if (OPTARG != NULL)
			{
				delayTIME = _ttoi((TCHARFORMAT*)OPTARG);
				if (delayTIME < 0)
				{
					_ftprintf(stderr, _T("The waiting time can't be less than 0\n"));
					exit(1);
				}
			}
			break;

		case 'o':
		case 'O':
			srcNAME = OPTARG;
			break;

		case 'b':
		case 'B':
			if (OPTARG != NULL)
			{
				int i = _ttoi((TCHARFORMAT*)OPTARG);
				if (i < 0 || 64 < i)
				{
					_ftprintf(stderr, _T("Wrong bit depth\n"));
					exit(1);
				}
				imgBIT = BITDEEP[i];
			}
			break;

		case 'x':
		case 'X':
			if (OPTARG != NULL)
			{
				inpa->startX = _ttoi((TCHARFORMAT*)OPTARG);
				if (inpa->startX < 0)
				{
					_ftprintf(stderr, _T("The swith '-x' Need a positive number\n"));
					exit(1);
				}
			}
			break;

		case 'y':
		case 'Y':
			if (OPTARG != NULL)
			{
				inpa->startY = _ttoi((TCHARFORMAT*)OPTARG);
				if (inpa->startY < 0)
				{
					_ftprintf(stderr, _T("The swith '-y' Need a positive number\n"));
					exit(1);
				}
			}
			break;

		case 'w':
		case 'W':
			if (OPTARG != NULL)
			{
				inpa->capWITH = _ttoi((TCHARFORMAT*)OPTARG);
				if (inpa->capWITH < 0)
				{
					_ftprintf(stderr, _T("The swith '-w' Need a positive number\n"));
					exit(1);
				}
			}
			break;

		case 'h':
		case 'H':
			if (OPTARG != NULL)
			{
				inpa->capHIGH = _ttoi((TCHARFORMAT*)OPTARG);
				if (inpa->capHIGH < 0)
				{
					_ftprintf(stderr, _T("The swith '-h' Need a positive number"));
					exit(1);
				}
			}
			break;

		default:
			_ftprintf(stderr, _T("Unknown switch '-%c'\n"), K);
			exit(1);
		}
	}

	if (srcNAME == NULL)
	{
		_ftprintf(stderr, _T("Needs output image name\n"));
		exit(1);
	}

	//初始化GDI+
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);

	//获取屏幕尺寸
	PAREA srcPAREA =
	{
		GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN)

	}, * ps = &srcPAREA;

	//当参数缺省时 修正截图区域
	if (inpa->capWITH == 0)
	{
		inpa->capWITH = ps->capWITH - inpa->startX;
	}
	if (inpa->capHIGH == 0)
	{
		inpa->capHIGH = ps->capHIGH - inpa->startY;
	}

	//执行等待
	Sleep(delayTIME);

	//截图核心
	CaptureScreenToImage((LPTSTR)srcNAME, inpa, imgBIT, intervalTIME, countNUM);

	//关闭GDI+
	GdiplusShutdown(gdiplustoken);
	return 0;
}