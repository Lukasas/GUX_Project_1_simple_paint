#include "drawing.h"

Drawing::Drawing(Widget w) : m_c(new Controller<Drawing>())
{
	m_drawingArea = w;
	m_gc = XCreateGC(XtDisplay(w), RootWindowOfScreen(XtScreen(m_drawingArea)), 0, NULL);
	m_colMap = DefaultColormapOfScreen(XtScreen(m_drawingArea));

	m_c->RegisterCallback(this, m_drawingArea, XmNexposeCallback, &Drawing::MyExpose, NULL);
	m_c->RegisterCallback(this, m_drawingArea, XmNresizeCallback, &Drawing::MyResize, NULL);


	m_bmp = XCreatePixmap (XtDisplay (m_drawingArea),
                            RootWindowOfScreen(XtScreen(m_drawingArea)),
                            800, 600,
                            DefaultDepthOfScreen(XtScreen(m_drawingArea)));
	m_base = XCreatePixmap (XtDisplay (m_drawingArea),
                            RootWindowOfScreen(XtScreen(m_drawingArea)),
                            800, 600,
                            DefaultDepthOfScreen(XtScreen(m_drawingArea)));

    /* clear pixmap with white */

	SetPenColor(0, 0, 0);

	XFillRectangle(XtDisplay(m_drawingArea), m_bmp, m_gc, 0, 0, 800, 600);
	XFillRectangle(XtDisplay(m_drawingArea), m_base, m_gc, 0, 0, 800, 600);
	XCopyArea (XtDisplay(m_drawingArea), m_bmp, RootWindowOfScreen(XtScreen(m_drawingArea)), m_gc, 0, 0, 800, 600, 0, 0);

	// XSetLineAttributes(XtDisplay(m_drawingArea), m_gc, 1, Line, 0, 0);
	SetPenColor(1, 0, 0);

}

void Drawing::SetPenColor(float R, float G, float B)
{
	// I guess XParseColor will work here
	m_xcol.red = 65535 * R;
	m_xcol.green = 65535 * G;
	m_xcol.blue = 65535 * B;
	m_xcol.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(XtDisplay(m_drawingArea), m_colMap, &m_xcol);

	XSetForeground(XtDisplay(m_drawingArea), m_gc, m_xcol.pixel);
	XFreeColors(XtDisplay(m_drawingArea), m_colMap, &m_xcol.pixel, 1, 0);
}

void Drawing::SetBrushColor(float R, float G, float B)
{

	// I guess XParseColor will work here
	m_xcol.red = 65535 * R;
	m_xcol.green = 65535 * G;
	m_xcol.blue = 65535 * B;
	m_xcol.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(XtDisplay(m_drawingArea), m_colMap, &m_xcol);

	XSetBackground(XtDisplay(m_drawingArea), m_gc, m_xcol.pixel);
	XFreeColors(XtDisplay(m_drawingArea), m_colMap, &m_xcol.pixel, 1, 0);
}

void Drawing::DrawLine(int x1, int y1, int x2, int y2)
{
	XCopyArea (XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, 800, 600, 0, 0);
	XDrawLine(XtDisplay(m_drawingArea), m_bmp, m_gc, x1, y1, x2, y2);
	XCopyArea (XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, 800, 600, 0, 0);
}

void Drawing::DrawPoint(int x, int y)
{
	XDrawPoint(XtDisplay(m_drawingArea), m_bmp, m_gc, x, y);
	XCopyArea (XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, 800, 600, 0, 0);
}

void Drawing::FixRectCoords(int& x1, int& y1, int& x2, int& y2)
{
	int temp;
	if (x1 >= x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y1 >= y2)
	{
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
}

void Drawing::DrawRectangle(int x1, int y1, int x2, int y2)
{
	XCopyArea (XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, 800, 600, 0, 0);
	// this buggs out on negative width and height
	FixRectCoords(x1, y1, x2, y2);
	XDrawRectangle (XtDisplay (m_drawingArea), m_bmp, m_gc, x1, y1, x2 - x1, y2 - y1);
	XCopyArea (XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, 800, 600, 0, 0);
}

void Drawing::FillRectangle(int x1, int y1, int x2, int y2)
{
	XCopyArea (XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, 800, 600, 0, 0);
	FixRectCoords(x1, y1, x2, y2);
	XFillRectangle (XtDisplay (m_drawingArea), m_bmp, m_gc, x1, y1, x2 - x1, y2 - y1);
	XCopyArea (XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, 800, 600, 0, 0);
}

void Drawing::Clear()
{
	//XClearWindow(XtDisplay(m_drawingArea), XtWindow(m_drawingArea));
	SetPenColor(0, 0, 0);
	XFillRectangle(XtDisplay(m_drawingArea), m_base, m_gc, 0, 0, 800, 600);
	XCopyArea (XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, 800, 600, 0, 0);
	XCopyArea (XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, 800, 600, 0, 0);
}

void Drawing::ToolDone()
{
	// Copy m_bmp into m_base
	// m_base will be the whole picture
	XCopyArea (XtDisplay(m_drawingArea), m_bmp, m_base, m_gc, 0, 0, 800, 600, 0, 0);
}

void Drawing::MyExpose(Widget widget, XtPointer user_data, XtPointer call_data)
{
	XCopyArea (XtDisplay(widget), m_base, XtWindow(widget), m_gc, 0, 0, 800, 600, 0, 0);
}



void Drawing::MyResize(Widget widget, XtPointer user_data, XtPointer call_data)
{

}

Drawing::~Drawing()
{
	XFreePixmap(XtDisplay(m_drawingArea), m_base);
	XFreePixmap(XtDisplay(m_drawingArea), m_bmp);
	XFreeGC(XtDisplay(m_drawingArea), m_gc);
}
