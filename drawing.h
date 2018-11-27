#pragma once

#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include "controller.h"

class Drawing
{
private:
	Widget m_drawingArea;
	GC m_gc;
	Pixmap m_base;
	Pixmap m_bmp;
	Controller<Drawing> *m_c;
	int m_width;
	int m_height;

	XColor m_xcol;
	Colormap m_colMap;

	float R, G, B;

	void FixRectCoords(int &,int &,int &,int &);
public:
	Drawing(Widget);

	void SetLineWidthStyle(int line_width, int line_style);

	void SetPenColor(long long color);
	void SetPenColor(float R, float G, float B);
	void SetBrushColor(long long color);
	void SetBrushColor(float R, float G, float B);
	void DrawLine(int, int, int, int);
	void DrawPoint(int, int);
	void DrawRectangle(int, int, int, int);
	void DrawEllipse(int, int, int, int);

	void FillRectangle(int, int, int, int);
	void FillEllipse(int, int, int, int);

	void Clear();

	void ToolDone(); // This must be called on mouse up.

	void MyExpose(Widget, XtPointer, XtPointer);
	void MyResize(Widget, XtPointer, XtPointer);
	virtual ~Drawing();
};