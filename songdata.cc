// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "songdata.hh"

namespace songs
{
	const structSongData data[NUMSONGS] = 
	{
		// space ghost
		{
			245,
			"space ghost",
			{
				{SAMPLE_SG_VOX, false, 995, 16},
				{SAMPLE_SG_CHUNK2, true, 510, 8},
				{SAMPLE_SG_CHUNKS, true, 510, 8},
				{SAMPLE_SG_SQBASS, false, 995, 16}
			}
		},
		
		// podfunk
		{
			160,
			"pod funk",
			{
				{SAMPLE_POD_MOO, false, 558, 32},
				{SAMPLE_POD_BASS, true, 641, 16},
				{SAMPLE_POD_SNARX, true, 1278, 16},
				{SAMPLE_PODFUNK, true, 1082, 32}
			}
		},
		
		// contact
		{
			133,
			"contact",
			{
				{SAMPLE_AMEN, false, 1015, 32},
				{SAMPLE_CT_BEAT, true, 1015, 32},
				{NULL, false, 0, 0},
				{NULL, false, 0, 0}
			}
		},
		
		// 180
		{
			180,
			"one eighty",
			{
				{SAMPLE_180_606, false, 812, 8},
				{SAMPLE_180_PHAT, true, 994, 16},
				{SAMPLE_180_DNB, true, 948, 4},
				{SAMPLE_180_SNARE, false, 1000, 2}
			}
		},

		// bizniz
		{
			125,
			"th' bizniz",
			{
				{SAMPLE_BIZ_BASS, false, 1024, 4},
				{SAMPLE_BIZ_SNAX, false, 1021, 16},
				{SAMPLE_BIZ_JAZ, true, 1021, 32},
				{NULL, false, 0, 0}
			}
		},

		// temp psych
		{
			170,
			"temp psych",
			{
				{SAMPLE_PSY_BASS, false, 420, 12},
				{SAMPLE_SG_CHUNKS, true, 440, 7},
				{SAMPLE_PSY_BLOOP, false, 560, 4},
				{SAMPLE_PSY_COOP, false, 502, 24}
			}
		},
		
		// eskimo
		{
			144,
			"eskimo",
			{
				{SAMPLE_ESKIMO_BRK, false, 1474, 8},
				{SAMPLE_ESKI_LOOP, true, 1090, 16},
				{NULL, false, 0, 0},
				{NULL, false, 0, 0}
			}
		},
	};
}
