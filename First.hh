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
	
	Widget *selected;
	structSongData *songdata;
	structSongData *currentsong;
	u16 oldsong;
	
public:
	First(Keys *inkeys);
	virtual ~First();
	void Draw();
	void Process();
	
	// this loads the save bank into ram
	void Load();
};
