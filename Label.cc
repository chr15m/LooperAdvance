#include "Label.hh"

Label::Label(u16 ix, u16 iy, char *instring): Widget (ix, iy, NULL)
{
	text = instring;
}

void Label::Draw()
{	
	cprintf(x, y, text);
}
