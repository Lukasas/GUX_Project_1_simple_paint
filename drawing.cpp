#include "drawing.h"

Drawing::Drawing(Widget w) : c(new Controller<Drawing>())
{
    Pixel	fg, bg;
	m_drawingArea = w;
	m_gc = XCreateGC(XtDisplay(w), RootWindowOfScreen(XtScreen(w)), 0, NULL);

	c->RegisterCallback(this, m_drawingArea, XmNexposeCallback, &Drawing::MyExpose, NULL);
	c->RegisterCallback(this, m_drawingArea, XmNresizeCallback, &Drawing::MyResize, NULL);

	bmp = XCreatePixmap (XtDisplay (m_drawingArea),
                            RootWindowOfScreen (XtScreen (m_drawingArea)),
                            800, 600,
                            DefaultDepthOfScreen (XtScreen (m_drawingArea)));

    /* clear pixmap with white */
    XFillRectangle (XtDisplay (m_drawingArea), bmp, m_gc, 0, 0, 800, 600);

}

void Drawing::DrawLine(int x1, int y1, int x2, int y2)
{
	XDrawLine(XtDisplay(m_drawingArea), XtWindow(m_drawingArea), m_gc, x1, y1, x2, y2);
}

void Drawing::MyExpose(Widget widget, XtPointer user_data, XtPointer call_data)
{
	//DrawLine(0, 0, 100, 100);
	XCopyArea (XtDisplay(widget), bmp, XtWindow(widget), m_gc, 0, 0, 800, 600, 0, 0);
}

void Drawing::MyResize(Widget widget, XtPointer user_data, XtPointer call_data)
{

}

Drawing::~Drawing()
{
	XFreeGC(XtDisplay(m_drawingArea), m_gc);
}
