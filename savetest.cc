/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#include "looper.hh"
#include "charset.hh"

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
	u16 i=0;
	
	globals.Init();
	
	// set up a nice screen mode
	REG_DISPCNT = SCREENMODE0 | BG1_ENABLE; // 4 | ( 1 << 10 );
	REG_IME = 1;			// enable interrupts
	REG_IME |= ( 1 << 13 ); 	// cart-remove-interrupt (handled by crt0)
	REG_IE |= BIT00;
	
	// make it so we can see the background
	REG_BG1CNT = BIT07 | BIT08 | BIT09 | BIT10 | BIT11 | BIT12; // high priority, 256 colour, point the banks at the right place etc
	
	// load charset into memory as a demo tileset
	DMACopy((void*)font::tiles, (u16*)VideoBuffer, 3232, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	DMACopy((void*)font::tiles_color, (u16*)&VideoBuffer[3232], 3232, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	
	// set up the background palette
	for (i=0;i<7;i++)
		BGPaletteMem[i] = RGB(0, 0, 0);
	
	// this starts from 7 becuase the kludgy legacy converter used a 7-color offset (for parallax-starfield reasons)
	BGPaletteMem[7] = RGB(0x1F, 0x1F, 0x1F);
	
	// set the H and V offset of our background to zero
	REG_BG1HOFS = 0;	
	REG_BG1VOFS = 0;
	
	// clear out the whole area
	BlankScreen();
	
	// CST_ROM0_1ST_3WAIT | CST_ROM0_2ND_1WAIT | CST_PREFETCH_ENABLE
	REG_WSCNT = ( 5 << 2 ) | ( 1 << 14 );	// set rom-timing
	
	dprintf("Loading and saving\n");
	
	globals.LoadSongs();
//	globals.NewSong();
//	globals.NewLoop();
//	globals.NewNote();
//	globals.DelSong();
	globals.DelSong();	
	globals.SetName("pants");
	globals.SaveSongs();
	
	while (1);
}

