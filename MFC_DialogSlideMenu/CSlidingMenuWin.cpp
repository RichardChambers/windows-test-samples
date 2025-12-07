#include "pch.h"
#include "CSlidingMenuWin.h"

#define ENABLE_DEBUG_OUTPUT   0

CSlidePane::CSlidePane(CWnd* pParent) : m_pParent(pParent), dwGetLastError(0), CWnd()
{
}

CSlidePane::~CSlidePane()
{

}


void CSlidePane::CreateSlideBar(CWnd* pDlgItem, CWnd *pInsertAfter)
{
    iCountAll++;

    m_pSlideParent = pDlgItem;

    pDlgItem->GetClientRect(&myRcSlideIn);            // size of window when collapsed
    m_pParent->GetClientRect(&myRcSlideOut);
    myRcSlideOut.right = myRcSlideOut.left + 150;     // size of window when expanded.

    BOOL bResult = Create(nullptr, L"SlidePane", WS_CHILD | WS_BORDER | WS_VISIBLE, myRcSlideIn, m_pParent, 0);

    dwGetLastError = 0;
    if (!bResult) {
        dwGetLastError = ::GetLastError();
    }

    //         L   T   R   B
    CRect  rc(20, 15, 120, 40);

    HWND  pButton;
    HWND  hWndParent = GetSafeHwnd();
    unsigned long ulIdcButton = 1400;

    for (int i = 1; i < 5; i++) {
        pButton = ::CreateWindow(L"BUTTON", L"Click", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
            rc.left, // x position
            (rc.top + 30) * i, // y position
            rc.right - rc.left, // Button width
            rc.bottom - rc.top, // Button height
            hWndParent, // Parent window handle
            (HMENU)(ulIdcButton + i), // Control ID (e.g., a #defined constant)
            (HINSTANCE)GetWindowLongPtr(hWndParent, GWLP_HINSTANCE), // Instance handle
            NULL // Pointer not needed
        );

        vButtonList.push_back(sButtonId{ ulIdcButton + i, pButton, nullptr });
    }
}

void CSlidePane::OnMouseMove(UINT nFlags, CPoint point)
{
    iCountAll++;

    BOOL  bResult;

    if (!m_bSlideOut) {
        iCount++;
        m_bSlideOut = true;

        SetCapture();

#if defined(ENABLE_DEBUG_OUTPUT) && ENABLE_DEBUG_OUTPUT
        char buffer[256] = { 0 };

        _snprintf_s(buffer, sizeof(buffer), "%d %d CSlidePane::OnMouseMove slideout  Safe hWnd %p  GetLastError %d m_bTracking %d\n", iCountAll, iCount, GetSafeHwnd(), dwGetLastError, m_bTracking);
        OutputDebugStringA(buffer);

        CRect rc;
        GetClientRect(&rc);
#endif

        bResult = SetWindowPos(nullptr, 0, 0, myRcSlideOut.right, myRcSlideOut.bottom, SWP_NOMOVE | SWP_NOZORDER );
        assert(bResult);

        dwGetLastError = 0;
        if (!bResult) {
            dwGetLastError = ::GetLastError();
        }

#if defined(ENABLE_DEBUG_OUTPUT) && ENABLE_DEBUG_OUTPUT
        _snprintf_s(buffer, sizeof(buffer), "%d %d CSlidePane::OnMouseMove resize  Safe hWnd %p  GetLastError %d m_bTracking %d\n", iCountAll, iCount, GetSafeHwnd(), dwGetLastError, m_bTracking);
        OutputDebugStringA(buffer);

        _snprintf_s(buffer, sizeof(buffer), "   %d %d CSlidePane::OnMouseMove  rc.right %d  rc.bottom %d \n", iCountAll, iCount, rc.right, rc.bottom);
        OutputDebugStringA(buffer);
#endif

    }
    else if (!m_bTracking) {
        m_bTracking = true;

#if defined(ENABLE_DEBUG_OUTPUT) && ENABLE_DEBUG_OUTPUT
        char buffer[256] = { 0 };

        _snprintf_s(buffer, sizeof(buffer), "%d %d CSlidePane::OnMouseMove tracking start  Safe hWnd %p  GetLastError %d m_bSlideOut %d\n", iCountAll, iCount, GetSafeHwnd(), dwGetLastError, m_bSlideOut);
        OutputDebugStringA(buffer);
#endif

        TRACKMOUSEEVENT tme = { 0 };
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = GetSafeHwnd();
        tme.dwHoverTime = HOVER_DEFAULT; // Or a specific time in millisecond
        bResult = ::TrackMouseEvent(&tme);

        assert(bResult);

        dwGetLastError = 0;
        if (!bResult) {
            dwGetLastError = ::GetLastError();
        }

#if defined(ENABLE_DEBUG_OUTPUT) && ENABLE_DEBUG_OUTPUT
        CRect rc2;
        GetClientRect(&rc2);

        char buffer[256] = { 0 };

        _snprintf_s(buffer, sizeof(buffer), "%d %d CSlidePane::OnMouseMove Tracking  Safe hWnd %p  GetLastError %d\n", iCountAll, iCount, GetSafeHwnd(), dwGetLastError);
        OutputDebugStringA(buffer);

        _snprintf_s(buffer, sizeof(buffer), "   %d %d CSlidePane::OnMouseMove  rc.right %d  rc.bottom %d \n", iCountAll, iCount, rc2.right, rc2.bottom);
        OutputDebugStringA(buffer);
#endif
    }
    else {
        CRect rc;
        GetClientRect(&rc);

        if (!rc.PtInRect(point)) {
            ReleaseCapture();
        }
    }

    CWnd::OnMouseMove(nFlags, point);
}


void CSlidePane::OnMouseLeave()
{
    iCountAll++;

#if defined(ENABLE_DEBUG_OUTPUT) && ENABLE_DEBUG_OUTPUT

    CRect rc;
    GetClientRect(&rc);

    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "%d %d CSlidePane::OnMouseLeave  Safe hWnd %p   m_bTracking = %d\n", iCountAll, iCount, GetSafeHwnd(), m_bTracking);
    OutputDebugStringA(buffer);

    _snprintf_s(buffer, sizeof(buffer), "   %d %d CSlidePane::OnMouseLeave  rc.right %d  rc.bottom %d \n", iCountAll, iCount, rc.right, rc.bottom);
    OutputDebugStringA(buffer);
#endif

    // Perform actions for mouse leave (e.g., hide sliding window)
    BOOL  bResult;
    bResult = SetWindowPos(nullptr, 0, 0, myRcSlideIn.right, myRcSlideIn.bottom, SWP_NOMOVE | SWP_NOZORDER);

    dwGetLastError = 0;
    if (!bResult) {
        dwGetLastError = ::GetLastError();
    }

#if defined(ENABLE_DEBUG_OUTPUT) && ENABLE_DEBUG_OUTPUT
    _snprintf_s(buffer, sizeof(buffer), "%d %d CSlidePane::OnMouseLeave WindowPOS  Safe hWnd %p  GetLastError %d\n", iCountAll, iCount, GetSafeHwnd(), dwGetLastError);
    OutputDebugStringA(buffer);
#endif

    m_bSlideOut = false;
    m_bTracking = false;    // mouse leave automatically cancels mouse tracking

    iCount++;
}

void CSlidePane::OnSize(UINT nType, int cx, int cy)
{
    iCountAll++;

    BOOL  bResult;

    TRACKMOUSEEVENT tme = { 0 };
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_CANCEL | TME_LEAVE;
    tme.hwndTrack = GetSafeHwnd();
    tme.dwHoverTime = HOVER_DEFAULT; // Or a specific time in millisecond
    bResult = ::TrackMouseEvent(&tme);

    m_bTracking = false;

    dwGetLastError = 0;
    if (!bResult) {
        dwGetLastError = ::GetLastError();
    }

#if defined(ENABLE_DEBUG_OUTPUT) && ENABLE_DEBUG_OUTPUT

    CRect rc;
    GetClientRect(&rc);

    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "%d %d CSlidePane::OnSize Tracking Cancel  Safe hWnd %p  GetLastError %d  m_bTracking %d\n", iCountAll, iCount, GetSafeHwnd(), dwGetLastError, m_bTracking);
    OutputDebugStringA(buffer);
    _snprintf_s(buffer, sizeof(buffer), "   %d %d CSlidePane::OnSize cx %d  cy %d  rc.right %d  rc.bottom %d \n", iCountAll, iCount, cx, cy, rc.right, rc.bottom);
    OutputDebugStringA(buffer);
#endif

    CWnd::OnSize(nType, cx, cy);

}

void CSlidePane::OnPaint()
{
    CPaintDC dc(this); // Device context for painting

    // Perform your drawing operations here using 'dc'
    // For example:
    dc.TextOut(20, 18, _T("Hello, Menu Win!"));
#if 0
    dc.SelectStockObject(GRAY_BRUSH);
    dc.Rectangle(50, 50, 150, 100);
    dc.SelectStockObject(WHITE_BRUSH);
    dc.Rectangle(20, 150, 150, 100);
#endif
}

void CSlidePane::OnButtonPress(UINT nID)
{
    char buffer[256] = { 0 };
    _snprintf_s(buffer, sizeof(buffer), "OnButtonPress() called nID= %d\n", nID);
    OutputDebugStringA(buffer);

}

void CSlidePane::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Convert mouse coordinates to screen coordinates
    ClientToScreen(&point);

    CRect buttonRect;
    // Assuming m_pTargetButton is a pointer to the CWnd you want to act as a button
    for (auto& a : vButtonList) {

        ::GetWindowRect(a.hWnd, &buttonRect);
        {
            if (buttonRect.PtInRect(point))
            {
                // Simulate button press visual
                // m_pTargetButton->DrawPressedState(); // Custom drawing function

                // Send a custom message to the target CWnd
                ::SendMessage(GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(a.ulId, BN_CLICKED), (LPARAM)a.hWnd);
                break;
            }
        }
    }

    CWnd::OnLButtonDown(nFlags, point);
}

BEGIN_MESSAGE_MAP(CSlidePane, CWnd)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSELEAVE()
    ON_CONTROL_RANGE(BN_CLICKED, 1400, 1500, &CSlidePane::OnButtonPress)
END_MESSAGE_MAP()


// ------------------------------

