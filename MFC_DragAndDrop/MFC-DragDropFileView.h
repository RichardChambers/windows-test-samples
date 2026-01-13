
// MFC-DragDropFileView.h : interface of the CMFCDragDropFileView class
//

#pragma once

#include <afxole.h>

#include "CDialogPane.h"

class CMFCDragDropFileView : public CView
{
protected: // create from serialization only
	CMFCDragDropFileView() noexcept;
	DECLARE_DYNCREATE(CMFCDragDropFileView)

// Attributes
public:
	CMFCDragDropFileDoc* GetDocument() const;

// Operations
public:
	static int iCount;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMFCDragDropFileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	COleDropTarget m_dropTarget;
	CPoint m_ptDragStart;
	BOOL   m_bDragging = FALSE;

	enum class DraggingState { None = 0, Entering, Dragging, Leaving };

	DraggingState m_DraggingState;

	int    myIcount;

	CDialogPane m_wndMyDialogPaneAC20; // Member variable for the dialog

	wchar_t* str[3];
	CRect rect;

	DragDrop_Data_AC20 m_Ac20Dummy;

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);


// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MFC-DragDropFileView.cpp
inline CMFCDragDropFileDoc* CMFCDragDropFileView::GetDocument() const
   { return reinterpret_cast<CMFCDragDropFileDoc*>(m_pDocument); }
#endif

