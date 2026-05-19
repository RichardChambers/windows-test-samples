#pragma once
#include <afxwin.h>

#include "Resource.h"

class CSlidePane :  public CWnd
{
private:
	// Add a member variable to track mouse hover state
	bool m_bMouseHover = false;
	CWnd* m_pSlideHolder = nullptr;

	CRect myRcSlideIn;
	CRect myRcSlideOut;


public:
	CWnd* m_pParent;
	DWORD     dwGetLastError;

	CSlidePane(CWnd* pParent = nullptr);
	virtual ~CSlidePane();

	void SetSizeSlideInSlideOut(CRect rcSlideIn, CRect rcSlideOut) { myRcSlideIn = rcSlideIn; myRcSlideOut = rcSlideOut; }

	virtual void CreateSlideBar(CWnd *pSlideHolder);
	virtual void ShowSlidingWindow() = 0;
	virtual void HideSlidingWindow() = 0;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()
};


class CMFCMenuWin : public CWnd
{
private:
	CWnd* m_pParent = nullptr;

public:
	CMFCMenuWin(CWnd* pParent = nullptr);
	~CMFCMenuWin();

protected:
	afx_msg void OnPaint();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	DECLARE_MESSAGE_MAP()
};

class CMFCSlidingMenuWin : public CSlidePane {
private:
	CWnd* m_pSlidingMenuWnd = nullptr;
	const UINT_PTR IDT_SLIDE_IN = 1;
	const UINT_PTR IDT_SLIDE_OUT = 2;

public:
	CMFCSlidingMenuWin(CWnd* pParent = nullptr);
	~CMFCSlidingMenuWin();

	virtual void ShowSlidingWindow();
	virtual void HideSlidingWindow();

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

