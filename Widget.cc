#include "Widget.hh"

Widget::Widget(u16 ix, u16 iy, Widget *inext, Keys *inkeys)
{
	x = ix;
	y = iy;
	
	left = right = up = down = NULL;
	
	keys = inkeys;
	next = inext;
}

void Widget::SetTransitions(Widget *newL, Widget *newR, Widget *newU, Widget *newD)
{
	// dprintf("Setting Transitions: %ld, %ld, %ld, %ld\n", newL, newR, newU, newD);
	left = newL;
	right = newR;
	up = newU;
	down = newD;
}

void Widget::Select()
{
	selected = true;
}
