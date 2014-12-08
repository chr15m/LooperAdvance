/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: looper.cc,v 1.10 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include "krawall.h"
#include "samples.h"
#include "instruments.h"

#include "looper.h"
#include "charset.h"

#include "midi.h"

#include "Loop.h"
#include "First.h"

#include "scr_splash.h"
#include "scr_krawall_splash.h"
#include "scr_bg.h"

GlobalData *globals;

int main()
{
	// set up interrupts
	irqInit();
	irqSet( IRQ_TIMER1, kradInterrupt );
	irqEnable( IRQ_TIMER1 );
	// midi setup
	midiInit();
	irqSet( IRQ_TIMER0, midiInterrupt );
	irqEnable( IRQ_TIMER0 );

	REG_IME = 1;
	
	SetMode( MODE_0 | BG1_ON );
	
	// starting
	SetBG(10, 10, 0);
	
	// create the globals object
	globals = new GlobalData();
	
	// starting
	SetBG(0, 10, 10);

	globals->LoadSongs();

	// starting
	SetBG(10, 0, 10);

	u16 i=0;
	Keys *keys = new Keys();
	First *firstpage = new First(keys);
	Page *selected = firstpage;
	
	kragInit( KRAG_INIT_STEREO );					// init krawall
	
	// make it so we can see the background
	REG_BG1CNT = BIT(7) | BIT(8) | BIT(9) | BIT(10) | BIT(11) | BIT(12); // high priority, 256 colour, point the banks at the right place etc
	
	// load the splash screen
	DMA3COPY((void*)splash_tiles, (u16*)VideoBuffer, SPLASH_TILESIZE | DMA16 | DMA_IMMEDIATE | DMA_SRC_INC | DMA_DST_INC);
	DMA3COPY((void*)splash_palette, (u16*)&BG_PALETTE[5], SPLASH_PALSIZE | DMA16 | DMA_IMMEDIATE | DMA_SRC_INC | DMA_DST_INC);
	DMA3COPY((void*)splash_map, (u16*)&VideoBuffer[0x7C00], 640 | DMA16 | DMA_IMMEDIATE | DMA_SRC_INC | DMA_DST_INC);
	
	// check keys
	while (keys->TestKey(keyA) != pressed)
	{
		keys->Jiffie();
	}
	keys->Jiffie();

	// load the krawall splash screen
	DMA3COPY((void*)krawall_splash_tiles, (u16*)VideoBuffer, KRAWALL_SPLASH_TILESIZE| DMA16 | DMA_IMMEDIATE | DMA_SRC_INC | DMA_DST_INC);
	DMA3COPY((void*)krawall_splash_palette, (u16*)&BG_PALETTE[5], KRAWALL_SPLASH_PALSIZE | DMA16 | DMA_IMMEDIATE | DMA_SRC_INC | DMA_DST_INC);
	DMA3COPY((void*)krawall_splash_map, (u16*)&VideoBuffer[0x7C00], 640 | DMA16 | DMA_IMMEDIATE | DMA_SRC_INC | DMA_DST_INC);

	// check keys
	while (keys->TestKey(keyA) != pressed)
	{
		keys->Jiffie();
	}
	keys->Jiffie();
	
	// load charset into memory as a demo tileset
	DMA3COPY((void*)font::tiles, (u16*)VideoBuffer, 3232 | DMA16 | DMA_IMMEDIATE | DMA_SRC_INC | DMA_DST_INC);
	DMA3COPY((void*)font::tiles_color, (u16*)&VideoBuffer[3232], 3232 | DMA16 | DMA_IMMEDIATE | DMA_SRC_INC | DMA_DST_INC);
	
	// set up the background palette
	for (i=0;i<7;i++)
		BG_PALETTE[i] = RGB(0, 0, 0);
	// this starts from 7 becuase the kludgy legacy converter used a 7-color offset (for parallax-starfield reasons)
	
	BG_PALETTE[7] = RGB(0x1F, 0x1F, 0x1F);
	BG_PALETTE[8] = RGB(0, 0, 0);
	
	// set the H and V offset of our background to zero
	// REG_BG1HOFS = 0;	
	// REG_BG1VOFS = 0;
	
	// clear out the whole area
	BlankScreen();
	
	// CST_ROM0_1ST_3WAIT | CST_ROM0_2ND_1WAIT | CST_PREFETCH_ENABLE
	REG_WSCNT = ( 5 << 2 ) | ( 1 << 14 );	// set rom-timing
	
	while (1)
	{
                // while( !REG_VCOUNT );
                // while( REG_VCOUNT );

		// this is zerosync
		SetBG(0, 0, 10);
		
		// figure out all our latest song positions
		globals->Tick();
		// calculate audio stuff
		kramWorker();
		// update midi progression
		midiUpdateBeat(globals->beat);
		// check keys
		keys->Jiffie();
		// automatically cascades all pages and processes them
		firstpage->Process();
		
		SetBG(0, 0, 0);
		
		// this is vsync
		// try and fit the drawing stuff into the off-screen sync
		while(REG_VCOUNT != 160);
		SetBG(10, 0, 0);
		// do all the display shit in here
		BlankScreen();
		selected = selected->Cycle();
		selected->Draw();
	}
	
	return 1;
}
