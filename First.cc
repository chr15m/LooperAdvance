// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page

#include "First.hh"
#include "songdata.hh"

First::First(Keys *inkeys)
{
	u16 i;
	
	// SelectBox(x, y, iwidth, *inext, *inkeys);
	sbSong = new SelectBox(6, 1, 8, NULL, inkeys);
	for (i=0; i<NUMSONGS; i++)
		sbSong->NewChoice(songs::data[i].name, i);

	// NumberBox(x, y, width, min, max, bigstep, next);
	nbBPM = new NumberBox(6, 3, 2, 10, 350, 10, NULL, inkeys);
	nbBeat = new NumberBox(24, 1, 4, 0, 9999, 1, NULL, inkeys);

	sbSong->SetTransitions(NULL, NULL, NULL, nbBPM);
	nbBPM->SetTransitions(NULL, NULL, sbSong, NULL);

	nbBPM->SetValue(bpm);
	oldsong = 1;
	
	UseKeys(inkeys);
	sbSong->Select();
	selected = sbSong;
}

First::~First()
{
	delete nbBPM;
	delete sbSong;
}

void First::Draw()
{
	u16 beat;
	
	selected = selected->Process();
	if (selected == nbBPM)
		bpm = ((NumberBox *)selected)->GetValue();

	// labels
	cprintf(1,1,"Song");
	sbSong->Draw();

	cprintf(1,3,"BPM");
	nbBPM->Draw();
	
	cprintf(20,1,"Beat");
	beat = counter * bpm/3600;
	nbBeat->SetValue(beat);
	nbBeat->Draw();
}

void First::Process()
{
	u16 i;
	Loop *ptrLoop;
	structSongData *dataptr;

	dprintf("First Process: %ld\n", (u32)this);
	
	// if they've chosen a new song, load it
	if (oldsong != sbSong->GetChoice())
	{
		dataptr = (structSongData *)&(songs::data[sbSong->GetChoice()]);
		nbBPM->SetValue(dataptr->tempo);
		bpm = dataptr->tempo;
		ptrLoop = (Loop *)right;
		for(i=0;i<4;i++)
		{
			ptrLoop->SetParameters(dataptr->loop[i].sample, dataptr->loop[i].pan, dataptr->loop[i].pitch, dataptr->loop[i].beats);
			ptrLoop = (Loop *)ptrLoop->right;
		}
		oldsong = sbSong->GetChoice();
	}
}
