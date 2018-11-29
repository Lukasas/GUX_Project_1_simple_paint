#include "mywindow.h"

MyWindow::MyWindow(int argc, char *argv[]) : m_c(new Controller<MyWindow>())
{
	m_Colors = std::vector<ColorButtons *>{
		new ColorButtons("Red", 0xff0000),
		new ColorButtons("Green", 0xff00),
		new ColorButtons("Blue", 0xff),
		new ColorButtons("Black", 0x0),
		new ColorButtons("White", 0xffffff),
	};

	m_LineSizes = std::vector<LineSize *>{
		new LineSize("Line - 0", 0),
		new LineSize("Line - 1", 1),
		new LineSize("Line - 3", 3),
		new LineSize("Line - 8", 8),
		new LineSize("Line - 10", 10),
	};

	m_ToolsSel = std::vector<ToolSel *>{
		new ToolSel("Line", Tools::tLine),
		new ToolSel("Rect", Tools::tRect),
		new ToolSel("Point", Tools::tPoint),
		new ToolSel("Ellipse", Tools::tEllipse),
	};

	XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);

	// Closing question
	char *fall[] = {
		"*question.dialogTitle: Quitting",
		"*question.messageString: Are you sure you would like to exit?",
		"*question.okLabelString: Yes",
		"*question.cancelLabelString: No",
		"*question.messageAlignment: XmALIGNMENT_CENTER",
		"*bar.file.labelString: File",
		"*bar.color.labelString: Colors",
		"*bar.backcolor.labelString: Back Colors",
		"*bar.tools.labelString: Tools",
		"*bar.fill.labelString: Fill",
		"*bar.size.labelString: Sizes",
		"*bar.style.labelString: Styles",
		0};

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
	m_c->RegisterCallback(this, m_Question, XmNokCallback, &MyWindow::QuitDialogResult, (XtPointer)1);
	//m_c->RegisterCallback(this, m_Question, XmNcancelCallback, &MyWindow::QuitDialogResult, (XtPointer)0);

	m_mainWindow = XtVaCreateManagedWidget(
		"mainWin",
		xmMainWindowWidgetClass,
		m_topLevel,
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		XmNwidth, 800,
		XmNheight, 600,
		NULL);

	CreateMenu();

	WM_DELETE = XInternAtom(XtDisplay(m_topLevel), "WM_DELETE_WINDOW", False);
	m_c->RegisterWMProtocolCallback(this, m_topLevel, WM_DELETE, &MyWindow::QuitDialogShow, NULL);
	XmActivateWMProtocol(m_topLevel, WM_DELETE);
	XtAddEventHandler(m_topLevel, 0, True, _XEditResCheckMessages, NULL);

	Current.Color = 0;
	Current.BkColor = 0;
	Current.Tool = Tools::tLine;
	Current.Fill = 0;
	Current.Size = 0;
	Current.Style = LineSolid;
}

void MyWindow::RunLoop()
{
	XtRealizeWidget(m_topLevel);
	XtAppMainLoop(m_appContext);
}

void MyWindow::CreateMenu()
{
	m_topBar = XtVaCreateManagedWidget(
		"bar",
		xmRowColumnWidgetClass,
		m_mainWindow,
		XmNrowColumnType,
		XmMENU_BAR,
		NULL);

	m_fileShell = XtVaCreatePopupShell(
		"file_shell",
		xmMenuShellWidgetClass,
		m_topBar,
		XmNwidth, 5,
		XmNheight, 5,
		XmNallowShellResize,
		True,
		XtNoverrideRedirect,
		True,
		NULL);

	m_fileMenu = XtVaCreateWidget(
		"file_menu",
		xmRowColumnWidgetClass,
		m_fileShell,
		XmNrowColumnType,
		XmMENU_PULLDOWN,
		NULL);

	m_menuFile = XtVaCreateManagedWidget(
		"file",
		xmCascadeButtonWidgetClass,
		m_topBar,
		XmNsubMenuId,
		m_fileMenu,
		NULL);

	m_colorsMenu = XtVaCreateWidget(
		"file_menu",
		xmRowColumnWidgetClass,
		m_fileShell,
		XmNrowColumnType,
		XmMENU_PULLDOWN,
		NULL);
	m_menuFile = XtVaCreateManagedWidget(
		"color",
		xmCascadeButtonWidgetClass,
		m_topBar,
		XmNsubMenuId,
		m_colorsMenu,
		NULL);

	m_backColorsMenu = XtVaCreateWidget(
		"file_menu",
		xmRowColumnWidgetClass,
		m_fileShell,
		XmNrowColumnType,
		XmMENU_PULLDOWN,
		NULL);
	m_menuFile = XtVaCreateManagedWidget(
		"backcolor",
		xmCascadeButtonWidgetClass,
		m_topBar,
		XmNsubMenuId,
		m_backColorsMenu,
		NULL);

	m_toolsMenu = XtVaCreateWidget(
		"file_menu",
		xmRowColumnWidgetClass,
		m_fileShell,
		XmNrowColumnType,
		XmMENU_PULLDOWN,
		NULL);
	m_menuFile = XtVaCreateManagedWidget(
		"tools",
		xmCascadeButtonWidgetClass,
		m_topBar,
		XmNsubMenuId,
		m_toolsMenu,
		NULL);

	m_fillMenu = XtVaCreateWidget(
		"file_menu",
		xmRowColumnWidgetClass,
		m_fileShell,
		XmNrowColumnType,
		XmMENU_PULLDOWN,
		NULL);
	m_menuFile = XtVaCreateManagedWidget(
		"fill",
		xmCascadeButtonWidgetClass,
		m_topBar,
		XmNsubMenuId,
		m_fillMenu,
		NULL);

	m_sizeMenu = XtVaCreateWidget(
		"file_menu",
		xmRowColumnWidgetClass,
		m_fileShell,
		XmNrowColumnType,
		XmMENU_PULLDOWN,
		NULL);
	m_menuFile = XtVaCreateManagedWidget(
		"size",
		xmCascadeButtonWidgetClass,
		m_topBar,
		XmNsubMenuId,
		m_sizeMenu,
		NULL);

	m_styleMenu = XtVaCreateWidget(
		"file_menu",
		xmRowColumnWidgetClass,
		m_fileShell,
		XmNrowColumnType,
		XmMENU_PULLDOWN,
		NULL);
	m_menuFile = XtVaCreateManagedWidget(
		"style",
		xmCascadeButtonWidgetClass,
		m_topBar,
		XmNsubMenuId,
		m_styleMenu,
		NULL);

	m_menuOpen = XtVaCreateManagedWidget(
		"Clear",
		xmPushButtonWidgetClass,
		m_fileMenu,
		NULL);
	m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::ClearButton, (XtPointer)0);

	m_menuOpen = XtVaCreateManagedWidget(
		"Quit",
		xmPushButtonWidgetClass,
		m_fileMenu,
		NULL);

	m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::QuitDialogShow, (XtPointer)0);
}

MyWindow::~MyWindow()
{
	if (m_g)
		delete m_g;

	if (m_c)
		delete m_c;

	ColorButtons *temp = NULL;
	while (m_Colors.size() > 0)
	{
		temp = m_Colors.back();
		delete temp;
		m_Colors.pop_back();
	}

	while (m_LineSizes.size() > 0)
	{
		temp = m_LineSizes.back();
		delete temp;
		m_LineSizes.pop_back();
	}

	while (m_ToolsSel.size() > 0)
	{
		temp = m_ToolsSel.back();
		delete temp;
		m_ToolsSel.pop_back();
	}

	temp = NULL;
}

void MyWindow::InitializeDrawing()
{
	m_g = new Drawing(m_drawingArea);
	m_g->SetLineWidthStyle(Current.Size, Current.Style);
	m_g->SetPenColor(m_Colors[Current.Style]->colorvalue);
	m_g->SetBrushColor(1, 0, 0);
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
		XmNorientation, XmVERTICAL,
		XmNpacking, XmPACK_COLUMN,
		NULL);

	m_rowColumn_0_status = XtVaCreateManagedWidget(
		"rowColumn",
		xmRowColumnWidgetClass,
		m_rowColumn,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_COLUMN,
		NULL);

	m_rowColumn_1_colors = XtVaCreateManagedWidget(
		"rowColumn",
		xmRowColumnWidgetClass,
		m_rowColumn,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_COLUMN,
		NULL);

	m_rowColumn_1_bkcolors = XtVaCreateManagedWidget(
		"rowColumn",
		xmRowColumnWidgetClass,
		m_rowColumn,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_COLUMN,
		NULL);

	m_rowColumn_2_tools = XtVaCreateManagedWidget(
		"rowColumn",
		xmRowColumnWidgetClass,
		m_rowColumn,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_COLUMN,
		NULL);

	m_rowColumn_3_line_size = XtVaCreateManagedWidget(
		"rowColumn",
		xmRowColumnWidgetClass,
		m_rowColumn,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_COLUMN,
		NULL);

	m_rowColumn_4_bar = XtVaCreateManagedWidget(
		"rowColumn",
		xmRowColumnWidgetClass,
		m_rowColumn,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_COLUMN,
		NULL);

	m_Label = XtVaCreateManagedWidget(
		"label",
		xmLabelWidgetClass,
		m_rowColumn_1_colors,
		NULL);
	XmString Output = XmStringCreate((char *)"Color: ", XmFONTLIST_DEFAULT_TAG);
	XtVaSetValues(m_Label, XmNlabelString, Output, NULL);
	XmStringFree(Output);

	m_Label = XtVaCreateManagedWidget(
		"label",
		xmLabelWidgetClass,
		m_rowColumn_1_bkcolors,
		NULL);
	Output = XmStringCreate((char *)"Back Color: ", XmFONTLIST_DEFAULT_TAG);
	XtVaSetValues(m_Label, XmNlabelString, Output, NULL);
	XmStringFree(Output);

	m_Label = XtVaCreateManagedWidget(
		"label",
		xmLabelWidgetClass,
		m_rowColumn_0_status,
		NULL);

	for (unsigned long long i = 0; i < m_Colors.size(); i++)
	{
		m_btnColor = XtVaCreateManagedWidget(
			m_Colors[i]->colorname,
			xmPushButtonWidgetClass,
			m_rowColumn_1_colors,
			NULL);
		m_c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnChangeColor, (XtPointer)i);

		m_menuOpen = XtVaCreateManagedWidget(
			m_Colors[i]->colorname,
			xmPushButtonWidgetClass,
			m_colorsMenu,
			NULL);
		m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::BtnChangeColor, (XtPointer)i);
	}

	for (unsigned long long i = 0; i < m_Colors.size(); i++)
	{
		m_btnColor = XtVaCreateManagedWidget(
			m_Colors[i]->colorname,
			xmPushButtonWidgetClass,
			m_rowColumn_1_bkcolors,
			NULL);
		m_c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnChangeBkColor, (XtPointer)i);

		m_menuOpen = XtVaCreateManagedWidget(
			m_Colors[i]->colorname,
			xmPushButtonWidgetClass,
			m_backColorsMenu,
			NULL);
		m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::BtnChangeBkColor, (XtPointer)i);
	}

	for (unsigned long long i = 0; i < m_ToolsSel.size(); i++)
	{
		m_btnColor = XtVaCreateManagedWidget(
			m_ToolsSel[i]->colorname,
			xmPushButtonWidgetClass,
			m_rowColumn_2_tools,
			NULL);
		m_c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetTool, (XtPointer)i);

		m_menuOpen = XtVaCreateManagedWidget(
			m_ToolsSel[i]->colorname,
			xmPushButtonWidgetClass,
			m_toolsMenu,
			NULL);
		m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::BtnSetTool, (XtPointer)i);
	}

	m_btnColor = XtVaCreateManagedWidget(
		"Fill",
		xmPushButtonWidgetClass,
		m_rowColumn_2_tools,
		NULL);
	m_c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetFill, (XtPointer)1);

	m_menuOpen = XtVaCreateManagedWidget(
		"Fill",
		xmPushButtonWidgetClass,
		m_fillMenu,
		NULL);
	m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::BtnSetFill, (XtPointer)1);

	m_btnColor = XtVaCreateManagedWidget(
		"No Fill",
		xmPushButtonWidgetClass,
		m_rowColumn_2_tools,
		NULL);
	m_c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetFill, (XtPointer)0);

	m_menuOpen = XtVaCreateManagedWidget(
		"No Fill",
		xmPushButtonWidgetClass,
		m_fillMenu,
		NULL);
	m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::BtnSetFill, (XtPointer)0);

	m_btnColor = XtVaCreateManagedWidget(
		"Solid",
		xmPushButtonWidgetClass,
		m_rowColumn_2_tools,
		NULL);
	m_c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetLineStyle, (XtPointer)LineSolid);

	m_menuOpen = XtVaCreateManagedWidget(
		"Solid",
		xmPushButtonWidgetClass,
		m_styleMenu,
		NULL);
	m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::BtnSetLineStyle, (XtPointer)LineSolid);

	m_btnColor = XtVaCreateManagedWidget(
		"DoubleDash",
		xmPushButtonWidgetClass,
		m_rowColumn_2_tools,
		NULL);
	m_c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetLineStyle, (XtPointer)LineDoubleDash);

	m_menuOpen = XtVaCreateManagedWidget(
		"DoubleDash",
		xmPushButtonWidgetClass,
		m_styleMenu,
		NULL);
	m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::BtnSetLineStyle, (XtPointer)LineDoubleDash);

	for (unsigned long long i = 0; i < m_LineSizes.size(); i++)
	{
		m_btnColor = XtVaCreateManagedWidget(
			m_LineSizes[i]->colorname,
			xmPushButtonWidgetClass,
			m_rowColumn_3_line_size,
			NULL);
		m_c->RegisterCallback(this, m_btnColor, XmNactivateCallback, &MyWindow::BtnSetLineSize, (XtPointer)i);

		m_menuOpen = XtVaCreateManagedWidget(
			m_LineSizes[i]->colorname,
			xmPushButtonWidgetClass,
			m_sizeMenu,
			NULL);
		m_c->RegisterCallback(this, m_menuOpen, XmNactivateCallback, &MyWindow::BtnSetLineSize, (XtPointer)i);
	}

	m_btnClear = XtVaCreateManagedWidget(
		"Clear",
		xmPushButtonWidgetClass,
		m_rowColumn_4_bar,
		NULL);

	m_btnQuit = XtVaCreateManagedWidget(
		"Quit",
		xmPushButtonWidgetClass,
		m_rowColumn_4_bar,
		NULL);

	XmMainWindowSetAreas(m_mainWindow, NULL, m_rowColumn, NULL, NULL, m_frame);

	m_c->RegisterCallback(this, m_btnClear, XmNactivateCallback, &MyWindow::ClearButton, NULL);
	m_c->RegisterCallback(this, m_btnQuit, XmNactivateCallback, &MyWindow::QuitButton, NULL);

	m_c->RegisterCallback(this, m_drawingArea, XmNinputCallback, &MyWindow::ButtonCallTest, NULL);
	m_c->RegisterEventHandler(this, m_drawingArea, ButtonMotionMask, False, &MyWindow::ButtonEvent, NULL);

	StatusChange();
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
			if (Current.Tool == Tools::tPoint)
			{
				if (Current.Size > 0)
				{
					int fixS = (m_LineSizes[Current.Size]->colorvalue / 4);
					m_g->FillEllipse(m_mouse_start_x - fixS, m_mouse_start_y - fixS, m_mouse_start_x + fixS, m_mouse_start_y + fixS);
					m_g->ToolDone();
				}
				else
					m_g->DrawPoint(m_mouse_start_x, m_mouse_start_y);
			}
		}

		if (be->type == ButtonRelease)
			m_g->ToolDone();
	}
}

void MyWindow::ButtonEvent(Widget widget, XtPointer user_data, XEvent *event, Boolean *cont)
{
	int x = event->xmotion.x;
	int y = event->xmotion.y;

	switch (m_ToolsSel[Current.Tool]->colorvalue)
	{
	case tLine:
		m_g->DrawLine(m_mouse_start_x, m_mouse_start_y, x, y);
		break;
	case tRect:
		if (Current.Fill)
			m_g->FillRectangle(m_mouse_start_x, m_mouse_start_y, x, y);
		else
			m_g->DrawRectangle(m_mouse_start_x, m_mouse_start_y, x, y);
		break;
	case tEllipse:
		if (Current.Fill)
			m_g->FillEllipse(m_mouse_start_x, m_mouse_start_y, x, y);
		else
			m_g->DrawEllipse(m_mouse_start_x, m_mouse_start_y, x, y);
		break;
	case tPoint:
		if (Current.Size > 0)
		{
			int fixS = (m_LineSizes[Current.Size]->colorvalue / 4);
			m_g->FillEllipse(x - fixS, y - fixS, x + fixS, y + fixS);
			m_g->ToolDone();
		}
		else
			m_g->DrawPoint(x, y);

		break;
	}
}

void MyWindow::BtnChangeColor(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	Current.Color = aa;
	m_g->SetPenColor(m_Colors[Current.Color]->colorvalue);
	StatusChange();
}
void MyWindow::BtnChangeBkColor(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	Current.BkColor = aa;
	m_g->SetBrushColor(m_Colors[Current.BkColor]->colorvalue);
	StatusChange();
}

void MyWindow::BtnSetTool(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	Current.Tool = (Tools)aa;
	StatusChange();
}

void MyWindow::BtnSetFill(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	Current.Fill = aa;
	StatusChange();
}

void MyWindow::BtnSetLineSize(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	Current.Size = aa;
	m_g->SetLineWidthStyle(m_LineSizes[Current.Size]->colorvalue, Current.Style);
	StatusChange();
}

void MyWindow::BtnSetLineStyle(Widget widget, XtPointer user_data, XtPointer call_data)
{
	int *parser = (int *)user_data;
	long long aa = (long long)parser;
	Current.Style = aa;
	m_g->SetLineWidthStyle(m_LineSizes[Current.Size]->colorvalue, Current.Style);
	StatusChange();
}

void MyWindow::ClearButton(Widget widget, XtPointer user_data, XtPointer call_data)
{
	m_g->Clear();
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

void MyWindow::StatusChange()
{
	std::string out =
		"Color: " + std::string(m_Colors[Current.Color]->colorname) +
		" | Back Color: " + std::string(m_Colors[Current.BkColor]->colorname) +
		" | Tool: " + std::string(m_ToolsSel[Current.Tool]->colorname) +
		" | Fill: " + (Current.Fill ? "Fill" : "No Fill") +
		" | Size: " + std::string(m_LineSizes[Current.Size]->colorname) +
		" | Style: " + (Current.Style == LineDoubleDash ? "Double Dash" : "Solid");

	XmString Output = XmStringCreate((char *)out.c_str(), XmFONTLIST_DEFAULT_TAG);
	XtVaSetValues(m_Label, XmNlabelString, Output, NULL);
	XmStringFree(Output);
}