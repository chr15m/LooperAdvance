/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: Loop.hh,v 1.13 2004/04/08 06:09:42 chrism Exp $

******************************************************/

// Represents a single loop-page

#include "looper.h"
#include "charset.h"
#include "Page.h"
#include "Widgets.h"
#include "samples.h"
#include "samplenames.h"
#include "krawall.h"
#include "krawall-override.h"

#ifndef _LOOP_HH_
#define _LOOP_HH_

class Loop : public Page
{
private:
	u32 lastbeat;
	u16 handle;
	u16 beat;
	u16 numnotes;
	ptrNoteData *notes;
	
	structLoopData *data;
	
	EditBox *ebName;
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
	Label *lbNotes;
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
	Loop(Keys *inkeys, structLoopData *whichloop);
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
