#pragma once

#include "CDialogPane.h"

struct PropertySheetData_AC20 {

};

class PropertyPane_AC20_1 : public CDialogTabPane
{
	DECLARE_DYNAMIC(PropertyPane_AC20_1)

	PropertySheetData_AC20  m_DataRecord;

protected:
	virtual BOOL OnInitDialog();


public:
	PropertyPane_AC20_1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PropertyPane_AC20_1();

	void PutData(PropertySheetData_AC20& x);

	void GetData(PropertySheetData_AC20& x);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

class PropertySheet_AC20 : public CDialogPropertySheet
{
	DECLARE_DYNAMIC(PropertySheet_AC20)

	PropertySheetData_AC20  m_DataRecord;

protected:

public:
	PropertySheet_AC20(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PropertySheet_AC20();

	void PutData(PropertySheetData_AC20& x);

	void GetData(PropertySheetData_AC20& x);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

