// CDialogPane.cpp : implementation file
//

#include "pch.h"
#include "MFC-DragDropFile.h"
#include "CDialogPane.h"
#include "afxdialogex.h"

#include "MainFrm.h"

#include <stdio.h>

// CDialogPane dialog

IMPLEMENT_DYNAMIC(CDialogDrop, CDialogEx)

BEGIN_MESSAGE_MAP(CDialogDrop, CDialogEx)
#if defined(ENABLE_WM_COPY_PASTE_OVERRIDE) && (ENABLE_WM_COPY_PASTE_OVERRIDE == 1)
    ON_WM_GETDLGCODE()
    ON_MESSAGE(WM_PASTE, OnPaste)
    ON_MESSAGE(WM_COPY, OnCopy)
#endif
END_MESSAGE_MAP()

// -----------------------------------
// replacement for the standard CEdit or "Edit" control 
#if defined(USE_CYELLOWEDIT) && USE_CYELLOWEDIT==1

IMPLEMENT_DYNAMIC(CYellowEdit, CEdit)

BEGIN_MESSAGE_MAP(CYellowEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
#endif
// ---------------------------------

#if defined(USE_CCOPYPASTEWND) && USE_CCOPYPASTEWND==1

IMPLEMENT_DYNAMIC(CCopyPasteWnd, CEdit)

// NOTE: Never send WM_SETFOCUS or WM_KILLFOCUS messages using ::SendMessage or ::PostMessage.
//       Instead, use the CWnd::SetFocus() member function(or the global::SetFocus Windows API function)
//       to actually change the keyboard focus.
//       Windows automatically sends the appropriate WM_KILLFOCUSand WM_SETFOCUS notifications
//       as a result of the SetFocus call.

BEGIN_MESSAGE_MAP(CCopyPasteWnd, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_MESSAGE(WM_PASTE, OnPaste)
    ON_MESSAGE(WM_COPY, OnCopy)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE() // Recommended to reset tracking state
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

#endif


// PaneCOleDataSource

CDialogDrop::PaneCOleDataSource::PaneCOleDataSource() : COleDataSource()
{

}

CDialogDrop::PaneCOleDataSource::~PaneCOleDataSource()
{

 }

// In your class or initialization logic
void CDialogDrop::PaneCOleDataSource::InitFormats()
{
    // Use for global memory (HGLOBAL) data
    DelayRenderData(CF_TEXT);

    // Use for file-based or large stream data
    DelayRenderFileData(RegisterClipboardFormat(_T("MyCustomFileData")));
}

	// Overidables
BOOL CDialogDrop::PaneCOleDataSource::OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal)
{
    if (lpFormatEtc->cfFormat == CF_UNICODETEXT)
    {
        HGLOBAL hData = ::GlobalAlloc(GMEM_MOVEABLE, 100);

        if (hData) {
            wchar_t* pData = (wchar_t *)::GlobalLock(hData);

            if (pData) {
                wcscpy_s(pData, 20, L"Global Data Content");
                ::GlobalUnlock(hData);
            }
        }

        *phGlobal = hData;
        return TRUE;
    }
    return COleDataSource::OnRenderGlobalData(lpFormatEtc, phGlobal);
}

BOOL CDialogDrop::PaneCOleDataSource::OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile)
{
    if (lpFormatEtc->cfFormat == RegisterClipboardFormat(_T("MyCustomFileData")))
    {
        pFile->Write("File-based stream content", 26);
        return TRUE;
    }
    return COleDataSource::OnRenderFileData(lpFormatEtc, pFile);
}

#if 0
// According to Gemini AI:
// 
// By default, COleDataSource::OnRenderData calls your specific overrides.
// You only need to override this directly if you want to handle TYMED_ISTORAGE
// or other advanced STGMEDIUM types manually.
BOOL CDialogDrop::PaneCOleDataSource::OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{

}
#endif

	// OnRenderFileData and OnRenderGlobalData are called by
	//  the default implementation of OnRenderData.

BOOL CDialogDrop::PaneCOleDataSource::OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
    // The base class will automatically route to OnRenderGlobalData 
    // or OnRenderFileData based on the lpStgMedium->tymed value.
    return COleDataSource::OnRenderData(lpFormatEtc, lpStgMedium);
}

BOOL CDialogDrop::PaneCOleDataSource::OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium, BOOL bRelease)
{
    // 1. Verify the format you are expecting
    if (lpFormatEtc->cfFormat == CF_UNICODETEXT)
    {
        // 2. Handle Global Memory Data
        if (lpStgMedium->tymed == TYMED_HGLOBAL)
        {
            HGLOBAL hData = lpStgMedium->hGlobal;
            if (hData) {
                wchar_t * pData = (wchar_t *)::GlobalLock(hData);
                if (pData != NULL)
                {
                    // Process your global data here
                    TRACE(L"Received Global Data: %s\n", pData);
                    ::GlobalUnlock(hData);

                    // If bRelease is TRUE, we own it; MFC will call ReleaseStgMedium 
                    // automatically if we return TRUE.
                    return TRUE;
                }

            }
        }
    }
    else if (lpFormatEtc->cfFormat == RegisterClipboardFormat(_T("MyCustomFileData")))
    {
        // 3. Handle File-based Data
        if (lpStgMedium->tymed == TYMED_FILE)
        {
            // Use CFile to read from the path provided in lpszFileName
            CFile file;
            if (file.Open(lpStgMedium->lpszFileName, CFile::modeRead))
            {
                // Read and process the file content
                file.Close();
                return TRUE;
            }
        }
    }

    // Fall back to base class for unsupported formats/mediums
    return COleDataSource::OnSetData(lpFormatEtc, lpStgMedium, bRelease);
}
	// used only in COleServerItem implementation
