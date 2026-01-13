
// MFC-DragDropFileView.cpp : implementation of the CMFCDragDropFileView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFC-DragDropFile.h"
#endif

#include "MFC-DragDropFileDoc.h"
#include "MFC-DragDropFileView.h"

#include "MainFrm.h"

#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDragDropFileView

int  CMFCDragDropFileView::iCount = 0;

IMPLEMENT_DYNCREATE(CMFCDragDropFileView, CView)

BEGIN_MESSAGE_MAP(CMFCDragDropFileView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCDragDropFileView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFCDragDropFileView construction/destruction

CMFCDragDropFileView::CMFCDragDropFileView() noexcept
{
	// TODO: add construction code here

	m_DraggingState = DraggingState::None;

	myIcount = ++iCount;

	str[0] = L"Text Type";
	str[1] = L"AC20 type";
	str[2] = L"AC114 Type";

	rect = CRect(CPoint(30, 20), CPoint(140, 45));

	m_Ac20Dummy = { 12, L"Dept 12", 0 };
}

CMFCDragDropFileView::~CMFCDragDropFileView()
{
}

BOOL CMFCDragDropFileView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

int CMFCDragDropFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CMFCDragDropFileView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndMyDialogPaneAC20.m_hWnd != NULL)
	{
		// Resize the dialog to fit within a desired area of the view
		// e.g., Fill the entire view
//		m_wndMyDialogPaneAC20.MoveWindow(0, 0, cx, cy);
	}
}

void CMFCDragDropFileView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// Link the view to the OLE drop target object
	if (!m_dropTarget.Register(this))
	{
		TRACE(_T("Failed to register drop target\n"));
	}

	// Check if the dialog window has already been created (m_hWnd is non-NULL)
	if (m_wndMyDialogPaneAC20.m_hWnd == NULL)
	{
		// Create the modeless dialog as a child of the view
		// IDD_MY_DIALOG_PANE is the dialog resource ID
		// "this" points to the CView parent
		if (!m_wndMyDialogPaneAC20.Create(IDD_DIALOG_AC20, this))
		{
			// Handle creation failure
			TRACE0("Warning: Dialog creation failed.\n");
			return;
		}

		m_wndMyDialogPaneAC20.DropType(DRAGDROP_DATA_AC20);

		// Optional: Position the dialog within the view's client area
		// Get the view's client rectangle
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcDialog;
		m_wndMyDialogPaneAC20.GetClientRect(&rcDialog);

		// Set the dialog's position and size
		// Example: place it in the top-left corner with specific dimensions
		m_wndMyDialogPaneAC20.SetWindowPos(NULL, 150, 10, 400, 300, SWP_SHOWWINDOW | SWP_NOZORDER);
	}

}

// Override these functions to provide feedback and handle the drop:
DROPEFFECT CMFCDragDropFileView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
	wchar_t xBuff[128] = { 0 };

	swprintf_s(xBuff, 128, L"    CMFCDragDropFileView[%d]::OnDragEnter()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	m_DraggingState = DraggingState::Entering;

	// Check if the data format is acceptable (e.g., CF_TEXT)
	if (pDataObject->IsDataAvailable(CF_UNICODETEXT))
		return DROPEFFECT_COPY; // Show copy cursor
	else if (pDataObject->IsDataAvailable(DRAGDROP_DATA_AC20))
		return DROPEFFECT_COPY; // Show copy cursor
	else
		return DROPEFFECT_NONE; // Show "not allowed" cursor
}

DROPEFFECT CMFCDragDropFileView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (m_DraggingState != DraggingState::Dragging) {
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
		wchar_t xBuff[128] = { 0 };

		swprintf_s(xBuff, 128, L"    CMFCDragDropFileView[%d]::OnDragOver()", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

		m_DraggingState = DraggingState::Dragging;
	}

	// Same check as OnDragEnter to provide continuous feedback
	if (pDataObject->IsDataAvailable(CF_UNICODETEXT))
		return DROPEFFECT_COPY;
	else if (pDataObject->IsDataAvailable(DRAGDROP_DATA_AC20))
		return DROPEFFECT_COPY; // Show copy cursor
	else
		return DROPEFFECT_NONE;
}

void CMFCDragDropFileView::OnDragLeave()
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	wchar_t xBuff[128] = { 0 };

	swprintf_s(xBuff, 128, L"    CMFCDragDropFileView[%d]::OnDragLeave()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	m_DraggingState = DraggingState::Leaving;
}

BOOL CMFCDragDropFileView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	USHORT  dataAvailable = 0;

	dataAvailable = (pDataObject->IsDataAvailable(CF_UNICODETEXT) * 0x01) | (pDataObject->IsDataAvailable(DRAGDROP_DATA_AC20) * 0x02);

	wchar_t xBuff[128] = { 0 };

	switch (dataAvailable) {
	case 0:
		swprintf_s(xBuff, 128, L"    CMFCDragDropFileView[%d]::OnDrop() case 0", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		break;
	case 1:
		swprintf_s(xBuff, 128, L"    CMFCDragDropFileView[%d]::OnDrop() case 1: CF_UNICODETEXT", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		break;
	case 2:
		swprintf_s(xBuff, 128, L"    CMFCDragDropFileView[%d]::OnDrop() case 2: DRAGDROP_DATA_AC20", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		break;
	default:
		swprintf_s(xBuff, 128, L"    CMFCDragDropFileView[%d]::OnDrop() default", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		break;
	}

	if (pDataObject->IsDataAvailable(CF_UNICODETEXT))
	{
		// Get the data
		HGLOBAL hGlobal = pDataObject->GetGlobalData(CF_UNICODETEXT);
		if (hGlobal != NULL)
		{
			LPCTSTR lpszText = (LPCTSTR)GlobalLock(hGlobal);
			if (lpszText != NULL)
			{
				// Process the dropped data (e.g., add to a list, display it)
				AfxMessageBox(CString(_T("CMFCDragDropFileView Dropped Text: ")) + lpszText);
				m_wndMyDialogPaneAC20.wsText = lpszText;
				m_wndMyDialogPaneAC20.UpdateData(FALSE);

				GlobalUnlock(hGlobal);
			}
			GlobalFree(hGlobal);
		}
		m_bDragging = FALSE;
		return TRUE;
	}
	if (pDataObject->IsDataAvailable(DRAGDROP_DATA_AC20))
	{
		HGLOBAL hGlobal = pDataObject->GetGlobalData(DRAGDROP_DATA_AC20);
		if (hGlobal != NULL)
		{
			DragDrop_Data_AC20* pAC20 = static_cast<DragDrop_Data_AC20*>(GlobalLock(hGlobal));

			if (pAC20 != NULL)
			{
				wchar_t   szText[64] = { 0 };
				wsprintf(szText, L"%d - %s", pAC20->deptNo, pAC20->deptName);

				// Process the dropped data (e.g., add to a list, display it)
				AfxMessageBox(CString(_T("CMFCDragDropFileView Dropped AC20: ")) + szText);
				GlobalUnlock(hGlobal);
			}
			GlobalFree(hGlobal);
		}
		m_bDragging = FALSE;
		return TRUE;
	}
	return FALSE;
}


// CMFCDragDropFileView drawing

void CMFCDragDropFileView::OnDraw(CDC* pDC)
{
	CMFCDragDropFileDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect  xRect(rect);

	for (auto& s : str) {
		pDC->DrawText(s, &xRect, DT_LEFT);

		xRect.top += 40;
		xRect.bottom += 40;
	}
}


// CMFCDragDropFileView printing


void CMFCDragDropFileView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCDragDropFileView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCDragDropFileView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCDragDropFileView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCDragDropFileView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCDragDropFileView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CMFCDragDropFileView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptDragStart = point;
	m_bDragging = TRUE;

	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
	wchar_t  xBuff[128] = { 0 };
	swprintf_s(xBuff, 128, L"CMFCDragDropFileView[%d]::OnLButtonDown()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	CView::OnLButtonDown(nFlags, point);
}

void CMFCDragDropFileView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging && (abs(point.x - m_ptDragStart.x) > 5 || abs(point.y - m_ptDragStart.y) > 5))
	{
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

		wchar_t  xBuff[128] = { 0 };
		swprintf_s(xBuff, 128, L"CMFCDragDropFileView[%d]::OnMouseMove() Start", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

		// Start the drag operation

		COleDataSource src;
		HGLOBAL hGlobal = NULL;

		int iData = 0;
		CRect xRect(rect);
		for (auto& x : str) {
			if (xRect.PtInRect(m_ptDragStart)) {
				break;
			}
			xRect.top += 40;
			xRect.bottom += 40;
			++iData;
		}

		// Call DoDragDrop
		DWORD dwDropEffect = DROPEFFECT_NONE;
		switch (iData) {
			case 0:
				{
					// Cache the data in CF_UNICODETEXT format
					hGlobal = GlobalAlloc(GMEM_MOVEABLE, 48 * sizeof(TCHAR));
					if (hGlobal == NULL) return;
					LPTSTR lpszText = (LPTSTR)GlobalLock(hGlobal);
					if (lpszText) {
#if 1
						m_wndMyDialogPaneAC20.UpdateData(TRUE);
						_tcscpy_s(lpszText, m_wndMyDialogPaneAC20.wsText.length() + 1, m_wndMyDialogPaneAC20.wsText.c_str());
#else
						CString sText = _T("Data being dragged");

						_tcscpy_s(lpszText, sText.GetLength() + 1, sText);
#endif
					}
					GlobalUnlock(hGlobal);
					src.CacheGlobalData(CF_UNICODETEXT, hGlobal);
					dwDropEffect = src.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);
				}
				break;

			case 1:
				{
					hGlobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(DragDrop_Data_AC20));
					if (hGlobal == NULL) return;
					DragDrop_Data_AC20* pAC20 = static_cast<DragDrop_Data_AC20 *>(GlobalLock(hGlobal));
					*pAC20 = m_Ac20Dummy;
					GlobalUnlock(hGlobal);
					src.CacheGlobalData(DRAGDROP_DATA_AC20, hGlobal);
					dwDropEffect = src.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);
				}
				break;

			case 2:
				break;
		}

		if (dwDropEffect == DROPEFFECT_NONE) {
			swprintf_s(xBuff, 128, L"CMFCDragDropFileView[%d]::OnMouseMove() dwDropEffect == DROPEFFECT_NONE", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		// Optional: Handle the result of the drop operation (e.g., delete source item if moved)
		if (dwDropEffect & DROPEFFECT_MOVE)
		{
			// Remove item from source view
			swprintf_s(xBuff, 128, L"CMFCDragDropFileView[%d]::OnMouseMove() dwDropEffect & DROPEFFECT_MOVE", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		if (dwDropEffect & DROPEFFECT_COPY)
		{
			swprintf_s(xBuff, 128, L"CMFCDragDropFileView[%d]::OnMouseMove() dwDropEffect & DROPEFFECT_COPY", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		m_bDragging = FALSE;
		swprintf_s(xBuff, 128, L"CMFCDragDropFileView[%d]::OnMouseMove() End", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
	}

	CView::OnMouseMove(nFlags, point);
}

// Don't forget OnLButtonUp to stop dragging if the button is released without moving enough
void CMFCDragDropFileView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
		wchar_t  xBuff[128] = { 0 };
		swprintf_s(xBuff, 128, L"CMFCDragDropFileView[%d]::OnLButtonUp()", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
	}

	m_bDragging = FALSE;

	CView::OnLButtonUp(nFlags, point);
}

// CMFCDragDropFileView diagnostics

#ifdef _DEBUG
void CMFCDragDropFileView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCDragDropFileView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCDragDropFileDoc* CMFCDragDropFileView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDragDropFileDoc)));
	return (CMFCDragDropFileDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCDragDropFileView message handlers
