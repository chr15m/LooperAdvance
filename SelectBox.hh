#include "looper.hh"
#include "Widget.hh"
#include "string.h"

#ifndef _SELECTBOX_HH_
#define _SELECTBOX_HH_

// this type is a linked list of select box items
typedef struct structSelectList
{
	char *text;
	u32 value;
	structSelectList *next;
	structSelectList *prev;
} structSelectList;

class SelectBox : public Widget
{
private:
	u16 width;			// how many characters wide is this?
	u16 timer;			// if it's an auto-reset button
	u16 maxtime;			// if set the select box will reset after this amount of time
	structSelectList *which;	// currently selected item
	structSelectList *last;		// last one in the list
	structSelectList *first; 	// first one in the list

public:
	SelectBox(u16 x, u16 y, u16 iwidth, Keys *inkeys);

	// choose the choice-th entry in the list
	void Choose(u16 choice);
	// choose the choice with the value given
	void ChooseByValue(u32 value);
	// get the current choice value
	u32 GetChoice();
	// create a new choice
	void NewChoice(char *text, u32 myval);
	// forget all the choice
	void ClearChoices();
	// if the list is empty
	bool IsEmpty();
	// get the string of the current choice
	char *GetChoiceString();

	// set this select box to turn off after a number of ticks
	void AutoOff(u16 time = 10);
	// get the current count-down frame
	u16 GetFrame();

	inline bool Pressed()
	{
		return (GetFrame() == 1);
	}
	
	Widget *Process();
	void Draw();
};

#endif
