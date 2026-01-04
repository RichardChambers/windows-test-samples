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

**Technical Note**

This source is in a somewhat messy and chaotic state as it is an exploration of the mechanics needed to implement
OLE Drag and Drop for this context. For instance the `CDialogPane` class should be generalized to allow multiple
different dialog templates to be used by relying on the underlying `CDialogEx` methods such as `DoDataExchange()`
and stripping the class down to only provide the affordances allowing someone to derive their own class from
it and implement their own `OnDragEnter()`, `OnDrop()`, `OnLButtonDown()`, etc.
