/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: EditBox.cc,v 1.9 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "EditBox.h"

EditBox::EditBox(u16 ix, u16 iy, u16 inwidth, Keys *inkeys): Widget (ix, iy, inkeys)
{
	u16 i;
	value = new char[width + 1];
	
	// fill with nulls
	for (i=0; i<width + 1; i++)
	{
		value[i] = NULL;
	}
	
	which = 0;
	width = inwidth;
	blink = 0;	
}

EditBox::~EditBox()
{
	delete[] value;
}

void EditBox::SetString(char *instring)
{
	// copy the incoming string over our text
	strncpy(value, instring, width);
	// make sure last char is 0
	value[strlen(instring)] = NULL;
	value[width] = NULL;
//	Callback(value);
}

char *EditBox::GetString()
{
	return value;
}

Widget *EditBox::Process()
{
	Widget *newselect = NULL;
	bool changed=false;
	
	if (keys->IsPressed(keyA))
	{	
		// what keys was pressed ey
		if (keys->TestKey(keyUp) == pressed)
		{
			if (value[which] == 0)
			{
				value[which] = 96;
			}
			value[which] += 1;
			changed = true;
		}
		if (keys->TestKey(keyDown) == pressed)
		{
			value[which] -= 1;
			changed = true;
		}
		
		if (keys->TestKey(keyLeft) == pressed)
		{
			which -= 1;
			blink = 0;
		}
		if (keys->TestKey(keyRight) == pressed)
		{
			which += 1;
			blink = 0;
		}
		
		// check our bounds
		if (which == 0xFF)
			which = 0;
		if (which >= width)
			which = width - 1;
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
	
	// if they've changed the text box
	if (changed)
	{
		Callback(value);
	}
	
	if (!newselect)
	{
		newselect = this;
	}
	
	newselect->Select();
	
	return newselect;
}

void EditBox::Draw()
{
	sprintf(text, "[%-*s]", width, value);
	
	if (selected)
	{
		// make the selected character toggle between _ and the letter
		if (!(blink / 10 % 2))
			text[which + 1] = '_';
		
		hprintf(x, y, text);
		selected = false;
		blink++;
	}
	else
	{
		cprintf(x, y, text);
		blink = 0;
	}
}
