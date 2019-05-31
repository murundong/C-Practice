
// DllHookDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DllHook.h"
#include "DllHookDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#pragma comment (lib,"HandDLL.lib")
//extern "C" _declspec(dllimport) void  SetHook(HWND hwnd);
// CDllHookDlg 对话框



CDllHookDlg::CDllHookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLLHOOK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDllHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDllHookDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDllHookDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDllHookDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDllHookDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDllHookDlg 消息处理程序
HHOOK g_keyHook;
HWND g_hwnd;
BOOL CDllHookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_keyHook = NULL;
	g_hwnd = m_hWnd;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDllHookDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDllHookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDllHookDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
LRESULT CALLBACK MouseProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	return 1;
}


LRESULT CALLBACK KeyboardProc(
	_In_ int    code,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	//if (VK_SPACE == wParam ) {
	//	return CallNextHookEx(g_keyHook, code, wParam, lParam); 
	//}
	//return 1;
	if (VK_F1 == wParam && (lParam >> 29 & 1)) {
		::SendMessage(g_hwnd, WM_CLOSE, 0, 0);
		UnhookWindowsHookEx(g_keyHook);
	}
	else
	{
		return 1;
	}
}

void CDllHookDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	/*SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());*/

	typedef void(*hook)(HWND m_hwnd);
	HINSTANCE hDll;
	hDll = LoadLibrary(L"HandDLL.dll");
	if (hDll == NULL) {
		DWORD dwErrorCode = ::GetLastError();
		TCHAR* buffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErrorCode, 0, (LPTSTR)&buffer, 0, NULL);
		MessageBox(buffer);
		return;
	}
	hook h;
	h = (hook)GetProcAddress(hDll, "SetHook");
	h(m_hWnd);
}


void CDllHookDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_keyHook =	SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
}
