/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

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
	char text[MAXSTRING];	// hold the text to print to screen
	u8 width;	// how wide is our widget?

public:
	Widget(u16 x, u16 y, Keys *inkeys);
	virtual ~Widget();
	cCallback *callback;
	
	void SetTransitions(Widget *newL, Widget *newR, Widget *newU, Widget *newD);
	void Select();
	void UseCallBack(cCallback *pCBFunc);
	void Render(char *itext);
	virtual void Draw()=NULL;
	virtual Widget *Process()=NULL;

	inline void *Callback(void *data)
	{
		if (callback)
		{
			debug("Making callback");
			return callback->Execute(data);
		}
		else
		{
			return NULL;
		}
	}
};

#endif
