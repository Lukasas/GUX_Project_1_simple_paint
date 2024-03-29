#include "drawing.h"

Drawing::Drawing(Widget w) : m_c(new Controller<Drawing>()), m_width(800), m_height(600)
{
	m_drawingArea = w;
	m_gc = XCreateGC(XtDisplay(w), RootWindowOfScreen(XtScreen(m_drawingArea)), 0, NULL);
	m_colMap = DefaultColormapOfScreen(XtScreen(m_drawingArea));

	m_bmp = XCreatePixmap(XtDisplay(m_drawingArea),
						  RootWindowOfScreen(XtScreen(m_drawingArea)),
						  m_width, m_height,
						  DefaultDepthOfScreen(XtScreen(m_drawingArea)));
	m_base = XCreatePixmap(XtDisplay(m_drawingArea),
						   RootWindowOfScreen(XtScreen(m_drawingArea)),
						   m_width, m_height,
						   DefaultDepthOfScreen(XtScreen(m_drawingArea)));

	m_c->RegisterCallback(this, m_drawingArea, XmNexposeCallback, &Drawing::MyExpose, NULL);
	m_c->RegisterCallback(this, m_drawingArea, XmNresizeCallback, &Drawing::MyResize, NULL);
	/* clear pixmap with white */

	SetPenColor(0, 0, 0);

	XFillRectangle(XtDisplay(m_drawingArea), m_bmp, m_gc, 0, 0, m_width, m_height);
	XFillRectangle(XtDisplay(m_drawingArea), m_base, m_gc, 0, 0, m_width, m_height);
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, RootWindowOfScreen(XtScreen(m_drawingArea)), m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::SetLineWidthStyle(int line_width, int line_style)
{
	XSetLineAttributes(XtDisplay(m_drawingArea), m_gc, line_width, line_style, 0, 0);
}

void Drawing::SetPenColor(long long color)
{
	float R = ((color & 0xFF0000) >> 16) / 0xFF;
	float G = ((color & 0xFF00) >> 8) / 0xFF;
	float B = (color & 0xFF) / 0xFF;

	SetPenColor(R, G, B);
}

void Drawing::SetPenColor(float R, float G, float B)
{
	this->R = R;
	this->G = G;
	this->B = B;
	// I guess XParseColor will work here
	m_xcol.red = 65535 * R;
	m_xcol.green = 65535 * G;
	m_xcol.blue = 65535 * B;
	m_xcol.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(XtDisplay(m_drawingArea), m_colMap, &m_xcol);

	XSetForeground(XtDisplay(m_drawingArea), m_gc, m_xcol.pixel);
	XFreeColors(XtDisplay(m_drawingArea), m_colMap, &m_xcol.pixel, 1, 0);
}

void Drawing::SetBrushColor(long long color)
{
	float R = ((color & 0xFF0000) >> 16) / 0xFF;
	float G = ((color & 0xFF00) >> 8) / 0xFF;
	float B = (color & 0xFF) / 0xFF;

	SetBrushColor(R, G, B);
}

void Drawing::SetBrushColor(float R, float G, float B)
{
	/*this->R = R;
	this->G = G;
	this->B = B;*/

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
	XCopyArea(XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, m_width, m_height, 0, 0);
	XDrawLine(XtDisplay(m_drawingArea), m_bmp, m_gc, x1, y1, x2, y2);
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::DrawPoint(int x, int y)
{
	XDrawPoint(XtDisplay(m_drawingArea), m_bmp, m_gc, x, y);
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::FixRectCoords(int &x1, int &y1, int &x2, int &y2)
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
	XCopyArea(XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, m_width, m_height, 0, 0);
	// this buggs out on negative width and height
	FixRectCoords(x1, y1, x2, y2);
	XDrawRectangle(XtDisplay(m_drawingArea), m_bmp, m_gc, x1, y1, x2 - x1, y2 - y1);
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::FillRectangle(int x1, int y1, int x2, int y2)
{
	XCopyArea(XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, m_width, m_height, 0, 0);
	FixRectCoords(x1, y1, x2, y2);
	XFillRectangle(XtDisplay(m_drawingArea), m_bmp, m_gc, x1, y1, x2 - x1, y2 - y1);
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::DrawEllipse(int x1, int y1, int x2, int y2)
{
	XCopyArea(XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, m_width, m_height, 0, 0);
	FixRectCoords(x1, y1, x2, y2);
	int w = x2 - x1;
	w /= 2;
	int h = y2 - y1;
	h /= 2;
	x1 -= w;
	y1 -= h;
	x2 += w;
	y2 += h;
	XDrawArc(XtDisplay(m_drawingArea), m_bmp, m_gc, x1, y1, x2 - x1, y2 - y1, 0, 360 * 64);
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::FillEllipse(int x1, int y1, int x2, int y2)
{
	XCopyArea(XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, m_width, m_height, 0, 0);
	FixRectCoords(x1, y1, x2, y2);
	int w = x2 - x1;
	w /= 2;
	int h = y2 - y1;
	h /= 2;
	x1 -= w;
	y1 -= h;
	x2 += w;
	y2 += h;
	XFillArc(XtDisplay(m_drawingArea), m_bmp, m_gc, x1, y1, x2 - x1, y2 - y1, 0, 360 * 64);
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::Clear()
{
	float R = this->R, G = this->G, B = this->B;
	SetPenColor(0, 0, 0);
	XFillRectangle(XtDisplay(m_drawingArea), m_base, m_gc, 0, 0, m_width, m_height);
	XCopyArea(XtDisplay(m_drawingArea), m_base, m_bmp, m_gc, 0, 0, m_width, m_height, 0, 0);
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, XtWindow(m_drawingArea), m_gc, 0, 0, m_width, m_height, 0, 0);
	SetPenColor(R, G, B);
}

void Drawing::ToolDone()
{
	// Copy m_bmp into m_base
	// m_base will be the whole picture
	XCopyArea(XtDisplay(m_drawingArea), m_bmp, m_base, m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::MyExpose(Widget widget, XtPointer user_data, XtPointer call_data)
{
	if (m_base)
		XCopyArea(XtDisplay(widget), m_base, XtWindow(widget), m_gc, 0, 0, m_width, m_height, 0, 0);
	else
		XCopyArea(XtDisplay(widget), m_bmp, XtWindow(widget), m_gc, 0, 0, m_width, m_height, 0, 0);
}

void Drawing::MyResize(Widget widget, XtPointer user_data, XtPointer call_data)
{
	Dimension selW, selH;

	Dimension width, height;
	XtVaGetValues(m_drawingArea,
				  XmNwidth, &width,
				  XmNheight, &height,
				  NULL);

	selW = width;
	if (width >= m_width)
		selW = m_width;

	selH = height;
	if (height >= m_height)
		selH = m_height;

	Pixmap temp  = XCreatePixmap(XtDisplay(m_drawingArea),
						   RootWindowOfScreen(XtScreen(m_drawingArea)),
						   selW, selH,
						   DefaultDepthOfScreen(XtScreen(m_drawingArea)));

	XCopyArea(XtDisplay(m_drawingArea), m_base, temp, m_gc, 0, 0, m_width, m_height, 0, 0);

	XFreePixmap(XtDisplay(m_drawingArea), m_base);
	m_base = XCreatePixmap(XtDisplay(m_drawingArea),
						   RootWindowOfScreen(XtScreen(m_drawingArea)),
						   width, height,
						   DefaultDepthOfScreen(XtScreen(m_drawingArea)));
	float R = this->R, G = this->G, B = this->B;
	SetPenColor(0, 0, 0);
	XFillRectangle(XtDisplay(m_drawingArea), m_base, m_gc, 0, 0, width, height);
	SetPenColor(R, G, B);
	XCopyArea(XtDisplay(m_drawingArea), temp, m_base, m_gc, 0, 0, selW, selH, 0, 0);

	XFreePixmap(XtDisplay(m_drawingArea), m_bmp);
	m_bmp = XCreatePixmap(XtDisplay(m_drawingArea),
						  RootWindowOfScreen(XtScreen(m_drawingArea)),
						  width, height,
						  DefaultDepthOfScreen(XtScreen(m_drawingArea)));
	XCopyArea(XtDisplay(m_drawingArea), temp, m_bmp, m_gc, 0, 0, selW, selH, 0, 0);
	XFreePixmap(XtDisplay(m_drawingArea), temp);

	m_width = width;
	m_height = height;

}

Drawing::~Drawing()
{
	XFreePixmap(XtDisplay(m_drawingArea), m_base);
	XFreePixmap(XtDisplay(m_drawingArea), m_bmp);
	XFreeGC(XtDisplay(m_drawingArea), m_gc);
}
