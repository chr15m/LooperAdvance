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
#include "Widget.hh"
#include "string.h"

#ifndef _EDITBOX_HH_
#define _EDITBOX_HH_

class EditBox : public Widget
{
private:
	char *value;	// an array of the characters themselves
	u8 which;	// currently selected character
	u8 width;	// how many characters wide it is
	u8 blink;	// this counter helps us to blink

public:
	EditBox(u16 x, u16 y, u16 inwidth, Keys *inkeys);
	virtual ~EditBox();
	
	void SetString(char *instring);
	char *GetString();
	Widget *Process();
	void Draw();
};

#endif
