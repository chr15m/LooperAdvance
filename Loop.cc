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
	numnotes = 0;
	notes = NULL;
	
	// set up the GUI
	debug("Setting up GUI");
	
	ebName = new EditBox(10, 1, 10, inkeys);
	sbOn = new SelectBox(10, 3, 3, inkeys);
	debug("sbOn: 0x%lx", (u32)sbOn);
	nbPitch = new NumberBox(10, 4, 6, 1, 100000, 100, inkeys);
	sbPan = new SelectBox(10, 5, 5, inkeys);
	nbBeats = new NumberBox(10, 6, 2, 1, 64, 8, inkeys);
	sbReset = new SelectBox(10, 7, 5, inkeys);
	sbSample = new SelectBox(10, 9, 10, inkeys);
	
	lbPitch = new Label(2, 4, "Pitch");
	lbBeats = new Label(2, 6, "Beats");
	lbSwing = new Label(2, 10, "Swing");
	
	nbSwing = new NumberBox(10, 10, 3, 0, 999, 20, inkeys);
	// note stuff
	
	// notes interface
	nbNotes = new NumberBox(10, 12, 3, 0, 256, 4, inkeys);
	lbNotes = new Label(2, 12, "Notes");
	
	nbNote = new NumberBox(10, 13, 3, 0, 0, 8, inkeys);
	nbNBeat = new NumberBox(10, 15, 3, 0, 0, 20, inkeys);
	nbNSwing = new NumberBox(10, 16, 3, 0, 255, 20, inkeys);
	nbNPitch = new NumberBox(10, 17, 3, 0, 255, 12, inkeys);
	sbNAction = new SelectBox(10, 18, 6, inkeys);
	
	// notes labels
	lbNBeat = new Label(3, 15, "Beat");
	lbNSwing = new Label(3, 16, "Swing");
	lbNPitch = new Label(3, 17, "Pitch");	
	lbNAction = new Label(3, 18, "Action");

	sbAddLoopButton = new SelectBox(19, 17, 8, inkeys);
	sbDelLoopButton = new SelectBox(19, 18, 8, inkeys);

	AddWidget(ebName);
	AddWidget(sbOn);
	AddWidget(nbPitch);
	AddWidget(lbPitch);
	AddWidget(sbPan);
	AddWidget(nbBeats);
	AddWidget(lbBeats);
	AddWidget(sbReset);
	AddWidget(sbSample);
	AddWidget(lbSwing);
	AddWidget(nbSwing);
	AddWidget(nbNotes);
	AddWidget(lbNotes);
	
	AddWidget(nbNote);
	
	AddWidget(lbNSwing);
	AddWidget(lbNBeat);
	AddWidget(lbNAction);
	AddWidget(lbNPitch);

	AddWidget(nbNBeat);
	AddWidget(nbNSwing);
	AddWidget(nbNPitch);
	AddWidget(sbNAction);

	AddWidget(sbAddLoopButton);
	AddWidget(sbDelLoopButton);
	
	ebName->SetTransitions(NULL, NULL, NULL, sbOn);
	sbOn->SetTransitions(NULL, NULL, ebName, nbPitch);
	nbPitch->SetTransitions(NULL, NULL, sbOn, sbPan);
	sbPan->SetTransitions(NULL, NULL, nbPitch, nbBeats);
	nbBeats->SetTransitions(NULL, NULL, sbPan, sbReset);
	sbReset->SetTransitions(NULL, NULL, nbBeats, sbSample);
	sbSample->SetTransitions(NULL, NULL, sbReset, nbSwing);
	nbSwing->SetTransitions(NULL, NULL, sbSample, nbNotes);
	nbNotes->SetTransitions(NULL, NULL, nbSwing, nbNote);
	nbNote->SetTransitions(NULL, NULL, nbNotes, nbNBeat);
	nbNBeat->SetTransitions(NULL, NULL, nbNote, nbNSwing);
	nbNSwing->SetTransitions(NULL, NULL, nbNBeat, nbNPitch);
	nbNPitch->SetTransitions(NULL, NULL, nbNSwing, sbNAction);
	sbNAction->SetTransitions(NULL, NULL, nbNPitch, NULL);

	sbNAction->NewChoice("cont", 0);
	sbNAction->NewChoice("cut", 1);
	sbNAction->NewChoice("loop", 2);
	sbNAction->NewChoice("bounce", 3);
	
//	sbAddLoopButton->SetTransitions(NULL, NULL, nbNotes, sbDelLoopButton);
//	sbDelLoopButton->SetTransitions(NULL, NULL, sbAddLoopButton, NULL);
	
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

	cbNotes.MakeCallback(this, &Loop::Notes);
	nbNotes->UseCallBack(&cbNotes);

	cbNote.MakeCallback(this, &Loop::NoteChange);
	nbNote->UseCallBack(&cbNote);

	cbNBeat.MakeCallback(this, &Loop::NBeat);
	nbNBeat->UseCallBack(&cbNBeat);

	cbNSwing.MakeCallback(this, &Loop::NSwing);
	nbNSwing->UseCallBack(&cbNSwing);

	cbNPitch.MakeCallback(this, &Loop::NPitch);
	nbNPitch->UseCallBack(&cbNPitch);

	cbNAction.MakeCallback(this, &Loop::NAction);
	sbNAction->UseCallBack(&cbNAction);

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

	globals.SetCurrentLoop(data);
	
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
	delete nbSwing;
	delete lbBeats;
	delete lbPitch;
	delete lbSwing;
	delete nbNotes;
	delete lbNotes;
	
	delete lbNSwing;
	delete lbNBeat;
	delete lbNAction;
	delete lbNPitch;
	
	delete sbAddLoopButton;
	delete sbDelLoopButton;
	delete[] notes;
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
	structNoteData *traverse = data->notes;
	
	nbPitch->SetValue(data->pitch);
	sbPan->ChooseByValue(data->pan);
	nbBeats->SetValue(data->divisions);
	nbNBeat->SetMax(data->divisions);
	sbSample->ChooseByValue(data->sample);
	ebName->SetString(data->name);
	
	// count the number of notes
	while (traverse)
	{
		numnotes++;
		traverse = traverse->next;
	}
	nbNotes->SetValue(numnotes);
	UpdateNotes();
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
	nbNBeat->SetMax(data->divisions);
	return NULL;
}

// if they change the panning of this one
void *Loop::Pan(void *pan)
{
	data->pan = ((structSelectList *)pan)->value;
	return NULL;
}

// if they change the number of loops that belong to this one
void *Loop::Notes(void *number)
{
	u16 num = *(u32 *)number;
	
	// if the number of loops is less than the amount of loops we actually have
	// then delete notes until we have the right number
	while (num < numnotes)
	{
		DelNote();
	}
	// if it's greater, than add notes until we have the right number
	while (num > numnotes)
	{
		AddNote();
	}
	
	return NULL;
}

// add a note
void Loop::AddNote()
{
	globals.NewNote();
	numnotes++;
	// set the current note's default beat to be the current beat
	globals.currentnote->offset = ((numnotes - 1) % globals.currentloop->divisions) + 1;
	UpdateNotes();
}

// delete a note
void Loop::DelNote()
{	
	globals.DelNote();
	numnotes--;
	UpdateNotes();
}

// update the list of notes
void Loop::UpdateNotes()
{
	u32 i=0;
	structNoteData *traverse = globals.currentloop->notes;
	
	debug("numnotes = %d", numnotes);
	
	if (notes)
	{
		debug("freeing notes array");
		delete[] notes;
		notes = NULL;
	}
	
	if (numnotes)
	{
		notes = new ptrNoteData[numnotes];
		debug("Base note array address: 0x%lx", (u32)notes);
		while (traverse)
		{
			notes[i] = traverse;
			debug("Element %ld address: 0x%lx", i, (u32)notes[i]);
			i++;
			traverse = traverse->next;
		}
	}
	else
	{
		debug("empty list");
	}
	
	// update the beat select number box
	if (numnotes)
		nbNote->SetMax(numnotes - 1);
	else
		nbNote->SetMax(0);
	
	i = nbNote->GetValue();
	// update the current note data
	NoteChange(&i);
}

// if they change the name of this loop
void *Loop::Name(void *name)
{
	globals.SetLoopName((char *)name);
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
//	u32 diff = 0;
	beat = globals.beat % nbBeats->GetValue();
	
	// are we playing a sample?
	if ((sbSample->GetChoice() != 0xFF) && kramHandleValid(handle))
	{
		// if we have notes, then things are handled a bit differently
		if (notes)
		{
			// figure out if we're at the swing point
			// diff = globals.beat *  - counter;
//			if (diff == notes[beat]->swing)
//			{
				// update the parameters
//				UpdateParameters();
//			}
				
			// if cont is not on
				// are we past the end of this note, or before the beginning?
					// if cut is on, then set vol to zero
					// if loop is on, then 
						// if bounce is on then i do'nt konw what the hell to do
		}
		else
		{
			// otherwise it's simple style parameter updates
			if (beat != lastbeat)
			{
				UpdateParameters();
				lastbeat = beat;
			}
		}
	}
	
	debugloop("Beat: %d", beat);
}

// when a swap occurs, select the correct loop
void Loop::DoSwap()
{
	globals.SetCurrentLoop(data);
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

// update all of the currently selected note parameters
void *Loop::NoteChange(void *note)
{
	u16 selected = *(u32 *)note;
	
	if (numnotes)
	{
		nbNBeat->SetValue(notes[selected]->offset);
		nbNPitch->SetValue(notes[selected]->pitch);
		nbNSwing->SetValue(notes[selected]->swing);
		sbNAction->ChooseByValue(notes[selected]->noteEnd);
	}
	else
	{
		// set everything to zeros
		nbNBeat->SetValue(0);
		nbNPitch->SetValue(0);
		nbNSwing->SetValue(0);
		sbNAction->ChooseByValue(0);
	}
	
	return NULL;
}

// these are what happens when the note's parameters are changed

void *Loop::NBeat(void *number)
{
	u8 offset = (u8)*(u32 *)number;
	notes[nbNote->GetValue()]->offset = offset;
	debug("note %d (0x%lx) Beat = %d", nbNote->GetValue(), notes[nbNote->GetValue()], notes[nbNote->GetValue()]->offset);
	return NULL;
}

void *Loop::NSwing(void *number)
{
	u8 swing = (u8)*(u32 *)number;
	notes[nbNote->GetValue()]->swing = swing;
	debug("note %d (0x%lx) swing = %d", nbNote->GetValue(), notes[nbNote->GetValue()], notes[nbNote->GetValue()]->swing);
	return NULL;
}

void *Loop::NPitch(void *number)
{
	u8 pitch = (u8)*(u32 *)number;
	notes[nbNote->GetValue()]->pitch = pitch;
	debug("note %d (0x%lx) pitch = %d", nbNote->GetValue(), notes[nbNote->GetValue()], notes[nbNote->GetValue()]->pitch);
	return NULL;
}

void *Loop::NAction(void *which)
{
	notes[nbNote->GetValue()]->noteEnd = (noteType)((structSelectList *)which)->value;
	debug("note %d (0x%lx) action = %d", nbNote->GetValue(), notes[nbNote->GetValue()], notes[nbNote->GetValue()]->noteEnd);
	return NULL;
}
