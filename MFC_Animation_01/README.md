## Overview

This is a simple animation test program to see how to use the DirectCompositionWindowsAnimationManager example
from the Microsoft repository of https://github.com/microsoft/Windows-classic-samples within an MFC application.

This is a MFC dialog based test application in order to simplify the code a bit.

The basic procedure was as follows:
 - use the MFC Desktop template in Visual Studio 2019 to generate a dialog based application
 - copy over the source for creating and manipulating the animation of a simple .png file

The animation source was placed in a separate class file, `CAnimateWAM`, to create a class that could
then be dropped into the MFC template generated source code.

The `OnInitDialog()` function that is called as part of perform the `DoModal()` member function of the dialog
has the `CAnimateWAM` member functions to initialize the Windows Animation Manager environment. The dialog window handle
is needed for the WAM initialization and that is first available in the OnInitDialog() member function.

Button event handlers were added to the `BEGIN_MESSAGE_MAP(CMFCAnimationtestDlg, CDialogEx)` with a call to 
`animate.Move(dir);` with the appropriate `DIRECTION` value.

I also added to `CMFCAnimationtestApp::InitInstance()` the code to initialize COM needed for the WAM COM objects:
```
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    assert(SUCCEEDED(hr));
```

## Reading

[Windows Animation Overview](https://learn.microsoft.com/en-us/windows/win32/uianimation/scenic-animation-api-overview)

[DirectComposition](https://learn.microsoft.com/en-us/windows/win32/directcomp/directcomposition-portal)

