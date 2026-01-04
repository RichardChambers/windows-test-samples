#pragma once

#include <afxole.h>

#include <string>

// CDialogPane dialog

#if !defined(DRAGDROP_DATA_AC20)

// Drag and Drop data types follow. These indicate
// the type of data that is being dragged allowing us
// to drag different types of data.
#define DRAGDROP_DATA_AC20   CF_PRIVATEFIRST


struct DragDrop_Data_AC20 {
	USHORT   deptNo;
	wchar_t  deptName[20];
	ULONG    ulStatus[6];
};

#endif

class CDialogPane : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogPane)

protected:
	class MyCOleDropTarget : public COleDropTarget
	{
	private:
		CDialogPane* p;

	public:
		MyCOleDropTarget() : COleDropTarget(), p(nullptr) {}

		BOOL Register(CWnd* pWnd)
		{
			ASSERT_VALID(this);
			return COleDropTarget::Register(pWnd);
		}

		BOOL RegisterDialogPane(CDialogPane* pPane)
		{
			ASSERT_VALID(this);
			p = pPane;
			return COleDropTarget::Register(pPane);
		}

		DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
		{
			ASSERT_VALID(this);
			if (p && p->m_hWnd == pWnd->m_hWnd)
				return p->OnDragEnter(pDataObject, dwKeyState, point);
			else
				return COleDropTarget::OnDragEnter(pWnd, pDataObject, dwKeyState, point);
		}

		DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
		{
			ASSERT_VALID(this);
			if (p && p->m_hWnd == pWnd->m_hWnd)
				return p->OnDragOver(pDataObject, dwKeyState, point);
			else
				return COleDropTarget::OnDragOver(pWnd, pDataObject, dwKeyState, point);
		}

		void OnDragLeave(CWnd* pWnd)
		{
			ASSERT_VALID(this);
			if (p && p->m_hWnd == pWnd->m_hWnd)
				return p->OnDragLeave();
			else
				return COleDropTarget::OnDragLeave(pWnd);
		}

		BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
		{
			ASSERT_VALID(this);
			if (p && p->m_hWnd == pWnd->m_hWnd)
				return p->OnDrop(pDataObject, dropEffect, point);
			else
				return COleDropTarget::OnDrop(pWnd, pDataObject, dropEffect, point);
		}
	};

	int   iDropType;
	MyCOleDropTarget m_dropTarget;
	CPoint m_ptDragStart;
	BOOL   m_bDragging = FALSE;

	enum class DraggingState { None = 0, Entering, Dragging, Leaving };

	DraggingState m_DraggingState;

	int    myIcount;

	virtual BOOL OnInitDialog();

	DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	void OnDragLeave();
	BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

public:
	CDialogPane(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogPane();

	int  DropType(int iType) { int iT = iDropType;  iDropType = iType; return iT; }

	void PutData(DragDrop_Data_AC20& x);
	void GetData(DragDrop_Data_AC20& x);

	std::wstring  wsText;
	bool  checkMark1;
	bool  checkMark2;

	static int iCount;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
