#include "mywindow.h"

int main(int argc, char *argv[])
{
	MyWindow *win = new MyWindow(argc, argv);
	win->InitComponents();
	win->InitializeDrawing();
	win->RunLoop();
	return 0;
}