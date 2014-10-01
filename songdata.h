// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#ifndef _SONGDATA_HH_
#define _SONGDATA_HH_

#include "looper.h"
#include "samples.h"

#define NUMSONGS 7

typedef struct structLoopData
{
	u16 sample;
	bool pan;
	u32 pitch;
	u16 beats;
} structLoopData;

typedef struct structSongData
{
	u16 tempo;
	char *name;
	structLoopData loop[4];
} structSongData;

namespace songs
{
	extern const structSongData data[NUMSONGS];
}

#endif
