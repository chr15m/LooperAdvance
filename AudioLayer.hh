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

extern void (*IntrTable[])();

class AudioLayer
{
private:
	// a pointer to a linked list of all the samples we're currently playing
	Sample *samples;
	// details about our mixer bank
	s16 *mixbank;
	u16 banksize;

public:
	AudioLayer(u16 mixlength);
	~AudioLayer();

	void Interrupt();
	void Manage(Sample *newsample);
};

namespace Audio
{
	extern void AudioCallBack();
	extern AudioLayer *AudioLayerPtr;
}

#endif
