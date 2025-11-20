
// MFCAnimationtestDlg.h : header file
//

#pragma once

#include <thread>

#include "CScrollPane.h"

// CMFCAnimationtestDlg dialog
class CMFCAnimationtestDlg : public CDialogEx
{
// Construction
public:
	CAnimateWAM  animate;

	CMFCAnimationtestDlg(CWnd* pParent = nullptr);	// standard constructor
	~CMFCAnimationtestDlg();	// standard destructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCANIMATIONTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	std::thread  fadeInThread;
	BOOL  m_bVisible = FALSE;

	CWnd   * pPlaceHolder;
	CRect    rcPlaceHolder;

	CScrollPane * pScrollPane;
	CRect         rcScrollPane;


	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // Your handler
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point); // Your handler
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point); // Your handler
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point); // Your handler

	void FadeIn(CWnd* ctl);
	void FadeOut(CWnd* ctl);

	DECLARE_MESSAGE_MAP()
};
