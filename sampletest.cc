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
#include "Sample.hh"
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
	Sample *mysample;
	u16 i;
	s8 *chunkptr;
	
	mysample = new Sample((SampleData *)&samples[0]);
	
	dprintf("Sample name: %s\n", mysample->GetName());
	dprintf("Sample length: %ld\n", mysample->GetLength());
	
	for (i = 0; i < 10; i++)
	{
		dprintf("\n");
		dprintf("Sample position: %ld\n", mysample->GetPosition());
		chunkptr = mysample->GetChunk();
		dprintf("Chunk: %d %d %d %d\n", chunkptr[0], chunkptr[1], chunkptr[2], chunkptr[3]);
	}
	
	dprintf("Jumping forward to the end of the sample\n");
	
	while (i < (mysample->GetLength() >> 2) - 2)
	{
		mysample->GetChunk();
		i++;
	}
	
	for (i = 0; i < 10; i++)
	{
		dprintf("\n");
		dprintf("Sample position: %ld\n", mysample->GetPosition());
		chunkptr = mysample->GetChunk();
		dprintf("Chunk: %d %d %d %d\n", chunkptr[0], chunkptr[1], chunkptr[2], chunkptr[3]);
	}
	
	while (1);
}
