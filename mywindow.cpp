#include "mywindow.h"

MyWindow::MyWindow(int argc, char *argv[]) : c(new Controller<MyWindow>())
{
	XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

	m_topLevel = XtVaAppInitialize(
		&m_appContext,
		"Draw",
		NULL, 0,
		&argc, argv,
		NULL,
		NULL);

	m_mainWindow = XtVaCreateManagedWidget(
		"mainWin",
		xmMainWindowWidgetClass,
		m_topLevel,
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		XmNwidth, 800,
		XmNheight, 600,
		NULL);
}

void MyWindow::RunLoop()
{
	XtRealizeWidget(m_topLevel);
	XtAppMainLoop(m_appContext);
}

MyWindow::~MyWindow()
{
	if (g)
		delete g;

	delete c;
}

void MyWindow::InitializeDrawing()
{
	g = new Drawing(m_drawingArea);
}

void MyWindow::InitComponents()
{
	m_frame = XtVaCreateManagedWidget(
		"frame",
		xmFrameWidgetClass,
		m_mainWindow,
		NULL);

	m_drawingArea = XtVaCreateManagedWidget(
		"drawingArea",
		xmDrawingAreaWidgetClass,
		m_frame,
		XmNwidth, 200,
		XmNheight, 100,
		NULL);

	m_rowColumn = XtVaCreateManagedWidget(
		"rowColumn",
		xmRowColumnWidgetClass,
		m_mainWindow,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_COLUMN,
		NULL
	);

    m_btnClear = XtVaCreateManagedWidget(
		"Clear",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL
	);

    m_btnQuit = XtVaCreateManagedWidget(
		"Quit",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL
	);

	XmMainWindowSetAreas(m_mainWindow, NULL, m_rowColumn, NULL, NULL, m_frame);

	c->RegisterCallback(this, m_btnClear, XmNactivateCallback, &MyWindow::ClearButton, NULL);
	c->RegisterCallback(this, m_btnQuit, XmNactivateCallback, &MyWindow::QuitButton, NULL);

	c->RegisterEventHandler(this, m_drawingArea, ButtonMotionMask, False, &MyWindow::ButtonEvent, NULL);

}

void MyWindow::ButtonEvent(Widget widget, XtPointer user_data, XEvent * event, Boolean * cont)
{
	g->DrawLine(10, 10, event->xmotion.x, event->xmotion.y);
}

void MyWindow::ClearButton(Widget w, XtPointer user_data, XtPointer call_data)
{
	g->Clear();
}

void MyWindow::QuitButton(Widget w, XtPointer user_data, XtPointer call_data)
{
	exit(0);
}