#include "SelectBox.hh"

SelectBox::SelectBox(u16 ix, u16 iy, u16 iwidth, Keys *inkeys): Widget (ix, iy, inkeys)
{
	width = iwidth;
	first = NULL;
	which = NULL;
	last = NULL;
	timer = 0;
	maxtime = 0;
//	dprintf("Width: %d\n", width);
}

// choose item number choice
void SelectBox::Choose(u16 choice)
{
	structSelectList *travel = first;
	u16 i = 0;
	
	while (i != choice && travel)
	{
		travel = travel->next;
		i++;
	}
	
	if (travel)
	{
		which = travel;
	}
}

// choose an item by it's value
void SelectBox::ChooseByValue(u16 inval)
{
	structSelectList *travel = first;
	
	while (travel)
	{
		if (travel->value == inval)
			which = travel;
		travel = travel->next;
	}
}

// get which choice we have selected
u16 SelectBox::GetChoice()
{
	if (which)
		return which->value;
	else
		return 0;
}

// get the string for the currently selected choice
char *SelectBox::GetChoiceString()
{
	if (which)
		return which->text;
	else
		return "";
}

// add a new choice to the linked list of choices
void SelectBox::NewChoice(char *text, s16 myval)
{
	debug("Address: 0x%x", last);
	// if we have at least one already then add another one
	if (last != NULL)
	{
		last->next = new structSelectList;
		last->next->prev = last;
		last = last->next;
	}
	else
	{
		// this is our first one
		last = new structSelectList;
		last->prev = NULL;
		first = last;
		which = last;
	}
	dprintf("Address: 0x%x\n", last);
	
	// make a new character array
	last->next = NULL;
	last->text = new char[width+1];
	strncpy(last->text, text, width);
	// make sure it's null terminated
	last->text[width] = '\0';
	last->value = myval;
	
	dprintf("%s -> %d\n\n", last->text, last->value);
}

// clear out our linked list of select items
void SelectBox::ClearChoices()
{
	structSelectList *traverse = first;
	
	while (traverse)
	{
		traverse = first->next;
		delete[] first->text;
		delete first;
		first = traverse;
	}
	first = NULL;
	last = NULL;
	which = NULL;
}

// check if we have any in the list
bool SelectBox::IsEmpty()
{
	return (first == NULL);
}

// set to zero after this number of seconds
void SelectBox::AutoOff(u16 time)
{
	maxtime = time;
}

// process when we're selected
Widget *SelectBox::Process()
{
	Widget *newselect = NULL;
	
	if (keys->IsPressed(keyA))
	{
		if (which)
		{
			// what keys was pressed ey
			if (keys->TestKey(keyUp) == pressed && which->next)
			{
				which = which->next;
				if (maxtime)
					timer = maxtime;
				else
					timer = 2;
			}
			
			if (keys->TestKey(keyDown) == pressed && which->prev)
			{
				which = which->prev;
				timer = 2;
			}
			
			if (keys->TestKey(keyLeft) == pressed && which->prev)
			{
				which = which->prev;
				timer = 2;
			}
			
			if (keys->TestKey(keyRight) == pressed && which->next)
			{
				which = which->next;
				if (maxtime)
					timer = maxtime;
				else
					timer = 2;
			}
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

// draw us
void SelectBox::Draw()
{
	// make sure we don't draw an empty select box
	if (which)
		sprintf(text, "[%-*s]", width, which->text);
	else
		sprintf(text, "[%-*s]", width, "");
	
	// if the timer is started, keep counting down
	if (timer==1 && maxtime)
		Choose(0);
	
	if (timer)
		timer--;
	
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

u16 SelectBox::GetFrame()
{
	return timer;
}
