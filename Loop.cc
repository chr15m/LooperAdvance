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

#include "Loop.hh"

Loop::Loop(Keys *inkeys, structLoopData *whichloop, ClarkMix *imixer)
{
	u16 i;
	debug("Loop() init at 0x%lx", (u32)whichloop);
	UseKeys(inkeys);
	
	data = whichloop;
	
	lastbeat = 0;
	numnotes = 0;
	notes = NULL;
	mixer = imixer;
	
	// set up the GUI
	debug("Setting up GUI");

	// initialize our number
	sprintf(beatNumberText, "%3d", beat);
	
	ebName = new EditBox(10, 1, 10, inkeys);
	lbBeatNumber = new Label(26, 1, beatNumberText);
	sbOn = new SelectBox(10, 2, 3, inkeys);
	debug("sbOn: 0x%lx", (u32)sbOn);
	nbPitch = new NumberBox(10, 4, 6, 1, 100000, 32, inkeys);
	sbPan = new SelectBox(10, 5, 5, inkeys);
	nbBeats = new NumberBox(10, 6, 2, 1, 64, 8, inkeys, true);
	sbReset = new SelectBox(10, 7, 5, inkeys);
	sbSample = new SelectBox(10, 8, 10, inkeys);
	
	lbPitch = new Label(2, 4, "Pitch");
	lbBeats = new Label(2, 6, "Slices");
	lbSample = new Label(2, 8, "Sample");
	lbSwing = new Label(2, 11, "Swing");
	
	nbSwing = new NumberBox(10, 11, 3, 0, 999, 20, inkeys);
	
	// notes interface
	nbNotes = new NumberBox(10, 12, 3, 0, 256, 4, inkeys);
	lbNotes = new Label(2, 12, "Steps");
	lbStep = new Label(2, 13, "Step");
	
	nbNote = new NumberBox(10, 13, 3, 0, 0, 8, inkeys);
	nbNBeat = new NumberBox(10, 15, 3, 0, 0, 8, inkeys, true);
	nbNSwing = new NumberBox(10, 16, 3, 0, 255, 10, inkeys, true);
	nbNPitch = new NumberBox(10, 17, 3, 0, 255, 12, inkeys);
	sbNAction = new SelectBox(9, 18, 6, inkeys);
	
	// notes labels
	lbNBeat = new Label(2, 15, "Slice");
	lbNSwing = new Label(2, 16, "Swing");
	lbNPitch = new Label(2, 17, "Note");	
	lbNAction = new Label(2, 18, "Action");

	sbAddLoopButton = new SelectBox(19, 17, 8, inkeys);
	sbDelLoopButton = new SelectBox(19, 18, 8, inkeys);

	AddWidget(ebName);
	AddWidget(lbBeatNumber);
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
	AddWidget(lbSample);
	
	AddWidget(lbStep);
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
	
	ebName->SetTransitions(NULL, sbAddLoopButton, NULL, sbOn);
	sbOn->SetTransitions(NULL, sbAddLoopButton, ebName, nbPitch);
	nbPitch->SetTransitions(NULL, sbAddLoopButton, sbOn, sbPan);
	sbPan->SetTransitions(NULL, sbAddLoopButton, nbPitch, nbBeats);
	nbBeats->SetTransitions(NULL, sbAddLoopButton, sbPan, sbReset);
	sbReset->SetTransitions(NULL, sbAddLoopButton, nbBeats, sbSample);
	sbSample->SetTransitions(NULL, sbAddLoopButton, sbReset, nbSwing);
	nbSwing->SetTransitions(NULL, sbAddLoopButton, sbSample, nbNotes);
	nbNotes->SetTransitions(NULL, sbAddLoopButton, nbSwing, nbNote);
	nbNote->SetTransitions(NULL, sbAddLoopButton, nbNotes, nbNBeat);
	nbNBeat->SetTransitions(NULL, sbAddLoopButton, nbNote, nbNSwing);
	nbNSwing->SetTransitions(NULL, sbAddLoopButton, nbNBeat, nbNPitch);
	nbNPitch->SetTransitions(NULL, sbAddLoopButton, nbNSwing, sbNAction);
	sbNAction->SetTransitions(NULL, sbAddLoopButton, nbNPitch, NULL);

	sbNAction->NewChoice("cont", note_continue);
	sbNAction->NewChoice("cut", note_cut);
	sbNAction->NewChoice("loop", note_loop);
	sbNAction->NewChoice("feed", note_feed);
	sbNAction->NewChoice("strtch", note_stretch);
	
	sbAddLoopButton->SetTransitions(sbSample, NULL, nbNotes, sbDelLoopButton);
	sbDelLoopButton->SetTransitions(sbSample, NULL, sbAddLoopButton, NULL);
	
	debug("Filling sample list");
	
	// what samples are available?
	for (i=0; i<NUMSAMPLES; i++)
	{
		sbSample->NewChoice((char *)samples[i].name, i);
		debug("Sample: %s", (char *)samples[i].name);
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
	
	cbSwing.MakeCallback(this, &Loop::Swing);
	nbSwing->UseCallBack(&cbSwing);
	
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
	
	// create a new sample and tell the mixer to start managing it
	sample = new Sample((SampleData *)&samples[0]);
	sample->Pause();
	mixer->Manage(sample);
	
	globals.SetCurrentLoop(data);
	
	SampleChange(NULL);
	UpdateWidgets();
	
	debug("Done setting up");
}

Loop::~Loop()
{
	debug("Deleting loop");
	
	sample->Pause();
	mixer->Forget(sample);
	delete sample;
	
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
void *Loop::AddLoopButton(void *mydata)
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
		right->left = new Loop(keys, globals.currentloop, mixer);
		right = right->left;
		right->left = this;
		right->right = old;
	}
	else
	{
		debug("Appending a new loop");
		right = new Loop(keys, globals.currentloop, mixer);
		right->left = this;
	}
	
	globals.SetCurrentLoop(data);
	
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
	debug("Set pitch to %d", data->pitch);
	return NULL;
}

// what to do if the number of beat divisions is changed
void *Loop::Beats(void *number)
{
	u16 i;
	
	data->divisions = *((u16 *)number);
	nbNBeat->SetMax(data->divisions);
	
	// check that none of our current notes are past the end of the max allowed beats
	for (i=0; i<numnotes; i++)
	{
		if (notes[i]->offset > data->divisions)
		{
			notes[i]->offset = (notes[i]->offset % globals.currentloop->divisions);
		}
	}
	return NULL;
}

// what to do if the swing number is changed
void *Loop::Swing(void *number)
{
	u16 i, swing = (u16)(*(u32*)number);
		
	// set every second note to be a swung note
	for (i=1; i<numnotes; i+=2)
	{
		notes[i]->swing = swing;
	}
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
	// (1000 * (tracks[selected].sample->len) / (tracks[selected].sample->freq) ) / (tracks[selected].bpl * T * 4 / 1000
	// (256 * (tracks[selected].sample->len) / (tracks[selected].sample->freq) ) / (tracks[selected].bpl * T * 4 / 256)
	
	// 256 * ((length / PLAYER_ACTUAL_FREQ) / (beats_per_loop * (60/BPM)))
	// = 4 * length * BPM / player_freq * bpl
	
	u32 p1 = (sample->GetLength() * globals.currentsong->bpm) << 2;
	u32 p2 = nbBeats->GetValue() * PLAYER_ACTUAL_FREQ;
	u32 result = p1 / p2;
	
	debug("New pitch: %ld", result);
	
	nbPitch->SetValue(result);
	Pitch(&result);
	return NULL;
}

// process the audio
void Loop::DoProcess()
{
	u32 diff = 0;
	// figure out if we're at the swing point
	diff = globals.counter - (globals.beat * 3600 / globals.currentsong->bpm);
	
	// are we playing a sample?
	if ((sbSample->GetChoice() != 0xFF))
	{
		// if we have notes, then things are handled a bit differently
		if (notes)
		{
			beat = globals.beat % numnotes;
			
			debugloop("diff: %ld", diff);
			if ((diff >= notes[beat]->swing) && (lastbeat != globals.beat))
			{
				debug("Swing detected: %ld", diff);
				// update the parameters
				UpdateParametersNotes();
				lastbeat = globals.beat;
			}
			
			// if cont is not on
			if (notes[beat]->noteEnd != note_continue && notes[beat]->noteEnd != note_feed)
			{
				debug("Pos: %ld > %ld > %ld", sample->GetPosition(), (sample->GetLength() / nbBeats->GetValue()) * (notes[beat]->offset - 1), (sample->GetLength() / nbBeats->GetValue()) * (notes[beat]->offset));
				
				// if cut is on, then set vol to zero
				if (notes[beat]->noteEnd == note_stretch)
				{
					UpdateParametersNotes();
				}
				// are we past the end of this note, or before the beginning?
				else if ((sample->GetPosition() < (sample->GetLength() / nbBeats->GetValue()) * (notes[beat]->offset - 1)) || (sample->GetPosition() > (sample->GetLength() / nbBeats->GetValue()) * (notes[beat]->offset)))
				{
					// if cut is on, then set vol to zero
					if (notes[beat]->noteEnd == note_cut)
					{
						sample->Pause();
					}
					else if (notes[beat]->noteEnd == note_loop)
					{
						// if loop is on, then set us to the start of this note again (call update parameters)
						UpdateParametersNotes();
					}
				}
			}
		}
		else
		{
			// otherwise it's simple style parameter updates
			beat = globals.beat % nbBeats->GetValue();
			
			debugloop("Diff: %ld", diff);
			
			if ((diff >= (u32)(nbSwing->GetValue() * (beat | 1))) && (lastbeat != globals.beat))
			{
				UpdateParameters();
				lastbeat = globals.beat;
			}
		}
	}
	
	// output our beat. Yeh!
	sprintf(beatNumberText, "%3d", beat % 256);
	
	debugloop("Beat: %d", beat);
}

// when a page swap occurs
void Loop::DoSwap()
{
	// update our background
	DMACopy((void*)loop_map, (u16*)&VideoBuffer[BG2], LOOP_MAP_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	// select the right loop
	globals.SetCurrentLoop(data);
}

// update the parameters of a specific loop
void Loop::UpdateParameters()
{
	//sample->SetFrequency();
	//kramSetFreq(handle, ((nbPitch->GetValue() * 441) / 10));
	sample->SetVelocity(nbPitch->GetValue());
	sample->SetPanning(sbPan->GetChoice() * 16 - 8);
	sample->SetPlaying(sbOn->GetChoice());
	sample->SetPosition((sample->GetLength() * (beat % nbBeats->GetValue()))/nbBeats->GetValue());
}

// update the parameters if we're using notes
void Loop::UpdateParametersNotes()
{
	u16 newpos = notes[beat]->offset;
	s16 octave = 0;
	s16 note = 0;
	u32 oct_pitch = 0;
	
	debug("Note lookup: %d", beat);
	
	octave = notes[beat]->pitch / 12;
	note = (notes[beat]->pitch % 12);
	oct_pitch = nbPitch->GetValue() << octave;
	debug("Octave: %d, Note: %d", octave, note);
	//sample->SetFrequency();
	// kramSetFreq(handle, (((nbPitch->GetValue() * frequency[(12 + note) % 12]) >> -(octave - 1)) / 1000));
	//sample->SetVelocity((notes[beat]->pitch << octave) * (note + 12) / 12);
	sample->SetVelocity((oct_pitch + (note * oct_pitch) / 12) >> 2);
	
	sample->SetPanning(sbPan->GetChoice() * 16 - 8);
	if (newpos)
	{
		sample->SetPlaying(sbOn->GetChoice());
		sample->SetPosition((sample->GetLength() * (newpos - 1))/nbBeats->GetValue());
	}
	else
	{
		// if feed is on then don't stop the note at the end of the loop
		if (notes[beat]->noteEnd != note_feed)
		{
			sample->Pause();
		}
	}
}

// what to do when they select a different sample
void *Loop::SampleChange(void *whichsample)
{
	debug("Changing sample");

	if (whichsample)
	{
		data->sample = ((structSelectList *)whichsample)->value;
		sample->SetData((SampleData *)&samples[data->sample]);
		Reset(NULL);
	}
	
	UpdateParameters();
	
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
	if (notes)
	{
		notes[nbNote->GetValue()]->offset = offset;
		debug("note %d (0x%lx) Beat = %d", nbNote->GetValue(), (u32)notes[nbNote->GetValue()], notes[nbNote->GetValue()]->offset);
	}
	return NULL;
}

void *Loop::NSwing(void *number)
{
	u8 swing = (u8)*(u32 *)number;
	if (notes)
	{
		notes[nbNote->GetValue()]->swing = swing;
		debug("note %d (0x%lx) swing = %d", nbNote->GetValue(), (u32)notes[nbNote->GetValue()], notes[nbNote->GetValue()]->swing);
	}
	return NULL;
}

void *Loop::NPitch(void *number)
{
	u8 pitch = (u8)*(u32 *)number;
	if (notes)
	{
		notes[nbNote->GetValue()]->pitch = pitch;
		debug("note %d (0x%lx) pitch = %d", nbNote->GetValue(), (u32)notes[nbNote->GetValue()], notes[nbNote->GetValue()]->pitch);
	}
	return NULL;
}

void *Loop::NAction(void *which)
{
	if (notes)
	{
		notes[nbNote->GetValue()]->noteEnd = (noteType)((structSelectList *)which)->value;
		debug("note %d (0x%lx) action = %d", nbNote->GetValue(), (u32)notes[nbNote->GetValue()], notes[nbNote->GetValue()]->noteEnd);
	}
	return NULL;
}
