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
	u32 lastbeat;
	u16 handle;
	u16 beat;
	
	structLoopData *data;
	
	NumberBox *nbPitch;
	SelectBox *sbPan;
	NumberBox *nbBeats;
	SelectBox *sbOn;
	SelectBox *sbSample;
	SelectBox *sbReset;
	SelectBox *sbAddLoopButton;
	SelectBox *sbDelLoopButton;
	
	Label *lbPitch;
	Label *lbBeats;
	
	TCallback<Loop> cbAddLoopButton;
	TCallback<Loop> cbDelLoopButton;
	
	TCallback<Loop> cbPitch;
	TCallback<Loop> cbPan;
	TCallback<Loop> cbBeats;
	TCallback<Loop> cbReset;
	TCallback<Loop> cbSample;
	
public:
	Loop(Keys *inkeys, structLoopData *whichloop);
	virtual ~Loop();

	inline void DoDraw() { debugloop("Draw"); };
	
	void DoProcess();
	void *AddLoopButton(void *data);
	void *DelLoopButton(void *data);
	structLoopData *GetAddress();
	void UpdateWidgets();
	void UpdateParameters();
	void *Pitch(void *number);
	void *Pan(void *number);
	void *Beats(void *number);
	void *SampleChange(void *whichsample);
	void *Reset(void *ignore);
	u32 GetSize();	
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
