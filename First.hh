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
	
	// callbacks
	TCallback<First> cbSaveButton;
	TCallback<First> cbSong;
	TCallback<First> cbNewButton;
	TCallback<First> cbDelButton;
	TCallback<First> cbAddLoopButton;
	TCallback<First> cbBPM;
	TCallback<First> cbSongName;

public:
	First(Keys *inkeys);
	virtual ~First();
	inline void DoDraw() {};
	void DoProcess() {};
		
	// build the choice list of song names
	void RebuildSongList();
	
	// if they've clicked the save button, then save
	void *SaveButton(void *data);
	void *ChangeSongName(void *data);
	void *Song(void *data);
	void *NewButton(void *data);
	void *DelButton(void *data);
	void *AddLoopButton(void *data);
	void *BPM(void *data);
};
