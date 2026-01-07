This folder contains an experiment with OLE Drag and Drop in a multi-document MFC C++ application.

The class `CDialogPane` which is derived from the MFC class `CDialogEx` is used to display a dialog
embedded into the MFC `CView` class that displays the document. Instead of a standard dialog template
using the standard dialog styles, we are instead using the `DS_CONTROL` style to turn the dialog
into a control.  See [What is the DS_CONTROL style for?](https://devblogs.microsoft.com/oldnewthing/20040730-00/?p=38293)
by Raymond Chen.

The `CView` class makes using `COleDropTarget` easy because the class offers the hooks for handling the `OnDrag()`,
`OnDrop()` and other methods the `COleDropTarget` class provides for making a window work with OLE Drag and Drop.

However the `CDialogEx` class is derived from the MFC class `CWnd` which does not have those affordances so we
have to derive a class from `COleDropTarget` to add a member variable that will then trigger the affordances.

**Notes**

See the CodeProject article [How to Implement Drag and Drop Between Your Program and Explorer](https://www.codeproject.com/articles/How-to-Implement-Drag-and-Drop-Between-Your-Progra#comments-section) for an explanation
about two ways to implement file drag and drop from Windows File Explorer to an application. The article 
contains a description of using OLE Drag and Drop to implement file drag and drop that provides a good overview
of the mechanics.

See the Microsoft Learn article [COleDropTarget Class](https://learn.microsoft.com/en-us/cpp/mfc/reference/coledroptarget-class?view=msvc-170) for a description of the drop target handling MFC class. There is also a Microsoft Learn article [COleDropSource Class](https://learn.microsoft.com/en-us/cpp/mfc/reference/coledropsource-class?view=msvc-170)
which describes the drop source handling MFC class.

The Microsoft Learn article [Accessing Run-Time Class Information](https://learn.microsoft.com/en-us/cpp/mfc/accessing-run-time-class-information?view=msvc-170) may be helpful for checking the correct MFC class. It can be
used with MFC classes as well as classes derived from MFC class that were derived from the MFC class `CObject` and used **DECLARE_DYNAMIC**.

**Technical Note**

This source is in a somewhat messy and chaotic state as it is an exploration of the mechanics needed to implement
OLE Drag and Drop for this context. For instance the `CDialogPane` class should be generalized to allow multiple
different dialog templates to be used by relying on the underlying `CDialogEx` methods such as `DoDataExchange()`
and stripping the class down to only provide the affordances allowing someone to derive their own class from
it and implement their own `OnDragEnter()`, `OnDrop()`, `OnLButtonDown()`, etc.
