#include "EditBox.hh"

EditBox::EditBox(u16 ix, u16 iy, u16 inwidth, Keys *inkeys): Widget (ix, iy, inkeys)
{
	text = new char[width + 1];
	
	// null terminate the text string always (incase strncpy results in no null termination
	text[width] = '\0';
	which = 0;
	width = inwidth;
	blink = 0;	
}
EditBox::~EditBox()
{
	delete[] text;
}

void EditBox::SetString(char *instring)
{
	// copy the incoming string over our text
	strncpy(instring, text, width);
}

char *EditBox::GetString()
{
	return text;
}

Widget *EditBox::Process()
{
	Widget *newselect = NULL;
	
	if (keys->IsPressed(keyA))
	{	
		// what keys was pressed ey
		if (keys->TestKey(keyUp) == pressed)
		{
			if (text[which] == 0)
			{
				text[which] = 96;
			}
			text[which] += 1;
		}
		if (keys->TestKey(keyDown) == pressed)
			text[which] -= 1;
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
	
	if (!newselect)
	{
		newselect = this;
	}
	
	newselect->Select();
	
	return newselect;
}

void EditBox::Draw()
{	
	char outxt[width+2];
	
	sprintf(outxt, "[%*-s]", width, text);
	
	if (selected)
	{
		// make the selected character toggle between _ and the letter
		if (!(blink / 10 % 2))
			outxt[which + 1] = '_';
		
		hprintf(x, y, outxt);
		selected = false;
		blink++;
	}
	else
	{
		cprintf(x, y, outxt);
		blink = 0;
	}
}
