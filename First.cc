// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "First.hh"

First::First(Keys *inkeys)
{
	lbSong = new Label(2, 1, "Song");
	sbSong = new SelectBox(7, 1, 13, inkeys);
	sbNewButton = new SelectBox(2, 3, 3, inkeys);
	sbDelButton = new SelectBox(9, 3, 6, inkeys);
	sbSaveButton = new SelectBox(2, 5, 13, inkeys);
	
	AddWidget(lbSong);
	AddWidget(sbSong);
	AddWidget(sbNewButton);
	AddWidget(sbDelButton);
	AddWidget(sbSaveButton);
	
	sbSong->SetTransitions(NULL, NULL, NULL, sbNewButton);
	sbNewButton->SetTransitions(NULL, sbDelButton, sbSong, sbSaveButton);
	sbDelButton->SetTransitions(sbNewButton, NULL, sbSong, sbSaveButton);
	sbSaveButton->SetTransitions(NULL, NULL, sbNewButton, NULL);
	
	sbNewButton->AutoOff();
	sbDelButton->AutoOff();
	sbSaveButton->AutoOff();
	
	sbNewButton->NewChoice("New", 0);
	sbNewButton->NewChoice("---", 0);
	
	sbDelButton->NewChoice("Delete", 0);
	sbDelButton->NewChoice("------", 0);
	
	sbSaveButton->NewChoice("Write Changes", 0);
	sbSaveButton->NewChoice("-------------", 0);
	
	UseKeys(inkeys);
	
	sbSong->ClearChoices();
	sbSong->Select();
	selected = sbSong;
}

First::~First()
{
	delete lbSong;
	delete sbSong;
	delete sbNewButton;
	delete sbDelButton;
	delete sbSaveButton;
}

void First::Draw()
{
	u16 beat;
	
	selected = selected->Process();
	
	if (globals.currentsong)
		beat = globals.counter * globals.currentsong->bpm/3600;
}

void First::Process()
{
	u16 i;
	Loop *ptrLoop;
	structSongData *dataptr;
	
	dprintf("First Process: %ld\n", (u32)this);
	
	// if they've chosen a new song, load it
	if (oldsong != sbSong->GetChoice())
	{
		//dataptr = (structSongData *)&(songs::data[sbSong->GetChoice()]);
		ptrLoop = (Loop *)right;
		while(ptrLoop)
		{
			ptrLoop->SetParameters(dataptr->loops[i].sample, dataptr->loops[i].pan, dataptr->loops[i].pitch, dataptr->loops[i].divisions);
			ptrLoop = (Loop *)ptrLoop->right;
		}
		oldsong = sbSong->GetChoice();
	}
}
