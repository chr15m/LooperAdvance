/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: Widget.hh,v 1.7 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "looper.h"
#include "charset.h"

#ifndef _WIDGET_HH_
#define _WIDGET_HH_

#include "Keys.h"

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

public:
	Widget(u16 x, u16 y, Keys *inkeys);
	virtual ~Widget();
	cCallback *callback;
	
	void SetTransitions(Widget *newL, Widget *newR, Widget *newU, Widget *newD);
	void Select();
	void UseCallBack(cCallback *pCBFunc);
	virtual void Draw()=0;
	virtual Widget *Process()=0;

	inline void *Callback(void *data)
	{
		if (callback)
		{
			debug("Making callback");
			return callback->Execute(data);
		}
		return NULL;
	}
	
	inline void PrintBoxString(char *out, int w, char* val)
	{
		// sprintf(out, "[%-*s]", w, val);
		/*out[0] = '[';
		out[1] = ']';
		out[2] = NULL;
		return;*/
		int l = strnlen(val, w);
		strncpy(out+1, val, l+1);
		memcpy(out+l+1, spaces, w-l);
		out[0] = '[';
		out[w+1] = ']';
		out[w+2] = (char)NULL;
	}
	
	inline void PrintBoxInt(char *out, int w, int val)
	{
	}
};

#endif
