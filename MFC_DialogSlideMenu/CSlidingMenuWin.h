#pragma once
#include <afxwin.h>
#include <vector>
#include <cassert>

#include "Resource.h"

class CSlidePane :  public CWnd
{
private:
	// Add a member variable to track mouse hover state
	bool m_bSlideOut = false;
	bool m_bTracking = false;

	CWnd* m_pParent = nullptr;

	CWnd* m_pSlideParent = nullptr;

	CRect myRcSlideIn;
	CRect myRcSlideOut;

	int iCount = 0;   // count of mouse leave.
	int iCountAll = 0;  // count of individual function calls.

public:
	DWORD     dwGetLastError;
	struct sButtonId {
		unsigned long  ulId;
		HWND   hWnd;
		int (*func)();
		sButtonId(unsigned long id, HWND h, int (*fu)()) : ulId(id), hWnd(h), func(fu) {
		}
		sButtonId(const sButtonId& x) : ulId(x.ulId), hWnd(x.hWnd), func(x.func) {    // copy constructor 
		}
		sButtonId(sButtonId&& x) noexcept {   // move constructor
			ulId = x.ulId;
			hWnd = x.hWnd;
			func = x.func;

			x.ulId = 0;
			x.hWnd = nullptr;
			x.func = nullptr;
		}
		~sButtonId() {
			if (hWnd) ::DestroyWindow(hWnd);
			hWnd = nullptr;
		}
	};
	std::vector<sButtonId> vButtonList;

	CSlidePane(CWnd* pParent = nullptr);
	virtual ~CSlidePane();

	void SetSizeSlideInSlideOut(CRect rcSlideIn, CRect rcSlideOut) { myRcSlideIn = rcSlideIn; myRcSlideOut = rcSlideOut; }

	virtual void CreateSlideBar(CWnd* pDlgItem, CWnd *pSlideHolder);

	afx_msg void OnButtonPress(UINT nID);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

