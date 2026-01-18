#pragma once
#include "CDialogPane.h"

#include "DialogPane_AC20.h"


struct DragDrop_Data_AC68 {
	ULONG    pluNo;
	wchar_t  pluName[20];
	USHORT   deptNo;
	ULONG    ulStatus[6];
};


class DialogPane_AC68 : public CDialogDrop
{
	DECLARE_DYNAMIC(DialogPane_AC68)

protected:

	virtual int DropTypeValid(COleDataObject* pDataObject) {
		int iDropType = 0;
		unsigned long  ulMask = 0;
		
		ulMask |= pDataObject->IsDataAvailable(DRAGDROP_DATA_AC20) ? 0x01 : 0;
		ulMask |= pDataObject->IsDataAvailable(DRAGDROP_DATA_AC68) ? 0x02 : 0;

		switch (ulMask) {
		case 0x01:
			iDropType = DRAGDROP_DATA_AC20;
			break;
		case 0x02:
			iDropType = DRAGDROP_DATA_AC68;
			break;
		case 0x03:
			break;
		}
		
		return iDropType;
	}

	int    myIcount;

	virtual BOOL OnInitDialog();

	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

public:
	DialogPane_AC68(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DialogPane_AC68();

	void PutData(DragDrop_Data_AC68& x);
	void PutDataAC20(DragDrop_Data_AC20& x);
	void GetData(DragDrop_Data_AC68& x);

	DragDrop_Data_AC68  m_DataRecord;

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
