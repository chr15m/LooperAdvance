// this version of clarkmix has been heavily modified for use in Looper Advance
// (it's been translated to C++ and the core mixing routine has been re-written)

// $Id$

/* The Clarkinou Sound System main mixer source code.
 * Copyright (C) 2003 by Mathieu Brethes
 *
 * Contact : thieumsweb@free.fr
 * Website : http://thieumsweb.free.fr/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

// Changes copyright 2004 by Chris McCormick

#ifndef __CLARKMIX_HH__
#define __CLARKMIX_HH__

// 1/25 sec buffer.
#define BUFFER_SIZE 640
// 16khz play back freq
#define PLAYER_FREQ 0xFBFF
#define SAMPLES_MIX  4

#include "gba.h"
#include "typedefs.h"
#include "directsound.h"
#include "stdio.h"
#include "Sample.hh"

#define DONT_LOOP 0xFFFFFFFF

extern void (*IntrTable[])();

// memory structure for a given sample.
struct sample {
    u32 data_begin;
    u32 data_end;
    u32 loop_begin;
    u32 loop_end;
    u8 volume_a; // internal use
    u8 volume_b; // internal use
    u8 volume;
    s8 panning;
    u8 in_loop;
    u16 pitch;   // used for FP 8.8 calculations. 0x100 is 1.
    u32 data_offset; // used for fixed-point 24.8 calculations.
};

class ClarkMix
{
private:
	s8 fifoAbuf0[BUFFER_SIZE];
	s8 fifoAbuf1[BUFFER_SIZE];
	s8 fifoBbuf0[BUFFER_SIZE];
	s8 fifoBbuf1[BUFFER_SIZE];
	
	s8* curBufA;
	s8* curBufB;
	s8* mixBufA;
	s8* mixBufB;
	
	struct sample toMix[SAMPLES_MIX];
	
	u16 bufferSwitch;
	
        // a pointer to a linked list of all the samples we're currently playing
        structSampleList *samplelist;
        // last one in the linked list
        structSampleList *last;
	
	// internal stuff
	void mixBuffers(void);
	void switchBuffers(void);
	
public:
	ClarkMix();

	void Manage(Sample* newsample);
	void Forget(Sample* which);

	void InterruptProcess(void);

	/* Play a sample */
	void playSample(u32 data_begin, u32 data_end, u32 loop_begin, u32 loop_end, u8 volume, s8 panning, u8 bank, u16 pitch);
	void stopSample(u8 bank);
	
	/* Volume goes from 0 (nil) to 255 (full volume) */
	void setVolume(u8 volume, u8 bank);
	
	/* Panning goes from -128 (full left) to 127 (full right) */
	void setPanning(s8 panning, u8 bank);
	
	/* Pitch goes from 0x0001 (very slow) to 0xFFFF (very fast).
	 * Effective values can be found in clarkfreqs.h
	 */
	void setPitch(u16 pitch, u8 bank);
};

namespace Audio
{
        extern void AudioCallBack();
        extern ClarkMix *ClarkMixPtr;
};

#endif
