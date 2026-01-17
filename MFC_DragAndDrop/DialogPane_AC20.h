#pragma once
#include "CDialogPane.h"


struct DragDrop_Data_AC20 {
	USHORT   deptNo;
	wchar_t  deptName[20];
	ULONG    ulStatus[6];
};



class DialogPane_AC20 : public CDialogDrop
{
	DECLARE_DYNAMIC(DialogPane_AC20)

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
	DialogPane_AC20(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DialogPane_AC20();

	void PutData(DragDrop_Data_AC20& x);
	void GetData(DragDrop_Data_AC20& x);

	DragDrop_Data_AC20  m_DataRecord;

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

