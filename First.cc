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
	sbDelLoopButton = new SelectBox(18, 18, 8, inkeys);
	
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
	AddWidget(sbDelLoopButton);
	
	sbSong->SetTransitions(NULL, NULL, NULL, sbNewButton);
	sbNewButton->SetTransitions(NULL, sbDelButton, sbSong, sbSaveButton);
	sbDelButton->SetTransitions(sbNewButton, NULL, sbSong, sbSaveButton);
	sbSaveButton->SetTransitions(NULL, NULL, sbNewButton, ebSongName);
	ebSongName->SetTransitions(NULL, NULL, sbSaveButton, nbBPM);
	nbBPM->SetTransitions(NULL, sbAddLoopButton, ebSongName, sbAddLoopButton);
	sbAddLoopButton->SetTransitions(nbBPM, NULL, nbBPM, sbDelLoopButton);
	sbDelLoopButton->SetTransitions(nbBPM, NULL, sbAddLoopButton, NULL);
	
	sbNewButton->AutoOff();
	sbDelButton->AutoOff();
	sbSaveButton->AutoOff();
	sbAddLoopButton->AutoOff();
	sbDelLoopButton->AutoOff();
	
	sbNewButton->NewChoice("New", 0);
	sbNewButton->NewChoice("---", 1);
	
	sbDelButton->NewChoice("Delete", 0);
	sbDelButton->NewChoice("------", 1);
	
	sbSaveButton->NewChoice("Write Changes", 0);
	sbSaveButton->NewChoice("-------------", 1);

	sbAddLoopButton->NewChoice("Add Loop", 0);
	sbAddLoopButton->NewChoice("--------", 1);

	sbDelLoopButton->NewChoice("Del Loop", 0);
	sbDelLoopButton->NewChoice("--------", 1);

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

	cbDelLoopButton.MakeCallback(this, &First::DelLoopButton);
	sbDelLoopButton->UseCallBack(&cbDelLoopButton);

	cbBPM.MakeCallback(this, &First::BPM);
	nbBPM->UseCallBack(&cbBPM);
	
	cbSongName.MakeCallback(this, &First::ChangeSongName);
	ebSongName->UseCallBack(&cbSongName);
	
	UseKeys(inkeys);
	
	sbSong->Select();
	selected = sbSong;

	globals.LoadSongs();
	RebuildSongList();
	Song(NULL);
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
	delete sbDelLoopButton;
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
	RebuildSongList();
	return NULL;
}

// if they've moved the song selecta
void *First::Song(void *data)
{
	structLoopData *loop=NULL;
	structSongData *newsong;
	if (data)
	{
		// if we were called in callback
		newsong = (structSongData *)((structSelectList *)data)->value;
	}
	else
	{
		// otherwise ask what we're doing
		newsong = (structSongData *)sbSong->GetChoice();
	}
	Page *newloop=this;

	debug("Song select callback");
	// change currentsong variable to point at the newly selected song
	globals.SetSong(newsong);
	// remove all our old loops
	while (right)
	{
		DelLoopButton(NULL);
	}
	
	// for every loop in this song, create it's loop
	loop = newsong->loops;
	while (loop)
	{
		newloop->right = new Loop(keys, loop);
		newloop->right->left = newloop;
		newloop = newloop->right;
		loop = loop->next;
	}
	
	// set this song's data
	nbBPM->SetValue(newsong->bpm);
	ebSongName->SetString(newsong->name);
	
	// reset the counters each time we change songs
	globals.Reset();
	
	return NULL;
}


// if they've pressed the new button
// creating a new song, add it on the end and switch to it
void *First::NewButton(void *data)
{
	debug("New button callback");
	// create a new song in the songdata
	globals.NewSong();
	// rebuild the choices
	RebuildSongList();
	
	return NULL;
}

// if they're deleting a song
void *First::DelButton(void *data)
{
	debug("Del button callback");
	// delete all the live loops
	while(right)
	{
		DelLoopButton(NULL);
	}
	// delete the current song
	globals.DelSong();
	// rebuild choices
	RebuildSongList();
	return NULL;
}

// if they press the loop-add button
void *First::AddLoopButton(void *data)
{
	Page *old;
	debug("AddLoop button callback");
	// add a loop to currentsong
	globals.NewLoop();
	// add a live loop
	if (right)
	{
		debug("Inserting a new loop");
		// make our next loop have a new loop
		old = right;
		right->left = new Loop(keys, globals.currentloop);
		right = right->left;
		right->left = this;
		right->right = old;
	}
	else
	{
		debug("Appending a new loop");
		right = new Loop(keys, globals.currentloop);
		right->left = this;
	}
	
	return NULL;
}

// if they press the del-loop button
void *First::DelLoopButton(void *data)
{
	Page *old;
	debug("DelLoop button callback");
	// delete the liveloop associated with it
	if (right)
	{
		// delete the current loop in currentsong
		globals.SetLoop(((Loop *)right)->GetAddress());
		globals.DelLoop();

		old = right->right;
		delete right;
		right = old;
		right->left = this;
	}
	
	return NULL;
}

// if they change the BPM
void *First::BPM(void *data)
{
	debug("BPM: %d", *(u16 *)data);
	globals.SetBPM(*(u16 *)data);
	return NULL;
}

// this function takes all the songs in songdata and makes a list of them
void First::RebuildSongList()
{
	structSongData *traverse = globals.songdata;
	sbSong->ClearChoices();
	
	while (traverse)
	{
		sbSong->NewChoice(traverse->name, (u32)traverse);
		traverse = traverse->next;
	}
	
	if (selected)
	{
		debug("Setting Name='%s' BPM='%d'", globals.currentsong->name, globals.currentsong->bpm);
		// with the selected song
		sbSong->ChooseByValue((u32)globals.currentsong);
		nbBPM->SetValue(globals.currentsong->bpm);
		ebSongName->SetString(globals.currentsong->name);
	}
}

