/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: Widget.cc,v 1.5 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "Widget.h"

Widget::Widget(u16 ix, u16 iy, Keys *inkeys)
{
	x = ix;
	y = iy;
	
	left = right = up = down = NULL;	
	keys = inkeys;
	
	callback = NULL;
}

Widget::~Widget()
{
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

void Widget::UseCallBack(cCallback *pCBFunc)
{
	callback = pCBFunc;
}
