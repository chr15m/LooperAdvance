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
	
	// default to four for the GBA
	chunksize = 4;
	// start of the sample
	nextchunk = 0;
	
	// initial settings
	volume = 255;
	panning = 0;
	frequency = 256;
	playing = false;
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

// set the frequency of this sample
void Sample::SetFrequency(u16 freq)
{
	frequency = freq;
}

// set the position of this sample
void Sample::SetPosition(u32 position)
{
	nextchunk = position;
}

// set the panning position of this sample
void Sample::SetPanning(s8 pan)
{
	panning = pan;
}

// set the volume of this sample
void Sample::SetVolume(u8 vol)
{
	volume = vol;
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

// set the size of the array of data to be returned by GetChunk
void Sample::SetChunkSize(u16 setsize)
{
	chunksize = setsize;
}

// this is used by audio layer to get the next chunk of audio to be sent to the hardware
s8 *Sample::GetChunk()
{
//	dprintf("Returning %d bytes.\n", chunksize);
	
	// use only 24 bits of next chunk
	s8 *returnchunk = (s8 *)sampledata->data + (nextchunk >> 8);
	
	//nextchunk += chunksize;
	// go forward at our frequency
	nextchunk += frequency;
	//dprintf("chunk: %ld\n", (nextchunk >> 8));
	
	// NOTE: because our chunksize is only 4 on the gba it's not really an issue reading past the end of the data
	// however if this is going to be ported to other platforms, this should be adressed with a temporary buffer
	if ((nextchunk >> 8) > sampledata->length)
		nextchunk = 0;
	
	return returnchunk;
}
