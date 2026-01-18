#pragma once

#include <afxole.h>

#include <string>

// CDialogPane dialog

// Drag and Drop data types follow. These indicate
// the type of data that is being dragged allowing us
// to drag different types of data.
#define DRAGDROP_DATA_AC20      CF_PRIVATEFIRST
#define DRAGDROP_DATA_AC68      (DRAGDROP_DATA_AC20 + 1)

class CDialogDrop : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogDrop)

protected:
	class PaneCOleDataSource : public COleDataSource
	{
	public:
		PaneCOleDataSource() : COleDataSource() { }
		virtual ~PaneCOleDataSource() {}

		// Overidables
		virtual BOOL OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal);
		virtual BOOL OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile);
		virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium);
		// OnRenderFileData and OnRenderGlobalData are called by
		//  the default implementation of OnRenderData.

		virtual BOOL OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
			BOOL bRelease);
		// used only in COleServerItem implementation

		DECLARE_MESSAGE_MAP()
	};

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

	CPoint m_ptDragStart;
	BOOL   m_bDragging = FALSE;

	enum class DraggingState { None = 0, Entering, Dragging, Leaving };

	DraggingState m_DraggingState;

	virtual int DropTypeValid(COleDataObject* pDataObject) {
		int iDropType = 0;

		return iDropType;
	}

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
	CDialogDrop(UINT nIDTemplate, CWnd* pParent = nullptr) : CDialogEx(nIDTemplate, pParent), m_DraggingState(DraggingState::None) {}   // standard constructor
	virtual ~CDialogDrop() {}

	DECLARE_MESSAGE_MAP()
};

