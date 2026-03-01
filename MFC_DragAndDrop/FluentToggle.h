#pragma once
#include <gdiplus.h>

class CFluentToggle : public CButton
{
    DECLARE_DYNAMIC(CFluentToggle)

public:
    CFluentToggle();
    virtual ~CFluentToggle();

    void SetCheck(BOOL bChecked) {
        // If the control hasn't been created yet through DDX_Control()
        // then there is no window to Invalidate() so we have to check
        // whether the window exists otherwise we'll get an exception.
        // However we'll aways set the checked state so that whenever the
        // window is created, we can show the correct checked state.
        m_bChecked = bChecked;
        if (GetSafeHwnd()) {
            Invalidate();
        }
    }

    BOOL GetCheck() const { return m_bChecked; }

protected:
    BOOL m_bChecked;
    BOOL m_bHovered;
    Gdiplus::Color m_clrAccent{ 255, 0, 120, 212 }; // Fluent Blue
    Gdiplus::Color m_clrGray{ 255, 100, 100, 100 };

    virtual void PreSubclassWindow() override;
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;

    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC) { return TRUE; }

    afx_msg LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam) {
        return m_bChecked;
    }

    afx_msg LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam) {
        // 1. Extract the new check state from wParam
        // Possible values: BST_CHECKED, BST_UNCHECKED
        int nNewChecked = (int)wParam;

        switch (nNewChecked) {
        case BST_UNCHECKED:
            nNewChecked = FALSE;
            break;
        default:
        case BST_CHECKED:
            nNewChecked = TRUE;
            break;
        }

        // 2. Only update if the state has actually changed
        if (m_bChecked != nNewChecked)
        {
            m_bChecked = nNewChecked;

            // 3. Invalidate the window to trigger a redraw (OnPaint)
            // This ensures the visual "check" or "dot" appears
            Invalidate();
            UpdateWindow();
        }

        // BM_SETCHECK always returns 0
        return 0;
    }

    DECLARE_MESSAGE_MAP()
};
