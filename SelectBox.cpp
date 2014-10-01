#include "SelectBox.h"

SelectBox::SelectBox(u16 ix, u16 iy, u16 iwidth, Widget *inext, Keys *inkeys): Widget (ix, iy, inext, inkeys)
{
	u16 i=0;
	x = ix;
	y = iy;
	width = iwidth;
	which = 0;
	
	left = right = up = down = NULL;

	for (i=0; i<MAXSELECTIONS; i++)
	{
		selection[i][0] = NULL;
		value[i] = 0;
	}
	
	next = inext;
}

void SelectBox::Choose(u16 choice)
{
	which = choice;
}

void SelectBox::ChooseByValue(u16 inval)
{
	u16 i = MAXSELECTIONS;
	while (--i)
	{
		if (value[i] == inval)
			which = i;
	}
}

u16 SelectBox::GetChoice()
{
	return value[which];
}

char *SelectBox::GetChoiceString()
{
	return selection[which];
}

void SelectBox::NewChoice(char *text, u16 myval)
{
	if (last < MAXSELECTIONS)
	{
		value[last] = myval;
		strcpy(selection[last], text);
		last ++;
	}
}

Widget *SelectBox::Process()
{
	Widget *newselect = NULL;
	s16 tmpval = which;
	
	if (keys->IsPressed(keyA))
	{	
		// what keys was pressed ey
		if (keys->TestKey(keyUp) == pressed)
			tmpval += 1;
		if (keys->TestKey(keyDown) == pressed)
			tmpval -= 1;
		if (keys->TestKey(keyLeft) == pressed)
			tmpval -= 1;
		if (keys->TestKey(keyRight) == pressed)
			tmpval += 1;
		
		// check our bounds
		if (tmpval >= last)
			tmpval = last - 1;
		if (tmpval < 0)
			tmpval = 0;
		
		which = tmpval;
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

void SelectBox::Draw()
{	
	char fmtstr[MAXSTRING+2];
	
	sprintf(fmtstr, "[%%%ds]", width);
	sprintf(text, fmtstr, selection[which]);
//	dprintf(text);
//	dprintf("\n");
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
