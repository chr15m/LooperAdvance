// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "looper.h"
#include "charset.h"
#include "Page.h"
#include "Loop.h"
#include "NumberBox.h"
#include "SelectBox.h"
#include "songdata.h"

class First : public Page
{
private:
	NumberBox *nbBPM;
	SelectBox *sbSong;
	NumberBox *nbBeat;
	Widget *selected;
	u16 oldsong;

public:
	First(Keys *inkeys);
	virtual ~First();
	void Draw();
	void Process();
};
