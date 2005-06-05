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

// These are the amounts to rotate down by instead of doing a divide
u8 Audio::mixDownSizes[32] = {1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

ClarkMix *Audio::ClarkMixPtr=NULL;

ClarkMix::ClarkMix(void)
{
	u16 i;
	
	Audio::ClarkMixPtr = this;
	// IntrTable[4] = &Audio::AudioCallBack;
	
	for (i = 0 ; i < BUFFER_SIZE ; i = i + 4)
	{
		(u32)fifoAbuf0[i] = 0;
		(u32)fifoAbuf1[i] = 0;
		(u32)fifoBbuf0[i] = 0;
		(u32)fifoBbuf1[i] = 0;
	}
	
	numberOfSamples = 0;
	
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
	
	// REG_TM1D = 0xFFFF - BUFFER_SIZE;
	// REG_TM1D = 0xFFFF - 0x04;
	REG_TM1D = 0x0000;
	// REG_TM1D = 0xFFFF;
	REG_TM1CNT = TIMER_ENABLE | TIMER_IRQ_ENABLE | TIMER_CASCADE;

	// REG_IE = INT_TM1;
	
	REG_IME = INT_ENABLE;
	DoRunInterrupt = false;
	upTo = 0;
} 

// Interrupt process
void ClarkMix::InterruptProcess(void)
{
	// dprintf("Int REG_VCOUNT: %d\n", REG_VCOUNT);
	
	// turn off interrupts
	// REG_IME = 0;
	
	if (DoRunInterrupt)
	{
		upTo = REG_TM1D;
		u16 newstart = 0;
		
		// stop the cascade (sample) timer
		REG_TM1CNT = 0;
		REG_TM2CNT = 0;
		
		// start the cascade timer again (when timer 0 starts again)
		// REG_TM1D = 0xFFFF - BUFFER_SIZE;
		// REG_TM1D = 0xFFFF - 0x04;
		REG_TM1D = 0x0000;
		// REG_TM1D = 0xFFFF;
		REG_TM1CNT = TIMER_ENABLE | TIMER_IRQ_ENABLE | TIMER_CASCADE;
		
		REG_DMA1CNT &= ~ENABLE_DMA;
		REG_DMA2CNT &= ~ENABLE_DMA;
		
		if (bufferSwitch == 0)
		{
			curBufA = fifoAbuf1;
			curBufB = fifoBbuf1;
			mixBufA = fifoAbuf0;
			mixBufB = fifoBbuf0;
			bufferSwitch = 1;
			//dprintf("---\nSwitch:\nmixbuf = 0x%lx\ncurbuf=0x%lx\n---\n", (u32)fifoAbuf0, (u32)fifoAbuf1);
		} 
		else
		{
			curBufA = fifoAbuf0;
			curBufB = fifoBbuf0;
			mixBufA = fifoAbuf1;
			mixBufB = fifoBbuf1;
			bufferSwitch = 0;
			//dprintf("---\nSwitch:\nmixbuf = 0x%lx\ncurbuf=0x%lx\n---\n", (u32)fifoAbuf1, (u32)fifoAbuf0);
		}
		
		newstart = BUFFER_OVERLAP - (BUFFER_SIZE - upTo);
		// dprintf("abcdabcdabcdabcdabcdabcdabcdabcdabcdabcd\n");
		// while (REG_TM1D == upTo);
		// make sure we don't interrupt ourself
		while (REG_TM1D < 3);
		// u16 x = REG_TM1D;
		
		// we've got to start reading from where we read up to (there is some overlap in buffer mixing)
		REG_DMA1SAD = (u32)curBufA + newstart;
		REG_DMA2SAD = (u32)curBufB + newstart;
		
		REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
		REG_DMA2CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT ;
		
		// clear the interrupt flag
		REG_IF |= REG_IF;
		
		// dprintf("Upto: %d\n", upTo);
		
		DoRunInterrupt = false;
		
		// dprintf("thing %d\n", x);
	}
	
	// turn on interrupts
	//REG_IME = 1;
}

void ClarkMix::DoMix()
{
	u32 b = 0;
	structSampleList *traverse = NULL;
	Sample *current = NULL;
	s8 *myBufA = mixBufA;
	s8 *myBufB = mixBufB;
	u16 trader = NULL;
	u32 tmpleft = 0;
	u32 tmpright = 0;
	
	// make sure we only do a mix if we have non-null buffers assigned
	if (myBufA)
	{
		// build a linked list of the data for each sample we're mixing
		traverse = samplelist;
		// for every stream we're mixing
		// rewind by the buffer_overlap amount
		while (traverse)
		{
			current = traverse->sample;
			
			if (current->IsPlaying())
			{
				current->Rewind(BUFFER_OVERLAP, BUFFER_SIZE);
			}
			traverse = traverse->next;
		}
		
		/*if (numberOfSamples)
		{
			for (b = 0; b < BUFFER_SIZE; b++)
			{
				tmpleft = 0;
				tmpright = 0;
				
				traverse = samplelist;
				while (traverse)
				{
					current = traverse->sample;
					
					if (current->IsPlaying())
					{
						trader = current->GetSamples();
						//tmpleft += trader >> 8;
						//tmpright += trader && 0xFF;
						//trader = 0;
					}
					traverse = traverse->next;
				}
				
				*((u8 *)myBufA + b) = tmpleft / numsamples;
				*((u8 *)myBufB + b) = tmpright / numsamples;
			}
		}*/
		
		// dprintf("---\nmybuf = 0x%lx\n---\n", (u32)mixBufA);
		
		// dprintf("Mix REG_VCOUNT: %d\n", REG_VCOUNT);
		
		// for some reason DMA copy doesn't work in here
		// if all our samples are off, we still want to zero out the buffer
		for (b = 0; b < (BUFFER_SIZE>>2); b++)
		{
			*((u32 *)myBufA + b) = 0;
			*((u32 *)myBufB + b) = 0;
		}
		
		/* new mix algorith:
		// build a linked list of the data for each sample we're mixing
		traverse = samplelist;
		// for every stream we're mixing
		while (traverse)
		{
			current = traverse->sample;
			
			if (current->IsPlaying())
			{
				// debug("Mixing sample: %s\n", traverse->sample->GetName());
				// get the stream block and add it to our linked list of stream blocks
				if (!top)
				{
					last = top = current->sample->GetStream(BUFFER_SIZE);
				}
				else
				{
					last->next = current->sample->GetStream(BUFFER_SIZE);
					last = last->next;
				}
			}
			traverse = traverse->next;
		}
		
		// rewind us to the first stream
		last = top;
		
		// for each frame of the buffer
		while (last)
		{
			// create 16 bit mix variable
			u16 mixdown = 0x0000;
			// for each stream
				// get the current byte at sample pointer rotated down 8 bits
				// add jump size to the sample pointer
				// if the sample pointer is past sample finish, set it to sample start
			// divide sixteen bit mixed variable by the number of streams we have
			// add 16 bit mix to the next frame of our buffer
			last = last->next;
		}
		
		// after mixing free up all the streams
		*/
		
		/*
		u32 chunkR = 0;
		u16 b;
		s8 *dataptr = (s8 *)sampledata->data;
		s8 val = 0;
		
		chunkR = (nextchunk += velocity) >> 8;
		
		// fill up our buffers byte by byte
		val = dataptr[chunkR] >> volume >> mixshifter;
		mixBufA[b] += val >> panshift[0];
		mixBufB[b] += val >> panshift[1];
		*/
		
		traverse = samplelist;
		// fill the buffer with nice fellows
		// for every sample in our list of managed samples
		while (traverse)
		{
			// debugloop("Mixing sample: %s\n", traverse->sample->GetName());
			traverse->sample->MixDown(myBufA, myBufB, BUFFER_SIZE, Audio::mixDownSizes[numberOfSamples - 1]);
			//left = traverse->sample->GetSamples() & 0x00FF;
			//right = (traverse->sample->GetSamples() & 0xFF00) >> 8;
			
			traverse = traverse->next;
		}
		
		//SetBG(SCREENS_BG_R, SCREENS_BG_B, SCREENS_BG_G);
	}
	
	DoRunInterrupt = true;
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
	
	numberOfSamples++;
	
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
			traverse = NULL;
			numberOfSamples--;
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
			numberOfSamples--;
		}
		if (traverse)
			traverse = traverse->next;
		else
			traverse = samplelist;
	}
}
