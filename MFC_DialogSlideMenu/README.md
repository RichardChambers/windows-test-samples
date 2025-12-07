## Overview

Working on the Animation test program, I wanted to have a vertical menu that would slide out. I took the source code
from the animation test program, trimmed out the animation part, and then worked on just the sliding menu.

Turns out that doing this was not easy in MFC however in the end I was able to get the window and mouse mechanics to work.

Along the way I took a look at a few other things such as accessing the client area of a CStatic dialog item and writing to it.

The main files modified have been CScrollPane and CSlidingMenuWin.

CScrollPane was about adding a scrolling window region in the dialog originally however I added the CStaticNotes and CWndNotes
classes for learning about how to make sub windows and other regions of an app's window to do various things.

I have found that dialog templates are actually rather versatile and can be used to create modal and non-modal dialogs as well
as being used to layout a CWnd surface. For instance I have used dialog templates with tab controls.

This uses an MFC DIALOG application model.
