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
	
	bool IsPlaying();

	inline bool MixDown(bool first, s8 *mixBufA, s8 *mixBufB, u16 buffSize)
	{
		register u16 chunkR = (nextchunk >> 8);
		register u32 nextchunkR = nextchunk;
		register u32 velocityR = velocity;
		register u16 b;
		register u32 lengthR = sampledata->length;
		register u32 dataptr = (u32)sampledata->data;
		
		if (playing)
		{
			if (first)
			{
				for (b = 0; b < buffSize; b++)
				{
					chunkR = (nextchunkR += velocityR) >> 8;
					// fill up our buffers byte by byte
					mixBufA[b] = *((s8 *)dataptr + chunkR);
					mixBufB[b] = *((s8 *)dataptr + chunkR);
					if (chunkR >= lengthR - 1)
						nextchunkR = 0;
				}
				first = false;
			}
			else
			{
				for (b = 0; b < buffSize; b++)
				{
					chunkR = (nextchunkR += velocityR) >> 8;
					// fill up our buffers byte by byte
					mixBufA[b] += *((s8 *)dataptr + chunkR);
					mixBufB[b] += *((s8 *)dataptr + chunkR);
					if (chunkR >= lengthR - 1)
						nextchunkR = 0;
				}
			}
			nextchunk = nextchunkR;
		}
		return first;
	}
	
	//inline s8 GetByte(panVal pan) __attribute__ ((section (".iwram")));
	inline s8 GetByte(panVal pan)
	{
		s8 returnchunk = 0;
		//u32 hop = (nextchunk >> 8);
		
		if (playing)
		{
			// we're going to have to set this anyway (else)
			nextchunk += velocity;
			
			/*if ((loopEnd) && (hop >= loopEnd || hop < loopStart))
			{
				nextchunk = loopStart << 8;
			}
			else if (hop >= sampledata->length - 1)
			*/
			//if (hop >= sampledata->length - 1)
			//{
			//	nextchunk = 0;
			//}
			
			// calculate the mixed value to return
			//returnchunk = *((s8 *)sampledata->data + (nextchunk >> 8));
		}
		
		return returnchunk;
	}
};

typedef struct structSampleList
{
	Sample *sample;
	structSampleList *next;
};
#endif
