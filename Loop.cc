// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "Loop.hh"

Loop::Loop(Keys *inkeys, structLoopData *whichloop)
{
	u16 i;
	debug("Loop() init at 0x%lx", (u32)whichloop);
	UseKeys(inkeys);
	
	data = whichloop;

	lastbeat = 0;
	handle = 0;
	
	// set up the GUI
	debug("Setting up GUI");
	
	ebName = new EditBox(10, 1, 10, inkeys);
	sbOn = new SelectBox(10, 3, 3, inkeys);
	debug("sbOn: 0x%lx", sbOn);
	nbPitch = new NumberBox(10, 4, 10, 1, 100000, 100, inkeys);
	sbPan = new SelectBox(10, 5, 5, inkeys);
	nbBeats = new NumberBox(10, 6, 2, 1, 64, 8, inkeys);
	sbReset = new SelectBox(10, 7, 5, inkeys);
	sbSample = new SelectBox(10, 9, 10, inkeys);
	
	lbPitch = new Label(2, 4, "Pitch");
	lbBeats = new Label(2, 6, "Beats");
	
	sbAddLoopButton = new SelectBox(18, 17, 8, inkeys);
	sbDelLoopButton = new SelectBox(18, 18, 8, inkeys);
	
	AddWidget(ebName);
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
	
	ebName->SetTransitions(NULL, NULL, NULL, sbOn);
	sbOn->SetTransitions(NULL, NULL, ebName, nbPitch);
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
	cbSample.MakeCallback(this, &Loop::SampleChange);
	sbSample->UseCallBack(&cbSample);
	
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

	cbName.MakeCallback(this, &Loop::Name);
	ebName->UseCallBack(&cbName);

	cbPitch.MakeCallback(this, &Loop::Pitch);
	nbPitch->UseCallBack(&cbPitch);

	cbBeats.MakeCallback(this, &Loop::Beats);
	nbBeats->UseCallBack(&cbBeats);
	
	sbReset->AutoOff();
	sbReset->NewChoice("Reset", 0);
	sbReset->NewChoice("-----", 1);
	cbReset.MakeCallback(this, &Loop::Reset);
	sbReset->UseCallBack(&cbReset);
	
	sbOn->NewChoice("Off", 0);
	sbOn->NewChoice("On", 1);
	
	sbPan->NewChoice("Left", 0);
	sbPan->NewChoice("Right", 1);
	cbPan.MakeCallback(this, &Loop::Pan);
	sbPan->UseCallBack(&cbPan);
	
	UseKeys(inkeys);
	sbOn->Select();
	selected = sbOn;
	
	UpdateWidgets();
	SampleChange(NULL);
	
	debug("Done setting up");
}

Loop::~Loop()
{
	debug("Deleting loop");
	
	// stop playing our sample
	if (kramHandleValid(handle))
		kramStop(handle);
	
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
		right->left = new Loop(keys, globals.currentloop);
		right = right->left;
		right->left = this;
		right->right = old;
	}
	else
	{
		debug("Appending a new loop");
		right = new Loop(keys, globals.currentloop);
		right->left = this;
	}
	
	return NULL;
}

// if they press the del-loop button
void *Loop::DelLoopButton(void *data)
{
	Page *old;
	debug("DelLoop button callback");
	// delete the liveloop associated with it
	if (right)
	{
		// delete the current loop in currentsong
		globals.SetLoop(((Loop *)right)->GetAddress());
		globals.DelLoop();
		
		old = right->right;
		delete right;
		right = old;
		right->left = this;
	}
	
	return NULL;
}

// return which bit of loop data we're using
structLoopData *Loop::GetAddress()
{
	return data;
}

// this resets all our widgets to their correct values
void Loop::UpdateWidgets()
{
	nbPitch->SetValue(data->pitch);
	sbPan->ChooseByValue(data->pan);
	nbBeats->SetValue(data->divisions);
	sbSample->ChooseByValue(data->sample);
	ebName->SetString(data->name);
}

// what to do if the pitch is changed
void *Loop::Pitch(void *number)
{
	data->pitch = *((u16 *)number);
	return NULL;
}

// what to do if the number of beat divisions is changed
void *Loop::Beats(void *number)
{
	data->divisions = *((u16 *)number);
	return NULL;
}

// if they change the panning of this one
void *Loop::Pan(void *pan)
{
	data->pan = ((structSelectList *)pan)->value;
	return NULL;
}

// if they change the name of this loop
void *Loop::Name(void *pan)
{
	globals.SetLoopName((char *)pan);
	return NULL;
}

// if the reset button is pressed
void *Loop::Reset(void *ignore)
{
	// (1000*(tracks[selected].sample->len)/(tracks[selected].sample->freq))/(tracks[selected].bpl*T*4/1000	
	u16 beats = nbBeats->GetValue();
	
	debug("Top = %ld", globals.currentsong->bpm * GetSize());
	debug("Bottom = %d", (2646 * beats));
	debug("New ratio = %ld", (globals.songdata->bpm * GetSize())/(2646 * beats));
	nbPitch->SetValue((globals.songdata->bpm * GetSize()) / (2646 * beats));
	return NULL;
}

// get the size of the current sample in samples
u32 Loop::GetSize()
{
	u16 sample = sbSample->GetChoice();
	return (u32)samples[sample]->end - (u32)samples[sample]->data;
}

// process the audio
void Loop::DoProcess()
{
	beat = globals.beat % nbBeats->GetValue();
	
	if ((sbSample->GetChoice() != 0xFF) && kramHandleValid(handle))
	{
		
		if (beat != lastbeat)
		{
			UpdateParameters();
			lastbeat = beat;
		}
	}
	
	debugloop("Beat: %d", beat);
}

// during the draw phase make sure we're the current loop
void Loop::DoDraw()
{
	globals.currentloop = data;
}

// update the parameters of a specific loop
void Loop::UpdateParameters()
{
	kramSetFreq(handle, ((nbPitch->GetValue() * 44100) / 1000));
	kramSetPan(handle, (sbPan->GetChoice() * 128 - 64));
	kramSetVol(handle, sbOn->GetChoice() * 64);
	kramSetPos(handle, (GetSize() * (beat % nbBeats->GetValue()))/nbBeats->GetValue());
}

// what to do when they select a different sample
void *Loop::SampleChange(void *whichsample)
{
	if (whichsample)
	{
		data->sample = ((structSelectList *)whichsample)->value;
	}
	
	debug("Changing sample");
	
	Reset(NULL);
	
	if ((data->sample != 0xFF) && kramHandleValid(handle))
	{
		// first stop the current sample
		kramStop(handle);
	}
	
	handle = kramPlay(samples[data->sample], 1, 0);
	UpdateParameters();
	debug("New Handle: %d", handle);

	return NULL;
}
