
// ChatRoomDlg.h : 头文件
//

#pragma once


// CChatRoomDlg 对话框
class CChatRoomDlg : public CDialogEx
{
// 构造
public:
	CChatRoomDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATROOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	BOOL InitSocket();
	static char* GetTimeNow();
	static	DWORD WINAPI RcvProc(
		_In_ LPVOID lpParameter
	);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRcvData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	SOCKET m_socket;
};
