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

// tell this sample which actual data to use
void Sample::SetData(SampleData *usedata)
{
	debug("Set Sampel Data to 0x%lx\n", (u32)usedata);
	sampledata = usedata;
	loopStart = 0;
	loopEnd = 0;
	volume = 0;
	panning = 0;
	velocity = 256;
	playing = true;
}

// Set this sample to play
void Sample::Play()
{
	debug("\n");
	playing = true;
}

// Pause this sample
void Sample::Pause()
{
	debug("\n");
	playing = false;
}

// play or pause depending on a value
void Sample::SetPlaying(bool play)
{
	debug("Value: %d\n", play);
	playing = play;
}

// set the velocity of this sample
void Sample::SetVelocity(u16 vel)
{
	debug("Velocity: %d\n", vel);
	velocity = vel;
}

// set the position of this sample
void Sample::SetPosition(u32 position)
{
	nextchunk = position << 8;
	debug("Set position to: %ld (%ld)\n", nextchunk, ((nextchunk>>8) / sampledata->length));
}

// set the loop end position
void Sample::SetLoopStart(u32 start)
{
	loopStart = start;
	debug("Set loop start to: %ld\n", start);
}

void Sample::SetLoopEnd(u32 end)
{
	loopEnd = end;
	debug("Set loop end to: %ld\n", end);
}

// set the panning position of this sample
void Sample::SetPanning(s8 pan)
{
	debug("Set pan to: %d\n", pan);
	panning = pan;	
}

// set the volume of this sample
void Sample::SetVolume(u8 vol)
{
	if (vol > 7)
		vol = 7;
	
	volume = 7 - vol;
	debug("Set volume to %d\n", volume);
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

// this function serves two purposes
// firstly it tells the mixer whether to bother continuing with this sample
// secondly it sets up how much to shift the pan register by
bool Sample::IsPlaying()
{
	if (playing)
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
	}
	
	return playing;
}
