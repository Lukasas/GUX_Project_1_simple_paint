#pragma once

/*
 * Standard XToolkit and OSF/Motif include files.
 */
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

/*
 * Public include files for widgets used in this file.
 */
#include <Xm/MainW.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>

#include <cstdlib>
#include "drawing.h"

class MyWindow
{
private:
	XtAppContext m_appContext;
	Widget m_topLevel;
	Widget m_mainWindow;
	Widget m_frame;
	Widget m_drawingArea;
	Widget m_rowColumn;
	Widget m_btnQuit;
	Widget m_btnClear;

	Drawing *g = NULL;
	Controller<MyWindow> *c;
public:
	MyWindow(int, char*[]);

	void InitComponents();

	void InitializeDrawing();

	void RunLoop();

	void QuitButton(Widget w, XtPointer user_data, XtPointer call_data);

	virtual ~MyWindow();
};