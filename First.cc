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
	
	lbSongName = new Label(2, 8, "Name");
	ebSongName = new EditBox(7, 8, 13, inkeys);
	lbBPM = new Label(2, 10, "BPM");
	nbBPM = new NumberBox(7, 10, 3, 1, 600, 10, inkeys);
	
	sbAddLoopButton = new SelectBox(18, 17, 8, inkeys);
	
	AddWidget(lbSong);
	AddWidget(sbSong);
	AddWidget(sbNewButton);
	AddWidget(sbDelButton);
	AddWidget(sbSaveButton);
	AddWidget(lbSongName);
	AddWidget(ebSongName);
	AddWidget(lbBPM);
	AddWidget(nbBPM);
	AddWidget(sbAddLoopButton);
	
	sbSong->SetTransitions(NULL, NULL, NULL, sbNewButton);
	sbNewButton->SetTransitions(NULL, sbDelButton, sbSong, sbSaveButton);
	sbDelButton->SetTransitions(sbNewButton, NULL, sbSong, sbSaveButton);
	sbSaveButton->SetTransitions(NULL, NULL, sbNewButton, ebSongName);
	ebSongName->SetTransitions(NULL, NULL, sbSaveButton, nbBPM);
	nbBPM->SetTransitions(NULL, sbAddLoopButton, ebSongName, sbAddLoopButton);
	sbAddLoopButton->SetTransitions(nbBPM, NULL, nbBPM, NULL);
		
	sbNewButton->AutoOff();
	sbDelButton->AutoOff();
	sbSaveButton->AutoOff();
	sbAddLoopButton->AutoOff();
	
	sbNewButton->NewChoice("New", 0);
	sbNewButton->NewChoice("---", 1);
	
	sbDelButton->NewChoice("Delete", 0);
	sbDelButton->NewChoice("------", 1);
	
	sbSaveButton->NewChoice("Write Changes", 0);
	sbSaveButton->NewChoice("-------------", 1);

	sbAddLoopButton->NewChoice("Add Loop", 0);
	sbAddLoopButton->NewChoice("-------------", 1);

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
	delete lbSongName;
	delete ebSongName;
	delete lbBPM;
	delete nbBPM;
	delete sbAddLoopButton;
}

void First::DoProcess()
{
	dprintf("First Process: %ld\n", (u32)this);
	
	// if they've clicked the save button, then save
	if (sbSaveButton->Pressed())
	{
		dprintf("Saving\n");
		globals.SaveSongs();
	}
	// if they've chosen a new song
	else if (sbSong->Pressed())
	{
		// remove all our old loops
		
		// for every loop in this song, create it's loop
		
	}
	// if they're creating a new song, add it on the end and switch to it
	else if (sbNewButton->Pressed())
	{
		
	}
	// if they're deleting a song
	else if (sbDelButton->Pressed())
	{
		// delete all our old loops
		
		// delete the current song
		
		// update the current interface data
	}
	else if (sbAddLoopButton->Pressed())
	{
		// insert a new loop into our section (and add one to the data)
		
	}
}
