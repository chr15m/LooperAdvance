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
#include "emudp.h"
#include "helpers.h"
#include "SampleData.hh"

class Sample
{
private:
	// what sample data we're using
	SampleData *sampledata;
	// the next position in the datastream (uses the top 24 bits)
	u32 nextchunk;
	
	// parameters for the sample
	u8 volume;
	s8 panning;
	u32 velocity;
	bool playing;
	s8 panshift[2];
	
	u32 loopStart;
	u32 loopEnd;

public:
	Sample(SampleData *usedata);
	~Sample();
	
	void Play();
	void Pause();
	
	void SetVelocity(u16 vel);
	void SetPosition(u32 position);
	void SetPanning(s8 pan);
	void SetVolume(u8 vol);
	
	void SetLoopStart(u32 start);
	void SetLoopEnd(u32 end);
	
	u32 GetPosition();
	u32 GetLength();
	char *GetName();
	
	bool IsPlaying();

	inline s8 GetByte(panVal pan)
	{
		u32 hop = (nextchunk >> 8);
		
		if (playing)
		{
			// we're going to have to set this anyway (else)
			nextchunk += velocity;
			
			if ((loopEnd) && (hop >= loopEnd || hop < loopStart))
			{
				nextchunk = loopStart << 8;
			}
			else if (hop >= sampledata->length - 1)
			{
				nextchunk = 0;
			}
		}
		
		// calculate the mixed value to return
		s8 returnchunk = *((s8 *)sampledata->data + (nextchunk >> 8)) >> volume >> panshift[pan];
		
		return returnchunk;
	}
};

typedef struct structSampleList
{
	Sample *sample;
	structSampleList *next;
};
#endif
