// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

#include "GlobalData.hh"

GlobalData::GlobalData()
{
	// initialize our globals
	counter = 0;
	songdata = NULL;
	currentsong = NULL;
}

// create a new blank song
void GlobalData::NewSong()
{
	// if we have other songs in memory already, then create a new blank one on the end
	if (songdata)
	{
		currentsong->next = new structSongData;
		currentsong = currentsong->next;
	}
	else
	{
		// initialise a fresh song
		songdata = new structSongData;
		currentsong = songdata;		
	}
	
	currentsong->bpm = 180;
	currentsong->name = new char[1];
	currentsong->name[0] = '\0';
	currentsong->loop = NULL;
	currentsong->next = NULL;
	
	// set the current song to be this blank song
	currentsong = songdata;
}

// delete the current song
void GlobalData::DelSong()
{
	
}

// choose a different song
void GlobalData::SetSong(u16 whichsong)
{
	
}

// change the name of the current song
void GlobalData::SetName(char *name)
{
	
}

// add another loop to this song
void GlobalData::AddLoop()
{
	
}

// delete a loop from this song
void GlobalData::DelLoop()
{
	
}

// set the beats per minute of this song
void GlobalData::SetBPM(u16 bpm)
{
	
}

// save all songs from memory into the writeable ROM
void GlobalData::SaveSongs()
{
	// this is going to walk the save ram, pulling out and reconstructing the data
	u16 offset = 0;
		
	#define NEXTBYTE 	*(u8 *)(SRAM + offset)
	
	// check first if there is any saved data in the databank
	if (*(u16 *)(SRAM) == 0xABCD)
	{
		offset += 2;
		while (NEXTBYTE != '\0' && offset < 0xFFFF)
		{
			
		}
	}
	// no saved data, so start afresh
	else
	{
		right = NULL;
		
		oldsong = 0;
	}
}

// load all songs from ROM into memory
void GlobalData::LoadSongs()
{
	
}

