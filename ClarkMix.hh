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

#define BUFFER_SIZE 548
#define BUFF_UP 160
#define BUFF_DOWN 161
#define BUFF_SMALL 547
#define BUFF_LARGE 548

// 16khz play back freq
#define PLAYER_FREQ 0xFBFF

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
	
	u16 buffersize[2];
	u16 sizeswitch[2];
	u16 bufferSwitch;
	
	// a pointer to a linked list of all the samples we're currently playing
	structSampleList *samplelist;
	// last one in the linked list
	structSampleList *last;
	
	// internal stuff
	void mixBuffers(void) __attribute__ ((section (".iwram")));
	void switchBuffers(void) __attribute__ ((section (".iwram")));
	
public:
	ClarkMix();

	void Manage(Sample* newsample) __attribute__ ((section (".iwram")));
	void Forget(Sample* which) __attribute__ ((section (".iwram")));

	void InterruptProcess(void) __attribute__ ((section (".iwram")));
};

namespace Audio
{
        extern void AudioCallBack() __attribute__ ((section (".iwram")));
        extern ClarkMix *ClarkMixPtr;
};

#endif
