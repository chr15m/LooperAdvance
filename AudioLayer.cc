/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

// this is the interface to the audio hardware

#include "AudioLayer.hh"

AudioLayer::AudioLayer(u16 mixlength)
{
	s16 newmixbank[mixlength];
	
	mixbank = newmixbank;
	banksize = mixlength;

	samples = NULL;
	
	// turn on all the sounds etc
	REG_SOUNDCNT_H = DSA_OUTPUT_RATIO_100 | DSA_FIFO_RESET | DSA_OUTPUT_TO_BOTH | DSA_TIMER0; 	//DirectSound A + fifo reset + max volume to L and R
	REG_SOUNDCNT_X = SND_ENABLED; 	//turn sound chip on
	
	REG_IE = INT_TM0; 	//enable timer 0 irq
	REG_IME = 1; 	//enable interrupts
	
	// REG_TM0D = 0xffff;
	// TODO: figure out the correct frequency of playback for 22050
	REG_TM0D = 0xf400;
	
	//REG_TM0CNT = 0x00C3;	//enable timer at CPU freq/1024 +irq =16384Khz sample rate
	REG_TM0CNT = TIMER_IRQ_ENABLE | TIMER_ENABLE | TIMER_FREQ_SYSTEM;
}

AudioLayer::~AudioLayer()
{
	delete[] mixbank;
}

void AudioLayer::Manage(Sample *newsample)
{
	newsample->SetChunkSize(banksize);
}

void AudioLayer::AudioInterrupt()
{
	Sample *next = samples;
	s8 *chunkdata = 0;
	
	//u32 *data = (u32 *)samples[0].data;
	//u8 *data = (u8 *)samples[0].data;
	
	//if(!(iNextSample & 3))
	// REG_SGFIFOA = (data[iNextSample]); // + (data[iNextSample + 1] >> 16) + (data[iNextSample + 2] >> 8) + (data[iNextSample + 3]);
	
	//REG_SGFIFOA = data[iNextSample] + (data[iNextSample + 1] << 8) + (data[iNextSample + 2] << 16) + (data[iNextSample + 3] << 24);
	//REG_SGFIFOA = (data[iNextSample] << 16) + (data[iNextSample + 1] << 24) + (data[iNextSample + 2]) + (data[iNextSample + 3] << 8);
	//REG_SGFIFOA = (data[iNextSample] << 24) + (data[iNextSample + 1] << 16) + (data[iNextSample + 2] << 8) + data[iNextSample + 3];
	//REG_SGFIFOA = (data[iNextSample] << 8) + (data[iNextSample + 1]) + (data[iNextSample + 2] << 24) + (data[iNextSample + 3] << 16);

	//REG_SGFIFOA = data[iNextSample + 3] + (data[iNextSample + 2] << 8) + (data[iNextSample + 1] << 16) + (data[iNextSample] << 24);
	//REG_SGFIFOA = (data[iNextSample + 3] << 16) + (data[iNextSample + 2] << 24) + (data[iNextSample + 1]) + (data[iNextSample] << 8);
	//REG_SGFIFOA = (data[iNextSample + 3] << 24) + (data[iNextSample + 2] << 16) + (data[iNextSample + 1] << 8) + data[iNextSample];
	//REG_SGFIFOA = (data[iNextSample + 3] << 8) + (data[iNextSample + 2]) + (data[iNextSample + 1] << 24) + (data[iNextSample] << 16);
	
	//REG_SGFIFOA = data[iNextSample];
	
	// bad
	// REG_SGFIFOA = (data[iNextSample] >> 24) + (data[iNextSample + 1] >> 16) + (data[iNextSample + 2] >> 8) + data[iNextSample + 3];
	// REG_SGFIFOA = data[iNextSample] + (data[iNextSample + 1] >> 8) + (data[iNextSample + 2] >> 16) + (data[iNextSample + 3] >> 24);
	
	//REG_DMA1SAD = (u32)&data[iNextSample];
	//REG_DMA1DAD = (u32)&REG_SGFIFOA;
	//REG_DMA1CNT = 1 | DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_32;
	
	// iNextSample += 4;

	// we're bitshifting the value of the u32 iNextSample because we want to multiply it by four (four bytes at a time)
	//if(iNextSample > samples[0].length)
	//{
		//sample finished!
		//REG_TM0CNT=0; //disable timer 0
	//	iNextSample = 0;
	//	dprintf("Loop at %ld\n", samples[0].length);
	//}
	
	u16 i;
	
	// initialise our mixbank to zeroes
	for (i=0; i<banksize; i++);
	{
		mixbank[i] = 0;
	}
	
	while (next)
	{
		// get the next four bytes of data from this sample
		chunkdata = next->GetChunk();
		for (i=0; i<banksize; i++);
		{
			mixbank[i] += chunkdata[i];
		}
		
		REG_SGFIFOA = mixbank[0] + (mixbank[1] << 8) + (mixbank[2] << 16) + (mixbank[3] << 24);
		
		// jump to the next sample in the queue
		//next = next->next;
	}
	
	//clear the interrupt(s)
	REG_IF |= REG_IF;
}
