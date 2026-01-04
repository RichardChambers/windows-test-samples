// CDialogPane.cpp : implementation file
//

#include "pch.h"
#include "MFC-DragDropFile.h"
#include "CDialogPane.h"
#include "afxdialogex.h"

#include "MainFrm.h"

#include <stdio.h>

// CDialogPane dialog

int  CDialogPane::iCount = 0;

IMPLEMENT_DYNAMIC(CDialogPane, CDialogEx)

CDialogPane::CDialogPane(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent), myIcount(++iCount), checkMark1(0), checkMark2(0)
{

}

CDialogPane::~CDialogPane()
{
}

BOOL CDialogPane::OnInitDialog()
{
	BOOL   bRet = TRUE;

	bRet = CDialogEx::OnInitDialog();

	if (!m_dropTarget.RegisterDialogPane(this))
	{
		TRACE(_T("Failed to register drop target\n"));
	}

	return bRet;
}

void CDialogPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	CString csText(wsText.c_str());
	int iCheckMark1 = checkMark1, iCheckMark2 = checkMark2;

	DDX_Text(pDX, IDC_EDIT1, csText);
	DDX_Check(pDX, IDC_CHECK1, iCheckMark1);
	DDX_Check(pDX, IDC_CHECK2, iCheckMark2);

	wsText = csText;
	checkMark1 = iCheckMark1;  checkMark2 = iCheckMark2;
}

void CDialogPane::PutData(DragDrop_Data_AC20& x)
{
	wsText = x.deptName;
	checkMark1 = x.ulStatus[0];
	checkMark2 = x.ulStatus[1];

	UpdateData(FALSE);
}

void CDialogPane::GetData(DragDrop_Data_AC20& x)
{
	UpdateData(TRUE);

	wcscpy_s(x.deptName, 20, wsText.c_str());
	x.ulStatus[0] = checkMark1;
	x.ulStatus[1] = checkMark2;
}


BEGIN_MESSAGE_MAP(CDialogPane, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()


// CDialogPane message handlers

// Override these functions to provide feedback and handle the drop:
DROPEFFECT CDialogPane::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
	wchar_t xBuff[128] = { 0 };

	swprintf_s(xBuff, 128, L"    CDialogPane[%d]::OnDragEnter()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	m_DraggingState = DraggingState::Entering;

	// Check if the data format is acceptable (e.g., CF_TEXT)
	if (pDataObject->IsDataAvailable(iDropType))
		return DROPEFFECT_COPY; // Show copy cursor
	else
		return DROPEFFECT_NONE; // Show "not allowed" cursor
}

DROPEFFECT CDialogPane::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (m_DraggingState != DraggingState::Dragging) {
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
		wchar_t xBuff[128] = { 0 };

		swprintf_s(xBuff, 128, L"    CDialogPane[%d]::OnDragOver()", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

		m_DraggingState = DraggingState::Dragging;
	}

	// Same check as OnDragEnter to provide continuous feedback
	if (pDataObject->IsDataAvailable(iDropType))
		return DROPEFFECT_COPY; // Show copy cursor
	else
		return DROPEFFECT_NONE;
}

void CDialogPane::OnDragLeave()
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	wchar_t xBuff[128] = { 0 };

	swprintf_s(xBuff, 128, L"    CDialogPane[%d]::OnDragLeave()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	m_DraggingState = DraggingState::Leaving;

}

BOOL CDialogPane::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	wchar_t xBuff[128] = { 0 };

	if (iDropType == DRAGDROP_DATA_AC20 && pDataObject->IsDataAvailable(iDropType))
	{
		swprintf_s(xBuff, 128, L"    CDialogPane[%d]::OnDrop() DRAGDROP_DATA_AC20", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

		HGLOBAL hGlobal = pDataObject->GetGlobalData(DRAGDROP_DATA_AC20);
		if (hGlobal != NULL)
		{
			DragDrop_Data_AC20* pAC20 = static_cast<DragDrop_Data_AC20*>(GlobalLock(hGlobal));

			if (pAC20 != NULL)
			{
				wchar_t   szText[64] = { 0 };
				wsprintf(szText, L"%d - %s", pAC20->deptNo, pAC20->deptName);

				PutData(*pAC20);
				UpdateData(FALSE);    // update the displayed data

				// Process the dropped data (e.g., add to a list, display it)
				AfxMessageBox(CString(_T("CDialogPane Dropped AC20: ")) + szText);
				GlobalUnlock(hGlobal);
			}
			GlobalFree(hGlobal);
		}
		m_bDragging = FALSE;
		return TRUE;
	}
	return FALSE;
}


void CDialogPane::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptDragStart = point;
	m_bDragging = TRUE;

	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
	wchar_t  xBuff[128] = { 0 };
	swprintf_s(xBuff, 128, L"CDialogPane[%d]::OnLButtonDown()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDialogPane::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging && (abs(point.x - m_ptDragStart.x) > 5 || abs(point.y - m_ptDragStart.y) > 5))
	{
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

		wchar_t  xBuff[128] = { 0 };
		swprintf_s(xBuff, 128, L"CDialogPane[%d]::OnMouseMove() Start", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

		// Start the drag operation

		COleDataSource src;
		HGLOBAL hGlobal = NULL;

		// Call DoDragDrop
		DWORD dwDropEffect = DROPEFFECT_NONE;
		switch (iDropType) {
			case DRAGDROP_DATA_AC20:
			{
				hGlobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(DragDrop_Data_AC20));
				ASSERT(hGlobal);
				if (hGlobal == NULL) return;
				DragDrop_Data_AC20* pAC20 = static_cast<DragDrop_Data_AC20*>(GlobalLock(hGlobal));

				UpdateData(TRUE);
				if (pAC20) GetData(*pAC20);

				GlobalUnlock(hGlobal);
				src.CacheGlobalData(DRAGDROP_DATA_AC20, hGlobal);
				dwDropEffect = src.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);
			}
			break;
		}

		if (dwDropEffect == DROPEFFECT_NONE) {
			swprintf_s(xBuff, 128, L"CDialogPane[%d]::OnMouseMove() dwDropEffect == DROPEFFECT_NONE", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		// Optional: Handle the result of the drop operation (e.g., delete source item if moved)
		if (dwDropEffect & DROPEFFECT_MOVE)
		{
			// Remove item from source view
			swprintf_s(xBuff, 128, L"CDialogPane[%d]::OnMouseMove() dwDropEffect & DROPEFFECT_MOVE", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		if (dwDropEffect & DROPEFFECT_COPY)
		{
			swprintf_s(xBuff, 128, L"CDialogPane[%d]::OnMouseMove() dwDropEffect & DROPEFFECT_COPY", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		m_bDragging = FALSE;
		swprintf_s(xBuff, 128, L"CDialogPane[%d]::OnMouseMove() End", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

// Don't forget OnLButtonUp to stop dragging if the button is released without moving enough
void CDialogPane::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
		wchar_t  xBuff[128] = { 0 };
		swprintf_s(xBuff, 128, L"CDialogPane[%d]::OnLButtonUp()", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
	}

	m_bDragging = FALSE;

	CDialogEx::OnLButtonUp(nFlags, point);
}

