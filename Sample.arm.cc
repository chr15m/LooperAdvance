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

#include "Sample.hh"

Sample::Sample(SampleData *usedata)
{
	sampledata = usedata;
	
	// start of the sample
	nextchunk = 0;
	
	// initial settings
	volume = 0;
	panshift[0] = 0;
	panshift[1] = 0;
	velocity = 256;
	playing = true;
	
	loopStart = 0;
	loopEnd = 0;
}

Sample::~Sample()
{
	
}

// tell this sample which actual data to use
void Sample::SetData(SampleData *usedata)
{
	debug("Set Sample Data to 0x%lx", (u32)usedata);
	sampledata = usedata;
	loopStart = 0;
	loopEnd = 0;
	volume = 0;
	panshift[0] = 0;
	panshift[1] = 0;
	velocity = 256;
	playing = true;
}

// Set this sample to play
void Sample::Play()
{
	debug("Play");
	playing = true;
}

// Pause this sample
void Sample::Pause()
{
	debug("Pause");
	playing = false;
}

// play or pause depending on a value
void Sample::SetPlaying(bool play)
{
	debug("Value: %d", play);
	playing = play;
}

// set the velocity of this sample
void Sample::SetVelocity(u16 vel)
{
	debug("Velocity: %d", vel);
	velocity = vel;
}

// set the position of this sample
void Sample::SetPosition(u32 position)
{
	nextchunk = position << 8;
	debug("Set position to: %ld (%ld)", nextchunk, ((nextchunk>>8) / sampledata->length));
}

// set the loop end position
void Sample::SetLoopStart(u32 start)
{
	loopStart = start;
	debug("Set loop start to: %ld", start);
}

void Sample::SetLoopEnd(u32 end)
{
	loopEnd = end;
	debug("Set loop end to: %ld", end);
}

// set the panning position of this sample
void Sample::SetPanning(s8 pan)
{
	// figure out our pan shifting
	panshift[0] = 0;
	panshift[1] = 0;
	/*if (pan < 0)
		panshift[0] = - pan + 2;
	else if (pan > 0)
		panshift[1] = pan + 2;
	*/
	debug("Set panning to l=%d r=%d", panshift[0], panshift[1]);
}

// set the volume of this sample
void Sample::SetVolume(u8 vol)
{
	if (vol > 8)
		vol = 8;
	
	volume = (8 - vol);
	debug("Set volume to %d", volume);
}

// get the current playback position of this sample
u32 Sample::GetPosition()
{
	return nextchunk >> 8;
}

// get the length of this sample
u32 Sample::GetLength()
{
	return sampledata->length;
}

// returns the name of this sample
char *Sample::GetName()
{
	return sampledata->name;
}

void Sample::MixDown(s8 *mixBufA, s8 *mixBufB, u16 buffSize, u8 mixshifter)
{
	dprintf("Starting mix on %s at %ld.\n", sampledata->name, nextchunk);
	
	if (playing)
	{
		u32 chunkR = 0;
		u16 b;
		s8 *dataptr = (s8 *)sampledata->data;
		s8 val = 0;
		
		// note, we lost a fair bit of precision doing all this
		// but it's only in chunks of four samples, so it's not so bad.
		for (b = 0; b < buffSize; b++)
		{
			chunkR = (nextchunk += velocity) >> 8;
			
			// fill up our buffers byte by byte
			val = dataptr[chunkR] >> volume >> mixshifter;
			mixBufA[b] += val >> panshift[0];
			mixBufB[b] += val >> panshift[1];
		}
	}
}

/**
  *	Check if we've overrun our sample size and rewind us by the length of the sample if we have.
  */
void Sample::Rewind(u16 buffSize)
{
	if (((nextchunk + velocity) >> 8) + buffSize > sampledata->length)
	{
		nextchunk -= (sampledata->length << 8);
		dprintf("Rewound! *****************************************************\n");
	}
}

