
// MFCAnimationtestDlg.h : header file
//

#pragma once

#include <thread>

#include "CScrollPane.h"
#include "CSlidingMenuWin.h"

// CMFCAnimationtestDlg dialog
class CMFCAnimationtestDlg : public CDialogEx
{
// Construction
public:

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

	CWnd   * pPlaceHolder = nullptr;
	CRect    rcPlaceHolder;

	CScrollPane * pScrollPane = nullptr;
	CRect         rcScrollPane;

	CWnd   * pSlideHolder = nullptr;
	CRect    rcSlideHolder;

	CSlidePane * pSlidePane = nullptr;
	CRect         rcSlidePane;

	CStaticNotes m_Notes;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void FadeIn(CWnd* ctl);
	void FadeOut(CWnd* ctl);

	DECLARE_MESSAGE_MAP()
};
