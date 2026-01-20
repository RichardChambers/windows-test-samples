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

	int    myIcount;

	virtual int DropTypeValid(COleDataObject* pDataObject) {
		int iDropType = 0;

		if(pDataObject->IsDataAvailable(DRAGDROP_DATA_AC20)) iDropType = DRAGDROP_DATA_AC20;

		return iDropType;
	}

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

#if defined(USE_CYELLOWEDIT) && USE_CYELLOWEDIT==1
	CYellowEdit m_wndYellowEdit;
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

	DECLARE_MESSAGE_MAP()
};

