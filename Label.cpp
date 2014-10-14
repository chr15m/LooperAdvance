/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: Label.cc,v 1.2 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "Label.h"

Label::Label(u16 ix, u16 iy, char *instring): Widget (ix, iy, NULL)
{
	text = instring;
}

void Label::Draw()
{	
	cprintf(x, y, text);
}
