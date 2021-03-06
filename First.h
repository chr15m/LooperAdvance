/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: First.hh,v 1.11 2004/04/08 06:09:42 chrism Exp $

******************************************************/

// Represents a single loop-page

#include "looper.h"
#include "charset.h"
#include "Page.h"
#include "Loop.h"
#include "Widgets.h"
#include "GlobalData.h"

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
	SelectBox *sbDelLoopButton;
	
	// callbacks
	TCallback<First> cbSaveButton;
	TCallback<First> cbSong;
	TCallback<First> cbNewButton;
	TCallback<First> cbDelButton;
	TCallback<First> cbAddLoopButton;
	TCallback<First> cbDelLoopButton;
	TCallback<First> cbBPM;
	TCallback<First> cbSongName;

public:
	First(Keys *inkeys);
	virtual ~First();
	inline void DoSwap() {};
	inline void DoProcess() {};
		
	// build the choice list of song names
	void RebuildSongList();
	// delete all live loops (but not loop data)
	void DelLiveLoops();
	
	// if they've clicked the save button, then save
	void *SaveButton(void *data);
	void *ChangeSongName(void *data);
	void *Song(void *data);
	void *NewButton(void *data);
	void *DelButton(void *data);
	void *AddLoopButton(void *data);
	void *DelLoopButton(void *data);
	void *BPM(void *data);
};
