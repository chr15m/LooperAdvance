// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "looper.hh"
#include "charset.hh"
#include "Page.hh"
#include "Widgets.hh"
#include "samples.h"
#include "samplenames.hh"
#include "krawall.h"

#ifndef _LOOP_HH_
#define _LOOP_HH_

class Loop : public Page
{
private:
	u16 sample;
	bool pan;
	bool vol;
	u32 pitch;
	u32 beats;
	u32 lastbeat;
	u32 beat;

	u16 handle;

	u32 size;

	NumberBox *nbPitch;
	NumberBox *nbPan;
	NumberBox *nbBeats;
	NumberBox *nbOn;
	SelectBox *sbSample;
	NumberBox *nbReset;
	Widget *selected;

public:
	Loop(Keys *inkeys);
	virtual ~Loop();
	void Draw();
	void Process();

	void ResetLoopPitch();
	u32 GetSize();
	void SetParameters(u16 newsample, bool newpan, u32 newpitch, u16 newbeats);
};

#endif

/*
unsigned int kramGetFreq (chandle c)
int kramGetPan (chandle c)
unsigned int kramGetPos (chandle c)
int kramHandleValid (chandle c)
chandle kramPlay (const Sample  s, int sfx, chandle c)
chandle kramPlayExt (const Sample  s, int sfx, chandle c, unsigned int freq, unsigned int vol, int pan)
int kramSetFreq (chandle c, unsigned int freq)
int kramSetPan (chandle c, int pan)
int kramSetPos (chandle c, unsigned int pos)
void kramSetSFXVol (unsigned int vol)   (128 = loud)
int kramSetVol (chandle c, unsigned int vol)
*/
