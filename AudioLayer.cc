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

void Audio::AudioCallBack()
{
	// turn off the timer callback so it doesn't override itself
	REG_IE &= ~INT_TM0;
	
	if (Audio::AudioLayerPtr)
		((AudioLayer *)Audio::AudioLayerPtr)->Interrupt();
	
	// turn the interrupt back on and clear interrupt flags
	REG_IF &= ~INT_TM0;
	REG_IE |= INT_TM0;
}

AudioLayer *Audio::AudioLayerPtr=NULL;

AudioLayer::AudioLayer(u16 mixlength)
{
	Audio::AudioLayerPtr = this;
	IntrTable[3] = &Audio::AudioCallBack;
	
	mixbank = new s16[mixlength];
	banksize = mixlength;

	samples = last = NULL;
	
	// turn on all the sounds etc
	REG_SOUNDCNT_H = DSA_OUTPUT_RATIO_100 | DSA_FIFO_RESET | DSA_OUTPUT_TO_BOTH | DSA_TIMER0; 	//DirectSound A + fifo reset + max volume to L and R
	REG_SOUNDCNT_X = SND_ENABLED; 	//turn sound chip on
	
	REG_IE = INT_TM0; 	//enable timer 0 irq
	REG_IME = 1; 	//enable interrupts
	
	//REG_TM0D = 0xffff;
	// TODO: figure out the correct frequency of playback for 22050
	// REG_TM0D = 0xf400;
	REG_TM0D = 0xfd50;
	
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
	
	if (samples)
	{
		last->next = new structSampleList();
		last->next->next = NULL;
		last->next->sample = newsample;
		last = last->next;
	}
	else
	{
		samples = new structSampleList();
		samples->next = NULL;
		samples->sample = newsample;
		last = samples;
	}

}

void AudioLayer::Forget(Sample *which)
{
	structSampleList *traverse = samples;
	structSampleList *tmpptr = NULL;
	
	while (traverse)
	{
		tmpptr = NULL;
		// if it's our first one, delete the head
		if (traverse->sample == which)
		{
			samples = traverse->next;
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

void AudioLayer::Interrupt()
{
//	Sample *next = samples;
//	s8 *chunkdata = 0;
//	u16 i = 0;
	
	// initialise our mixbank to zeroes
//	for (i=0; i<banksize; i++);
//	{
//		mixbank[i] = 0;
//	}
	
	//Audio::counter++;
	//if (Audio::i==1)
	//	dprintf("%d\n", Audio::counter);
	
	//while (next)
	//{
		// get the next four bytes of data from this sample
		//chunkdata = next->GetChunk();
//		for (i=0; i<banksize; i++);
//		{
//			mixbank[i] += chunkdata[i];
//		}
		
//		if (mixbank[i] > 255)
//			mixbank[i] = mixbank[i] >> 1;
		
	//	REG_SGFIFOA = mixbank[0] + (mixbank[1] << 8) + (mixbank[2] << 16) + (mixbank[3] << 24);
	
	//chunkdata = samples->GetChunk();
	
	REG_DMA1SAD = (u32)samples->sample->GetChunk();
	REG_DMA1DAD = (u32)&REG_SGFIFOA;
	REG_DM1CNT_L = 4;
	REG_DM1CNT_H = DMA32NOW | DMA_ENABLE;
	
//	for (i = 0; i < banksize; i++)
//	{
//		mixbank[i] = chunkdata[i];
		//dprintf("%d ", mixbank[i]);
		//dprintf("%d ", i);
//		if (mixbank[i] > 255) mixbank[i] = 255;
//	}
	
	//REG_SGFIFOA = chunkdata[0] + (chunkdata[1] << 8) + (chunkdata[2] << 16) + (chunkdata[3] << 24);
	//REG_SGFIFOA = mixbank[0] + (mixbank[1] << 8) + (mixbank[2] << 16) + (mixbank[3] << 24);
	
		// jump to the next sample in the queue
		//next = next->next;
	//	next = NULL;
		
	//	dprintf("a\n");
	//}

	//dprintf("done\n");
	
	//clear the interrupt(s)
	//REG_IF |= REG_IF;
	
	//dprintf("\n");
}
