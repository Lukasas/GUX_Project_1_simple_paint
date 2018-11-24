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

	// m_rowColumn_1 = XtVaCreateManagedWidget(
	// 	"rowColumn",
	// 	xmRowColumnWidgetClass,
	// 	m_rowColumn,
	// 	XmNentryAlignment, XmALIGNMENT_CENTER,
	// 	XmNorientation, XmHORIZONTAL,
	// 	XmNpacking, XmPACK_COLUMN,
	// 	NULL
	// );

	// m_rowColumn_2 = XtVaCreateManagedWidget(
	// 	"rowColumn",
	// 	xmRowColumnWidgetClass,
	// 	m_rowColumn,
	// 	XmNentryAlignment, XmALIGNMENT_CENTER,
	// 	XmNorientation, XmHORIZONTAL,
	// 	XmNpacking, XmPACK_COLUMN,
	// 	NULL
	// );

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

	c->RegisterCallback(this, m_drawingArea, XmNinputCallback, &MyWindow::ButtonCallTest, NULL);
	c->RegisterEventHandler(this, m_drawingArea, ButtonMotionMask, False, &MyWindow::ButtonEvent, NULL);

}

void MyWindow::ButtonCallTest(Widget widget, XtPointer user_data, XtPointer call_data)
{
	XmDrawingAreaCallbackStruct *ds = (XmDrawingAreaCallbackStruct*)call_data;
	XButtonEvent * be = (XButtonEvent*)ds->event;

	if (be->button == Button1)
	{
		if (be->type == ButtonPress)
		{
			m_mouse_start_x = be->x,
			m_mouse_start_y = be->y;
		}


		if (be->type == ButtonRelease)
			g->ToolDone();
	}

}

void MyWindow::ButtonEvent(Widget widget, XtPointer user_data, XEvent * event, Boolean * cont)
{
	int x = event->xmotion.x;
	int y = event->xmotion.y;
	//g->DrawLine(m_mouse_start_x, m_mouse_start_y, event->xmotion.x, event->xmotion.y);
	// g->DrawPoint(x, y);
	// g->DrawLine(m_mouse_start_x, m_mouse_start_y, x, y);
	g->FillRectangle(m_mouse_start_x, m_mouse_start_y, x, y);
}

void MyWindow::ClearButton(Widget widget, XtPointer user_data, XtPointer call_data)
{
	g->Clear();
}

void MyWindow::QuitButton(Widget widget, XtPointer user_data, XtPointer call_data)
{
	exit(0);
}