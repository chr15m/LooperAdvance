/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#include "gba.h"
#include <stdio.h>

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "screens.hh"
#include "screenmode.h"

#define CHAR_BUFFER_SIZE 255

#define debug(format, args...)	\
({	\
	dprintf("[%s - %s()] Line %d: ", __FILE__, __FUNCTION__, __LINE__);	\
	dprintf(format, ## args);	\
	dprintf("\n");	\
})

#define debugloop(format, args...) ;

#ifdef DBLOOPS
#define debugloop(format, args...)	\
({	\
	dprintf("[%s - %s()] Line %d: ", __FILE__, __FUNCTION__, __LINE__);	\
	dprintf(format, ## args);	\
	dprintf("\n");	\
})
#endif

#define cprintf(x, y, format, args...)  ({    \
	u16 i=0;\
        char buffer[CHAR_BUFFER_SIZE];                                    \
        snprintf( buffer, CHAR_BUFFER_SIZE, format , ## args); \
        while(buffer[i] != '\0')\
	{\
	VideoBuffer[TEXT_BG + x + y * 32 + i] = font::charoffset[buffer[i]] + (FONT_TILES_BASE>>5);\
	i++;\
	}\
})

#define hprintf(x, y, format, args...)  ({    \
	u16 i=0;\
        char buffer[CHAR_BUFFER_SIZE];                                    \
        snprintf( buffer, CHAR_BUFFER_SIZE, format , ## args); \
        while(buffer[i] != '\0')\
	{\
	VideoBuffer[TEXT_BG + x + y * 32 + i] = font::charoffset[buffer[i]] + 104 + (FONT_TILES_BASE>>5);\
	i++;\
	}\
})

#define hchar(x, y, char)  ({    \
	VideoBuffer[TEXT_BG + x + y * 32] = font::charoffset[char] + 104 + (FONT_TILES_BASE>>5);\
})


inline void DMACopy(void* a_Source, void* a_Destination, u32 a_WordCount, u32 a_Control)
{
	REG_DMA3SAD = (u32) a_Source;
	REG_DMA3DAD = (u32) a_Destination;
	REG_DM3CNT_L = a_WordCount;
	REG_DM3CNT_H = a_Control | DMA_ENABLE;
}

inline u16 RGB(u16 r, u16 g, u16 b)
{
	return ((r)+((g)<<5)+((b)<<10)); //you can have 0-31 levels of each red,green,blue component.
}       

inline void SetBG(u16 r, u16 g, u16 b)
{
		(*(u16*)(BGPaletteMem + 6)=(RGB(r,g,b)));
}

inline void BlankScreen(u16 screenbase=TEXT_BG)
{
	u32 zero = 0;
	
	DMACopy((void*)&zero, (u16*)&VideoBuffer[screenbase], 0x400, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_FIXED | DMA_DEST_INCREMENT);
}

inline void bzero(void *memstart, u32 size)
{
	u32 zero=0;
	DMACopy((void*)&zero, (void *)memstart, size, WORD_DMA | DMA_TIMING_IMMEDIATE | DMA_SOURCE_FIXED | DMA_DEST_INCREMENT);	
}
#endif //_HELPERS_H_
