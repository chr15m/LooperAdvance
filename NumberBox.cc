#include "NumberBox.hh"

NumberBox::NumberBox(u16 ix, u16 iy, u16 iwidth, u16 imin, u16 imax, u16 ibigstep, Keys *inkeys): Widget (ix, iy, inkeys)
{
	width = iwidth;
	min = imin;
	max = imax;
	bigstep = ibigstep;	
	value = min;
}

void NumberBox::SetValue(u16 newVal)
{
	value = newVal;
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
			
			if (callback)
				callback->Execute(&value);
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
	sprintf(text, "[%*d]", width, value);
	
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
