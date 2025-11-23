
// MFCAnimationtestDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCAnimationtest.h"
#include "MFCAnimationtestDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCAnimationtestDlg dialog

CMFCAnimationtestDlg::CMFCAnimationtestDlg(CWnd* pParent /*=nullptr*/)
	: pScrollPane(nullptr), CDialogEx(IDD_MFCANIMATIONTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CMFCAnimationtestDlg::~CMFCAnimationtestDlg()
{
	if (pScrollPane) delete pScrollPane;
	pScrollPane = nullptr;
}

void CMFCAnimationtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	if (!pDX->m_bSaveAndValidate) {
		fadeInThread = std::thread(&CMFCAnimationtestDlg::FadeIn, this, GetDlgItem(1000));
	}
}

BEGIN_MESSAGE_MAP(CMFCAnimationtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


void CMFCAnimationtestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	char buffer[256] = { 0 };
	_snprintf_s(buffer, sizeof(buffer), "CMFCAnimationtestDlg::OnSize  nType %d  cx %d cy %d\n", nType, cx, cy);
	OutputDebugStringA(buffer);

	CRect rc;
	GetClientRect(&rc);
	_snprintf_s(buffer, sizeof(buffer), "CMFCAnimationtestDlg::GetClientRect  left %d  top %d   right %d  bottom %d\n", rc.left, rc.top, rc.right, rc.bottom);
	OutputDebugStringA(buffer);


	if (pPlaceHolder) {
		int cyTemp = cy;
		if (cyTemp > rcPlaceHolder.bottom) cyTemp = rcPlaceHolder.bottom;
		pPlaceHolder->GetClientRect(&rc);
		_snprintf_s(buffer, sizeof(buffer), "pPlaceHolder::GetClientRect  left %d  top %d   right %d  bottom %d  cyTemp %d\n", rc.left, rc.top, rc.right, rc.bottom, cyTemp);
		OutputDebugStringA(buffer);

		pPlaceHolder->SendMessage(WM_SIZE, nType, MAKELPARAM(cyTemp, rcPlaceHolder.right));

		pPlaceHolder->SetWindowPos(nullptr, 0, 0, rcPlaceHolder.right, cyTemp, SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOZORDER);
	}

	if (pScrollPane) {
		int cyTemp = cy;
		if (cyTemp > rcScrollPane.bottom) cyTemp = rcScrollPane.bottom;
		pScrollPane->GetClientRect(&rc);
		_snprintf_s(buffer, sizeof(buffer), "pScrollPane::GetClientRect  left %d  top %d   right %d  bottom %d  cyTemp %d\n", rc.left, rc.top, rc.right, rc.bottom, cyTemp);
		OutputDebugStringA(buffer);
		pScrollPane->SetWindowPos(nullptr, 0, 0, rcScrollPane.right, cyTemp, SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOZORDER);
		pScrollPane->SendMessage(WM_SIZE, nType, MAKELPARAM(cyTemp, rcScrollPane.right));
	}
}


void CMFCAnimationtestDlg::OnWindowPosChanging(WINDOWPOS * lpwndpos)
{
#if 0
	if (!m_bVisible)
	{
		lpwndpos->flags &= ~SWP_SHOWWINDOW; // Prevent the dialog from showing
	}
#endif
	WINDOWPOS  savePos = *lpwndpos;

	CDialog::OnWindowPosChanging(lpwndpos); // Call the base class handler

	char buffer[256] = { 0 };
	_snprintf_s(buffer, sizeof(buffer), "lpwndpos x %d, y %d  width %d height %d   flags0x%x\n", savePos.x, savePos.y, savePos.cx, savePos.cy, savePos.flags);
	OutputDebugStringA(buffer);

//	pScrollPane->SetWindowPos(NULL, 0, 0, savePos.cx, savePos.cy, savePos.flags);

}

void CMFCAnimationtestDlg::FadeIn(CWnd *ctl)
{
	::Sleep(500);
	m_bVisible = true;
//	ShowWindow(SW_SHOW);
	// Animate the window to fade in
//	ctl->AnimateWindow(2500, AW_BLEND); // 500ms fade duration
	ctl->AnimateWindow(2500, AW_BLEND); // 500ms fade duration
}

void CMFCAnimationtestDlg::FadeOut (CWnd* ctl)
{
	// Animate the window to fade in
	ctl->AnimateWindow(2500, AW_HIDE); // 500ms fade duration
}

void CMFCAnimationtestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	HRESULT result = animate.Move(forward);
}

void CMFCAnimationtestDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	HRESULT result = animate.Move(backward);
}

void CMFCAnimationtestDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	HRESULT result = animate.Move(stopForward);
}

void CMFCAnimationtestDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	HRESULT result = animate.Move(stopBackward);
}

// CMFCAnimationtestDlg message handlers

INT_PTR CALLBACK dlgFunc1002(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return FALSE;
}

BOOL CMFCAnimationtestDlg::OnInitDialog()
{
	static const WCHAR* filename = L"220Strawberry.png";

	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	animate.SetHwndSizeTileSpacing(m_hWnd, 500, 170.0f);

	animate.BeforeEnteringMessageLoop(filename);

	pPlaceHolder = GetDlgItem(IDD_MFCANIMATIONTEST_SCROLLPANE);
	if (pPlaceHolder) {
		pPlaceHolder->GetClientRect(&rcPlaceHolder);

		pScrollPane = new CScrollPane(this);
		pScrollPane->Create(IDD_MFCANIMATIONTEST_SCROLLPANE, this);
		pScrollPane->GetClientRect(&rcScrollPane);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCAnimationtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCAnimationtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCAnimationtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

