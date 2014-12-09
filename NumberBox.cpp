/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: NumberBox.cc,v 1.6 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "NumberBox.h"

NumberBox::NumberBox(u16 ix, u16 iy, u16 iwidth, u16 imin, u16 imax, u16 ibigstep, Keys *inkeys): Widget (ix, iy, inkeys)
{
	width = iwidth;
	min = imin;
	max = imax;
	bigstep = ibigstep;	
	value = min;
	sprintf(text, "[%*d]", width, value);
}

void NumberBox::SetValue(u16 newVal)
{
	if (value != newVal) {
		value = newVal;
		sprintf(text, "[%*d]", width, value);
	}
}

void NumberBox::SetMax(u16 imax)
{
	if (value > imax)
	{
		value = imax;
		sprintf(text, "[%*d]", width, value);
	}
	max = imax;
}

u16 NumberBox::GetValue()
{
	return value;
}

Widget *NumberBox::Process()
{
	Widget *newselect = NULL;
	s16 tmpval = value;
	
	if (keys->IsPressed(keyA))
	{	
		// what keys was pressed ey
		if (keys->TestKey(keyUp) == pressed)
			tmpval += bigstep;
		if (keys->TestKey(keyDown) == pressed)
			tmpval -= bigstep;
		if (keys->TestKey(keyLeft) == pressed)
			tmpval -= 1;
		if (keys->TestKey(keyRight) == pressed)
			tmpval += 1;
		
		// check our bounds
		if (tmpval > max)
			tmpval = max;
		if (tmpval < min)
			tmpval = min;
		
		if (value != tmpval)
		{
			value = tmpval;
			sprintf(text, "[%*d]", width, value);
			Callback(&value);
		}
	}
	else
	{
		// gimme some sugar
		if (keys->TestKey(keyUp) == pressed)
			newselect = up;
		
		if (keys->TestKey(keyDown) == pressed)
			newselect = down;
		
		if (keys->TestKey(keyLeft) == pressed)
			newselect = left;
		
		if (keys->TestKey(keyRight) == pressed)
			newselect = right;
	}
	
	if (!newselect)
	{
		newselect = this;
	}
	
	newselect->Select();
	
	return newselect;
}

void NumberBox::Draw()
{
	// char *text = "[NMBX]";
	if (selected)
	{
		hprintf(x, y, text);
		selected = false;
	}
	else
	{
		cprintf(x, y, text);
	}
}
