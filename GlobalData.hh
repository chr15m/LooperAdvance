// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#ifndef _GLOBALDATA_HH_
#define _GLOBALDATA_HH_

#include "looper.hh"
#include "samples.h"

// the different types of things that can happen to notes at their end (before timeslot end)
typedef enum noteType { note_cut, note_continue, note_loop, note_bounce };

// this holds extra data relevant to samples
typedef struct structSampleData
{
	u16 sample; 	// which sample it is
	u16 beats;	// how many beats in it's default length (this is reasonably subjective)
	char *name;	// a name for this sample (pointer to samplenames.cc)
} structSampleData;

// this holds a single note
typedef struct structNote
{
	noteType noteEnd;	// what to do at the end of the note
	u8 offset;		// which beat in the loop to offset to
	u8 pitch;		// what pitch to play this segment at (this is note-relative to base pitch)
	structNote *next;	// next note in the linked list of notes
} structNote;

typedef struct structLoopData
{
	char *name;		// name for our loop (user can modify this)
	u16 sample;		// which sample we're using
	bool pan;		// which way to pan it
	u32 pitch;		// base pitch to play it at (1000 = normal pitch for the sample)
	u16 divisions;		// how many divisions to divide the loop up into
	structNote *notes;	// linked list of notes to play
	structLoopData *next;	// next loop in the list of loops for this song
} structLoopData;

typedef struct structSongData
{
	u16 bpm;		// tempo to play this song at
	char *name;		// name of this song (user modifiable)
	structLoopData *loop;	// linked list of loops in this song
	structSongData *next;	// next song in the linked list of all songs
} structSongData;

//
//	This class holds and manipulates global data
//

class GlobalData
{
private:
	
public:
	u32 counter;
	struct structSongData *songdata;
	struct structSongData *currentsong;
	struct structLoopData *currentloop;
	struct structNote *currentnote;
	
	GlobalData();
};

extern GlobalData globals;

#endif // GLOBALDATA
