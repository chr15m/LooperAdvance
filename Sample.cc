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
	panning = 0;
	velocity = 256;
	playing = true;
	
	loopStart = 0;
	loopEnd = 0;
}

Sample::~Sample()
{
	
}

// Set this sample to play
void Sample::Play()
{
	playing = true;
}

// Pause this sample
void Sample::Pause()
{
	playing = false;
}

// set the velocity of this sample
void Sample::SetVelocity(u16 vel)
{
	velocity = vel;
}

// set the position of this sample
void Sample::SetPosition(u32 position)
{
	nextchunk = position;
}

// set the loop end position
void Sample::SetLoopStart(u32 start)
{
	loopStart = start;
}

void Sample::SetLoopEnd(u32 end)
{
	loopEnd = end;
}

// set the panning position of this sample
void Sample::SetPanning(s8 pan)
{
	panning = pan;	
}

// set the volume of this sample
void Sample::SetVolume(u8 vol)
{
	if (vol > 7)
		vol = 7;
	
	volume = 7 - vol;
}

// get the current playback position of this sample
u32 Sample::GetPosition()
{
	return nextchunk;
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

// this function serves two purposes
// firstly it tells the mixer whether to bother continuing with this sample
// secondly it sets up how much to shift the pan register by
bool Sample::IsPlaying()
{
	panshift[0] = 0;
	panshift[1] = 0;
	
	if (panning < 0)
	{
		panshift[0] = - panning;
	}
	else if (panning > 0)
	{
		panshift[1] = panning;
	}
	
	debug("Pan shift: %d %d\n", panshift[0], panshift[1]);
	
	return playing;
}
