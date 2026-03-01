#include "pch.h"

#if 1

#include "FluentToggle.h"

IMPLEMENT_DYNAMIC(CFluentToggle, CButton)

CFluentToggle::CFluentToggle() : m_bChecked(FALSE), m_bHovered(FALSE) {}

CFluentToggle::~CFluentToggle() {}

BEGIN_MESSAGE_MAP(CFluentToggle, CButton)
    ON_MESSAGE(BM_GETCHECK, &CFluentToggle::OnGetCheck)
    ON_MESSAGE(BM_SETCHECK, &CFluentToggle::OnSetCheck)
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CFluentToggle::PreSubclassWindow() {
    // Ensure the button is owner-drawn
    ModifyStyle(0, BS_OWNERDRAW | BS_AUTORADIOBUTTON);
    CButton::PreSubclassWindow();
}

void CFluentToggle::DrawItem(LPDRAWITEMSTRUCT lpDIS) {
    CRect rect = lpDIS->rcItem;
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);

    // Double Buffering
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap bmp;
    bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBmp = memDC.SelectObject(&bmp);

    Gdiplus::Graphics g(memDC.GetSafeHdc());
    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    // Clear background (Match parent's color or use transparency)
    g.Clear(Gdiplus::Color(255, 255, 255, 255));

    // 1. Draw Label Text
    CString strText;
    GetWindowText(strText);
    Gdiplus::Font font(L"Segoe UI", 10);
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 0, 0, 0));
    Gdiplus::RectF textRect(0, 0, (Gdiplus::REAL)rect.Width() - 45, (Gdiplus::REAL)rect.Height());

    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    g.DrawString(strText, -1, &font, textRect, &format, &textBrush);

    // 2. Track Dimensions
    Gdiplus::REAL trackW = 40.0f, trackH = 20.0f;
    Gdiplus::REAL trackX = (Gdiplus::REAL)rect.Width() - trackW - 2;
    Gdiplus::REAL trackY = (rect.Height() - trackH) / 2.0f;

    Gdiplus::GraphicsPath path;
    path.AddArc(trackX, trackY, trackH, trackH, 90, 180);
    path.AddArc(trackX + trackW - trackH, trackY, trackH, trackH, 270, 180);
    path.CloseFigure();

    // Fill/Outline Track
    if (m_bChecked) {
        Gdiplus::SolidBrush accBrush(m_clrAccent);
        g.FillPath(&accBrush, &path);
    }
    else {
        Gdiplus::Pen grayPen(m_clrGray, 1.5f);
        g.DrawPath(&grayPen, &path);
    }

    // 3. Thumb
    Gdiplus::REAL thumbSize = 12.0f;
    Gdiplus::REAL thumbY = trackY + (trackH - thumbSize) / 2.0f;
    Gdiplus::REAL thumbX = m_bChecked ? (trackX + trackW - thumbSize - 4) : (trackX + 4);

    Gdiplus::SolidBrush thumbBrush(m_bChecked ? Gdiplus::Color::White : m_clrGray);
    g.FillEllipse(&thumbBrush, thumbX, thumbY, thumbSize, thumbSize);

    // Final Blit
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldBmp);
}

void CFluentToggle::OnLButtonUp(UINT nFlags, CPoint point) {
    m_bChecked = !m_bChecked;
    Invalidate();
    // Notify parent of the click
    GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
    CButton::OnLButtonUp(nFlags, point);
}

void CFluentToggle::OnMouseMove(UINT nFlags, CPoint point) {
    if (!m_bHovered) {
        m_bHovered = TRUE;
        TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_hWnd, 0 };
        TrackMouseEvent(&tme);
        Invalidate();
    }
    CButton::OnMouseMove(nFlags, point);
}

void CFluentToggle::OnMouseLeave() {
    m_bHovered = FALSE;
    Invalidate();
    CButton::OnMouseLeave();
}


#else

#include "FluentToggle.h"

IMPLEMENT_DYNAMIC(CFluentToggle, CButton)

CFluentToggle::CFluentToggle() : m_bChecked(FALSE), m_iState(BST_UNCHECKED), m_bHovered(FALSE) {}

CFluentToggle::~CFluentToggle() {}

BEGIN_MESSAGE_MAP(CFluentToggle, CButton)
    ON_MESSAGE(BM_GETCHECK, &CFluentToggle::OnGetCheck)
    ON_MESSAGE(BM_GETSTATE, &CFluentToggle::OnGetState)
    ON_MESSAGE(BM_SETCHECK, &CFluentToggle::OnSetCheck)
    ON_MESSAGE(BM_SETSTATE, &CFluentToggle::OnSetState)
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CFluentToggle::PreSubclassWindow() {
    // Ensure the button is owner-drawn
    ModifyStyle(0, BS_OWNERDRAW | BS_AUTOCHECKBOX);
    CButton::PreSubclassWindow();
}

void CFluentToggle::DrawItem(LPDRAWITEMSTRUCT lpDIS) {
    CRect rect = lpDIS->rcItem;
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);

    // Double Buffering
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap bmp;
    bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBmp = memDC.SelectObject(&bmp);

    Gdiplus::Graphics g(memDC.GetSafeHdc());
    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    
    // Clear background (Match parent's color or use transparency)
    g.Clear(Gdiplus::Color(255, 255, 255, 255));

    // 1. Draw Label Text
    CString strText;
    GetWindowText(strText);
    Gdiplus::Font font(L"Segoe UI", 10);
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 0, 0, 0));
    Gdiplus::RectF textRect(0, 0, (Gdiplus::REAL)rect.Width() - 45, (Gdiplus::REAL)rect.Height());
    
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    g.DrawString(strText, -1, &font, textRect, &format, &textBrush);

    // 2. Track Dimensions
    Gdiplus::REAL trackW = 40.0f, trackH = 20.0f;
    Gdiplus::REAL trackX = (Gdiplus::REAL)rect.Width() - trackW - 2;
    Gdiplus::REAL trackY = (rect.Height() - trackH) / 2.0f;

    Gdiplus::GraphicsPath path;
    path.AddArc(trackX, trackY, trackH, trackH, 90, 180);
    path.AddArc(trackX + trackW - trackH, trackY, trackH, trackH, 270, 180);
    path.CloseFigure();

    // Fill/Outline Track
    if (m_bChecked) {
        Gdiplus::SolidBrush accBrush(m_clrAccent);
        g.FillPath(&accBrush, &path);
    } else {
        Gdiplus::Pen grayPen(m_clrGray, 1.5f);
        g.DrawPath(&grayPen, &path);
    }

    // 3. Thumb
    Gdiplus::REAL thumbSize = 12.0f;
    Gdiplus::REAL thumbY = trackY + (trackH - thumbSize) / 2.0f;
    Gdiplus::REAL thumbX = m_bChecked ? (trackX + trackW - thumbSize - 4) : (trackX + 4);

    Gdiplus::SolidBrush thumbBrush(m_bChecked ? Gdiplus::Color::White : m_clrGray);
    g.FillEllipse(&thumbBrush, thumbX, thumbY, thumbSize, thumbSize);

    // Final Blit
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldBmp);
}

void CFluentToggle::OnLButtonUp(UINT nFlags, CPoint point) {
#if 1
//    SetCheck(!m_bChecked);
    m_bChecked = !m_bChecked;
    // Notify parent of the click
    GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
#endif
    CButton::OnLButtonUp(nFlags, point);
}

void CFluentToggle::OnMouseMove(UINT nFlags, CPoint point) {
    if (!m_bHovered) {
        m_bHovered = TRUE;
        TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_hWnd, 0 };
        TrackMouseEvent(&tme);
        Invalidate();
    }
    CButton::OnMouseMove(nFlags, point);
}

void CFluentToggle::OnMouseLeave() {
    m_bHovered = FALSE;
    Invalidate();
    CButton::OnMouseLeave();
}

BOOL CFluentToggle::OnEraseBkgnd(CDC* pDC) { return TRUE; }
#endif
