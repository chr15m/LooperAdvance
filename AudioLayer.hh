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

#ifndef _AUDIOLAYER_HH_
#define _AUDIOLAYER_HH_

#include "gba.h"
#include "typedefs.h"
#include "emudp.h"
#include "Sample.hh"
#include "samples/samples.hh"

#define BUFFERSIZE	256
#define OVERRUN		0

extern void (*IntrTable[])();

class AudioLayer
{
private:
	// a pointer to a linked list of all the samples we're currently playing
	structSampleList *samplelist;
	// last one in the linked list
	structSampleList *last;
	
	// chunks are always going to be half of buffersize
	u16 chunksize;
	
	u8 Buffer_L[BUFFERSIZE];
	u8 Buffer_R[BUFFERSIZE];
	
	bool resetDMA;

public:
	AudioLayer();
	~AudioLayer();
	
	void Interrupt();
	void Manage(Sample *newsample);
	void Forget(Sample *which);
};

namespace Audio
{
	extern void AudioCallBack();
	extern AudioLayer *AudioLayerPtr;
}

#endif
