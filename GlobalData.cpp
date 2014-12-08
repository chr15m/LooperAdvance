/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: GlobalData.cc,v 1.23 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "GlobalData.h"

const u32 frequency[12] = {44100, 46722, 49500, 52444, 55562, 58866, 62366, 66075, 70004, 74167, 78577, 83249};

GlobalData::GlobalData()
{
	Init();
}

// initialise everything
void GlobalData::Init()
{
	// initialize our globals
	Reset();
	songdata = NULL;
	currentsong = NULL;
	offset = 0;
	magic = 0xABCD;
	zero = 0x00;
	full = 0xFF;
}

// tick over once and recalculate all the song positions etc.
void GlobalData::Tick()
{
	counter++;
	if (currentsong)
	{
		beat = (u32)(counter * currentsong->bpm / 3600);
		//if (!(counter % 60/(currentsong->bpm/60)))
		//	beat++;
	}
}

void GlobalData::Reset()
{
	counter = 0;
	setbeat = 0;
	beat = 0;
}

// create a new blank song
void GlobalData::NewSong()
{
	// if we have other songs in memory already, then create a new blank one on the end
	debug("Adding a song");
	
	if (songdata)
	{
		currentsong->next = new structSongData;
		currentsong = currentsong->next;
		debug("Next song data at: 0x%lx", (u32)currentsong);
	}
	else
	{
		// initialise a fresh song
		songdata = new structSongData;
		currentsong = songdata;	
		debug("New song data at: 0x%lx", (u32)songdata);
	}
	
	debug("currentsong: 0x%lx", (u32)currentsong);
	currentsong->bpm = 180;
	currentsong->name = new char[1];
	currentsong->name[0] = '\0';
	currentsong->loops = NULL;
	currentsong->next = NULL;
}

// delete the current song
void GlobalData::DelSong()
{
	structSongData *songtrav = songdata;
	structSongData *deletesong = NULL;
	
	// if we even have any songs (which we should always have becuase there's at least a blank song)
	if (songtrav)
	{
		debug("Deleting a song.");

		// if the first loop is the selected loop
		if (songtrav == currentsong)
		{
			debug("[First song]");
			
			// set the current song's loops to point to the next one (NULL is ok)
			songdata = songtrav->next;
			
			// remember what we want to delete
			deletesong = currentsong;			
			
			// set the traveller to point at something sane
			songtrav = songdata;
		}
		else
		{
			debug("[Finding song]");
			
			// find the current song
			while (songtrav->next != currentsong)
			{
				songtrav = songtrav->next;
			}
			
			// remember what we want to delete
			deletesong = songtrav->next;
			
			// unlink the song to be deleted
			songtrav->next = songtrav->next->next;
		}
		
		// delete all the loops associated with the deleted song
		while (deletesong->loops)
		{
			DelLoop();
		}

		// set currentsong to be the song we're on now
		currentsong = songtrav;
		
		// delete the name array associated
		delete[] deletesong->name;
		
		// delete the song itself
		delete deletesong;
	}
	
	// if we've run out of songs (our current song is null) then make sure we have one blank song
	if (!songdata)
	{
		NewSong();
	}
}

// find a song by index number
void GlobalData::SetSong(structSongData *whichsong)
{
	currentsong = whichsong;
	debug("Set current song: 0x%lx, '%s', %d", (u32)currentsong, currentsong->name, currentsong->bpm);
	SetCurrentLoop(currentsong->loops);
}

// find a song by index number
void GlobalData::FindSong(u16 whichsong)
{
	u16 counter=0;
	structSongData *songtrav = songdata;
	
	while(counter != whichsong && songtrav)
	{
		songtrav = songtrav->next;
		counter++;
	}
	
	if (songtrav)
	{
		currentsong = songtrav;
		currentloop = currentsong->loops;
		if (currentloop)
		{
			currentnote = currentloop->notes;
		}
	}
}

// change the name of the current song
void GlobalData::SetName(char *inname)
{
	if (currentsong)
	{
		// forget whatever data this char used to be attached to
		delete[] currentsong->name;
		// create a new array of characters one longer
		currentsong->name = new char[strlen(inname) + 1];
		// read in a string
		strncpy(currentsong->name, inname, strlen(inname));
		// make sure the last element is zero
		currentsong->name[strlen(inname)] = '\0';
		debug("set song name to %s", currentsong->name);
	}
}

// set the current loop to be this loop
void GlobalData::SetCurrentLoop(structLoopData *which)
{
	structNoteData *traverse;
	
	debug("Setting new currentloop: 0x%lx", (u32)which);
	currentnote = NULL;
	currentloop = which;
	if (currentloop)
	{
		// find the last note and make it the current one
		traverse = currentloop->notes;
		while (traverse)
		{
			currentnote = traverse;
			traverse = traverse->next;
		}
		debug("Set current loop: 0x%lx, '%s', Notes=0x%lx", (u32)currentloop, currentloop->name, (u32)currentloop->notes);
	}
}

// change the name of the current loop
void GlobalData::SetLoopName(char *inname)
{
	if (currentloop)
	{
		// forget whatever data this char used to be attached to
		delete[] currentloop->name;
		// create a new array of characters one longer
		currentloop->name = new char[strlen(inname) + 1];
		// read in a string
		strncpy(currentloop->name, inname, strlen(inname));
		// make sure the last element is zero
		currentloop->name[strlen(inname)] = '\0';
		debug("set loop name to %s", currentloop->name);
	}
}

// add another loop to this song
void GlobalData::NewLoop()
{
	structLoopData *holder=NULL;
	
	// if we have some loops, then append it to the end
	if (currentsong)
	{
		debug("Adding a loop.");
		
		if (currentsong->loops)
		{
			debug("Appending loop");
			if (currentloop->next)
				holder = currentloop->next;
			currentloop->next = new structLoopData;
			currentloop = currentloop->next;
		}
		else
		{
			debug("First loop");
			// initialise a fresh song
			currentsong->loops = new structLoopData;
			currentloop = currentsong->loops;
		}
		
		// set defaults
		currentloop->name = new char[1];
		currentloop->name[0] = '\0';
		currentloop->sample = 0;
		currentloop->pan = false;
		currentloop->pitch = 1000;
		currentloop->divisions = 1;
		currentloop->notes = NULL;
		currentloop->next = holder;
		
		debug("Loop addresses (base, current): 0x%lx, 0x%lx", (u32)currentsong->loops, (u32)currentloop);
	}
}

// delete a loop from somewhere in this song
void GlobalData::DelLoop()
{
	structLoopData *looptrav = currentsong->loops;
	structLoopData *deleteloop = NULL;
	
	debug("Loop addresses (base, current): 0x%lx, 0x%lx", (u32)looptrav, (u32)currentloop);
	
	// if we even have any loops
	if (looptrav)
	{
		debug("Deleting a loop.");
		
		// if the first loop is the selected loop
		if (looptrav == currentloop)
		{
			debug("[First loop]");
			
			// set the current song's loops to point to the next one (NULL is ok)
			currentsong->loops = looptrav->next;
			
			// remember what we want to delete
			deleteloop = currentloop;
			
			// set the current loop to be the new first loop
			looptrav = currentsong->loops;
		}
		else
		{
			debug("[Finding loop]");
			
			// find the current loop
			while (looptrav->next != currentloop)
			{
				looptrav = looptrav->next;
			}
			
			// remember what we want to delete
			deleteloop = looptrav->next;
			
			// unlink the loop to be deleted
			looptrav->next = looptrav->next->next;
		}
		
		debug("Removing note data.");
		
		// delete all the notes associated with the unused loop
		while (currentloop->notes)
		{
			DelNote();
		}

		// now we can update our current song pointer
		// set currentloop to be the loop we're on now
		currentloop = looptrav;
		
		debug("Set currentloop to 0x%lx", (u32)currentloop);
		
		// delete the name array
		delete[] deleteloop->name;

		// delete the loop itself
		delete deleteloop;
	}
}

// add another note to this loop
void GlobalData::NewNote()
{	
	// if we have some loops, then append it to the end
	if (currentloop)
	{
		debug("Adding a note after 0x%lx", (u32)currentnote);
		
		if (currentloop->notes)
		{
			currentnote->next = new structNoteData;
			currentnote = currentnote->next;
		}
		else
		{
			// initialise a fresh song
			currentloop->notes = new structNoteData;
			currentnote = currentloop->notes;
		}
		
		// set defaults
		currentnote->noteEnd = note_continue;
		currentnote->offset = 0;
		currentnote->pitch = 128;
		currentnote->swing = 0;
		currentnote->next = NULL;
	}
}

// delete a note from the end of this loop
void GlobalData::DelNote()
{
	structNoteData *notetrav = currentloop->notes;
	
	// if we even have a note
	if (notetrav)
	{
		debug("Deleting a note");
		
		if (notetrav->next)
		{
			// find the last note
			while (notetrav->next->next)
			{
				notetrav = notetrav->next;
			}
			
			// if we've selected the last one, select the one before
			if (currentnote == notetrav->next)
			{
				currentnote = notetrav;
			}
			
			// the next note is the last one so delete it
			debug("Deleting 0x%lx", notetrav->next);
			delete notetrav->next;
			
			// set the current ref to NULL so we know we're the end of the list
			notetrav->next = NULL;
		}
		else
		{
			// tell this loop we don't currently have any notes
			debug("Deleting 0x%lx", notetrav);
			delete notetrav;
			currentloop->notes = NULL;
			currentnote = NULL;
		}
	}
	debug("Currentnote: 0x%lx", currentnote);
	debug("First: 0x%lx", currentloop->notes);
	debug("Next: 0x%lx", currentnote->next);
}

// set the beats per minute of this song
void GlobalData::SetBPM(u16 bpm)
{
	currentsong->bpm = bpm;
}

#define NEXTBYTE 	*(u8 *)(SRAM + offset)

// save all songs from memory into the writeable ROM
void GlobalData::SaveSongs()
{
	REG_IME = 0; // disable interrupts

	// pointers for traversing the data
	structSongData *songtrav = songdata;
	structLoopData *looptrav = NULL;
	structNoteData *notetrav = NULL;
	offset = 0;
	debug("Saving Song data Starting at 0x%lx", (u32)songtrav);
	
	// write the magic string
	WriteNumber(magic, sizeof(u16));

	// go through each song
	while (songtrav)
	{		
		// write song name
		WriteString(songtrav->name);
		debug("Song name: %s", songtrav->name);

		// write bpm
		WriteNumber(songtrav->bpm, sizeof(u16));
		debug("Song speed: %d", songtrav->bpm);
		
		// go through each loop in this song
		looptrav = songtrav->loops;
		debug("looptrav initial address: 0x%lx", (u32)looptrav);
		while (looptrav)
		{
			debug("Writing loop %s", looptrav->name);
			
			// write the loop name
			WriteString(looptrav->name);
			// write the sample number
			WriteNumber(looptrav->sample, sizeof(u16));
			// write the panning direction
			WriteNumber(looptrav->pan, sizeof(bool));
			// write the pitch
			WriteNumber(looptrav->pitch, sizeof(u16));
			// write the number of divisions
			WriteNumber(looptrav->divisions, sizeof(u16));

			// go through each note
			notetrav = looptrav->notes;
			while (notetrav)
			{
				debug("Writing note 0x%lx", notetrav);
				
				// write the note end action;
				WriteNumber(notetrav->noteEnd, sizeof(u8));
				// write the beat offset
				WriteNumber(notetrav->offset, sizeof(u8));
				// write the pitch
				WriteNumber(notetrav->pitch, sizeof(u8));
				// write the swing
				WriteNumber(notetrav->swing, sizeof(u8));
				
				// next note
				notetrav = notetrav->next;
			}
			// write the magic string for end-of-notes
			WriteNumber(magic, sizeof(u16));
			
			// next loop
			looptrav = looptrav->next;
			debug("Next loop: 0x%lx", looptrav);
		}
		// write the magic string for end-of-loops
		WriteNumber(magic, sizeof(u16));
				
		// next song
		songtrav = songtrav->next;
	}

	// write the magic string again for end-of-songs
	WriteNumber(magic, sizeof(u16));

	// reset the offset
	offset = 0;

	REG_IME = 1; // enable interrupts
}

// writes a string into the SRAM
void GlobalData::WriteString(char *instr)
{
	// write our letters to disk
	bcopy(instr, (char *)(SRAM + offset), strlen(instr) + 1);
	bcopy((const char*)&zero, (char *)(SRAM + offset + strlen(instr)), 1);
	offset += strlen(instr) + 1;
	debug("Wrote: %s", instr);
	// if (offset % 2) offset++;
}

// reads a string from the SRAM
void GlobalData::ReadString(char **instr)
{
	char buffer[16];

	// forget whatever data this char used to be attached to
	delete[] *instr;
	// create a new array of characters one longer
	*instr = new char[strlen((char *)(SRAM + offset)) + 1];
	// read in a string
	bcopy((char *)(SRAM + offset), buffer, 15);
	strncpy(*instr, buffer, strlen(buffer));
	// make sure the last element is zero
	*instr[strlen(*instr)] = 0;
	offset += strlen(*instr) + 1;
	debug("Read: %s", *instr);
}

// writes a number into the SRAM
void GlobalData::WriteNumber(u32 number, u8 size)
{
	
	// write a bunch
	bcopy((const char*)&number, (char *)(SRAM + offset), size);
	offset += size;
	debug("Wrote: %ld", number);
}

// reads a number from the SRAM
void GlobalData::ReadNumber(void *number, u8 size)
{
	bcopy((const char*)(SRAM + offset), (char *)number, size);
	offset += size;
	debug("Read: %d", *((u16 *)number));
}

// this checks if the current offset contains magic! (doesn't update offset at all)
bool GlobalData::CheckMagic()
{
	u16 checkmagic=0;
	bcopy((const char*)(SRAM + offset), (char *)&checkmagic, sizeof(u16));
	if (checkmagic == magic)
	{
		offset += sizeof(magic);
		return true;
	}
	else
	{
		return false;
	}
}

// load all songs from ROM into memory
void GlobalData::LoadSongs()
{
	u16 checkmagic=0;
	offset = 0;
	
	REG_IME = 0; // disable interrupts
	
	debug("Loading songs.");

	// black = starting
	SetBG(0, 0, 0);
	
	ReadNumber(&checkmagic, sizeof(u16));
	
	// check first if there is any saved data in the databank
	if (checkmagic == magic)
	{
		// until we find the end of the songs
		while (!CheckMagic())
		{
			// red = song
			SetBG(10, 0, 0);
			
			// start a new song
			debug("Creating a new song struct.");
			NewSong();
			
			debug("Reading basic song data.");
			
			// read in the song name
			ReadString(&currentsong->name);
			debug("Song Name: %s", currentsong->name);

			// write bpm
			ReadNumber(&currentsong->bpm, sizeof(u16));
			debug("Song Speed: %d", currentsong->bpm);
			
			// until we find the end of the loops
			while (!CheckMagic())
			{
				// green = loop
				SetBG(0, 10, 0);
				
				// create a new loop
				debug("Creating a new loop struct.");
				NewLoop();
				
				debug("Reading basic loop data.");
				
				// write the loop name
				ReadString(&currentloop->name);
				// write the sample number
				ReadNumber(&currentloop->sample, sizeof(u16));
				// write the panning direction
				ReadNumber(&currentloop->pan, sizeof(bool));
				// write the pitch
				ReadNumber(&currentloop->pitch, sizeof(u16));
				// write the number of divisions
				ReadNumber(&currentloop->divisions, sizeof(u16));
				
				while (!CheckMagic())
				{
					// blue = note
					SetBG(0, 0, 10);
					
					// create a new note
					debug("Creating a new note struct.");
					NewNote();
					
					debug("Reading basic note data.");
					
					// write the note end action;
					ReadNumber(&currentnote->noteEnd, sizeof(u8));
					// write the beat offset
					ReadNumber(&currentnote->offset, sizeof(u8));
					// write the pitch
					ReadNumber(&currentnote->pitch, sizeof(u8));
					// write the swing
					ReadNumber(&currentnote->swing, sizeof(u8));
				}
			}
		}
	}

	// if we don't have a default song set yet
	if (!songdata)
	{
		debug("No load data; creating a new song.");
		NewSong();
	}

	REG_IME = 1; // enable interrupts
}

