
#pragma once

#include <afxext.h>

#include "Resource.h"

class CPanex : public CDialogEx
{
protected:

public:
	CPanex(UINT iDD, CWnd* pParent = nullptr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

		// Generated message map functions
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

class CScrollPane : public CDialogEx
{
protected:

public:
	enum {IDD = IDD_MFCANIMATIONTEST_SCROLLPANE};

	CScrollPane(CWnd* pParent = nullptr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

		// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	CPanex* pPane;

	int m_scrollY = 0;
	int m_scrollX = 0;

	void UpdateScrollInfo();
	void ScrollYControls(int deltaY);
	void ScrollXControls(int deltaX);

};

