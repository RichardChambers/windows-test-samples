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




CScrollPane::CScrollPane(CWnd* pParent /*=nullptr*/) : pPane(nullptr), CDialogEx(IDD, pParent)
{
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
	CRect rcPaneViewPort = { 0 };
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CScrollPane, CDialogEx)
	ON_WM_VSCROLL()
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
		ScrollControls(m_scrollY - nNewScrollY); // Move amount is old position - new position
		m_scrollY = nNewScrollY;
		si.fMask = SIF_POS;
		si.nPos = m_scrollY;
		SetScrollInfo(SB_VERT, &si, TRUE);
	}
}


void CScrollPane::ScrollControls(int deltaY)
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

