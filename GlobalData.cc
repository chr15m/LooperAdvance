// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

#include "GlobalData.hh"

GlobalData::GlobalData()
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
	
	// write the magic string
	bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));
	offset += sizeof(u16);
	
	// go through each song
	while (songtrav)
	{
		// write song name
		
		// write bpm
		
		// go through each loop in this song
		looptrav = songtrav->loops;
		if (looptrav)
		{
			while (looptrav)
			{
				// write the loop name
				// write the sample number
				// write the panning direction
				// write the pitch
				// write the number of divisions
				// go through each note
				notetrav = looptrav->notes;
				if (notetrav)
				{
					while (notetrav)
					{
						// write the note end action;
						// write the beat offset
						// write the pitch
						// write the swing
						
						// write the magic string for end-of-notes
						bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));
						offset += sizeof(u16);
						
						// next note
						notetrav = notetrav->next;
					}
				}
				else
				{
					// write the magic string for end-of-notes
					bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));
					offset += sizeof(u16);
				}
				
				// write the magic string for end-of-loops
				bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));
				offset += sizeof(u16);
				
				// next loop
				looptrav = looptrav->next;
			}
		}
		else
		{
			// write the magic string for end-of-loops
			bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));
			offset += sizeof(u16);
		}
		
		// write the magic string for end-of-song
		bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));
		offset += sizeof(u16);		
		
		// next song
		songtrav = songtrav->next;
	}
	
	// write the magic string again for end-of-songs
	bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));	
	
	// reset the offset
	offset = 0;
}

// writes a string into the SRAM
void GlobalData::WriteString(char *str)
{
	// write our letters to disk
	bcopy(str, (char *)(SRAM + offset), sizeof(str));
	offset += sizeof(str);
	if (offset % 2) offset++;
}

// reads a string from the SRAM
void GlobalData::ReadString(char **str)
{
	// forget whatever data this char used to be attached to
	delete *str;
	// create a new array of characters one longer
	*str = new char[strlen((char *)(SRAM + offset)) + 1];
	// read in a string
	strncpy(*str, (const char*)(SRAM + offset), strlen((char *)SRAM + offset));
	offset += sizeof(strlen(*str)) + 1;
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
	// check first if there is any saved data in the databank
	if (*(u16 *)(SRAM) == 0xABCD)
	{
		
	}
	// no saved data, so set up an initial blank song with a save
	else
	{
		NewSong();
	}
}

