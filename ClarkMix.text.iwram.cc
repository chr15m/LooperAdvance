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
	
	sizeswitch[0] = BUFF_LARGE;
	sizeswitch[1] = BUFF_SMALL;
	
	buffersize[0] = BUFF_LARGE;
	buffersize[1] = BUFF_LARGE;
	
	// let's wait for the start of the vcount before going
	//while (REG_VCOUNT < 200);
	
	REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
	REG_DMA2CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
	
	REG_TM0CNT = TIMER_ENABLE;
	
	REG_TM1D = 0xFFFF - (BUFF_LARGE);
	REG_TM1CNT = TIMER_ENABLE | TIMER_IRQ_ENABLE | TIMER_CASCADE;
	
	REG_IE = INT_TM1;
	
	REG_IME = INT_ENABLE;
} 

// swap which buffer we're using
void ClarkMix::switchBuffers(void)
{
	if (bufferSwitch == 0)
	{
		curBufA = fifoAbuf1;
		curBufB = fifoBbuf1;
		mixBufA = fifoAbuf0;
		mixBufB = fifoBbuf0;
		bufferSwitch = 1;
	} 
	else
	{
		curBufA = fifoAbuf0;
		curBufB = fifoBbuf0;
		mixBufA = fifoAbuf1;
		mixBufB = fifoBbuf1;
		bufferSwitch = 0;
	}
	
	// make sure the trigger is happening in a sane range not to interrupt on-screen activity
	//buffersize[bufferSwitch] = sizeswitch[REG_VCOUNT == BUFF_UP];
}

// mix together everything down into the buffers
void ClarkMix::mixBuffers(void)
{
	SetBG(31, 0, 0);
	u16 b;
	structSampleList *traverse = samplelist;
	bool initBuffer = true;
	
	// fill the buffer with nice fellows
	// for every sample in our list of managed samples
	while (traverse)
	{
		debugloop("Mixing sample: %s\n", traverse->sample->GetName());
		
		initBuffer = traverse->sample->MixDown(initBuffer, mixBufA, mixBufB, BUFFER_SIZE);
		traverse = traverse->next;
	}
	
	// if all our samples are off, we still want to zero out the buffer
	if (initBuffer)
	{
		// First initialize buffer to 0 - 32-bit operation is faster.
		for (b = 0 ; b < BUFFER_SIZE ; b += 4)
		{
			*(u32*)(mixBufA + b) = 0;
			*(u32*)(mixBufB + b) = 0;
		}
	}
	SetBG(SCREENS_BG_R, SCREENS_BG_B, SCREENS_BG_G);
}

// Interrupt process
void ClarkMix::InterruptProcess(void)
{
 	REG_DMA1CNT &= ~ENABLE_DMA;
	REG_DMA2CNT &= ~ENABLE_DMA;
	
	// REG_TM1D = 0xFFFF - buffersize[bufferSwitch];
	
	switchBuffers();
	
	REG_DMA1SAD = (u32)curBufA;
	REG_DMA2SAD = (u32)curBufB;
	
	REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
	REG_DMA2CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
	
	debug("VCount started mix: %d\n", REG_VCOUNT);
	
	mixBuffers();
	
	debug("VCount finished mix: %d\n", REG_VCOUNT);
	debug("currentBufferA: 0x%lx\n", (u32)curBufA);
	debug("currentBufferB: 0x%lx\n", (u32)curBufB);
	//debug("buffersize: %d\n", buffersize[bufferSwitch]);
	
        REG_IF |= REG_IF;
}

// add a sample to the linked list of samples to be managed
void ClarkMix::Manage(Sample *newsample)
{
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
	
	debug("%s\n", newsample->GetName());
	debug("samples: 0x%lx\n", (u32)samplelist);
	debug("last: 0x%lx\n", (u32)last);
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
		// nice linked list traversal variable name chris!!
		traverse = traverse->next;
	}
}
