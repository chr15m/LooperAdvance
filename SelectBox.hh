#include "looper.hh"
#include "Widget.hh"
#include "string.h"

#ifndef _SELECTBOX_HH_
#define _SELECTBOX_HH_

class SelectBox : public Widget
{
// this type is a linked list of select box items
typedef struct structSelectList
{
	char *text;
	s16 value;
	structSelectList *next;
	structSelectList *prev;
} structSelectList;

private:
	u16 width;			// how many characters wide is this?
	u16 timer;			// if it's an auto-reset button
	u16 maxtime;			// if set the select box will reset after this amount of time
	structSelectList *which;	// currently selected item
	structSelectList *last;		// last one in the list
	structSelectList *first; 	// first one in the list

public:
	SelectBox(u16 x, u16 y, u16 iwidth, Keys *inkeys);
	
	void Choose(u16 choice);
	void ChooseByValue(u16 value);
	u16 GetChoice();
	void NewChoice(char *text, s16 myval);
	void ClearChoices();
	bool IsEmpty();
	char *GetChoiceString();

	void AutoOff(u16 time = 10);
	u16 GetFrame();

	inline bool Pressed()
	{
		return (GetFrame() == 1);
	}

	Widget *Process();
	void Draw();
};

#endif
