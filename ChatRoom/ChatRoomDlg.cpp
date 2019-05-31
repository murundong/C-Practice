
// ChatRoomDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "ChatRoomDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_PORT 6000
#define WM_RCVDATA WM_USER+1
// CChatRoomDlg 对话框

struct ThreadParam
{
	SOCKET sock;
	HWND hwd;
};


CChatRoomDlg::CChatRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATROOM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChatRoomDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChatRoomDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_RCVDATA,OnRcvData)
END_MESSAGE_MAP()


// CChatRoomDlg 消息处理程序

BOOL CChatRoomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitSocket();

	ThreadParam *pRcvParam = new ThreadParam;
	pRcvParam->sock = m_socket;
	pRcvParam->hwd = m_hWnd;

	HANDLE h_Thread = CreateThread(NULL, 0, RcvProc, (LPVOID)pRcvParam,0, NULL);
	CloseHandle(h_Thread);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatRoomDlg::OnPaint()
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
HCURSOR CChatRoomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatRoomDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwIp;
	((CIPAddressCtrl*)GetDlgItem(IDC_IP_Addr))->GetAddress(dwIp);
	SOCKADDR_IN add_send;
	add_send.sin_addr.S_un.S_addr = htonl(dwIp);
	add_send.sin_family = AF_INET;
	add_send.sin_port = htons(DEFAULT_PORT);

	CString strSend;
	GetDlgItemText(IDC_EDIT_Send, strSend);
	/*USES_CONVERSION;*/
	if (SOCKET_ERROR == sendto(m_socket ,strSend, strSend.GetLength() + 1, 0, (SOCKADDR*)&add_send, sizeof(SOCKADDR))) {
		MessageBox("发送失败！");
		return;
	}
	SetDlgItemText(IDC_EDIT_Send, "");
}


BOOL CChatRoomDlg::InitSocket() {
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == m_socket) 
	{
		MessageBox("套接字创建失败！");
		return FALSE; 
	}

	SOCKADDR_IN sock_addr;
	sock_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(DEFAULT_PORT);

	if (SOCKET_ERROR == bind(m_socket, (SOCKADDR*)&sock_addr, sizeof(SOCKADDR))) { 
		closesocket(m_socket);
		MessageBox("绑定失败！");
		return FALSE; 
	}
}


char* CChatRoomDlg::GetTimeNow() {
	static char time_buff[20];
	time_t _time;
	time(&_time);
	strftime(time_buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&_time));
	return time_buff;
}

DWORD CChatRoomDlg::RcvProc(LPVOID lpParameter)
{
	SOCKET sock = ((ThreadParam*)lpParameter)->sock;
	HWND wnd = ((ThreadParam*)lpParameter)->hwd;

	SOCKADDR_IN add_from;
	int len = sizeof(add_from);

	
	char rcv_buff[1024];
	char temp_buff[1024];
	
	while (TRUE)
	{
		if (SOCKET_ERROR == recvfrom(sock,rcv_buff, 1024, 0, (SOCKADDR*)&add_from, &len)) break;
		/*CString send_temp;
		send_temp.Format("%s_%s:%s",GetTimeNow(), inet_ntoa(add_from.sin_addr), rcv_buff);*/
		sprintf(temp_buff, "%s %s: %s", GetTimeNow(), inet_ntoa(add_from.sin_addr), rcv_buff);
		::PostMessage(wnd, WM_RCVDATA, 0, (LPARAM)temp_buff);
	}
	return 0;
}

LRESULT CChatRoomDlg::OnRcvData(WPARAM wParam, LPARAM lParam)
{
	/*CString* str_temp = (CString*)lParam;*/
	CString str((char*)lParam);
	CString strTemp;
	GetDlgItemText(IDC_EDIT_Rcv, strTemp);
	str += "\r\n";
	str += strTemp;
	SetDlgItemText(IDC_EDIT_Rcv, str);
	return 0;
}

