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

#ifndef _SAMPLE_HH_
#define _SAMPLE_HH_

#include "gba.h"
#include "typedefs.h"
#include "SampleData.hh"

class Sample
{
private:
	// number of samples to jump forward each time
	u16 chunksize;
	// what sample data we're using
	SampleData *sampledata;
	// the next position in the datastream
	u32 nextchunk;

	// parameters for the sample
	u8 volume;
	s8 panning;
	u16 frequency;
	bool playing;
public:
	Sample(SampleData *usedata);
	~Sample();
	
	void Play();
	void Pause();
	
	void SetFrequency(u16 freq);
	void SetPosition(u32 position);
	void SetPanning(s8 pan);
	void SetVolume(u8 vol);
	
	u32 GetPosition();
	u32 GetLength();
	char *GetName();
	
	void SetChunkSize(u16 chunksize);
	s8* GetChunk();
};

#endif
