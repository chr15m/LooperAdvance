/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#include "gba.h"
#include "typedefs.h"
#include "emudp.h"
#include "samples/samples.hh"

// This is a test of the new kind of audio management which uses a bigger buffer into which we can mix down sound

void RefillBuffers(void)
{
	//disable timer 0
	REG_TM0CNT = 0;
	//stop DMA
	REG_DMA1CNT = 0;

	//clear the interrupt(s)
	REG_IF |= REG_IF;
	dprintf("done\n");
}

// IntrTable for crt0
void (*IntrTable[])() =
{
	0, // v-blank
	0, // h-blank
	0, // vcount
	0, // timer0
	&RefillBuffers, // timer1
	0, // timer2
	0, // timer3
	0, // serial
	0, // dma0
	0, // dma1
	0, // dma2
	0, // dma3
	0, // key
	0  // cart
};

int main()
{
	//play a sample at 16Khz using interrupt mode
	//DirectSound A + fifo reset + max volume to L and R
	REG_SOUNDCNT_H = DSA_OUTPUT_RATIO_100 | DSA_FIFO_RESET | DSA_OUTPUT_TO_BOTH | DSA_TIMER0;
	REG_SOUNDCNT_X = SND_ENABLED; 	//turn sound chip on
	
	// the DMA source address is the buffer
	REG_DMA1SAD = (u32)&Click[0];
	//REG_DMA1DAD = (u32)&REG_SGFIFOA;
	REG_DMA1DAD = 0x040000a0;
	REG_DMA1CNT = ENABLE_DMA | START_ON_FIFO_EMPTY | DMA_32 | DMA_REPEAT | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT;
	
	// set up timer 1 to advance each time timer 0 triggers
	//REG_TM1D = 0xff7f; 	// 0xffff - the number of samples to play (our buffer is 128 bytes long)
	REG_TM1D = 0x0000;
	REG_TM1CNT = TIMER_ENABLE | TIMER_IRQ_ENABLE | TIMER_CASCADE; 	// enable timer1 + irq and cascade from timer 0
	
	REG_IE |= INT_TM1; 	//enable irq for timer 1
	REG_IME = 1; 	//master enable interrupts
	
	// Formula for playback frequency is: 0xFFFF - round(cpuFreq / playbackFreq)
	// timer count=65536-round(2^24/16000)=0xFBE8
	// echo "print hex(0xffff - pow(2, 24) / 22050)" | python
	REG_TM0D = 0xfd07; 		// 22050 KHz playback
	REG_TM0CNT = TIMER_ENABLE; 	// enable the timer to let the sound run
	
	while(1);
}
