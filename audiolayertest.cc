/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#include "samples/samples.hh"
#include "charset.hh"
#include "AudioLayer.hh"
#include "emudp.h"

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

int main()
{
	AudioLayer *audio = new AudioLayer();
	Sample *mysample = new Sample((SampleData *)&samples[0]);
	Sample *other = new Sample((SampleData *)&samples[1]);
	
	audio->Manage(mysample);
	//audio->Manage(other);
	
	mysample->SetFrequency(BUFFERSIZE << 7);
	
	while(1);
}
