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
	//load FIFO each 4 samples with 4 bytes
	if (!(iNextSample & 3))
		REG_SGFIFOA = Click[iNextSample >> 2];

	iNextSample++;
	if(iNextSample > 39683)
	{
		//sample finished!
		//REG_TM0CNT=0; //disable timer 0
		iNextSample = 0;
		dprintf("Loop\n");
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
	REG_SOUNDCNT_H=0x0B0F; //DirectSound A + fifo reset + max volume to L and R
	REG_SOUNDCNT_X=0x0080; //turn sound chip on
	
	REG_IE=0x8; //enable timer 0 irq
	REG_IME=1; //enable interrupts
	
	REG_TM0D=0xffff;
	REG_TM0CNT=0x00C3; //enable timer at CPU freq/1024 +irq =16384Khz sample rate
	
	while(1);
}
