/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

// Represents a single loop-page

#include "looper.hh"
#include "charset.hh"
#include "Page.hh"
#include "Widgets.hh"
#include "samples/samples.hh"
#include "screens.hh"
#include "Sample.hh"
#include "ClarkMix.hh"

#ifndef _LOOP_HH_
#define _LOOP_HH_

class Loop : public Page
{
private:
	u32 lastbeat;
	u16 beat;
	u16 numnotes;
	ptrNoteData *notes;
	char beatNumberText[4];

	Sample *sample;
	ClarkMix *mixer;

	structLoopData *data;
	
	EditBox *ebName;
	Label *lbBeatNumber;
	NumberBox *nbPitch;
	SelectBox *sbPan;
	NumberBox *nbBeats;
	SelectBox *sbOn;
	SelectBox *sbSample;
	SelectBox *sbReset;
	SelectBox *sbAddLoopButton;
	SelectBox *sbDelLoopButton;
	NumberBox *nbSwing;
	
	// notes interface
	NumberBox *nbNotes;
	NumberBox *nbNote;
	NumberBox *nbNBeat;
	NumberBox *nbNPitch;
	NumberBox *nbNSwing;
	SelectBox *sbNAction;
	
	// notes labels
	Label *lbSample;
	Label *lbNotes;
	Label *lbStep;
	Label *lbNote;
	Label *lbNSwing;
	Label *lbNBeat;
	Label *lbNAction;
	Label *lbNPitch;
	
	// other labels
	Label *lbPitch;
	Label *lbBeats;
	Label *lbSwing;
	
	// callbacks
	TCallback<Loop> cbAddLoopButton;
	TCallback<Loop> cbDelLoopButton;
	
	TCallback<Loop> cbName;
	TCallback<Loop> cbPitch;
	TCallback<Loop> cbPan;
	TCallback<Loop> cbBeats;
	TCallback<Loop> cbReset;
	TCallback<Loop> cbSample;
	TCallback<Loop> cbSwing;
	
	// note action callbacks
	TCallback<Loop> cbNotes;
	TCallback<Loop> cbNote;
	TCallback<Loop> cbNBeat;
	TCallback<Loop> cbNPitch;
	TCallback<Loop> cbNSwing;
	TCallback<Loop> cbNAction;
	
public:
	Loop(Keys *inkeys, structLoopData *whichloop, ClarkMix *imixer);
	virtual ~Loop();
	
	void DoSwap();
	
	void DoProcess();
	void *AddLoopButton(void *data);
	void *DelLoopButton(void *data);
	structLoopData *GetAddress();

	void UpdateWidgets();
	void UpdateParameters();
	void UpdateParametersNotes();

	void *Name(void *name);
	void *Pitch(void *number);
	void *Pan(void *number);
	void *Beats(void *number);
	void *Swing(void *number);
	void *SampleChange(void *whichsample);
	void *Reset(void *ignore);
	void *Notes(void *number);
	void *NoteChange(void *note);

	void *NBeat(void *number);
	void *NSwing(void *number);
	void *NPitch(void *number);
	void *NAction(void *which);

	void AddNote();
	void DelNote();
	void UpdateNotes();
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
