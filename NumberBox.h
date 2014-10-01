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
	NumberBox(u16 x, u16 y, u16 iwidth, u16 imin, u16 imax, u16 ibigstep, NumberBox *inext, Keys *inkeys);
	
	void SetValue(u16 newVal);
	u16 GetValue();
	Widget *Process();
	void Draw();
};

#endif
