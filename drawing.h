#pragma once

#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include "controller.h"

class Drawing
{
private:
	Widget m_drawingArea;
	GC m_gc;
	Pixmap bmp;
	Controller<Drawing> *c;
public:
	Drawing(Widget);

	void DrawLine(int, int, int, int);

	void MyExpose(Widget, XtPointer, XtPointer);
	void MyResize(Widget, XtPointer, XtPointer);
	virtual ~Drawing();
};