/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#include "ClarkMix.hh"
#include "Keys.hh"
#include "samples/samples.hh"

// IntrTable for crt0
void (*IntrTable[])() =
{
	0, // v-blank
	0, // h-blank
	0, // vcount
	0, // timer0
	0, // timer1
	0, // timer2
	0, // timer3
	0, // serial
	0, // dma0
	0, // dma1
	0, // dma2
	0, // dma3
	0, // key
	0  // cart
};

// This is a test of the clarkinou mixing system

int main()
{
	Keys *keys = new Keys();
	ClarkMix *mixer = new ClarkMix();
	Sample *mysample = new Sample((SampleData *)&samples[0]);
	Sample *other = new Sample((SampleData *)&samples[1]);
	s8 rotpan=0;
	
	mixer->Manage(other);
	mixer->Manage(mysample);
	
	// check keys
        while (keys->TestKey(keyA) != pressed)
        {
                keys->Jiffie();
		// mix our buffer if it needs it
		mixer->DoMix();
        }
        keys->Jiffie();

	dprintf("Turning off volume of mysample\n");
	// try volume tests
	mysample->SetVolume(0);
	dprintf("---\n");
	
	// check keys
        while (keys->TestKey(keyA) != pressed)
        {
                keys->Jiffie();
		// mix our buffer if it needs it
		mixer->DoMix();
        }
        keys->Jiffie();

	dprintf("Turning on volume of mysample to half\n");
	dprintf("Setting Panning of samples to opposite ends\n");
	// try volume tests
	mysample->SetVolume(6);
	mysample->SetPanning(8);
	other->SetPanning(-8);
	dprintf("---\n");

	// check keys
        while (keys->TestKey(keyA) != pressed)
        {
                keys->Jiffie();
		// mix our buffer if it needs it
		mixer->DoMix();
        }
        keys->Jiffie();

	dprintf("Pausing mysample\n");
	dprintf("setting other sample loop start to %ld\n", other->GetLength()/8 * 1);
	dprintf("setting other sample loop end to %ld\n", other->GetLength()/8 * 2);
	// try volume tests
	mysample->Pause();
	other->SetLoopStart(other->GetLength()/8 * 1);
	other->SetLoopEnd(other->GetLength()/8 * 2);
	dprintf("---\n");
	
	// check keys
        while (keys->TestKey(keyA) != pressed)
        {
                keys->Jiffie();
		// mix our buffer if it needs it
		mixer->DoMix();
        }
        keys->Jiffie();

	dprintf("Unpausing mysample\n");
	dprintf("Pausing other sample\n");
	dprintf("setting mysample velocity to half\n");
	// try volume tests
	mysample->Play();
	other->Pause();
	mysample->SetVelocity(128);
	dprintf("---\n");
	
	while(1)
	{
		if (keys->TestKey(keyA) == pressed)
		{
			debug("Length: %ld\n", mysample->GetLength());
			debug("Pos: %ld\n", mysample->GetLength()/16 * (rotpan + 8));
			mysample->SetPosition(mysample->GetLength()/16 * (rotpan + 8));
			rotpan++;
			if (rotpan == 9)
				rotpan = -8;
			mysample->SetPanning(rotpan);
			dprintf("Reset loop, pan: %d\n", rotpan);
		}
		keys->Jiffie();
		// mix our buffer if it needs it
		mixer->DoMix();
	}

	
	//mixer->playSample((u32)samples[0].data, (u32)&samples[0].data + samples[0].length, (u32)samples[0].data, (u32)samples[0].data + samples[0].length, 250, 0, 0, 0x0100);
	//mixer->playSample((u32)samples[1].data, (u32)&samples[1].data + samples[1].length, (u32)samples[1].data, (u32)samples[1].data + samples[1].length, 250, 0, 1, 0x0100);
}
