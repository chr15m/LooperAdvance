// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "First.hh"

// initialise
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

	// set up our callback functions for each UI element
	cbSaveButton.MakeCallback(this, &First::SaveButton);
	sbSaveButton->UseCallBack(&cbSaveButton);
	
	cbSong.MakeCallback(this, &First::Song);
	sbSong->UseCallBack(&cbSong);

	cbNewButton.MakeCallback(this, &First::NewButton);
	sbNewButton->UseCallBack(&cbNewButton);

	cbDelButton.MakeCallback(this, &First::DelButton);
	sbDelButton->UseCallBack(&cbDelButton);

	cbAddLoopButton.MakeCallback(this, &First::AddLoopButton);
	sbAddLoopButton->UseCallBack(&cbAddLoopButton);

	cbBPM.MakeCallback(this, &First::BPM);
	nbBPM->UseCallBack(&cbBPM);
	
	cbSongName.MakeCallback(this, &First::ChangeSongName);
	ebSongName->UseCallBack(&cbSongName);
	
	UseKeys(inkeys);
	
	globals.LoadSongs();
	RebuildSongList();
	sbSong->Select();
	selected = sbSong;
	debug("Songdata: 0x%lx", (u32)globals.songdata);
}

// destructor
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

// if they've clicked the save button, then save
void *First::SaveButton(void *data)
{
	debug("Save button callback");
	globals.SaveSongs();
	return NULL;
}

// if they change the name of this song
void *First::ChangeSongName(void *data)
{
	globals.SetName((char *)data);
	return NULL;
}

// if they've moved the song selecta
void *First::Song(void *data)
{
	debug("Song select callback");
	// change currentsong variable to point at the newly selected song
	
	// remove all our old loops
	// for every loop in this song, create it's loop
	// set the BPM to be this song's BPM
	// set the songname field to be this song's songname
	return NULL;
}


// if they've pressed the new button
// creating a new song, add it on the end and switch to it
void *First::NewButton(void *data)
{
	debug("New button callback");
	// create a new song in the songdata
	// rebuild the choices
	// choose the new song
	return NULL;
}

// if they're deleting a song
void *First::DelButton(void *data)
{
	debug("Del button callback");
	// delete all the live loops
	// delete the current song
	// rebuild choices
	return NULL;
}

// if they press the loop-add button
void *First::AddLoopButton(void *data)
{
	debug("AddLoop button callback");
	// add a loop to currentsong
	globals.NewLoop();
	// add a live loop
	if (this->right)
	{
		// make our next loop have a new loop
		this->right->left = new Loop(keys);
		this->right = this->right->left;
	}
	else
	{
		this->right = new Loop(keys);
	}
	return NULL;
}

// if they change the BPM
void *First::BPM(void *data)
{
	debug("BPM: %d", *(u16 *)data);
	return NULL;
}

// we don't need to do anything in here as it's all handled by callbacks
void First::DoProcess()
{
	debugloop("Songdata: 0x%lx", (u32)globals.songdata);
}

// this function takes all the songs in songdata and makes a list of them
void First::RebuildSongList()
{
	structSongData *selected = (structSongData *)sbSong->GetChoice();
	structSongData *traverse = globals.songdata;
	
	sbSong->ClearChoices();
	
	while (traverse)
	{
		sbSong->NewChoice(traverse->name, (u32)traverse);
		traverse = traverse->next;
	}
	
	// if we haven't selected one, select the first one
	if (!selected)
	{
		selected = globals.songdata;
	}
	
	if (selected)
	{
		// with the selected song
		sbSong->ChooseByValue((u32)selected);
		nbBPM->SetValue(selected->bpm);
		ebSongName->SetString(selected->name);
	}
}
