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
		PaneCOleDataSource();
		virtual ~PaneCOleDataSource();

		void InitFormats();

		// Overidables
		virtual BOOL OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal);
		virtual BOOL OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile);
		virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium);
		// OnRenderFileData and OnRenderGlobalData are called by
		//  the default implementation of OnRenderData.

		virtual BOOL OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
			BOOL bRelease);
		// used only in COleServerItem implementation

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

		TRACE(L"** CDialogDrop.DropTypeValid() not overriden.\n");
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


// Extending a standard dialog control type for special effects and behavior.
// See TN062: Message Reflection for Windows Controls.
//https://learn.microsoft.com/en-us/cpp/mfc/tn062-message-reflection-for-windows-controls?view=msvc-170

// Using this new control in a dialog requires a couple of changes from a standard control.
//  - change to the resource file
//  - registration of the new window class
//  - change to the DoDataExchange() logic

// Resource file directive
//         CONTROL         "", IDC_EDIT1, "YellowEdit", WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, 75, 31, 100, 12

// Register the Window class before the dialog is created.

// Properly initialize the user defined control in the DoDataExchange() function then set/get the value
//    	DDX_Control(pDX, IDC_EDIT1, m_wndYellowEdit);     // initialize the user define control
//      DDX_Text(pDX, IDC_EDIT1, csText);                 // perform data transfer

// The following define will allow use of an Edit box with a yellow background
// rather than a stadard Edit box.
// Setting this to 1 will include the source code changes needed.
// You can also search this defined constant to see the various changes needed.
#define USE_CYELLOWEDIT  0

#if defined(USE_CYELLOWEDIT) && USE_CYELLOWEDIT==1

class CYellowEdit : public CEdit
{
	DECLARE_DYNAMIC(CYellowEdit)

private:
	COLORREF  m_clrText;       // color for the text.
	COLORREF  m_clrBkgnd;      // color for the solid brush used to pain the background.
	CBrush    m_brBkgnd;       // solid brush for painting the background

public:
	CYellowEdit() : CEdit::CEdit()
	{

		m_clrText = RGB(0, 0, 0);          // color of the text. all zeros is black.
		m_clrBkgnd = RGB(255, 255, 255);     // color of the background. 
		m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
	}

	COLORREF  SetChangeColor(int iState) {
		COLORREF savedColor = m_clrBkgnd;

		switch (iState) {
		case 0:
			m_clrBkgnd = RGB(255, 255, 255);     // color of the background. 
			break;
		case 1:
			m_clrBkgnd = RGB(255, 255, 0);     // color of the background. 
			break;
		}

		m_brBkgnd.DeleteObject();
		m_brBkgnd.CreateSolidBrush(m_clrBkgnd);

		return savedColor;
	}

	static BOOL RegisterWindowClass() {
		// Code to be invoked at application startup to register the window
		// class for our new version of the Edit box control.
		// The new window class must be registered before it can be used
		// in a dialog template with the CONTROL resource directive.

		WNDCLASS wc;

		// Get info from the standard EDIT class to inherit its base behavior
		if (!::GetClassInfo(NULL, _T("EDIT"), &wc))
			return FALSE;

		// Modify the essential fields for your custom class
		wc.lpszClassName = _T("YellowEdit"); // Must match the CONTROL directive used in your resource file
		wc.hInstance = AfxGetInstanceHandle();

		// Register with MFC's helper to ensure automatic cleanup
		return AfxRegisterClass(&wc);
	}

	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor)
	{
		// TODO: Change any attributes of the DC here
		// TODO: Return a non-NULL brush if the
		//       parent's handler should not be called

		pDC->SetTextColor(m_clrText);   // color of the text
		pDC->SetBkColor(m_clrBkgnd);    // color of the text background
		return (HBRUSH)m_brBkgnd.GetSafeHandle();     // ctl bkgnd
	}

	DECLARE_MESSAGE_MAP()
};

#endif
