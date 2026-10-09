#include <windows.h>
#include <stdio.h>
//#include<conio.h>
int main(int argc, char* argv[])
{
	if(argc == 1){
		printf("Usage: %s [hwnd] - \n", argv[0]);
		return 1;
	}
	HWND hwnd = NULL;
	int bit = 10;
	if (argv[1][0] == '0' && argv[1][1] == 'x' || argv[1][1] == 'X') bit = 16;
	char* stop;
	int ans = strtol(argv[1], &stop, bit);
	hwnd = (HWND)ans;
	HWND m_hWnd = hwnd;
	
if(argc > 2){
	//添加标题栏
	::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) | WS_CAPTION );  
	::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	//最小化按钮有效
	::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) | WS_MINIMIZEBOX );  
	::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	//最大化按钮有效
	::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) | WS_MAXIMIZEBOX );  
	::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	//关闭按钮有效
	::EnableMenuItem(::GetSystemMenu(m_hWnd,false),SC_CLOSE,MF_BYCOMMAND | MF_ENABLED);
	if (IsZoomed(hwnd))
	{
		// 恢复显示为最小化之前的窗口位置和大小，激活窗口
		::ShowWindow(hwnd, SW_SHOWNOACTIVATE);  // 以最近的大小和位置显示窗口，窗口不激活
		::ShowWindow(hwnd, SW_SHOW);            // 激活窗口并以当前大小和位置显示
	}
	return 1;
}
	// 判断窗口是否为最小化状态
	//if (IsIconic(hwnd))
	if (!IsZoomed(hwnd))
	{
		// 恢复显示为最小化之前的窗口位置和大小，激活窗口
		::ShowWindow(hwnd, SW_MAXIMIZE);  // 以最近的大小和位置显示窗口，窗口不激活
		::ShowWindow(hwnd, SW_SHOW);            // 激活窗口并以当前大小和位置显示
	}
	
	/* HMENU hmenu = GetSystemMenu(hwnd, false);		// 复制或修改而访问窗口菜单
	RemoveMenu(hmenu, SC_SIZE               , MF_BYCOMMAND);	// 从指定菜单删除一个菜单项或分离一个子菜单 禁用最大化按钮
	RemoveMenu(hmenu, SC_MOVE               , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_MINIMIZE           , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_MAXIMIZE           , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_NEXTWINDOW         , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_PREVWINDOW         , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_CLOSE              , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_VSCROLL            , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_HSCROLL            , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_MOUSEMENU          , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_KEYMENU            , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_ARRANGE            , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_RESTORE            , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_TASKLIST           , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_SCREENSAVE         , MF_BYCOMMAND);
	RemoveMenu(hmenu, SC_HOTKEY             , MF_BYCOMMAND);
	DrawMenuBar(hwnd); */
	
	/* LONG style = GetWindowLong(hwnd, GWL_STYLE);	// GWL_STYLE	-16		设定一个新的窗口风格。
	style &= ~(WS_MAXIMIZEBOX);						// 取消最大化按钮显示
	SetWindowLong(hwnd, GWL_STYLE, style);			// 设置窗口属性 最大化按钮变为灰色，且点击无效
	
	style &= ~(WS_MINIMIZEBOX);						// 取消最大化按钮显示
	SetWindowLong(hwnd, GWL_STYLE, style);			// 设置窗口属性 最大化按钮变为灰色，且点击无效
	 */
	//最小化按钮无效
	::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) &~WS_MINIMIZEBOX );  
	::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	//最大化按钮无效
	::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) &~WS_MAXIMIZEBOX );  
	::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	//关闭按钮无效
	::EnableMenuItem(::GetSystemMenu(m_hWnd,false),SC_CLOSE,MF_BYCOMMAND | MF_GRAYED);
	//工具栏窗口。在任务栏上没有程序显示,需要添加在OnInitDialog()里
	//SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE) &~WS_EX_APPWINDOW|WS_EX_TOOLWINDOW ); 
	
	//取消标题栏
	::SetWindowLong(hwnd,GWL_STYLE,GetWindowLong(hwnd,GWL_STYLE) & ~WS_CAPTION );
	::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	//getch();
	
	//取消标题栏和3d边框，保留一个线条的细边框，不能调整窗口大小
	//::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME | WS_BORDER );  
	//::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	
	//取消所有边框
	//::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME );  
	//::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	
	return 0;
}
/*
//GetSystemMenu 来禁用关闭时，关闭按钮在禁用的同时会变成灰色
#include<Windows.h>    
int main() {
	HWND hwnd = GetConsoleWindow();		
	HMENU hmenu = GetSystemMenu(hwnd, false);
	RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
	DrawMenuBar(hwnd);
	return 0;
}

// RemoveMenu可支持的其他菜单选项：
// System Menu Command Values

#define SC_SIZE         0xF000
#define SC_MOVE         0xF010
#define SC_MINIMIZE     0xF020
#define SC_MAXIMIZE     0xF030
#define SC_NEXTWINDOW   0xF040
#define SC_PREVWINDOW   0xF050
#define SC_CLOSE        0xF060
#define SC_VSCROLL      0xF070
#define SC_HSCROLL      0xF080
#define SC_MOUSEMENU    0xF090
#define SC_KEYMENU      0xF100
#define SC_ARRANGE      0xF110
#define SC_RESTORE      0xF120
#define SC_TASKLIST     0xF130
#define SC_SCREENSAVE   0xF140
#define SC_HOTKEY       0xF150

#include<Windows.h>    
//使用 SetWindowLong 禁用最大化按钮
int main() {
	HWND hwnd = GetConsoleWindow();
	LONG style = GetWindowLong(hwnd, GWL_STYLE);	// GWL_STYLE	-16		设定一个新的窗口风格。
	style &= ~(WS_MAXIMIZEBOX);						// 取消最大化按钮显示
	SetWindowLong(hwnd, GWL_STYLE, style);			// 设置窗口属性 最大化按钮变为灰色，且点击无效
	return 0;
}

GWL_EXSTYLE 设置新的扩展窗口风格。 
GWL_STYLE 设置新的窗口风格 
GWL_WNDPROC 为窗口过程设置新地址。 
GWL_HINSTANCE 设置一个新的应用程序的实例句柄。 
GWL_ID 设置一人新的窗口标识符。 
GWL_USERDATA 设置与窗口相联系的长值。每个窗口都有一个供创建它的应用 

//Window Styles 除此之外，窗口风格这里还支持其他属性：

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L     WS_BORDER | WS_DLGFRAME 
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW

#include <windows.h>
#include <stdio.h>
int main(int argc, char* argv[])
{
	HWND hwnd = NULL;
	int bit = 10;
	if (argv[1][0] == '0' && argv[1][1] == 'x' || argv[1][1] == 'X') bit = 16;
	char* stop;
	int ans = strtol(argv[1], &stop, bit);
	hwnd = (HWND)ans;
	// 判断窗口是否为最小化状态
	//if (IsIconic(hwnd))
	if (IsZoomed(hwnd))
	{
		// 恢复显示为最小化之前的窗口位置和大小，激活窗口
		::ShowWindow(hwnd, SW_SHOWNOACTIVATE);  // 以最近的大小和位置显示窗口，窗口不激活
		::ShowWindow(hwnd, SW_SHOW);            // 激活窗口并以当前大小和位置显示
	}
	return 0;
}

ShowWindow(
  hWnd: HWND;       {要显示的窗口的句柄}
  nCmdShow: Integer {选项, 参加下表}
): BOOL;

//uCmdShow 参数可选值:
SW_HIDE            = 0;  {隐藏, 并且任务栏也没有最小化图标}
SW_SHOWNORMAL      = 1;  {用最近的大小和位置显示, 激活}
SW_NORMAL          = 1;  {同 SW_SHOWNORMAL}
SW_SHOWMINIMIZED   = 2;  {最小化, 激活}
SW_SHOWMAXIMIZED   = 3;  {最大化, 激活}
SW_MAXIMIZE        = 3;  {同 SW_SHOWMAXIMIZED}
SW_SHOWNOACTIVATE  = 4;  {用最近的大小和位置显示, 不激活}
SW_SHOW            = 5;  {同 SW_SHOWNORMAL}
SW_MINIMIZE        = 6;  {最小化, 不激活}
SW_SHOWMINNOACTIVE = 7;  {同 SW_MINIMIZE}
SW_SHOWNA          = 8;  {同 SW_SHOWNOACTIVATE}
SW_RESTORE         = 9;  {同 SW_SHOWNORMAL}
SW_SHOWDEFAULT     = 10; {同 SW_SHOWNORMAL}
SW_MAX             = 10; {同 SW_SHOWNORMAL} 

//如果有WS_EX_DLGMODALFRAME还要取消WS_EX_DLGMODALFRAME
// ::SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) 
//   & ~WS_EX_DLGMODALFRAME); 
// ::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);

//取消所有边框
::SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) 
   & ~WS_CAPTION & ~WS_THICKFRAME );  
::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

// //如果有WS_EX_DLGMODALFRAME还要取消WS_EX_DLGMODALFRAME
// ::SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) 
//   & ~WS_EX_DLGMODALFRAME); 
// ::SetWindowPos(m_hWnd,NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);

//VC++6.0 如何去掉MFC向导生成的SDI程序中视图边框的3D效果2010-03-24 17:48先要去掉view的边框，代码如下： 
BOOL CSDIView::PreCreateWindow(CREATESTRUCT& cs) 
{
cs.style &=~WS_BORDER;//加入的代码 
return CFormView::PreCreateWindow(cs); 
} 
//然后在去掉外部Frame的客户区边框，代码如下： 
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{ 
	if( !CFrameWnd::PreCreateWindow(cs) ) 
	return FALSE; 
	cs.dwExStyle&=~WS_EX_CLIENTEDGE;//加入的代码，一定要在CFrameWnd::PreCreateWindow(cs)之后执行 
	return TRUE; 
}
*/



