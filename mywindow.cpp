#include "mywindow.h"

MyWindow::MyWindow(int argc, char *argv[]) : c(new Controller<MyWindow>())
{
	XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

	// Closing question
	char *fall[] = {
		"*question.dialogTitle: Malá otázka",
		"*question.messageString: Konec aplikace?",
		"*question.okLabelString: Ano",
		"*question.cancelLabelString: Ne",
		"*question.messageAlignment: XmALIGNMENT_CENTER",
		NULL};

	m_topLevel = XtVaAppInitialize(
		&m_appContext,
		"Draw",
		NULL, 0,
		&argc, argv,
		fall,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);

	m_Question = XmCreateQuestionDialog(
		m_topLevel,
		"question",
		NULL,
		0);

	XtVaSetValues(
		m_Question,
		XmNdialogStyle,
		XmDIALOG_FULL_APPLICATION_MODAL,
		NULL);

	XtUnmanageChild(XmMessageBoxGetChild(m_Question, XmDIALOG_HELP_BUTTON));
	c->RegisterCallback(this, m_Question, XmNokCallback, &MyWindow::QuitDialogResult, (XtPointer)1);
	//c->RegisterCallback(this, m_Question, XmNcancelCallback, &MyWindow::QuitDialogResult, (XtPointer)0);

	m_mainWindow = XtVaCreateManagedWidget(
		"mainWin",
		xmMainWindowWidgetClass,
		m_topLevel,
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		XmNwidth, 800,
		XmNheight, 600,
		NULL);

	WM_DELETE = XInternAtom(XtDisplay(m_topLevel), "WM_DELETE_WINDOW", False);
  	c->RegisterWMProtocolCallback(this, m_topLevel, WM_DELETE, &MyWindow::QuitDialogShow, NULL);
	XmActivateWMProtocol(m_topLevel, WM_DELETE);
	XtAddEventHandler(m_topLevel, 0, True, _XEditResCheckMessages, NULL);
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
		NULL);

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

	m_btnColor = XtVaCreateManagedWidget(
		"Red",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnChangeColor, (XtPointer)0xff0000);

	m_btnColor = XtVaCreateManagedWidget(
		"Green",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnChangeColor, (XtPointer)0xff00);

	m_btnColor = XtVaCreateManagedWidget(
		"Blue",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnChangeColor, (XtPointer)0xff);

	m_btnColor = XtVaCreateManagedWidget(
		"Black",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnChangeColor, (XtPointer)0x0);

	m_btnColor = XtVaCreateManagedWidget(
		"White",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnChangeColor, (XtPointer)0xffffff);

	m_btnColor = XtVaCreateManagedWidget(
		"Line",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetTool, (XtPointer)Tools::tLine);

	m_btnColor = XtVaCreateManagedWidget(
		"Rect",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetTool, (XtPointer)Tools::tRect);

	m_btnColor = XtVaCreateManagedWidget(
		"Point",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetTool, (XtPointer)Tools::tPoint);

	m_btnColor = XtVaCreateManagedWidget(
		"Ellipse",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetTool, (XtPointer)Tools::tEllipse);

	m_btnColor = XtVaCreateManagedWidget(
		"Fill",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetFill, (XtPointer)1);

	m_btnColor = XtVaCreateManagedWidget(
		"No Fill",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);
	c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetFill, (XtPointer)0);

	m_btnClear = XtVaCreateManagedWidget(
		"Clear",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);

	m_btnQuit = XtVaCreateManagedWidget(
		"Quit",
		xmPushButtonWidgetClass,
		m_rowColumn,
		NULL);

	XmMainWindowSetAreas(m_mainWindow, NULL, m_rowColumn, NULL, NULL, m_frame);

	c->RegisterCallback(this, m_btnClear, XmNactivateCallback, &MyWindow::ClearButton, NULL);
	c->RegisterCallback(this, m_btnQuit, XmNactivateCallback, &MyWindow::QuitButton, NULL);

	c->RegisterCallback(this, m_drawingArea, XmNinputCallback, &MyWindow::ButtonCallTest, NULL);
	c->RegisterEventHandler(this, m_drawingArea, ButtonMotionMask, False, &MyWindow::ButtonEvent, NULL);
}

void MyWindow::ButtonCallTest(Widget widget, XtPointer user_data, XtPointer call_data)
{
	XmDrawingAreaCallbackStruct *ds = (XmDrawingAreaCallbackStruct *)call_data;
	XButtonEvent *be = (XButtonEvent *)ds->event;

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

void MyWindow::ButtonEvent(Widget widget, XtPointer user_data, XEvent *event, Boolean *cont)
{
	int x = event->xmotion.x;
	int y = event->xmotion.y;

	switch (tool)
	{
	case tLine:
		g->DrawLine(m_mouse_start_x, m_mouse_start_y, x, y);
		break;
	case tRect:
		if (fill)
			g->FillRectangle(m_mouse_start_x, m_mouse_start_y, x, y);
		else
			g->DrawRectangle(m_mouse_start_x, m_mouse_start_y, x, y);
		break;
	case tEllipse:
		if (fill)
			g->FillEllipse(m_mouse_start_x, m_mouse_start_y, x, y);
		else
			g->DrawEllipse(m_mouse_start_x, m_mouse_start_y, x, y);
		break;
	case tPoint:
		g->DrawPoint(x, y);

		break;
	}
}

void MyWindow::BtnChangeColor(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	float R = ((aa & 0xFF0000) >> 16) / 0xFF;
	float G = ((aa & 0xFF00) >> 8) / 0xFF;
	float B = (aa & 0xFF) / 0xFF;

	g->SetPenColor(R, G, B);
}

void MyWindow::BtnSetTool(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	tool = (Tools)aa;
}

void MyWindow::BtnSetFill(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	fill = aa;
}

void MyWindow::ClearButton(Widget widget, XtPointer user_data, XtPointer call_data)
{
	g->Clear();
}

void MyWindow::QuitDialogResult(Widget widget, XtPointer user_data, XtPointer call_data)
{
	exit(0);
}

void MyWindow::QuitDialogShow(Widget widget, XtPointer user_data, XtPointer call_data)
{
	XtManageChild(m_Question);
}

void MyWindow::QuitButton(Widget widget, XtPointer user_data, XtPointer call_data)
{
	QuitDialogShow(widget, user_data, call_data);
}