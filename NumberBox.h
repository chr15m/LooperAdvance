/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: NumberBox.hh,v 1.4 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "looper.h"
#include "Widget.h"

#ifndef _NUMBERBOX_HH_
#define _NUMBERBOX_HH_

class NumberBox : public Widget
{
private:
	u16 width;	// how many digits wide is this?
	u16 min;	// minimum value this number box can go to
	u16 max;	// maximum value this number box can go to
	u16 value;	// current value
	u16 bigstep;	// how big the big steps should be
	
public:
	NumberBox(u16 x, u16 y, u16 iwidth, u16 imin, u16 imax, u16 ibigstep, Keys *inkeys);
	
	void SetValue(u16 newVal);
	u16 GetValue();
	void SetMax(u16 imax);
	Widget *Process();
	void Draw();
};

#endif
