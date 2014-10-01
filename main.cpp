#include "krawall.h"
#include "samples.h"
#include "instruments.h"

#include "looper.h"
#include "charset.h"

#include "Loop.h"
#include "First.h"

// IntrTable for crt0
void (*IntrTable[])() = {
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

u32 counter=0;
u32 bpm=180;
// char __EH_FRAME_BEGIN__[] = {};
// to resolve link-errors w/o crtbegin.o (-nostartfiles), which we don't really need in C

int main()
{
	u16 i=0;
	Keys *keys = new Keys();
	First *firstpage = new First(keys);
	Page *selected = firstpage;
	Page *traverse;
	// Loop *which;
	
	firstpage->right = new Loop(keys);
	firstpage->right->left = firstpage;
	firstpage->right->right = new Loop(keys);
	firstpage->right->right->left = firstpage->right;
	firstpage->right->right->right = new Loop(keys);
	firstpage->right->right->right->left = firstpage->right->right;
	firstpage->right->right->right->right = new Loop(keys);
	firstpage->right->right->right->right->left = firstpage->right->right->right;
	
	// set up a nice screen mode
	REG_DISPCNT = SCREENMODE0 | BG1_ENABLE; // 4 | ( 1 << 10 );
	REG_IME = 1;			// enable interrupts
	REG_IME |= ( 1 << 13 ); 	// cart-remove-interrupt (handled by crt0)
	
	// make it so we can see the background
	REG_BG1CNT = BIT07 | BIT08 | BIT09 | BIT10 | BIT11 | BIT12; // high priority, 256 colour, point the banks at the right place etc
	
	// load charset into memory as a demo tileset
	DMACopy((void*)font::tiles, (u16*)VideoBuffer, 3232, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	DMACopy((void*)font::tiles_color, (u16*)&VideoBuffer[3232], 3232, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_INCREMENT | DMA_DEST_INCREMENT);
	
	// set up the background palette
	for (i=0;i<7;i++)
		BGPaletteMem[i] = RGB(0, 0, 0);
	// this starts from 7 becuase the kludgy legacy converter used a 7-color offset (for parallax-starfield reasons)
	//for (i=7;i<0x1F;i++)
	//	BGPaletteMem[i] = RGB(0x1F-i, 0x1F-i, 0x1F-i);
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

//	which = (Loop *)firstpage->right;
//	which->SetParameters(SAMPLE_AMEN, true, 1000, 8);
	
	while (1)
	{
//		dprintf("Main loop\n");
		// this is zerosync
		while(REG_VCOUNT);
		SetBG(10, 0, 0);
//		krapInstProcess();	// update sfx-instruments
		kramWorker();		// do the stuff
		keys->Jiffie();
		// cycle through each page and process it
		traverse = firstpage;
		while (traverse)
		{
			traverse->Process();
			traverse = traverse->right;
		}
		SetBG(3, 3, 3);
		
		// this is vsync
		while(REG_VCOUNT != 160);
		SetBG(0, 0, 10);
		// do all the display shit in here
		BlankScreen();
		selected = selected->Cycle();
		selected->Draw();
		
		// increment the global counter
		counter++;
		SetBG(0, 0, 0);
	}
}


