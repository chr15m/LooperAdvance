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

#include "ClarkMix.hh"

void Audio::AudioCallBack()
{
	if (Audio::ClarkMixPtr)
		((ClarkMix *)Audio::ClarkMixPtr)->InterruptProcess();
}

ClarkMix *Audio::ClarkMixPtr=NULL;

ClarkMix::ClarkMix(void)
{
	u16 i;
	
	Audio::ClarkMixPtr = this;
	IntrTable[4] = &Audio::AudioCallBack;
	
	for (i = 0 ; i < BUFFER_SIZE ; i = i + 4)
	{
		(u32)fifoAbuf0[i] = 0;
		(u32)fifoAbuf1[i] = 0;
		(u32)fifoBbuf0[i] = 0;
		(u32)fifoBbuf1[i] = 0;
	}
	
	// turn on the sound chip
	REG_SOUNDCNT_X = SND_ENABLED;
	
	// make sure sound channels 1-4 are turned off
	REG_SOUNDCNT_L = 0;
	
	REG_TM0CNT = 0;
	REG_DMA1CNT = 0;
	REG_DMA2CNT = 0;
	
	REG_TM0D = PLAYER_FREQ;
	
	curBufA = fifoAbuf0;
	curBufB = fifoBbuf0;
	
	bufferSwitch = 0;
	REG_DMA1SAD = (u32)curBufA;
	REG_DMA1DAD = (u32)REG_FIFO_A;
	REG_DMA2SAD = (u32)curBufB;
	REG_DMA2DAD = (u32)REG_FIFO_B;
	
	// set the direct sound output control register
	REG_SOUNDCNT_H = DSA_OUTPUT_TO_LEFT | DSA_TIMER0 | DSA_FIFO_RESET | DSB_OUTPUT_TO_RIGHT | DSB_TIMER0 | DSB_FIFO_RESET ;
	
	REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
	REG_DMA2CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
	
	REG_TM0CNT = TIMER_ENABLE;
	
	REG_TM1D = 0xFFFF - (BUFFER_SIZE);
	REG_TM1CNT = TIMER_ENABLE | TIMER_IRQ_ENABLE | TIMER_CASCADE;
	
	REG_IE = INT_TM1;
	
	REG_IME = INT_ENABLE;
} 

// swap which buffer we're using
void ClarkMix::switchBuffers(void) {
    if (bufferSwitch == 0) {
        curBufA = fifoAbuf1;
        curBufB = fifoBbuf1;
        mixBufA = fifoAbuf0;
        mixBufB = fifoBbuf0;
        bufferSwitch = 1;
    } else {
        curBufA = fifoAbuf0;
        curBufB = fifoBbuf0;
        mixBufA = fifoAbuf1;
        mixBufB = fifoBbuf1;
        bufferSwitch = 0;
    }
}

void ClarkMix::mixBuffers(void) {
	u16 i;
	u8 sampMixed;
	u8  curvolumea, curvolumeb, curinloop;
	u32 curdatabegin, curdataoffset;
	u32 currentptr;
	u32 curdataend, curloopend;
	u16 curpitch;
    
	// First initialize buffer to 0 - 32-bit operation is faster.
	for (i = 0 ; i < BUFFER_SIZE ; i+=4) {
		*(u32*)(mixBufA + i) = 0;
		*(u32*)(mixBufB + i) = 0;
	}

	for (sampMixed = 0 ; sampMixed < SAMPLES_MIX ; sampMixed ++) {
		// save values in local variables - seems to be faster
		curdatabegin = toMix[sampMixed].data_begin;
		curvolumea = toMix[sampMixed].volume_a;
		curvolumeb = toMix[sampMixed].volume_b;
		curdataend = toMix[sampMixed].data_end;
		curloopend = toMix[sampMixed].loop_end;
		curinloop = toMix[sampMixed].in_loop;
		curdataoffset = toMix[sampMixed].data_offset;
		curpitch = toMix[sampMixed].pitch;

		// FIFO A & B mixing...
		for (i = 0 ; i < BUFFER_SIZE ; i++) {
			currentptr = (curdatabegin + (curdataoffset >> 8));
			if ((currentptr >= curloopend) && curinloop) curdataoffset = toMix[sampMixed].loop_begin - curdatabegin;
			if (currentptr >= curdataend) break ;
			*(mixBufA + i) += (*(s8 *)currentptr * curvolumea) >> 8;
			*(mixBufB + i) += (*(s8 *)currentptr * curvolumeb) >> 8;
			curdataoffset += curpitch;			
		}

		// update the mix pointer.
		toMix[sampMixed].data_offset = curdataoffset;
	}

}

void ClarkMix::InterruptProcess(void) {	
 	REG_DMA1CNT &= ~ENABLE_DMA;
	REG_DMA2CNT &= ~ENABLE_DMA;

	switchBuffers();
	REG_DMA1SAD = (u32)curBufA;
	REG_DMA2SAD = (u32)curBufB;
 
	REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
	REG_DMA2CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
	mixBuffers();
        REG_IF |= REG_IF;
}

void ClarkMix::playSample(u32 data_begin, u32 data_end, u32 loop_begin, u32 loop_end, u8 volume, s8 panning, u8 bank, u16 pitch) {
    toMix[bank].data_begin = data_begin;
    toMix[bank].data_end = data_end;
    toMix[bank].volume_a = ((128 - panning) * volume) >> 8;
    toMix[bank].volume_b = ((128 + panning) * volume) >> 8;
    toMix[bank].volume = volume;
    toMix[bank].panning = panning;
    toMix[bank].loop_begin = loop_begin;
    toMix[bank].loop_end = loop_end;
    toMix[bank].in_loop = 1;
    toMix[bank].data_offset = 0;
    toMix[bank].pitch = pitch;
}

void ClarkMix::stopSample(u8 bank) {
	toMix[bank].in_loop = 0;
}

void ClarkMix::setVolume(u8 volume, u8 bank) {
	toMix[bank].volume_a = ((128 - toMix[bank].panning) * volume) >> 8 ;
	toMix[bank].volume_b = ((128 + toMix[bank].panning) * volume) >> 8;
	toMix[bank].volume = volume;
}

void ClarkMix::setPanning(s8 panning, u8 bank) {
	toMix[bank].volume_a = ((128 - panning) * toMix[bank].volume) >> 8 ;
	toMix[bank].volume_b = ((128 + panning) * toMix[bank].volume) >> 8;
	toMix[bank].panning = panning;
}

void ClarkMix::setPitch(u16 pitch, u8 bank) {
	toMix[bank].pitch = pitch;
}

// add a sample to the linked list of samples to be managed
void ClarkMix::Manage(Sample *newsample)
{
	newsample->SetChunkSize(BUFFER_SIZE);
	/*
	dprintf("%s\n", newsample->GetName());
	dprintf("samples: 0x%lx\n", (u32)samplelist);
	dprintf("last: 0x%lx\n", (u32)last);
	*/
	if (samplelist)
	{
		last->next = new structSampleList();
		last->next->next = NULL;
		last->next->sample = newsample;
		last = last->next;
	}
	else
	{
		samplelist = new structSampleList();
		samplelist->next = NULL;
		samplelist->sample = newsample;
		last = samplelist;
	}
	/*
	dprintf("samplelist: 0x%lx\n", (u32)samplelist);
	dprintf("last: 0x%lx\n", (u32)last);
	dprintf("\n");
	*/
}

// forget about a sample which we were managing (drop it from the linked list)
void ClarkMix::Forget(Sample *which)
{
	structSampleList *traverse = samplelist;
	structSampleList *tmpptr = NULL;
	
	while (traverse)
	{
		tmpptr = NULL;
		// if it's our first one, delete the head
		if (traverse->sample == which)
		{
			samplelist = traverse->next;
			delete traverse;
		}
		// if it's the next on in line, pop it out
		else if (traverse->next->sample == which)
		{
			tmpptr = traverse->next->next;
			delete traverse->next;
			traverse->next = tmpptr;
			// if we've deleted the last one, make sure we reset last
			if (traverse->next == NULL)
				last = traverse;
		}
		traverse = traverse->next;
	}
}
