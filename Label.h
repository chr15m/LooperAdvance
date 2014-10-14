/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: Label.hh,v 1.2 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "looper.h"
#include "Widget.h"
#include "string.h"

#ifndef _LABEL_HH_
#define _LABEL_HH_

class Label : public Widget
{
private:
	char *text; 	// the text of this label

public:
	Label(u16 x, u16 y, char *instring);
	
	void SetString(char *instring);
	char *GetString();
	void Draw();
	inline Widget *Process(void) { return this;	};
};

#endif
