// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "looper.hh"
#include "charset.hh"
#include "Page.hh"
#include "Loop.hh"
#include "Widgets.hh"
#include "GlobalData.hh"

class First : public Page
{
private:
	Label *lbSong;
	SelectBox *sbSong;
	SelectBox *sbNewButton;
	SelectBox *sbDelButton;
	SelectBox *sbSaveButton;
	SelectBox *sbAddButton;
	Label *lbSongName;
	EditBox *ebSongName;
	Label *lbBPM;
	NumberBox *nbBPM;
	SelectBox *sbAddLoopButton;
	
public:
	First(Keys *inkeys);
	virtual ~First();
	inline void DoDraw() {};
	void DoProcess();
};
