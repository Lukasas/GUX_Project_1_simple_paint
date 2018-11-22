#include "drawing.h"

int x1 = 0, x2 = 100, y1 = 0, y2 = 100;

void InputLineEH(Widget w, XtPointer client_data, XEvent *event, Boolean *cont)
{
    Pixel	fg, bg;
	GC inputGC = NULL;
    if (true) {
	if (!inputGC) {
	    inputGC = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
	    /*XSetFunction(XtDisplay(w), inputGC, GXxor);
	    XSetPlaneMask(XtDisplay(w), inputGC, ~0);
	    XtVaGetValues(w, XmNforeground, &fg, XmNbackground, &bg, NULL);
	    XSetForeground(XtDisplay(w), inputGC, fg ^ bg);*/
	}

	/*if (button_pressed > 1) {

	    XDrawLine(XtDisplay(w), XtWindow(w), inputGC, x1, y1, x2, y2);
	} else {

	    //button_pressed = 2;
	}*/

	x2 = event->xmotion.x;
	y2 = event->xmotion.y;

	XDrawLine(XtDisplay(w), XtWindow(w), inputGC, x1, y1, x2, y2);
    }
}

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