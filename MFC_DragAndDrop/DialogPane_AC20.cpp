#include "pch.h"
#include "MFC-DragDropFile.h"
#include "DialogPane_AC20.h"
#include "afxdialogex.h"

#include "MainFrm.h"

#include <stdio.h>

int  DialogPane_AC20::iCount = 0;

IMPLEMENT_DYNAMIC(DialogPane_AC20, CDialogDrop)

DialogPane_AC20::DialogPane_AC20(CWnd* pParent /*=nullptr*/)
	: CDialogDrop(IDD_DIALOG_AC20, pParent), iDropType(DRAGDROP_DATA_AC20), m_DraggingState(DraggingState::None),
	myIcount(++iCount)
{
	std::memset(&m_DataRecord, 0, sizeof(m_DataRecord));
}

DialogPane_AC20::~DialogPane_AC20()
{
}

BOOL DialogPane_AC20::OnInitDialog()
{
	BOOL   bRet = TRUE;

	bRet = CDialogDrop::OnInitDialog();

	return bRet;
}

void DialogPane_AC20::DoDataExchange(CDataExchange* pDX)
{
	CDialogDrop::DoDataExchange(pDX);

	int  iDeptNo = m_DataRecord.deptNo;
	CString csText(m_DataRecord.deptName);
	int iCheckMark1 = (m_DataRecord.ulStatus[0] & 0x01) != 0;
	int iCheckMark2 = (m_DataRecord.ulStatus[0] & 0x02) != 0;

	DDX_Text(pDX, IDC_EDIT2, iDeptNo);
	DDX_Text(pDX, IDC_EDIT1, csText);
	DDX_Check(pDX, IDC_CHECK1, iCheckMark1);
	DDX_Check(pDX, IDC_CHECK2, iCheckMark2);

	if (pDX->m_bSaveAndValidate) {
		m_DataRecord.ulStatus[0] &= ~(ULONG)0x01;
		m_DataRecord.ulStatus[0] &= ~(ULONG)0x02;

		wcscpy_s(m_DataRecord.deptName, 20, (LPCWSTR)csText);
		m_DataRecord.deptNo = iDeptNo;
		m_DataRecord.ulStatus[0] |= (iCheckMark1) ? 0x01 : 0x00;
		m_DataRecord.ulStatus[0] |= (iCheckMark2) ? 0x02 : 0x00;
	}
}

void DialogPane_AC20::PutData(DragDrop_Data_AC20& x)
{
	m_DataRecord = x;

	UpdateData(FALSE);
}

void DialogPane_AC20::GetData(DragDrop_Data_AC20& x)
{
	UpdateData(TRUE);

	x = m_DataRecord;
}


BEGIN_MESSAGE_MAP(DialogPane_AC20, CDialogDrop)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()


// CDialogPane message handlers

// Override these functions to provide feedback and handle the drop:
DROPEFFECT DialogPane_AC20::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
	wchar_t xBuff[128] = { 0 };

	swprintf_s(xBuff, 128, L"    DialogPane_AC20[%d]::OnDragEnter()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	m_DraggingState = DraggingState::Entering;

	// Check if the data format is acceptable (e.g., CF_TEXT)
	if (pDataObject->IsDataAvailable(iDropType))
		return DROPEFFECT_COPY; // Show copy cursor
	else
		return DROPEFFECT_NONE; // Show "not allowed" cursor
}

DROPEFFECT DialogPane_AC20::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (m_DraggingState != DraggingState::Dragging) {
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
		wchar_t xBuff[128] = { 0 };

		swprintf_s(xBuff, 128, L"    DialogPane_AC20[%d]::OnDragOver()", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

		m_DraggingState = DraggingState::Dragging;
	}

	// Same check as OnDragEnter to provide continuous feedback
	if (pDataObject->IsDataAvailable(iDropType))
		return DROPEFFECT_COPY; // Show copy cursor
	else
		return DROPEFFECT_NONE;
}

void DialogPane_AC20::OnDragLeave()
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	wchar_t xBuff[128] = { 0 };

	swprintf_s(xBuff, 128, L"    DialogPane_AC20[%d]::OnDragLeave()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	m_DraggingState = DraggingState::Leaving;

}

BOOL DialogPane_AC20::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	wchar_t xBuff[128] = { 0 };

	if (iDropType == DRAGDROP_DATA_AC20 && pDataObject->IsDataAvailable(iDropType))
	{
		swprintf_s(xBuff, 128, L"    DialogPane_AC20[%d]::OnDrop() DRAGDROP_DATA_AC20", myIcount);
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
				AfxMessageBox(CString(_T("DialogPane_AC20 Dropped DRAGDROP_DATA_AC20: ")) + szText);
				GlobalUnlock(hGlobal);
			}
			GlobalFree(hGlobal);
		}
		m_bDragging = FALSE;
		return TRUE;
	}
	return FALSE;
}


void DialogPane_AC20::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptDragStart = point;
	m_bDragging = TRUE;

	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
	wchar_t  xBuff[128] = { 0 };
	swprintf_s(xBuff, 128, L"DialogPane_AC20[%d]::OnLButtonDown()", myIcount);
	p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);

	CDialogDrop::OnLButtonDown(nFlags, point);
}

void DialogPane_AC20::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging && (abs(point.x - m_ptDragStart.x) > 5 || abs(point.y - m_ptDragStart.y) > 5))
	{
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

		wchar_t  xBuff[128] = { 0 };
		swprintf_s(xBuff, 128, L"DialogPane_AC20[%d]::OnMouseMove() Start", myIcount);
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
			swprintf_s(xBuff, 128, L"DialogPane_AC20[%d]::OnMouseMove() dwDropEffect == DROPEFFECT_NONE", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		// Optional: Handle the result of the drop operation (e.g., delete source item if moved)
		if (dwDropEffect & DROPEFFECT_MOVE)
		{
			// Remove item from source view
			swprintf_s(xBuff, 128, L"DialogPane_AC20[%d]::OnMouseMove() dwDropEffect & DROPEFFECT_MOVE", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		if (dwDropEffect & DROPEFFECT_COPY)
		{
			swprintf_s(xBuff, 128, L"DialogPane_AC20[%d]::OnMouseMove() dwDropEffect & DROPEFFECT_COPY", myIcount);
			p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
		}

		m_bDragging = FALSE;
		swprintf_s(xBuff, 128, L"DialogPane_AC20[%d]::OnMouseMove() End", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
	}

	CDialogDrop::OnMouseMove(nFlags, point);
}

// Don't forget OnLButtonUp to stop dragging if the button is released without moving enough
void DialogPane_AC20::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
		wchar_t  xBuff[128] = { 0 };
		swprintf_s(xBuff, 128, L"DialogPane_AC20[%d]::OnLButtonUp()", myIcount);
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, xBuff);
	}

	m_bDragging = FALSE;

	CDialogDrop::OnLButtonUp(nFlags, point);
}
