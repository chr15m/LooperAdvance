/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#include "ClarkMix.hh"
#include "samples/samples.hh"

// IntrTable for crt0
void (*IntrTable[])() =
{
	0, // v-blank
	0, // h-blank
	0, // vcount
	0, // timer0
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

// This is a test of the clarkinou mixing system

int main()
{
	ClarkMix *mixer = new ClarkMix();
	
	mixer->playSample((u32)samples[0].data, (u32)&samples[0].data + samples[0].length, (u32)samples[0].data, (u32)samples[0].data + samples[0].length, 250, 0, 0, 0x0100);
	mixer->playSample((u32)samples[1].data, (u32)&samples[1].data + samples[1].length, (u32)samples[1].data, (u32)samples[1].data + samples[1].length, 250, 0, 1, 0x0100);

	while(1);
}
