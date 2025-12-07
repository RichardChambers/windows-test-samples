
#pragma once

#include <afxext.h>

#include <string>
#include <vector>

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

class CWndNotes : public CWnd
{
private:
	struct  sPrintLine {
		POINT  pt;
		std::wstring  str;
		sPrintLine(POINT p, std::wstring s) : pt(p), str(s) {
		}
		sPrintLine(const sPrintLine& x) : pt(x.pt), str(x.str) {    // copy constructor 
		}
		sPrintLine(sPrintLine&& x) noexcept {   // move constructor
			pt = x.pt;
			str = x.str;
			x.str.empty();
		};
	};

	std::vector<sPrintLine> vPrintLines;

	bool m_bAttached = false;

public:
	CWndNotes();
	~CWndNotes();
	CWndNotes(CWnd* p);

	void AttachClientArea(CWnd* p);

	int PrintClientArea(POINT topleft, WCHAR* pText);
	int UpdateClientArea();

protected:

	afx_msg void OnPaint();
#if 0
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
#endif

	DECLARE_MESSAGE_MAP()
};


class CStaticNotes : public CStatic
{
private:
	struct  sPrintLine {
		POINT  pt;
		std::wstring  str;
		sPrintLine(POINT p, std::wstring s) : pt(p), str(s) {
		}
		sPrintLine(const sPrintLine& x) : pt(x.pt), str(x.str) {    // copy constructor 
		}
		sPrintLine(sPrintLine&& x) noexcept {   // move constructor
			pt = x.pt;
			str = x.str;
			x.str.empty();
		};
	};

	std::vector<sPrintLine> vPrintLines;

	bool m_bAttached = false;

public:
	CStaticNotes();
	~CStaticNotes();
	CStaticNotes(CDialog *pDlg, int nDlgItem);

	void AttachClientArea(CDialog* pDlg, int nDlgItem);

	int PrintClientArea(POINT topleft, WCHAR* pText);
	int UpdateClientArea();

protected:

	afx_msg void OnPaint();
#if 0
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
#endif

	DECLARE_MESSAGE_MAP()
};


class CScrollPane : public CDialogEx
{
protected:

public:
	enum {IDD = IDD_MFCANIMATIONTEST_SCROLLPANE};

	CScrollPane(CWnd* pParent = nullptr);
	~CScrollPane();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

		// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	CPanex* pPane = nullptr;

	int m_scrollY = 0;
	int m_scrollX = 0;

	void UpdateScrollInfo();
	void ScrollYControls(int deltaY);
	void ScrollXControls(int deltaX);

};

