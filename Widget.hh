#include "looper.hh"
#include "charset.hh"

#ifndef _WIDGET_HH_
#define _WIDGET_HH_

#include "Keys.hh"

#define MAXSTRING	30

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
	u16 x,y;	// where on the screen
	char text[16];	// hold the text to print to screen

public:
	Widget(u16 x, u16 y, Keys *inkeys);
	cCallback *callback;
	
	void SetTransitions(Widget *newL, Widget *newR, Widget *newU, Widget *newD);
	void Select();
	void UseCallBack(cCallback *pCBFunc);
	virtual void Draw()=NULL;
	virtual Widget *Process()=NULL;
};

#endif
