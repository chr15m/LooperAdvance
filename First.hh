// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "looper.hh"
#include "charset.hh"
#include "Page.hh"
#include "Loop.hh"
#include "NumberBox.hh"
#include "SelectBox.hh"
#include "songdata.hh"

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
