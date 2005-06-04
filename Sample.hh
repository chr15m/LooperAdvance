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
#include "SampleTrader.hh"

class Sample
{
private:
	// what sample data we're using
	SampleData *sampledata;
	// the next position in the datastream (uses the top 24 bits)
	u32 nextchunk;
	
	// parameters for the sample
	u8 volume;
	u32 velocity;
	bool playing;
	s8 panshift[2];
	
	u32 loopStart;
	u32 loopEnd;

public:
	Sample(SampleData *usedata);
	~Sample();

	void SetData(SampleData *usedata);
	
	void Play();
	void Pause();
	void SetPlaying(bool);
	void SetVelocity(u16 vel);
	void SetPosition(u32 position);
	void SetPanning(s8 pan);
	void SetVolume(u8 vol);
	
	void SetLoopStart(u32 start);
	void SetLoopEnd(u32 end);
	
	u32 GetPosition() __attribute__ ((section (".iwram")));
	u32 GetLength() __attribute__ ((section (".iwram")));
	char *GetName() __attribute__ ((section (".iwram")));
	
	void MixDown(s8 *mixBufA, s8 *mixBufB, u16 buffSize, u8 mixshifter) __attribute__ ((section (".iwram")));
	u16 GetSamples() __attribute__ ((section (".iwram")));
	void Rewind(u8 howfar, u16 buffSize) __attribute__ ((section (".iwram")));
	
	inline bool IsPlaying()
	{
		return playing;
	}
};

typedef struct structSampleList
{
	Sample *sample;
	structSampleList *next;
};
#endif
