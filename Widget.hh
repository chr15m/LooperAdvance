#include "looper.hh"
#include "charset.hh"
#include "Keys.hh"

#ifndef _WIDGET_HH_
#define _WIDGET_HH_

class Widget
{
private:
protected:
	bool selected;	// is this the selected widget?
	Keys *keys;	// what's our keypad doing?
	Widget *left;
	Widget *right;
	Widget *up;
	Widget *down;	// these are the transition vectors (tell what other UI widgets are to which side)
	Widget *next;	// this is a linked list of all number boxes
	u16 x,y;	// where on the screen
	char text[16];	// hold the text to print to screen

public:
	Widget(u16 x, u16 y, Widget *inext, Keys *inkeys);
	
	void SetTransitions(Widget *newL, Widget *newR, Widget *newU, Widget *newD);
	void Select();
	virtual void Draw()=NULL;
	virtual Widget *Process()=NULL;
};

#endif
