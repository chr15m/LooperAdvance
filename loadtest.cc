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
#include "Keys.hh"

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
	u16 songcounter=0;
	u16 loopcounter=0;
	u16 notecounter=0;
	
	globals.Init();
	
	Keys *keys = new Keys();

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
	
	u16 checkmagic=0;
	u16 magic = 0xABCD;
	
	REG_IME = 0; // disable interrupts
	
	debug("Loading songs.");
	cprintf(1, 1, "load test");

	globals.ReadNumber(&checkmagic, sizeof(u16));
	
	// check first if there is any saved data in the databank
	if (checkmagic == magic)
	{
		cprintf(1, 2, "found magic number");
		
		while (keys->TestKey(keyA) != pressed)
			keys->Jiffie();
		keys->Jiffie();
		BlankScreen();

		// until we find the end of the songs
		while (!globals.CheckMagic())
		{
			cprintf(1, 2, "song number %d", songcounter);

			// start a new song
			debug("Creating a new song struct.");
			globals.NewSong();
			
			debug("Reading basic song data.");
			
			// read in the song name
			globals.ReadString(&globals.currentsong->name);
			debug("Song Name: %s", globals.currentsong->name);
			cprintf(1, 3, "Name: %s", globals.currentsong->name);

			// write bpm
			globals.ReadNumber(&globals.currentsong->bpm, sizeof(u16));
			debug("Song Speed: %d", globals.currentsong->bpm);
			cprintf(1, 4, "BPM: %d", globals.currentsong->bpm);
			
			while (keys->TestKey(keyA) != pressed)
				keys->Jiffie();
			keys->Jiffie();
			BlankScreen();
			
			// until we find the end of the loops
			while (!globals.CheckMagic())
			{
				// create a new loop
				debug("Creating a new loop struct.");
				globals.NewLoop();
				cprintf(1, 2, "loop number %d", loopcounter);
				
				debug("Reading basic loop data.");
				
				// write the loop name
				globals.ReadString(&globals.currentloop->name);
				cprintf(1, 3, "Name: %s", globals.currentloop->name);
				// write the sample number
				globals.ReadNumber(&globals.currentloop->sample, sizeof(u16));
				cprintf(1, 4, "Sample: %d", globals.currentloop->sample);
				// write the panning direction
				globals.ReadNumber(&globals.currentloop->pan, sizeof(bool));
				cprintf(1, 5, "Pan: %d", globals.currentloop->pan);
				// write the pitch
				globals.ReadNumber(&globals.currentloop->pitch, sizeof(u16));
				cprintf(1, 6, "Pitch: %d", globals.currentloop->pitch);
				// write the number of divisions
				globals.ReadNumber(&globals.currentloop->divisions, sizeof(u16));
				cprintf(1, 7, "Divisions: %d", globals.currentloop->divisions);

				while (keys->TestKey(keyA) != pressed)
					keys->Jiffie();
				keys->Jiffie();
				BlankScreen();
				
				while (!globals.CheckMagic())
				{						
					// create a new note
					debug("Creating a new note struct.");
					globals.NewNote();
					cprintf(1, 2, "note number %d", notecounter);
					
					debug("Reading basic note data.");
					
					// write the note end action;
					globals.ReadNumber(&globals.currentnote->noteEnd, sizeof(u8));
					cprintf(1, 3, "note end action %d", globals.currentnote->noteEnd);
					// write the beat offset
					globals.ReadNumber(&globals.currentnote->offset, sizeof(u8));
					cprintf(1, 4, "Offset: %d", globals.currentnote->offset);
					// write the pitch
					globals.ReadNumber(&globals.currentnote->pitch, sizeof(u8));
					cprintf(1, 5, "Pitch: %d", globals.currentnote->pitch);
					// write the swing
					globals.ReadNumber(&globals.currentnote->swing, sizeof(u8));
					cprintf(1, 6, "Swing: %d", globals.currentnote->swing);
					
					while (keys->TestKey(keyA) != pressed)
						keys->Jiffie();
					keys->Jiffie();
					BlankScreen();

					notecounter++;
				}
				loopcounter++;
			}
			songcounter++;
		}
	}
	cprintf(1, 2, "done");
		
	REG_IME = 1; // enable interrupts
	
	while (1);
}
