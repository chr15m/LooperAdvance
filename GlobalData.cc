// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

#include "GlobalData.hh"

GlobalData::GlobalData()
{
	Init();
}

// initialise everything
void GlobalData::Init()
{
	// initialize our globals
	counter = 0;
	songdata = NULL;
	currentsong = NULL;
	offset = 0;
	magic = 0xABCD;
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
	currentsong->loops = NULL;
	currentsong->next = NULL;
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
void GlobalData::NewLoop()
{
	// if we have some loops, then append it to the end
	if (currentsong->loops)
	{
		currentloop->next = new structLoopData;
		currentloop = currentloop->next;
	}
	else
	{
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
	currentloop->divisions = 0;
	currentloop->notes = NULL;
	currentloop->next = NULL;
}

// delete a loop from this song
void GlobalData::DelLoop()
{
	
}

// add another note to this loop
void GlobalData::NewNote()
{
	// if we have some loops, then append it to the end
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
	currentnote->noteEnd = note_cut;
	currentnote->offset = 0;
	currentnote->pitch = 0;
	currentnote->swing = 0;
	currentnote->next = NULL;
}

// delete a note from the end of this loop
void GlobalData::DelNote()
{
	structNoteData *notetrav = currentloop->notes;
	
	// if we even have a note
	if (notetrav)
	{
/*		// if we only have one note
		if (notetrav == currentnote)
		{
			// will set to null if there's no other note
			currentloop->notes = currentnote->next;
			delete currentnote;
			currentnote = NULL;
		}
		else
		{
			if (notetrav->next = currentnote)
				
			while ((notetrav->next != NULL) && (notetrav->next))
			{
				notetrav = notetrav->next;
			}
			
			if (notetrav->next)
			{
				
			}
		}
*/
	}
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
	// pointers for traversing the data
	structSongData *songtrav = songdata;
	structLoopData *looptrav = NULL;
	structNoteData *notetrav = NULL;
	offset = 0;
	debug("Saving Song data.");

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
		while (looptrav)
		{
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
			if (notetrav)
			{
				while (notetrav)
				{
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
			}
			// write the magic string for end-of-notes
			WriteNumber(magic, sizeof(u16));
			
			// next loop
			looptrav = looptrav->next;
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
}

// writes a string into the SRAM
void GlobalData::WriteString(char *instr)
{
	// write our letters to disk
	bcopy(instr, (char *)(SRAM + offset), strlen(instr) + 1);
	debug("String length: %d", strlen(instr) + 1);
	offset += sizeof(strlen(instr) + 1);
	bcopy((const char*)0xFF, (char *)(SRAM + offset), 1);
	offset += 2;
	// if (offset % 2) offset++;
}

// reads a string from the SRAM
void GlobalData::ReadString(char **instr)
{
	// forget whatever data this char used to be attached to
	delete[] *instr;
	// create a new array of characters one longer
	*instr = new char[strlen((char *)(SRAM + offset)) + 1];
	// read in a string
	strncpy(*instr, (const char*)(SRAM + offset), strlen((char *)SRAM + offset));
	// make sure the last element is zero
	*instr[strlen(*instr)] = 0;
	offset += sizeof(strlen(*instr)) + 1;
	if (offset % 2) offset++;
}

// writes a number into the SRAM
void GlobalData::WriteNumber(u32 number, u8 size)
{
	// write a bunch
	bcopy((const char*)&number, (char *)(SRAM + offset), size);
	offset += size;
}

// reads a number from the SRAM
void GlobalData::ReadNumber(void *number, u8 size)
{
	bcopy((const char*)(SRAM + offset), (char *)number, size);
	offset += size;
}

// load all songs from ROM into memory
void GlobalData::LoadSongs()
{
	u16 checkmagic=0;
	offset = 0;
	
	debug("Loading songs.");
	/// first delete all songs
	
	ReadNumber(&checkmagic, sizeof(u16));
	
	// check first if there is any saved data in the databank
	if (checkmagic == magic)
	{
		// until we find the end of the songs
		while (*(u16 *)(SRAM + offset) != magic)
		{
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
			while (*(u16 *)(SRAM + offset) != magic)
			{
				debug("Raw: %x", *(u16 *)(SRAM + offset + 1));
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
				
				while (*(u16 *)(SRAM + offset) != magic)
				{
					debug("Raw: %x", *(u16 *)(SRAM + offset));
					// create a new note
					debug("Creating a new note struct.");
					NewNote();
					
					debug("Reading basic  data.");
					
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
}

