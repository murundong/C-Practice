// ChatRoomV2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoomV2.h"
#include "ChatRoomV2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_PORT 6000
#define UM_SOCK WM_USER+1
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatRoomV2Dlg 对话框



CChatRoomV2Dlg::CChatRoomV2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATROOMV2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_socket = 0;
}

CChatRoomV2Dlg::~CChatRoomV2Dlg()
{
	if (m_socket) {
		closesocket(m_socket);
	}
}

void CChatRoomV2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChatRoomV2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_SOCK, OnSockMessage)
	ON_BN_CLICKED(IDC_BUTTON_Send, &CChatRoomV2Dlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CChatRoomV2Dlg 消息处理程序

BOOL CChatRoomV2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitSocket();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatRoomV2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatRoomV2Dlg::OnPaint()
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

LRESULT CChatRoomV2Dlg::OnSockMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(lParam))
	{
	case FD_READ:
		WSABUF wsaBuff;
		wsaBuff.buf = new char[200];
		wsaBuff.len = 200;

		DWORD dwRead;
		DWORD dwFlag=0;

		SOCKADDR_IN add_from;
		int len = sizeof(SOCKADDR);
		CString str;
		CString strTemp;
		HOSTENT *pHost;
		if (SOCKET_ERROR == WSARecvFrom(m_socket, &wsaBuff, 1, &dwRead, &dwFlag, (SOCKADDR*)&add_from, &len, NULL, NULL)) {
			MessageBox("接收数据失败！");
			return 0;
		}
		pHost = gethostbyaddr( (char *)&add_from.sin_addr.S_un.S_addr,4,AF_INET);
		if (0 == pHost) {
			str.Format("%s:%s", inet_ntoa(add_from.sin_addr), wsaBuff.buf);
		}
		else {
			str.Format("%s:%s", pHost->h_name, wsaBuff.buf);
		}
		str += "\r\n";
		GetDlgItemText(IDC_EDIT_Rcv, strTemp);
		str += strTemp;
		SetDlgItemText(IDC_EDIT_Rcv, str);
		break;
	}
	return 0;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChatRoomV2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CChatRoomV2Dlg::InitSocket()
{
	m_socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, 0);
	if (INVALID_SOCKET == m_socket) {
		MessageBox("创建套接字失败！");
		return FALSE;
	}
	SOCKADDR_IN addr_in;
	addr_in.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(DEFAULT_PORT);

	if (SOCKET_ERROR == bind(m_socket, (SOCKADDR*)&addr_in, sizeof(SOCKADDR))) {
		MessageBox("绑定端口失败！");
		return FALSE;
	}
	if (SOCKET_ERROR == WSAAsyncSelect(m_socket, m_hWnd, UM_SOCK, FD_READ)) {
		MessageBox("异步选择失败！");
		return FALSE;
	}
	return TRUE;
}



void CChatRoomV2Dlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwIp;
	CString strHostName;
	HOSTENT* pHostent;
	SOCKADDR_IN addr_to;
	if (GetDlgItemText(IDC_EDIT_Name, strHostName), strHostName == "") {
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIp);
		addr_to.sin_addr.S_un.S_addr = htonl(dwIp);
	}
	else {
		pHostent = gethostbyname(strHostName);
		if (0 == pHostent) {
			MessageBox("不能使用的主机名！");
			return;
		}
		else {
			addr_to.sin_addr.S_un.S_addr = *(DWORD*)pHostent->h_addr_list[0];
		}
	
	}

	
	
	addr_to.sin_family = AF_INET;
	addr_to.sin_port = htons(DEFAULT_PORT);

	CString str;
	WSABUF wsaBuff;
	DWORD dwSend;


	GetDlgItemText(IDC_EDIT_Send, str);
	SetDlgItemText(IDC_EDIT_Send, "");
	int len = str.GetLength();
	wsaBuff.buf = (char*)str.GetBuffer(len);
	wsaBuff.len = len+1;

	if (SOCKET_ERROR == WSASendTo(m_socket, &wsaBuff, 1, &dwSend, 0, (SOCKADDR*)&addr_to, sizeof(SOCKADDR), NULL, NULL)) {
		MessageBox("发送数据失败！");
		return;
	}

}
