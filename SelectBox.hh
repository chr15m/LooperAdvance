#include "looper.hh"
#include "Widget.hh"
#include "string.h"

#ifndef _SELECTBOX_HH_
#define _SELECTBOX_HH_

#define MAXSELECTIONS 40
#define MAXSTRING 18

class SelectBox : public Widget
{
private:
	u16 width;	// how many digits wide is this?
	u16 last;	// what's our top number in the array
	u16 which;	// currently selected item
	char selection[MAXSELECTIONS][MAXSTRING]; 	// an array to hold all of the selections
	u16 value[MAXSELECTIONS];		// array to hold all values for each selection

public:
	SelectBox(u16 x, u16 y, u16 iwidth, Widget *inext, Keys *inkeys);
	
	void Choose(u16 choice);
	void ChooseByValue(u16 value);
	u16 GetChoice();
	void NewChoice(char *text, u16 myval);
	char *GetChoiceString();
	Widget *Process();
	void Draw();
};

#endif
