// this version of clarkmix has been heavily modified for use in Looper Advance
// (it's been translated to C++ and the core mixing routine has been re-written)
// thanks to Mathieu Brethes for the original code

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

// 547 - VCOUNT goes down slow (14)
// 548 - VCOUNT goes up fast (one or two every count)

#define BUFFER_SIZE 256

// pow(2, 14) = 16384

// python: hex(pow(2,16) - pow(2,24) / freq)

// freq = pow(2, 14) = 16384 Hz
#define PLAYER_FREQ 0xFC00
#define PLAYER_ACTUAL_FREQ 16384

// 16kHz:
// #define PLAYER_FREQ 0xFBFF

// 22050:
// #define PLAYER_FREQ 0xFD06

#include "gba.h"
#include "typedefs.h"
#include "directsound.h"
#include "stdio.h"
#include "helpers.h"
#include "Sample.hh"

extern void (*IntrTable[])();

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
	
	bool mix;
	u16 bufferSwitch;
	
	// a pointer to a linked list of all the samples we're currently playing
	structSampleList *samplelist;
	// last one in the linked list
	structSampleList *last;
	u8 numberOfSamples;
	
	// internal stuff
	void mixBuffers(void) __attribute__ ((section (".iwram")));
	
public:
	ClarkMix();
	
	// this does the actual work of mixing fresh buffers full
	void DoMix(void) __attribute__ ((section (".iwram")));

	void Manage(Sample* newsample) __attribute__ ((section (".iwram")));
	void Forget(Sample* which) __attribute__ ((section (".iwram")));

	void InterruptProcess(void) __attribute__ ((section (".iwram")));
};

namespace Audio
{
        extern void AudioCallBack() __attribute__ ((section (".iwram")));
        extern ClarkMix *ClarkMixPtr;
	extern u8 mixDownSizes[32];
};

#endif
