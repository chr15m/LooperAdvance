// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "Loop.hh"

Loop::Loop(Keys *inkeys)
{
	u16 i;
	debug("Loop() init");
	UseKeys(inkeys);
	
	sample=0;
	pan = false;
	vol = false;
	debug("Init setting pan to %d", pan);
	pitch = 0;
	beats = 0;
	
	lastbeat = 0;
	handle = 0;
	
	// set up the GUI
	debug("Setting up GUI");
	
	sbOn = new SelectBox(10, 3, 3, inkeys);
	nbPitch = new NumberBox(10, 4, 10, 1, 100000, 20, inkeys);
	sbPan = new SelectBox(10, 5, 5, inkeys);
	nbBeats = new NumberBox(10, 6, 2, 1, 64, 8, inkeys);
	sbReset = new SelectBox(10, 7, 5, inkeys);
	sbSample = new SelectBox(10, 9, 18, inkeys);
	
	lbPitch = new Label(2, 4, "Pitch");
	lbBeats = new Label(2, 6, "Beats");
	
	sbAddLoopButton = new SelectBox(18, 17, 8, inkeys);
	sbDelLoopButton = new SelectBox(18, 18, 8, inkeys);
	
	AddWidget(sbOn);
	AddWidget(nbPitch);
	AddWidget(lbPitch);
	AddWidget(sbPan);
	AddWidget(nbBeats);
	AddWidget(lbBeats);
	AddWidget(sbReset);
	AddWidget(sbSample);
	AddWidget(sbAddLoopButton);
	AddWidget(sbDelLoopButton);
	
	sbOn->SetTransitions(NULL, NULL, NULL, nbPitch);
	nbPitch->SetTransitions(NULL, NULL, sbOn, sbPan);
	sbPan->SetTransitions(NULL, NULL, nbPitch, nbBeats);
	nbBeats->SetTransitions(NULL, NULL, sbPan, sbReset);
	sbReset->SetTransitions(NULL, NULL, nbBeats, sbSample);
	sbSample->SetTransitions(NULL, NULL, sbReset, sbAddLoopButton);
	sbAddLoopButton->SetTransitions(NULL, NULL, sbSample, sbDelLoopButton);
	sbDelLoopButton->SetTransitions(NULL, NULL, sbAddLoopButton, NULL);
	
	debug("Filling sample list");
	
	// what samples are available?
	for (i=0; i<NUMSAMPLES; i++)
	{
		sbSample->NewChoice((char *)samplenames[i], i);
		debug("Sample: %s", (char *)samplenames[i]);
	}
	
	sbAddLoopButton->AutoOff();
	sbAddLoopButton->NewChoice("Add Loop", 0);
	sbAddLoopButton->NewChoice("-------------", 1);	
	cbAddLoopButton.MakeCallback(this, &Loop::AddLoopButton);
	sbAddLoopButton->UseCallBack(&cbAddLoopButton);

	sbDelLoopButton->AutoOff();
	sbDelLoopButton->NewChoice("Del Loop", 0);
	sbDelLoopButton->NewChoice("-------------", 1);	
	cbDelLoopButton.MakeCallback(this, &Loop::DelLoopButton);
	sbDelLoopButton->UseCallBack(&cbDelLoopButton);
	
	sbReset->AutoOff();
	sbReset->NewChoice("Reset", 0);
	sbReset->NewChoice("-----", 1);
	
	sbOn->NewChoice("Off", 0);
	sbOn->NewChoice("On", 1);
	
	sbPan->NewChoice("Left", 0);
	sbPan->NewChoice("Right", 1);
	
	UseKeys(inkeys);
	sbOn->Select();
	selected = sbOn;
	
	debug("Done setting up");
}

Loop::~Loop()
{
	delete sbOn;
	delete nbPitch;
	delete sbPan;
	delete nbBeats;
	delete sbReset;
	delete sbSample;
	delete lbBeats;
	delete lbPitch;
	delete sbAddLoopButton;
	delete sbDelLoopButton;
}

// if they press the loop-add button
void *Loop::AddLoopButton(void *data)
{
	Page *old;
	debug("AddLoop button callback");
	// add a loop to currentsong
	globals.NewLoop();
	// add a live loop
	if (right)
	{
		debug("Inserting a new loop");
		// make our next loop have a new loop
		old = right;
		right->left = new Loop(keys);
		right = right->left;
		right->left = this;
		right->right = old;
	}
	else
	{
		debug("Appending a new loop");
		right = new Loop(keys);
		right->left = this;
	}
	
	return NULL;
}

// if they press the del-loop button
void *Loop::DelLoopButton(void *data)
{
	Page *old;
	debug("DelLoop button callback");
	// delete the current loop in currentsong
	globals.DelLoop();
	// delete the liveloop associated with it
	if (right)
	{
		old = right->right;
		delete right;
		right = old;
		right->left = this;
	}
	
	return NULL;
}

/*
void Loop::DoDraw()
{

	if ((selected == nbReset) && (((NumberBox *)nbReset)->GetValue() == 1))
	{
		ResetLoopPitch();
		((NumberBox *)nbReset)->SetValue(0);
	}
	
	// if they choose a new sample, compute some nice defaults
	if ((selected == sbSample) && (sample != sbSample->GetChoice()))
	{
		SetParameters(sbSample->GetChoice(), pan, pitch, beats);
		ResetLoopPitch();
	}
	
	// labels
	if (sample != 0xFF)
		cprintf(1,1, samplenames[sample]);
	
	cprintf(1,3,"On");
	nbOn->Draw();

	cprintf(1,4,"Pitch");
	nbPitch->Draw();

	cprintf(1,5,"Pan");
	nbPan->Draw();
	
	cprintf(1,6,"Beats");
	nbBeats->Draw();

	cprintf(1,7,"Reset");
	nbReset->Draw();

	sbSample->Draw();
}

void Loop::DoProcess()
{
//	dprintf("Loop Process: %ld, sample handle = %d\n", (u32)this, handle);
	if ((sample != 0xFF) && kramHandleValid(handle))
	{
		// update our position
		beat = globals.counter * globals.currentsong->bpm/3600;
		
		if (beat != lastbeat)
		{
			kramSetFreq(handle, ((pitch * 44100) / 1000));
			kramSetPan(handle, (pan * 128 - 64));
			kramSetVol(handle, vol * 64);
			kramSetPos(handle, (size * (beat % beats))/beats);
			lastbeat = beat;
		}
	}
}

void Loop::ResetLoopPitch()
{	
	// (1000*(tracks[selected].sample->len)/(tracks[selected].sample->freq))/(tracks[selected].bpl*T*4/1000	
	
	debug("Top = %ld\n", globals.currentsong->bpm * GetSize());
	debug("Bottom = %ld\n", (2646*beats));
	debug("New ratio = %d\n", (globals.songdata->bpm * GetSize())/(2646*beats));
	pitch = (globals.songdata->bpm * GetSize()) / (2646 * beats);
	nbPitch->SetValue(pitch);
}

u32 Loop::GetSize()
{
	return (u32)samples[sample]->end - (u32)samples[sample]->data;
}

void Loop::SetParameters(u16 newsample, bool newpan, u32 newpitch, u16 newbeats)
{
	dprintf("SetParameters\n");
	sample = newsample;
	pan = newpan;
	pitch = newpitch;
	beats = newbeats;
	size = GetSize();
	
	nbPitch->SetValue(pitch);
	nbPan->SetValue(pan);
	nbBeats->SetValue(beats);
	sbSample->Choose(sample);
	
	if ((sample != 0xFF) && kramHandleValid(handle))
	{
		// first stop the current sample
		kramStop(handle);
	}
	
	handle = kramPlay(samples[sample], 1, 0);
}
*/
