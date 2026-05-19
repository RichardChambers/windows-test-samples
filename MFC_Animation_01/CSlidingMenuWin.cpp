#include "pch.h"
#include "CSlidingMenuWin.h"

CSlidePane::CSlidePane(CWnd* pParent) : m_pParent(pParent), dwGetLastError(0), CWnd()
{
}

CSlidePane::~CSlidePane()
{
}


void CSlidePane::CreateSlideBar(CWnd *pSlideHolder)
{
    m_pSlideHolder = pSlideHolder;

    CRect rc;
    m_pSlideHolder->GetClientRect(&rc);
    BOOL bResult = Create(nullptr, L"SlidePane", WS_CHILD | WS_BORDER | WS_VISIBLE, rc, m_pSlideHolder, 0);

    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CSlidePane::CreateSlideBar  Safe hWnd %p\n", GetSafeHwnd());
    OutputDebugStringA(buffer);

    dwGetLastError = ::GetLastError();
    if (!bResult) {
        dwGetLastError = ::GetLastError();
    }
}

void CSlidePane::OnMouseMove(UINT nFlags, CPoint point)
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CSlidePane::OnMouseMove  Safe hWnd %p\n", GetSafeHwnd());
    OutputDebugStringA(buffer);
    if (!m_bMouseHover)
    {
        // First time mouse enters, set tracking
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = GetSafeHwnd();
        tme.dwHoverTime = HOVER_DEFAULT; // Or a specific time in millisecond
        TrackMouseEvent(&tme);

        m_bMouseHover = true;
        // Perform actions for mouse entry (e.g., show sliding window)
//        ShowSlidingWindow();
    }

    CWnd::OnMouseMove(nFlags, point);
}

void CSlidePane::OnMouseHover(UINT nFlags, CPoint point)
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CSlidePane::OnMouseHover  Safe hWnd %p\n", GetSafeHwnd());
    OutputDebugStringA(buffer);

#if 1
    // Perform actions for mouse entry (e.g., show sliding window)
    ShowSlidingWindow();
#else
    // Show the menu and start the sliding animation
    if (m_pSlidingMenuWnd)
    {
        // Calculate starting position and target position
        CRect rcStart, rcEnd;
        // ... define start and end rects ...

        m_pSlidingMenuWnd->ShowWindow(SW_SHOWNA);
        // Implement animation logic here (e.g., using a timer or thread)
        // Example using a hypothetical AnimateMenu() function
        AnimateMenu(m_pSlidingMenuWnd, rcStart, rcEnd, 500); // 500ms duration
    }
#endif

    CWnd::OnMouseHover(nFlags, point);
}

void CSlidePane::OnMouseLeave()
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CSlidePane::OnMouseLeave  Safe hWnd %p\n", GetSafeHwnd());
    OutputDebugStringA(buffer);

    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_CANCEL | TME_HOVER | TME_LEAVE;
    tme.hwndTrack = GetSafeHwnd();
    tme.dwHoverTime = HOVER_DEFAULT; // Or a specific time in millisecond
    TrackMouseEvent(&tme);
    m_bMouseHover = false;
    // Perform actions for mouse leave (e.g., hide sliding window)
    HideSlidingWindow();
    CWnd::OnMouseLeave();
}

BEGIN_MESSAGE_MAP(CSlidePane, CWnd)
END_MESSAGE_MAP()


CMFCMenuWin::CMFCMenuWin(CWnd* pParent) : m_pParent(pParent), CWnd()
{

}

CMFCMenuWin::~CMFCMenuWin()
{

}


BEGIN_MESSAGE_MAP(CMFCMenuWin, CWnd)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

void CMFCMenuWin::OnMouseMove(UINT nFlags, CPoint point)
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CMFCMenuWin::OnMouseMove  Safe hWnd %p\n", GetSafeHwnd());
    OutputDebugStringA(buffer);

    ShowWindow(SW_SHOW);

    CWnd::OnMouseMove(nFlags, point);
}

void CMFCMenuWin::OnMouseHover(UINT nFlags, CPoint point)
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CMFCMenuWin::OnMouseHover  Safe hWnd %p\n", GetSafeHwnd());
    OutputDebugStringA(buffer);
    CWnd::OnMouseHover(nFlags, point);
}

void CMFCMenuWin::OnMouseLeave()
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CMFCMenuWin::OnMouseLeave  Safe hWnd %p\n", GetSafeHwnd());
    OutputDebugStringA(buffer);

    ReleaseCapture();
//    ShowWindow(SW_HIDE);

    CWnd::OnMouseLeave();
}


void CMFCMenuWin::OnPaint()
{
    CPaintDC dc(this); // Device context for painting

    // Perform your drawing operations here using 'dc'
    // For example:
    dc.TextOut(1, 18, _T("Hello, Menu Win!"));
    dc.SelectStockObject(GRAY_BRUSH);
    dc.Rectangle(50, 50, 150, 100);
}


// ------------------------------

CMFCSlidingMenuWin::CMFCSlidingMenuWin(CWnd* pParent) : CSlidePane(pParent)
{

}

CMFCSlidingMenuWin::~CMFCSlidingMenuWin()
{
    if (m_pSlidingMenuWnd) delete m_pSlidingMenuWnd;
}

BEGIN_MESSAGE_MAP(CMFCSlidingMenuWin, CSlidePane)
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


void CMFCSlidingMenuWin::ShowSlidingWindow()
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CMFCSlidingMenuWin::ShowSlidingWindow  Safe hWnd %p\n", m_pSlidingMenuWnd->GetSafeHwnd());
    OutputDebugStringA(buffer);

    if (!m_pSlidingMenuWnd)
    {
        CRect rc(0, 0, 200, 400);
        CWnd* pParent = this->GetParent();

        m_pSlidingMenuWnd = new CMFCMenuWin(pParent);
        SetSizeSlideInSlideOut(rc, rc);
        m_pSlidingMenuWnd->Create(NULL, L"Win Menu", WS_CHILD | WS_BORDER, rc, m_pParent, 0); // Adjust size and position

        m_pSlidingMenuWnd->SetActiveWindow();

        _snprintf_s(buffer, sizeof(buffer), "CMFCSlidingMenuWin::ShowSlidingWindow  Create() Safe hWnd %p\n", m_pSlidingMenuWnd->GetSafeHwnd());
        OutputDebugStringA(buffer);
        m_pSlidingMenuWnd->SetCapture();

#if 0
        // First time mouse enters, set tracking
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_CANCEL | TME_HOVER | TME_LEAVE;
        tme.hwndTrack = GetSafeHwnd();
        tme.dwHoverTime = HOVER_DEFAULT; // Or a specific time in millisecond
        TrackMouseEvent(&tme);
#endif

    }

//    m_pSlidingMenuWnd->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE);
    m_pSlidingMenuWnd->ShowWindow(SW_SHOW);   // show window but not active with input focus
        // First time mouse enters, set tracking
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = m_pSlidingMenuWnd->GetSafeHwnd();
        tme.dwHoverTime = HOVER_DEFAULT; // Or a specific time in millisecond
        TrackMouseEvent(&tme);

    // Start sliding animation
    SetTimer(IDT_SLIDE_IN, 20, NULL);
}

void CMFCSlidingMenuWin::HideSlidingWindow()
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "CMFCSlidingMenuWin::HideSlidingWindow  Safe hWnd %p\n", GetSafeHwnd());
    OutputDebugStringA(buffer);
    if (m_pSlidingMenuWnd) {
//        m_pSlidingMenuWnd->ShowWindow(SW_HIDE);
    }
}

void CMFCSlidingMenuWin::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == IDT_SLIDE_IN)
    {
        // Update m_pSlidingMenuWnd's position for sliding in
        // ...
    }
    else if (nIDEvent == IDT_SLIDE_OUT)
    {
        // Update m_pSlidingMenuWnd's position for sliding out
        // ...
    }
    CSlidePane::OnTimer(nIDEvent);
}

void CMFCSlidingMenuWin::OnPaint()
{
    CPaintDC dc(this); // Device context for painting

    // Perform your drawing operations here using 'dc'
    // For example:
    dc.TextOut(1, 10, _T("SlidingMenuWin!"));
    dc.Rectangle(50, 50, 150, 100);
}
