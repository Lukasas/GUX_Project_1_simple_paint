#pragma once

/*
 * Standard XToolkit and OSF/Motif include files.
 */
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <X11/Xmu/Editres.h>

/*
 * Public include files for widgets used in this file.
 */
#include <Xm/MainW.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/MessageB.h>
#include <Xm/MenuShell.h>
#include <Xm/CascadeB.h>

#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include "drawing.h"

class MyWindow
{
private:

	enum Tools
	{
		tLine,
		tRect,
		tPoint,
		tEllipse
	};
	struct _Current
	{
		int Color;
		int BkColor;
		Tools Tool;
		int Fill;
		int Size;
		int Style;
	} Current;

	struct ColorButtons
	{
		char colorname[50];
		long long colorvalue;

		ColorButtons(const char * name, long long value)
		{
			strcpy(colorname, name);
			colorname[49] = 0;
			colorvalue = value;
		}
	};

	using LineSize = ColorButtons;
	using ToolSel = ColorButtons;

	std::vector<ColorButtons*> m_Colors;
	std::vector<LineSize*> m_LineSizes;
	std::vector<ToolSel*> m_ToolsSel;

	XtAppContext m_appContext;
	Widget m_topLevel;
	Widget m_mainWindow;
	Widget m_frame;
	Widget m_drawingArea;
	Widget m_rowColumn;
	Widget m_rowColumn_0_status;
	Widget m_rowColumn_1_colors;
	Widget m_rowColumn_1_bkcolors;
	Widget m_rowColumn_2_tools;
	Widget m_rowColumn_3_line_size;
	Widget m_rowColumn_4_bar;
	Widget m_btnQuit;
	Widget m_btnClear;
	Widget m_btnColor;
	Widget m_Question;
	Widget m_Label;
	Widget m_topBar;
	Widget m_fileShell;
	Widget m_fileMenu;
	Widget m_menuFile;
	Widget m_menuOpen;
	Widget m_colorsMenu;
	Widget m_backColorsMenu;
	Widget m_toolsMenu;
	Widget m_fillMenu;
	Widget m_sizeMenu;
	Widget m_styleMenu;

	Atom WM_DELETE;

	Drawing *m_g = NULL;
	Controller<MyWindow> *m_c;

	int m_mouse_start_x;
	int m_mouse_start_y;

	void CreateMenu();

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
	void BtnChangeBkColor(Widget widget, XtPointer user_data, XtPointer call_data);
	void BtnSetTool(Widget widget, XtPointer user_data, XtPointer call_data);
	void BtnSetFill(Widget widget, XtPointer user_data, XtPointer call_data);

	void BtnSetLineSize(Widget widget, XtPointer user_data, XtPointer call_data);
	void BtnSetLineStyle(Widget widget, XtPointer user_data, XtPointer call_data);

	void QuitDialogResult(Widget widget, XtPointer user_data, XtPointer call_data);
	void QuitDialogShow(Widget widget, XtPointer user_data, XtPointer call_data);

	void StatusChange();

	virtual ~MyWindow();
};