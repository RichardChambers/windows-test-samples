#pragma once
#include "CDialogPane.h"

#include "DialogPane_AC20.h"


struct DragDrop_Data_AC68 {
	ULONG    pluNo;
	wchar_t  pluName[20];
	USHORT   deptNo;
	ULONG    ulStatus[6];
};


class DialogPane_AC68 : public CDialogDrop
{
	DECLARE_DYNAMIC(DialogPane_AC68)

protected:

	virtual int DropTypeValid(COleDataObject* pDataObject) {
		int iDropType = 0;
		unsigned long  ulMask = 0;
		
		ulMask |= pDataObject->IsDataAvailable(DRAGDROP_DATA_AC20) ? 0x01 : 0;
		ulMask |= pDataObject->IsDataAvailable(DRAGDROP_DATA_AC68) ? 0x02 : 0;

		switch (ulMask) {
		case 0x01:
			iDropType = DRAGDROP_DATA_AC20;
			break;
		case 0x02:
			iDropType = DRAGDROP_DATA_AC68;
			break;
		case 0x03:
			break;
		}
		
		return iDropType;
	}

	int    myIcount;

	virtual BOOL OnInitDialog();

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

public:
	DialogPane_AC68(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DialogPane_AC68();

	void PutData(DragDrop_Data_AC68& x);
	void PutDataAC20(DragDrop_Data_AC20& x);
	void GetData(DragDrop_Data_AC68& x);

	DragDrop_Data_AC68  m_DataRecord;

#if defined(USE_CCOPYPASTEWND) && USE_CCOPYPASTEWND==1
	CCopyPasteWnd  m_wndCopyPaste;
#endif

	static int iCount;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AC20 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// ----------------------------------------------------------
	// Following implements the behavior for a mouse hover over the dialog.
	// The desired behavior is to indicate to the user that the dialog is
	// now available for use. this requires us to:
	//   - bring this window to the top of the Z-order so that it gets all messages
	//   - modify the style of the window to create a border in order to change the visual
	//   - invalidate the window so that it will be redrawn
	//   - set the focus of the window.
	//
	// When the mouse leaves the window then we will remove the border visual to indicate
	// the window is no longer the active window.
	// 
	// WARNING: When the mouse cursor is over a control such as an Edit control in a
	//          dialog then any mouse movement messages go to the control and not to
	//          the dialog. This also means that when the mouse cursor moves over a
	//          control the dialog will receive a Mouse Leave message.
	//
	// ModifyStyle(0, WS_BORDER, SWP_FRAMECHANGED);
	// The WS_BORDER style affects the Non-Client (NC) area of the window. When you change
	// this style, Windows must recalculate the size of the client area (via the WM_NCCALCSIZE
	// message) so that your controls don't overlap the new border. Passing SWP_FRAMECHANGED
	// triggers this recalculation.
	//
	// STYLE DS_SETFONT | DS_CONTROL | WS_CHILD | WS_CLIPSIBLINGS
	// The WS_CLIPSIBLINGS style is needed so that the active window will be completely displayed
	// and any sibling windows will not clip it when the active windows is moved to the top of
	// the Z-order.

	BOOL m_bHovering = FALSE;
	BOOL m_bMouseTracking = FALSE;

	afx_msg void OnMouseHover(UINT nFlags, CPoint point) {
		m_bHovering = TRUE;
		BringWindowToTop();     // bring this window to the top to clip any siblings
		ModifyStyle(0, WS_BORDER, SWP_FRAMECHANGED);    // add the border around the window
		Invalidate();          // trigger a redraw of the window
		SetFocus();
	}

	afx_msg void OnMouseLeave() {
		m_bHovering = m_bMouseTracking = FALSE;
		ModifyStyle(WS_BORDER, 0, SWP_FRAMECHANGED);    // remove the border around the window.
		Invalidate(); // Reset to normal colors
	}

	DECLARE_MESSAGE_MAP()
};
