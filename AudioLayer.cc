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
	
	//if (Audio::AudioLayerPtr)
	//	((AudioLayer *)Audio::AudioLayerPtr)->Interrupt();
}

AudioLayer *Audio::AudioLayerPtr=NULL;

AudioLayer::AudioLayer()
{	
	/* ********************** */
	/* real thing starts here */
	/* ********************** */


	//Audio::AudioLayerPtr = this;
	//IntrTable[4] = &Audio::AudioCallBack;
	
	samplelist = last = NULL;
	
	chunksize = BUFFERSIZE / 2;

	//play a sample at 16Khz using interrupt mode
	//DirectSound A + fifo reset + max volume to L and R
	REG_SOUNDCNT_X = SND_ENABLED; 	//turn sound chip on
	REG_SOUNDCNT_H = DSA_FIFO_RESET | DSB_FIFO_RESET | DSA_OUTPUT_TO_LEFT | DSB_OUTPUT_TO_RIGHT | DSA_TIMER0 | DSB_TIMER0;
	
	// the DMA source address is the buffer
	REG_DMA1SAD = (u32)Buffer_L;
	REG_DMA1DAD = (u32)&REG_SGFIFOA;
	
	REG_DMA2SAD = (u32)Buffer_R;
	REG_DMA2DAD = (u32)&REG_SGFIFOB;

	//REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT;
	//REG_DMA2CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT;

	REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT | DMA_SOURCE_INCREMENT | DEST_REG_SAME;
	REG_DMA2CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT | DMA_SOURCE_INCREMENT | DEST_REG_SAME;

	dprintf("REG_DMA1CNT = 0x%lx\n", REG_DMA1CNT);
	
	//REG_DM1CNT_L = BUFFERSIZE;
	//REG_DM2CNT_L = BUFFERSIZE;

	REG_DMA1SAD = (u32)&Click[0];

	// set up our timers
	// set up timer 1 to advance each time timer 0 triggers
	// REG_TM1D = 0xff7f; 	// 0xffff - the number of samples to play
	// our buffer is 128 bytes long and we want to trigger twice every buffer for refills of 64 bytes
	//REG_TM1D = 0xffff - chunksize;
	//REG_TM1CNT = TIMER_ENABLE | TIMER_IRQ_ENABLE | TIMER_CASCADE; 	// enable timer1 + irq and cascade from timer 0
	
	//REG_IE |= INT_TM1; 	//enable irq for timer 1
	//REG_IME = 1; 	//master enable interrupts
	
	// Formula for playback frequency is: 0xFFFF - round(cpuFreq / playbackFreq)
	// timer count=65536-round(2^24/16000)=0xFBE8
	// echo "print hex(0xffff - pow(2, 24) / 22050)" | python
	REG_TM0D = 0xfd07; 		// 22050 KHz playback
	REG_TM0CNT = TIMER_ENABLE; 	// enable the timer to let the sound run
*/
}

AudioLayer::~AudioLayer()
{
	
}

void AudioLayer::Manage(Sample *newsample)
{
	newsample->SetChunkSize(chunksize);
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

void AudioLayer::Forget(Sample *which)
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

void AudioLayer::Interrupt()
{
/*	u16 b;
	s8 *sampledata;
	structSampleList *traverse = samplelist;

	//dprintf("started\n");
	
	// fill the buffer with nice fellows
	while (traverse)
	{
		//dprintf("0x%lx\n", (u32)traverse);
		//dprintf("sample: %s\n", traverse->sample->GetName());
		sampledata = traverse->sample->GetChunk();
		//dprintf("Filling at: %ld\n", resetDMA * chunksize);
		for (b = 0; b < chunksize; b++)
		{
			// we need to start filling the buffer at either 0 or chunksize (depending on where the pointer is)
			Buffer_L[resetDMA * chunksize + b] = sampledata[b];
			Buffer_R[resetDMA * chunksize + b] = sampledata[b];
		}
		traverse = traverse->next;
	}
	
	// do we need to start the dma transfer from the beginning again?
	if (resetDMA)
	{
		//stop DMA
		//REG_DMA1CNT = 0;
		//REG_DMA2CNT = 0;
		
		// start both DMAs again
		//REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT;
		//REG_DMA2CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT;
		
		// stop the sound from playing by disabling the timer
		//REG_TM0CNT &= ~TIMER_ENABLE;
		
		//REG_DMA1CNT &= ~ENABLE_DMA;
		//REG_DMA2CNT &= ~ENABLE_DMA;
		
		//REG_DMA1CNT |= ENABLE_DMA;
		//REG_DMA2CNT |= ENABLE_DMA;
		
		// enable the timer to let the sound run
		//REG_TM0CNT = TIMER_ENABLE;
		
		//REG_DMA1SAD = (u32)Buffer_L;
		//REG_DMA1DAD = (u32)&REG_SGFIFOA;
		
		//REG_DMA2SAD = (u32)Buffer_R;
		//REG_DMA2DAD = (u32)&REG_SGFIFOB;
		
		resetDMA = false;
		//dprintf("reset dma\n");
	}
	else
	{
		resetDMA = true;
		//dprintf("didn't reset dma\n");
	}

	//dprintf("\n");*/
}
