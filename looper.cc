/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#include "samples/samples.hh"

#include "looper.hh"
#include "charset.hh"

#include "Loop.hh"
#include "First.hh"
#include "ClarkMix.hh"

#include "splash.hh"
#include "screens.hh"

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

GlobalData globals;

int main()
{
	// starting
	//SetBG(10, 10, 0);
	
	// i don't get why this isn't run automatically when the globaldata object is created
	// perhaps my understanding of c++ isn't so strong :]
	globals.Init();
	
	// starting
	//SetBG(0, 10, 10);

	globals.LoadSongs();

	// starting
	//SetBG(10, 0, 10);

	// set up the krawall interrupt now that this is done
	//IntrTable[4] = kradInterrupt;
	
	Keys *keys = new Keys();
	
	// set up a nice screen mode
	REG_DISPCNT = SCREENMODE0 | BG3_ENABLE | BG2_ENABLE | BG1_ENABLE | BG0_ENABLE;
	REG_IME = 1;			// enable interrupts
	REG_IME |= ( 1 << 13 ); 	// cart-remove-interrupt (handled by crt0)
	REG_IE |= BIT00;
	
	REG_BG0CNT = BIT07 | BIT10 | BIT11 | BIT12;
	REG_BG1CNT = BIT07 | BIT08 | BIT10 | BIT11 | BIT12;
	REG_BG2CNT = BIT07 | BIT09 | BIT10 | BIT11 | BIT12;
	REG_BG3CNT = BIT07 | BIT08 | BIT09 | BIT10 | BIT11 | BIT12;
	
	// initialise video memory
	bzero((char *)VideoBuffer, 0x8000);
	
	// make it white for the splash
	SetBG(SPLASH_BG_R, SPLASH_BG_B, SPLASH_BG_G);
	
	// load the splash screen
	DMACopy((void*)splash_tiles, (u16*)VideoBuffer, SPLASH_TILES_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	DMACopy((void*)splash_palette, (u16*)BGPaletteMem, SPLASH_PALETTE_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	DMACopy((void*)splash_map, (u16*)&VideoBuffer[BG0], SPLASH_MAP_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	
	// check keys
	while (keys->TestKey(keyA) != pressed)
	{
		keys->Jiffie();
	}
	keys->Jiffie();
	
	// blank our writing surface
	BlankScreen(BG0);
	BlankScreen(BG1);
	BlankScreen(BG2);
	BlankScreen(BG3);

	debug("Setting Background Colour to %d,%d,%d\n", SCREENS_BG_R, SCREENS_BG_B, SCREENS_BG_G);
	SetBG(SCREENS_BG_R, SCREENS_BG_B, SCREENS_BG_G);
	
	DMACopy((void*)screens_palette, (u16*)BGPaletteMem, SCREENS_PALETTE_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	DMACopy((void*)font_tiles, (u16*)&VideoBuffer[FONT_TILES_BASE], FONT_TILES_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	DMACopy((void*)altfont_tiles, (u16*)&VideoBuffer[FONT_TILES_BASE + FONT_TILES_SIZE], ALTFONT_TILES_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	
	// load in the tiles for our backgrounds
	DMACopy((void*)screens_tiles, (u16*)&VideoBuffer[0], SCREENS_TILES_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	DMACopy((void*)first_map, (u16*)&VideoBuffer[BG2], FIRST_MAP_SIZE, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	
	// set the H and V offset of our background to zero
	REG_BG1HOFS = 0;
	REG_BG1VOFS = 0;
	
	// CST_ROM0_1ST_3WAIT | CST_ROM0_2ND_1WAIT | CST_PREFETCH_ENABLE
	//REG_WSCNT = ( 5 << 2 ) | ( 1 << 14 );	// set rom-timing
	
	ClarkMix *mixer = new ClarkMix();
	First *firstpage = new First(keys, mixer);
	Page *selected = firstpage;
	
	while (1)
	{
		// this is zerosync
		//SetBG(0, 0, 10);
		
		// figure out all our latest song positions
		globals.Tick();
		// check keys
		keys->Jiffie();
		// automatically cascades all pages and processes them
		firstpage->Process();
		
		// mix our buffer if it needs it
		mixer->DoMix();
		
		debug("Loop");
		//SetBG(0, 0, 0);
		
		// this is vsync
		// try and fit the drawing stuff into the off-screen sync
		while(REG_VCOUNT != 160);
		//SetBG(10, 0, 0);
		// do all the display shit in here
		BlankScreen();
		selected = selected->Cycle();
		selected->Draw();
		
		// mix our buffer if it needs it
		mixer->DoMix();
	}
}
