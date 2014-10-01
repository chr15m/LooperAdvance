// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "Loop.h"

const SampleOverride* const *samplesall = (const SampleOverride* const*)samples;

Loop::Loop(Keys *inkeys)
{
	u16 i;
	dprintf("Loop() init\n");
	UseKeys(inkeys);
	
	sample=0;
	pan = false;
	vol = false;
	dprintf("Init setting pan to %d.\n", pan);
	pitch = 0;
	beats = 0;
	
	lastbeat = 0;
	handle = 0;
	
	// set up the GUI
	nbOn = new NumberBox(10, 3, 1, 0, 1, 1, NULL, inkeys);
	nbPitch = new NumberBox(10, 4, 10, 1, 100000, 20, NULL, inkeys);
	nbPan = new NumberBox(10, 5, 1, 0, 1, 1, NULL, inkeys);
	nbBeats = new NumberBox(10, 6, 2, 1, 64, 8, NULL, inkeys);
	nbReset = new NumberBox(10, 7, 1, 0, 1, 1, NULL, inkeys);
	sbSample = new SelectBox(10, 9, 1, NULL, inkeys);
	
	nbOn->SetTransitions(NULL, NULL, NULL, nbPitch);
	nbPitch->SetTransitions(NULL, NULL, nbOn, nbPan);
	nbPan->SetTransitions(NULL, NULL, nbPitch, nbBeats);
	nbBeats->SetTransitions(NULL, NULL, nbPan, nbReset);
	nbReset->SetTransitions(NULL, NULL, nbBeats, sbSample);
	sbSample->SetTransitions(NULL, NULL, nbReset, NULL);
	
	// what samples are available?
	for (i=0; i<NUMSAMPLES; i++)
	{
		sbSample->NewChoice((char *)samplenames[i], i);
		dprintf((char *)samplenames[i]);
		dprintf("\n");
	}
	
	UseKeys(inkeys);
	nbOn->Select();
	selected = nbOn;
}

Loop::~Loop()
{
	delete nbPitch;
	delete nbPan;
	delete nbBeats;
}

void Loop::Draw()
{
	selected = selected->Process();

	if (selected == nbOn)
		vol = ((NumberBox *)nbOn)->GetValue();
	if (selected == nbBeats)
		beats = ((NumberBox *)nbBeats)->GetValue();
	if (selected == nbPitch)
		pitch = ((NumberBox *)nbPitch)->GetValue();
	if (selected == nbPan)
		pan = ((NumberBox *)nbPan)->GetValue();
	
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

void Loop::Process()
{
//	dprintf("Loop Process: %ld, sample handle = %d\n", (u32)this, handle);
	if ((sample != 0xFF) && kramHandleValid(handle))
	{
		// update our position
		beat = counter * bpm/3600;
		
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
	
	dprintf("Top = %ld\n", bpm * GetSize());
	dprintf("Bottom = %ld\n", (2646*beats));
	dprintf("New ratio = %d\n", (bpm * GetSize())/(2646*beats));
	pitch = (bpm * GetSize()) / (2646 * beats);
	nbPitch->SetValue(pitch);
}

u32 Loop::GetSize()
{
	return (u32)samplesall[sample]->end - (u32)samplesall[sample]->data;
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
