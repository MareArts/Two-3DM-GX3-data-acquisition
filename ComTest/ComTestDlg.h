// ComTestDlg.h : header file
//

#include "3DM_GX3_interface.h"

#pragma once


// CComTestDlg dialog
class CComTestDlg : public CDialog
{
// Construction
public:
	CComTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COMTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendData();
	afx_msg LRESULT OnMsgStatusCtoListBox(WPARAM wParam, LPARAM lParam);


	//functions
	interface_3DM_GX3 C3DM_GX3;
	interface_3DM_GX3 C3DM_GX3_2;
	FILE* fp1,*fp2;

	

	CListBox m_List;
	CListBox m_List10;

	afx_msg void OnBnClickedOpenButton1();
	afx_msg void OnBnClickedAbort();
	afx_msg LRESULT OnMsgSensorDtoShareM(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgSensorDtoShareM_2(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CString m_port1;
	CString m_port2;
};
