
// MFC-DragDropFileDoc.cpp : implementation of the CMFCDragDropFileDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFC-DragDropFile.h"
#endif

#include "MFC-DragDropFileDoc.h"

#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCDragDropFileDoc

IMPLEMENT_DYNCREATE(CMFCDragDropFileDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCDragDropFileDoc, CDocument)
END_MESSAGE_MAP()


// CMFCDragDropFileDoc construction/destruction

CMFCDragDropFileDoc::CMFCDragDropFileDoc() noexcept
{
	// TODO: add one-time construction code here

}

CMFCDragDropFileDoc::~CMFCDragDropFileDoc()
{
}

BOOL CMFCDragDropFileDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMFCDragDropFileDoc serialization

void CMFCDragDropFileDoc::Serialize(CArchive& ar)
{
	CMainFrame* p = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

	if (ar.IsStoring())
	{
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, L"Storing a file.");

		// TODO: add storing code here
	}
	else
	{
		p->WriteToOutputWindow(COutputWnd::WindowType::Build, L"loading a file.");

		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMFCDragDropFileDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMFCDragDropFileDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCDragDropFileDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCDragDropFileDoc diagnostics

#ifdef _DEBUG
void CMFCDragDropFileDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCDragDropFileDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCDragDropFileDoc commands
