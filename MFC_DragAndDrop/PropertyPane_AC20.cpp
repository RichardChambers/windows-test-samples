#include "pch.h"
#include "PropertyPane_AC20.h"


IMPLEMENT_DYNAMIC(PropertyPane_AC20_1, CDialogTabPane)

PropertyPane_AC20_1::PropertyPane_AC20_1(CWnd* pParent /*=nullptr*/)
	: CDialogTabPane(IDD_DIALOG_AC20, pParent)
{
	std::memset(&m_DataRecord, 0, sizeof(m_DataRecord));
}

PropertyPane_AC20_1::~PropertyPane_AC20_1()
{
}

BOOL PropertyPane_AC20_1::OnInitDialog()
{
	BOOL   bRet = TRUE;

	bRet = CDialogTabPane::OnInitDialog();

	return bRet;
}

void PropertyPane_AC20_1::DoDataExchange(CDataExchange* pDX)
{
	CDialogTabPane::DoDataExchange(pDX);

}

void PropertyPane_AC20_1::PutData(PropertySheetData_AC20& x)
{
	m_DataRecord = x;

	UpdateData(FALSE);
}

void PropertyPane_AC20_1::GetData(PropertySheetData_AC20& x)
{
	UpdateData(TRUE);

	x = m_DataRecord;
}


BEGIN_MESSAGE_MAP(PropertyPane_AC20_1, CDialogTabPane)
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()



IMPLEMENT_DYNAMIC(PropertySheet_AC20, CDialogPropertySheet)

PropertySheet_AC20::PropertySheet_AC20(CWnd* pParent /*=nullptr*/)
	: CDialogPropertySheet(L"AC20 Property Sheet", pParent)
{
	std::memset(&m_DataRecord, 0, sizeof(m_DataRecord));
}

PropertySheet_AC20::~PropertySheet_AC20()
{
}

void PropertySheet_AC20::DoDataExchange(CDataExchange* pDX)
{
	CDialogPropertySheet::DoDataExchange(pDX);

}

void PropertySheet_AC20::PutData(PropertySheetData_AC20& x)
{
	m_DataRecord = x;

	UpdateData(FALSE);
}

void PropertySheet_AC20::GetData(PropertySheetData_AC20& x)
{
	UpdateData(TRUE);

	x = m_DataRecord;
}


BEGIN_MESSAGE_MAP(PropertySheet_AC20, CDialogPropertySheet)
END_MESSAGE_MAP()
