#include "drawing.h"

Drawing::Drawing(Widget w) : c(new Controller<Drawing>())
{
    Pixel	fg, bg;
	m_drawingArea = w;
	m_gc = XCreateGC(XtDisplay(w), RootWindowOfScreen(XtScreen(w)), 0, NULL);

	c->RegisterCallback(this, m_drawingArea, XmNexposeCallback, &Drawing::MyExpose, NULL);
	c->RegisterCallback(this, m_drawingArea, XmNresizeCallback, &Drawing::MyResize, NULL);


	SetPenColor();
	bmp = XCreatePixmap (XtDisplay (m_drawingArea),
                            RootWindowOfScreen (XtScreen (m_drawingArea)),
                            800, 600,
                            DefaultDepthOfScreen (XtScreen (m_drawingArea)));

    /* clear pixmap with white */


	XDrawLine(XtDisplay(m_drawingArea), bmp, m_gc, 0, 200, 200, 200);
	XFillRectangle (XtDisplay (m_drawingArea), bmp, m_gc, 0, 0, 0, 0);

}

void Drawing::SetPenColor()
{
	XColor xcolour;

	Colormap cmap = DefaultColormapOfScreen(XtScreen(m_drawingArea));

	// I guess XParseColor will work here
	xcolour.red = 65535; xcolour.green = 0; xcolour.blue = 0;
	xcolour.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(XtDisplay(m_drawingArea), cmap, &xcolour);

	XSetForeground(XtDisplay(m_drawingArea), m_gc, xcolour.pixel);
	XSetBackground(XtDisplay(m_drawingArea), m_gc, xcolour.pixel);
	// Free Color
}

void Drawing::SetBrushColor()
{

}

void Drawing::DrawLine(int x1, int y1, int x2, int y2)
{
	//SetPenColor();
	// XDrawLine(XtDisplay(m_drawingArea), XtWindow(m_drawingArea), m_gc, x1, y1, x2, y2);
	XDrawLine(XtDisplay(m_drawingArea), XtWindow(m_drawingArea), m_gc, x1, y1, x2, y2);
	XDrawLine(XtDisplay(m_drawingArea), bmp, m_gc, x1, y1, x2, y2);
}

void Drawing::Clear()
{
	//XClearWindow(XtDisplay(m_drawingArea), XtWindow(m_drawingArea));
	XFillRectangle(XtDisplay(m_drawingArea), bmp, m_gc, 0, 0, 0, 0);
}

void Drawing::ToolDone()
{
	// Copy bmp into base
	// base will be the whole picture
}

void Drawing::MyExpose(Widget widget, XtPointer user_data, XtPointer call_data)
{
	DrawLine(0, 100, 100, 100);
	XCopyArea (XtDisplay(widget), bmp, XtWindow(widget), m_gc, 0, 0, 800, 600, 0, 0);
}



void Drawing::MyResize(Widget widget, XtPointer user_data, XtPointer call_data)
{

}

Drawing::~Drawing()
{
	XFreeGC(XtDisplay(m_drawingArea), m_gc);
}
