#include "krawall.h"
#include "samples.h"
#include "instruments.h"

#include "looper.hh"
#include "charset.hh"

#include "Loop.hh"
#include "First.hh"

// IntrTable for crt0
void (*IntrTable[])() =
{
	0, // v-blank
	0, // h-blank
	0, // vcount
	0, // timer0
	kradInterrupt, // timer1
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
	Keys *keys = new Keys();
	First *firstpage = new First(keys);
	Page *selected = firstpage;

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
	//BGPaletteMem[8] = RGB(0, 15, 26);
	
	// set the H and V offset of our background to zero
	REG_BG1HOFS = 0;	
	REG_BG1VOFS = 0;
	
	// clear out the whole area
	BlankScreen();
	
	// CST_ROM0_1ST_3WAIT | CST_ROM0_2ND_1WAIT | CST_PREFETCH_ENABLE
	REG_WSCNT = ( 5 << 2 ) | ( 1 << 14 );	// set rom-timing
	
	// initialize krawall
	kragInit( KRAG_INIT_STEREO );					// init krawall
	
	while (1)
	{
		// this is zerosync
		while(REG_VCOUNT);
		// figure out all our latest song positions
		globals.Tick();
		// calculate audio stuff
		kramWorker();
		// check keys
		keys->Jiffie();
		// automatically cascades all pages and processes them
		firstpage->Process();
		
		// this is vsync
		while(REG_VCOUNT != 160);

		// do all the display shit in here
		BlankScreen();
		selected = selected->Cycle();
		selected->Draw();
	}
}
