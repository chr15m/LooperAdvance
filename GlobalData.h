/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: GlobalData.hh,v 1.16 2004/04/08 06:09:42 chrism Exp $

******************************************************/


// Represents a single loop-page

#ifndef _GLOBALDATA_HH_
#define _GLOBALDATA_HH_

#include "looper.h"
#include "samples.h"

// the different types of things that can happen to notes at their end (before timeslot end)
typedef enum noteType { note_continue, note_cut, note_loop, note_feed, note_stretch };

// this holds extra data relevant to samples
typedef struct structSampleData
{
	u16 sample; 	// which sample it is
	u16 beats;	// how many beats in it's default length (this is reasonably subjective)
	char *name;	// a name for this sample (pointer to samplenames.cc)
} structSampleData;

// this holds a single note
typedef struct structNoteData
{
	noteType noteEnd;	// what to do at the end of the note
	u8 offset;		// which beat in the loop to offset to
	u8 pitch;		// what pitch to play this segment at (this is note-relative to base pitch)
	u8 swing;		// fraction of 255 to swing this note by
	structNoteData *next;	// next note in the linked list of notes
} structNoteData;

// pointer to a note (so we can declare an array of pointers in Loop.cc)
typedef structNoteData *ptrNoteData;

typedef struct structLoopData
{
	char *name;		// name for our loop (user can modify this)
	u16 sample;		// which sample we're using
	bool pan;		// which way to pan it
	u16 pitch;		// base pitch to play it at (1000 = normal pitch for the sample)
	u16 divisions;		// how many divisions to divide the loop up into
	structNoteData *notes;	// linked list of notes to play
	structLoopData *next;	// next loop in the list of loops for this song
} structLoopData;

typedef struct structSongData
{
	char *name;		// name of this song (user modifiable)
	u16 bpm;		// tempo to play this song at
	structLoopData *loops;	// linked list of loops in this song
	structSongData *next;	// next song in the linked list of all songs
} structSongData;

//
//	This class holds and manipulates global data
//

class GlobalData
{
private:
	u32 offset;
	u16 magic;
	u8 zero;
	u8 full;

public:
	u32 counter;
	u32 beat;	// this is the beat which increments ever time the counter flips over
	
	structSongData *songdata;
	structSongData *currentsong;
	structLoopData *currentloop;
	structNoteData *currentnote;

	GlobalData();
	void Init();
	
	void Tick();
	void Reset();
	
	void SetSong(structSongData *whichsong);
	void FindSong(u16 whichsong);
	void SetName(char *name);
	void SetLoopName(char *name);
	void SetBPM(u16 bpm);
	void SetCurrentLoop(structLoopData *which);

	void NewSong();
	void DelSong();
	void NewLoop();
	void DelLoop();
	void NewNote();
	void DelNote();
	
	void SaveSongs();
	void LoadSongs();
	
	void WriteString(char *str);
	void WriteNumber(u32 number, u8 size);
	void ReadString(char **str);
	void ReadNumber(void *number, u8 size);
	bool CheckMagic();
	
	inline void SetLoop(structLoopData *which)
	{
		currentloop = which;
	}
};

extern GlobalData *globals;
extern const u32 frequency[12];

#endif // GLOBALDATA
