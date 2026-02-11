#include "pch.h"

#include "MFC-DragDropFile.h"
#include "DialogPane_AC68.h"
#include "afxdialogex.h"

#include "MainFrm.h"

#include <stdio.h>

int  DialogPane_AC68::iCount = 0;

IMPLEMENT_DYNAMIC(DialogPane_AC68, CDialogDrop)

DialogPane_AC68::DialogPane_AC68(CWnd* pParent /*=nullptr*/)
	: CDialogDrop(IDD_DIALOG_AC68, pParent), myIcount(++iCount)
{
	std::memset(&m_DataRecord, 0, sizeof(m_DataRecord));
}

DialogPane_AC68::~DialogPane_AC68()
{
}

BOOL DialogPane_AC68::OnInitDialog()
{
	BOOL   bRet = TRUE;

	bRet = CDialogDrop::OnInitDialog();

	return bRet;
}

void DialogPane_AC68::DoDataExchange(CDataExchange* pDX)
{
	CDialogDrop::DoDataExchange(pDX);

	int  iPluNo = m_DataRecord.pluNo;
	CString csText(m_DataRecord.pluName);
	int  iDeptNo = m_DataRecord.deptNo;
	int iCheckMark1 = (m_DataRecord.ulStatus[0] & 0x01) != 0;
	int iCheckMark2 = (m_DataRecord.ulStatus[0] & 0x02) != 0;
	int iRadioMark1 = (m_DataRecord.ulStatus[1] & 0x01);

	DDX_Text(pDX, IDC_EDIT1, iPluNo);
	DDX_Text(pDX, IDC_EDIT2, csText);
	DDX_Check(pDX, IDC_CHECK1, iCheckMark1);
	DDX_Check(pDX, IDC_CHECK2, iCheckMark2);
	DDX_Text(pDX, IDC_EDIT3, iDeptNo);
	DDX_Radio(pDX, IDC_RADIO1, iRadioMark1);
#if defined(USE_CCOPYPASTEWND) && USE_CCOPYPASTEWND==1
	DDX_Control(pDX, IDC_COPYPASTE, m_wndCopyPaste);  // initialize the user defined control before using it.
#endif

	if (pDX->m_bSaveAndValidate) {
		m_DataRecord.ulStatus[0] &= ~(ULONG)0x01;
		m_DataRecord.ulStatus[0] &= ~(ULONG)0x02;
		m_DataRecord.ulStatus[1] &= ~(ULONG)0x01;

		wcscpy_s(m_DataRecord.pluName, 20, (LPCWSTR)csText);
		m_DataRecord.pluNo = iPluNo;
		m_DataRecord.deptNo = iDeptNo;
		m_DataRecord.ulStatus[0] |= (iCheckMark1) ? 0x01 : 0x00;
		m_DataRecord.ulStatus[0] |= (iCheckMark2) ? 0x02 : 0x00;
		m_DataRecord.ulStatus[1] |= iRadioMark1;
	}
}

void DialogPane_AC68::PutData(DragDrop_Data_AC68& x)
{
	m_DataRecord = x;

	UpdateData(FALSE);
}

void DialogPane_AC68::PutDataAC20(DragDrop_Data_AC20& x)
{
	m_DataRecord.deptNo = x.deptNo;
	m_DataRecord.ulStatus[0] = x.ulStatus[0];
	m_DataRecord.ulStatus[1] = x.ulStatus[1];

	UpdateData(FALSE);
}


void DialogPane_AC68::GetData(DragDrop_Data_AC68& x)
{
	UpdateData(TRUE);

	x = m_DataRecord;
}


BEGIN_MESSAGE_MAP(DialogPane_AC68, CDialogDrop)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE() // Recommended to reset tracking state
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()


// CDialogPane message handlers

// Override these functions to provide feedback and handle the drop:
DROPEFFECT DialogPane_AC68::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
	p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"    DialogPane_AC68[%d]::OnDragEnter()", myIcount);

	m_DraggingState = DraggingState::Entering;

	// Check if the data format is acceptable (e.g., CF_TEXT)
	if (DropTypeValid(pDataObject))
		return DROPEFFECT_COPY; // Show copy cursor
	else
		return DROPEFFECT_NONE; // Show "not allowed" cursor
}

DROPEFFECT DialogPane_AC68::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (m_DraggingState != DraggingState::Dragging) {
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
		p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"    DialogPane_AC68[%d]::OnDragOver()", myIcount);

		m_DraggingState = DraggingState::Dragging;
	}

	// Same check as OnDragEnter to provide continuous feedback
	if (DropTypeValid(pDataObject))
		return DROPEFFECT_COPY; // Show copy cursor
	else
		return DROPEFFECT_NONE;
}

void DialogPane_AC68::OnDragLeave()
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"    DialogPane_AC68[%d]::OnDragLeave()", myIcount);

	m_DraggingState = DraggingState::Leaving;

}

BOOL DialogPane_AC68::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	switch (DropTypeValid(pDataObject)) {

		case DRAGDROP_DATA_AC20:
		{
			p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"    DialogPane_AC68[%d]::OnDrop() DRAGDROP_DATA_AC20", myIcount);

			HGLOBAL hGlobal = pDataObject->GetGlobalData(DRAGDROP_DATA_AC20);
			if (hGlobal != NULL)
			{
				DragDrop_Data_AC20* pAC20 = static_cast<DragDrop_Data_AC20*>(GlobalLock(hGlobal));

				if (pAC20 != NULL)
				{
					wchar_t   szText[64] = { 0 };
					wsprintf(szText, L"%d - %s", pAC20->deptNo, pAC20->deptName);

					PutDataAC20(*pAC20);
					UpdateData(FALSE);    // update the displayed data

					// Process the dropped data (e.g., add to a list, display it)
					AfxMessageBox(CString(_T("DialogPane_AC68 Dropped DRAGDROP_DATA_AC20: ")) + szText);
					GlobalUnlock(hGlobal);
				}
				GlobalFree(hGlobal);
			}
			m_bDragging = FALSE;
			return TRUE;
		}

		case DRAGDROP_DATA_AC68:
		{
			p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"    DialogPane_AC68[%d]::OnDrop() DRAGDROP_DATA_AC68", myIcount);

			HGLOBAL hGlobal = pDataObject->GetGlobalData(DRAGDROP_DATA_AC68);
			if (hGlobal != NULL)
			{
				DragDrop_Data_AC68* pAC68 = static_cast<DragDrop_Data_AC68*>(GlobalLock(hGlobal));

				if (pAC68 != NULL)
				{
					wchar_t   szText[64] = { 0 };
					wsprintf(szText, L"%d - %s", pAC68->pluNo, pAC68->pluName);

					PutData(*pAC68);
					UpdateData(FALSE);    // update the displayed data

					// Process the dropped data (e.g., add to a list, display it)
					AfxMessageBox(CString(_T("DialogPane_AC68 Dropped DRAGDROP_DATA_AC68: ")) + szText);
					GlobalUnlock(hGlobal);
				}
				GlobalFree(hGlobal);
			}
			m_bDragging = FALSE;
			return TRUE;
		}
	}
	return FALSE;
}


void DialogPane_AC68::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptDragStart = point;
	m_bDragging = TRUE;

	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
	p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"DialogPane_AC68[%d]::OnLButtonDown()", myIcount);

	CDialogDrop::OnLButtonDown(nFlags, point);
}

void DialogPane_AC68::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTracking && !m_bDragging)
	{
		m_bHovering = FALSE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE; // Request both hover and leave
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT; // Standard system hover time (400ms)

		if (::TrackMouseEvent(&tme))
		{
			m_bMouseTracking = TRUE;
		}
	}

	if (m_bDragging && (abs(point.x - m_ptDragStart.x) > 5 || abs(point.y - m_ptDragStart.y) > 5))
	{
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

		p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"DialogPane_AC68[%d]::OnMouseMove() Start", myIcount);

		// Start the drag operation

		COleDataSource src;
		HGLOBAL hGlobal = NULL;

		// Call DoDragDrop
		DWORD dwDropEffect = DROPEFFECT_NONE;
		{
			hGlobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(DragDrop_Data_AC68));
			ASSERT(hGlobal);
			if (hGlobal) {

				DragDrop_Data_AC68* pAC68 = static_cast<DragDrop_Data_AC68*>(GlobalLock(hGlobal));

				UpdateData(TRUE);
				if (pAC68) GetData(*pAC68);

				GlobalUnlock(hGlobal);
				src.CacheGlobalData(DRAGDROP_DATA_AC68, hGlobal);
				dwDropEffect = src.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);
			}
		}

		if (dwDropEffect == DROPEFFECT_NONE) {
			p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"DialogPane_AC68[%d]::OnMouseMove() dwDropEffect == DROPEFFECT_NONE", myIcount);
		}

		// Optional: Handle the result of the drop operation (e.g., delete source item if moved)
		if (dwDropEffect & DROPEFFECT_MOVE)
		{
			// Remove item from source view
			p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"DialogPane_AC68[%d]::OnMouseMove() dwDropEffect & DROPEFFECT_MOVE", myIcount);
		}

		if (dwDropEffect & DROPEFFECT_COPY)
		{
			p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"DialogPane_AC68[%d]::OnMouseMove() dwDropEffect & DROPEFFECT_COPY", myIcount);
		}

		m_bDragging = FALSE;
		p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"DialogPane_AC68[%d]::OnMouseMove() End", myIcount);
	}

	CDialogDrop::OnMouseMove(nFlags, point);
}

// Don't forget OnLButtonUp to stop dragging if the button is released without moving enough
void DialogPane_AC68::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
		p->WriteFormattedToOutput(COutputWnd::WindowType::Build, L"DialogPane_AC68[%d]::OnLButtonUp()", myIcount);
	}

	m_bDragging = FALSE;

	CDialogDrop::OnLButtonUp(nFlags, point);
}
