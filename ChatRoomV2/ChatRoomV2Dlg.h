
// ChatRoomV2Dlg.h : 头文件
//

#pragma once


// CChatRoomV2Dlg 对话框
class CChatRoomV2Dlg : public CDialogEx
{
// 构造
public:
	CChatRoomV2Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~CChatRoomV2Dlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATROOMV2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg LRESULT OnSockMessage(WPARAM wParam,LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	SOCKET m_socket;

	BOOL InitSocket();
	afx_msg void OnBnClickedButtonSend();
};
