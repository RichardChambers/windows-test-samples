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


class CDialogDrop : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogDrop)

protected:
	class PaneCOleDropTarget : public COleDropTarget
	{
	private:
		CDialogDrop* m_pDropWnd;

	public:
		PaneCOleDropTarget(CDialogDrop *pWnd = nullptr) : COleDropTarget(), m_pDropWnd(pWnd) { }

		BOOL Register(CWnd* pWnd)
		{
			ASSERT_VALID(this);
			return COleDropTarget::Register(pWnd);
		}

		BOOL RegisterDialogPane(CDialogDrop* pPane)
		{
			ASSERT_VALID(this);
			m_pDropWnd = pPane;
			return COleDropTarget::Register(pPane);
		}

		DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
		{
			ASSERT_VALID(this);
			BOOL b = pWnd->IsKindOf(RUNTIME_CLASS(CDialogDrop));
			if (m_pDropWnd && m_pDropWnd->m_hWnd == pWnd->m_hWnd)
				return m_pDropWnd->OnDragEnter(pDataObject, dwKeyState, point);
			else
				return DROPEFFECT_NONE;
		}

		DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
		{
			ASSERT_VALID(this);
			BOOL b = pWnd->IsKindOf(RUNTIME_CLASS(CDialogDrop));
			if (m_pDropWnd && m_pDropWnd->m_hWnd == pWnd->m_hWnd)
				return m_pDropWnd->OnDragOver(pDataObject, dwKeyState, point);
			else
				return DROPEFFECT_NONE;
		}

		void OnDragLeave(CWnd* pWnd)
		{
			ASSERT_VALID(this);
			BOOL b = pWnd->IsKindOf(RUNTIME_CLASS(CDialogDrop));
			if (m_pDropWnd && m_pDropWnd->m_hWnd == pWnd->m_hWnd)
				m_pDropWnd->OnDragLeave();
		}

		BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
		{
			ASSERT_VALID(this);
			BOOL b = pWnd->IsKindOf(RUNTIME_CLASS(CDialogDrop));
			if (m_pDropWnd && m_pDropWnd->m_hWnd == pWnd->m_hWnd)
				return m_pDropWnd->OnDrop(pDataObject, dropEffect, point);
			else
				return FALSE;
		}

#if 0
		// if this method is not provided then OnDrop() is called.
		DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
		{
			ASSERT_VALID(this);
			BOOL b = pWnd->IsKindOf(RUNTIME_CLASS(CDialogDrop));
			if (m_pDropWnd && m_pDropWnd->m_hWnd == pWnd->m_hWnd)
				return m_pDropWnd->OnDropEx(pDataObject, dropDefault, dropList, point);
			else
				return DROPEFFECT_NONE;
		}
#endif
	};

	PaneCOleDropTarget m_dropTarget;

	virtual BOOL OnInitDialog() {
		BOOL   bRet = TRUE;

		bRet = CDialogEx::OnInitDialog();

		if (!m_dropTarget.RegisterDialogPane(this))
		{
			TRACE(_T("Failed to register drop target\n"));
		}

		return bRet;
	}

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) { return DROPEFFECT_NONE; }
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) { return DROPEFFECT_NONE; }
	virtual void OnDragLeave() { }
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) { return FALSE; }
	virtual DROPEFFECT OnDropEx(COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point) { return DROPEFFECT_NONE; }

public:
	CDialogDrop(UINT nIDTemplate, CWnd* pParent = nullptr) : CDialogEx(nIDTemplate, pParent) {}   // standard constructor
	virtual ~CDialogDrop() {}

	DECLARE_MESSAGE_MAP()
};


class CDialogPane : public CDialogDrop
{
	DECLARE_DYNAMIC(CDialogPane)

protected:

	int   iDropType;
	CPoint m_ptDragStart;
	BOOL   m_bDragging = FALSE;

	enum class DraggingState { None = 0, Entering, Dragging, Leaving };

	DraggingState m_DraggingState;

	int    myIcount;

	virtual BOOL OnInitDialog();

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

public:
	CDialogPane(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogPane();

	int  DropType(int iType) { int iT = iDropType;  iDropType = iType; return iT; }

	void PutData(DragDrop_Data_AC20& x);
	void GetData(DragDrop_Data_AC20& x);

	USHORT   usDeptNo;
	std::wstring  wsText;
	bool  checkMark1;
	bool  checkMark2;

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

	DECLARE_MESSAGE_MAP()
};

