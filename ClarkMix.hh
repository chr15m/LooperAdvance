/* The Clarkinou Sound System main mixer header.
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

// This is a heavily modified version of ClarkMix (the clarkinou mixer)
// This has been ported to C++ and the core mixing routines re-written

#ifndef __CLARKMIX_HH__
#define __CLARKMIX_HH__

#define BUFFER_SIZE 640   // 1/25 sec buffer.
#define PLAYER_FREQ 0xFBFF // 16khz play back freq
#define SAMPLES_MIX  4

#include "gba.h"
#include "typedefs.h"
#include "directsound.h"

#define DONT_LOOP 0xFFFFFFFF

// internal stuff
void InterruptProcess(void);
void mixBuffers(void);
void switchBuffers(void);


void InitSystem();

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

#endif
