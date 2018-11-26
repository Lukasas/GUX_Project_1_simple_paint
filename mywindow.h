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
	Widget m_rowColumn_1;
	Widget m_rowColumn_2;
	Widget m_btnQuit;
	Widget m_btnClear;
	Widget m_btnColor;

	Drawing *g = NULL;
	Controller<MyWindow> *c;


	int m_mouse_start_x;
	int m_mouse_start_y;

	enum Tools
	{
		tLine,
		tRect,
		tPoint,
		tEllipse
	};

	Tools tool;

	int fill;
public:
	MyWindow(int, char*[]);

	void InitComponents();

	void InitializeDrawing();

	void RunLoop();

	void ClearButton(Widget widget, XtPointer user_data, XtPointer call_data);
	void QuitButton(Widget widget, XtPointer user_data, XtPointer call_data);

	void ButtonCallTest(Widget widget, XtPointer user_data, XtPointer call_data);
	void ButtonEvent(Widget widget, XtPointer user_data, XEvent * event, Boolean * cont);

	void BtnChangeColor(Widget widget, XtPointer user_data, XtPointer call_data);
	void BtnSetTool(Widget widget, XtPointer user_data, XtPointer call_data);
	void BtnSetFill(Widget widget, XtPointer user_data, XtPointer call_data);

	virtual ~MyWindow();
};