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

u32 iNextSample = 0;

void AudioInterrupt(void)
{
	//u32 *data = (u32 *)samples[0].data;
	u8 *data = (u8 *)samples[0].data;
	
	//if(!(iNextSample & 3))
	// REG_SGFIFOA = (data[iNextSample]); // + (data[iNextSample + 1] >> 16) + (data[iNextSample + 2] >> 8) + (data[iNextSample + 3]);
	
	//REG_SGFIFOA = data[iNextSample] + (data[iNextSample + 1] << 8) + (data[iNextSample + 2] << 16) + (data[iNextSample + 3] << 24);
	//REG_SGFIFOA = (data[iNextSample] << 16) + (data[iNextSample + 1] << 24) + (data[iNextSample + 2]) + (data[iNextSample + 3] << 8);
	REG_SGFIFOA = (data[iNextSample] << 24) + (data[iNextSample + 1] << 16) + (data[iNextSample + 2] << 8) + data[iNextSample + 3];
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
	
	iNextSample += 4;

	// we're bitshifting the value of the u32 iNextSample because we want to multiply it by four (four bytes at a time)
	if(iNextSample > samples[0].length)
	{
		//sample finished!
		//REG_TM0CNT=0; //disable timer 0
		iNextSample = 0;
		dprintf("Loop at %ld\n", samples[0].length);
	}
	
	//clear the interrupt(s)
	REG_IF |= REG_IF;
}

// IntrTable for crt0
void (*IntrTable[])() =
{
	0, // v-blank
	0, // h-blank
	0, // vcount
	&AudioInterrupt, // timer0
	0, // timer1
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
	//REG_SOUNDCNT_H = 0x0B0F; 	//DirectSound A + fifo reset + max volume to L and R
	//REG_SOUNDCNT_X = 0x0080; 	//turn sound chip on

	REG_SOUNDCNT_H = DSA_OUTPUT_RATIO_100 | DSA_FIFO_RESET | DSA_OUTPUT_TO_BOTH | DSA_TIMER0; 	//DirectSound A + fifo reset + max volume to L and R
	REG_SOUNDCNT_X = SND_ENABLED; 	//turn sound chip on
	
	REG_IE = INT_TM0; 	//enable timer 0 irq
	REG_IME = 1; 	//enable interrupts
	
	// REG_TM0D = 0xffff;
	REG_TM0D = 0xf400;
	//REG_TM0CNT = 0x00C3;	//enable timer at CPU freq/1024 +irq =16384Khz sample rate
	REG_TM0CNT = TIMER_IRQ_ENABLE | TIMER_ENABLE | TIMER_FREQ_SYSTEM;
	
	while(1);
}
