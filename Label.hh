#include "looper.hh"
#include "Widget.hh"
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
