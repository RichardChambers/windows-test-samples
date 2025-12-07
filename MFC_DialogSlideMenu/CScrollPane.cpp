#include "pch.h"
#include "CScrollPane.h"


CPanex::CPanex(UINT iDD, CWnd* pParent) : CDialogEx(iDD, pParent)
{

}

void CPanex::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	if (!pDX->m_bSaveAndValidate) {
	}
}

BOOL CPanex::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CPanex, CDialogEx)
END_MESSAGE_MAP()


CWndNotes::CWndNotes() : CWnd()
{

}

CWndNotes::CWndNotes(CWnd *p) : CWnd()
{
	m_bAttached = Attach(p->GetSafeHwnd());
}

void CWndNotes::AttachClientArea(CWnd* p)
{
	m_bAttached = Attach(p->GetSafeHwnd());
}


CWndNotes::~CWndNotes()
{
	m_bAttached = false;
}


int CWndNotes::PrintClientArea(POINT topleft, WCHAR* pText)
{
	vPrintLines.push_back(sPrintLine(topleft, std::wstring(pText)));
	return 0;
}

int CWndNotes::UpdateClientArea()
{
	Invalidate();
	UpdateWindow();

	return 0;
}

BEGIN_MESSAGE_MAP(CWndNotes, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CWndNotes::OnPaint()
{
	CPaintDC dc(this); // Device context for painting

	// Perform your drawing operations here using 'dc'
	// For example:
	for (auto x : vPrintLines) {
		dc.TextOut(x.pt.x, x.pt.y, x.str.c_str());
	}
}

//--------------

CStaticNotes::CStaticNotes() : CStatic()
{

}

CStaticNotes::CStaticNotes(CDialog *pDlg, int nDlgItem) : CStatic()
{
	//	You are using Attach(), which associates the HWND with the C++ object
	//  but does not modify the underlying Windows Window Procedure to redirect
	//  messages(like WM_PAINT) into the MFC Message Map system(BEGIN_MESSAGE_MAP).
	//  To fix this and ensure your OnPaint() is called, you must use the CWnd::SubclassDlgItem()
	//  function instead of Attach().Subclassing replaces the standard window procedure with
	//  the MFC - specific procedure that knows how to route messages to your OnPaint() handler.
#if 0
	CWnd* p = pDlg->GetDlgItem(nDlgItem);

	m_bAttached = Attach(p->GetSafeHwnd());
#endif
	m_bAttached = SubclassDlgItem(nDlgItem, pDlg);

}

void CStaticNotes::AttachClientArea(CDialog* pDlg, int nDlgItem)
{
	//	You are using Attach(), which associates the HWND with the C++ object
	//  but does not modify the underlying Windows Window Procedure to redirect
	//  messages(like WM_PAINT) into the MFC Message Map system(BEGIN_MESSAGE_MAP).
	//  To fix this and ensure your OnPaint() is called, you must use the CWnd::SubclassDlgItem()
	//  function instead of Attach().Subclassing replaces the standard window procedure with
	//  the MFC - specific procedure that knows how to route messages to your OnPaint() handler.
#if 0
	CWnd* p = pDlg->GetDlgItem(nDlgItem);

	m_bAttached = Attach(p->GetSafeHwnd());
#endif

	m_bAttached = SubclassDlgItem(nDlgItem, pDlg);
}

CStaticNotes::~CStaticNotes()
{
	m_bAttached = false;
}

int CStaticNotes::PrintClientArea(POINT topleft, WCHAR* pText)
{
	vPrintLines.push_back(sPrintLine(topleft, std::wstring(pText)));
	return 0;
}

int CStaticNotes::UpdateClientArea()
{
	Invalidate();
	UpdateWindow();

	return 0;
}

BEGIN_MESSAGE_MAP(CStaticNotes, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CStaticNotes::OnPaint()
{
	CPaintDC dc(this); // Device context for painting

	// Perform your drawing operations here using 'dc'
	// For example:
	for (auto x : vPrintLines) {
		dc.TextOut(x.pt.x, x.pt.y, x.str.c_str());
	}
}


CScrollPane::CScrollPane(CWnd* pParent /*=nullptr*/) : pPane(nullptr), CDialogEx(IDD, pParent)
{
}

CScrollPane::~CScrollPane() {
	if (pPane) delete pPane;
	pPane = nullptr;
}


void CScrollPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	if (!pDX->m_bSaveAndValidate) {
	}
}


BOOL CScrollPane::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	IDD_MFCANIMATIONTEST_PANE

	CWnd* pPlaceHolder = GetDlgItem(IDD_MFCANIMATIONTEST_SCROLLPANE);
	if (pPlaceHolder) {
		CRect rcPaneViewPort;
		pPlaceHolder->GetClientRect(&rcPaneViewPort);

		pPane = new CPanex(IDD_MFCANIMATIONTEST_PANE, this);  // create the dialog object
		pPane->Create(IDD_MFCANIMATIONTEST_PANE, this);       // create the dialog window
		CRect rcPaneSize;
		pPane->GetClientRect(&rcPaneSize);
	}

	// The vertical scrolling range is defined by 
	// (bitmap_height) - (client_height). The current vertical 
	// scroll value remains within the vertical scrolling range. 
	SCROLLINFO si;
	CRect rcPaneViewPort;
	GetClientRect(&rcPaneViewPort);

	CRect rcPaneSize = { 0 };
	rcPaneSize.right = 250;
	rcPaneSize.bottom = 400;

	int yMaxScroll = rcPaneSize.bottom;
	int yCurrentScroll = 0;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = rcPaneSize.bottom;
	si.nPage = rcPaneSize.bottom / rcPaneViewPort.bottom;
	si.nPos = yCurrentScroll;
	SetScrollInfo(SB_VERT, &si, TRUE);

	int xMaxScroll = rcPaneSize.right;
	int xCurrentScroll = 0;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = rcPaneSize.right;
	si.nPage = rcPaneSize.right / rcPaneViewPort.right;
	si.nPos = xCurrentScroll;
	SetScrollInfo(SB_HORZ, &si, TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CScrollPane, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CScrollPane::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// When the dialog containing the scrollbar resizes, update the scroll info page size
	if (::IsWindow(m_hWnd))
	{
		UpdateScrollInfo();
	}
}

void CScrollPane::UpdateScrollInfo()
{
	CRect rcPane;
	if (pPane) {
		pPane->GetClientRect(&rcPane);
	}
	else {
		rcPane.bottom = SIZEOF_PANE_X;
		rcPane.right = SIZEOF_PANE_Y;
		rcPane.left = rcPane.right = 0;
	}

	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = rcPane.bottom - rcPane.top + 46;

	CRect rc;
	GetClientRect(&rc);
	si.nPage = rc.bottom;  // SIZEOF_DIALOG_X; // Page size is the visible client height

	si.nPos = m_scrollY;
//	SetScrollInfo(SB_VERT, &si, TRUE);
}

void CScrollPane::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDeltaY = 0;
	int nLineHeight = 20; // Scroll increment

	switch (nSBCode)
	{
	case SB_LINEUP: nDeltaY = -nLineHeight; break;
	case SB_LINEDOWN: nDeltaY = nLineHeight; break;
	case SB_PAGEUP:
	{
		CRect rc;
		GetClientRect(&rc);
		nDeltaY = -rc.Height();
		break;
	}
	case SB_PAGEDOWN:
	{
		CRect rc;
		GetClientRect(&rc);
		nDeltaY = rc.Height();
		break;
	}
	case SB_THUMBTRACK: nDeltaY = (int)nPos - m_scrollY; break;
	default: CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar); return;
	}

	int nNewScrollY = m_scrollY + nDeltaY;

	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_RANGE | SIF_PAGE;
	GetScrollInfo(SB_VERT, &si);
	int nMaxScrollY = si.nMax - (int)si.nPage + 1;

	if (nNewScrollY < si.nMin) nNewScrollY = si.nMin;
	if (nNewScrollY > nMaxScrollY) nNewScrollY = nMaxScrollY;

	if (nNewScrollY != m_scrollY)
	{
		ScrollYControls(m_scrollY - nNewScrollY); // Move amount is old position - new position
		m_scrollY = nNewScrollY;
		si.fMask = SIF_POS;
		si.nPos = m_scrollY;
		SetScrollInfo(SB_VERT, &si, TRUE);
	}
}


void CScrollPane::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDeltaX = 0;
	int nLineHeight = 20; // Scroll increment

	switch (nSBCode)
	{
	case SB_LINEUP: nDeltaX = -nLineHeight; break;
	case SB_LINEDOWN: nDeltaX = nLineHeight; break;
	case SB_PAGEUP:
	{
		CRect rc;
		GetClientRect(&rc);
		nDeltaX = -rc.Width();
		break;
	}
	case SB_PAGEDOWN:
	{
		CRect rc;
		GetClientRect(&rc);
		nDeltaX = rc.Width();
		break;
	}
	case SB_THUMBTRACK: nDeltaX = (int)nPos - m_scrollX; break;
	default: CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar); return;
	}

	int nNewScrollX = m_scrollX + nDeltaX;

	SCROLLINFO si = { sizeof(si) };
	si.fMask = SIF_RANGE | SIF_PAGE;
	GetScrollInfo(SB_HORZ, &si);
	int nMaxScrollX = si.nMax - (int)si.nPage + 1;

	if (nNewScrollX < si.nMin) nNewScrollX = si.nMin;
	if (nNewScrollX > nMaxScrollX) nNewScrollX = nMaxScrollX;

	if (nNewScrollX != m_scrollX)
	{
		ScrollXControls(m_scrollX - nNewScrollX); // Move amount is old position - new position
		m_scrollX = nNewScrollX;
		si.fMask = SIF_POS;
		si.nPos = m_scrollX;
		SetScrollInfo(SB_HORZ, &si, TRUE);
	}
}


void CScrollPane::ScrollYControls(int deltaY)
{
	// Iterate through all children and move them relative to the new scroll position
	CWnd* pChild = GetWindow(GW_CHILD);
	while (pChild)
	{
		CRect rc;
		pChild->GetWindowRect(&rc);
		ScreenToClient(&rc); // Map screen coords to our dialog's client coords

		pChild->SetWindowPos(NULL, rc.left, rc.top + deltaY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
	InvalidateRect(NULL, TRUE);
}


void CScrollPane::ScrollXControls(int deltaX)
{
	// Iterate through all children and move them relative to the new scroll position
	CWnd* pChild = GetWindow(GW_CHILD);
	while (pChild)
	{
		CRect rc;
		pChild->GetWindowRect(&rc);
		ScreenToClient(&rc); // Map screen coords to our dialog's client coords

		pChild->SetWindowPos(NULL, rc.left + deltaX, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
	InvalidateRect(NULL, TRUE);
}

