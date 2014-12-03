/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: helpers.h,v 1.6 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include <gba.h>
#include <stdio.h>
#include <cstdio>

#ifndef _HELPERS_H_
#define _HELPERS_H_

#define VideoBuffer 		((vu16*)0x6000000)

#define BIT00 1
#define BIT01 2
#define BIT02 4
#define BIT03 8
#define BIT04 16
#define BIT05 32
#define BIT06 64
#define BIT07 128
#define BIT08 256
#define BIT09 512
#define BIT10 1024
#define BIT11 2048
#define BIT12 4096
#define BIT13 8192
#define BIT14 16384
#define BIT15 32768

#define REG_WSCNT      *(volatile u16*)0x4000204

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
	VideoBuffer[0x7C00 + x + y * 32 + i] = font::charoffset[buffer[i]];\
	i++;\
	}\
})

#define hprintf(x, y, format, args...)  ({    \
	u16 i=0;\
        char buffer[CHAR_BUFFER_SIZE];                                    \
        snprintf( buffer, CHAR_BUFFER_SIZE, format , ## args); \
        while(buffer[i] != '\0')\
	{\
	VideoBuffer[0x7C00 + x + y * 32 + i] = font::charoffset[buffer[i]] + 101;\
	i++;\
	}\
})

inline u16 RGB(u16 r, u16 g, u16 b)
{
	return ((r)+((g)<<5)+((b)<<10)); //you can have 0-31 levels of each red,green,blue component.
}       

inline void SetBG(u16 r, u16 g, u16 b)
{
	(*(u16*)(BG_PALETTE + 6)=(RGB(r,g,b)));
}

inline void BlankScreen()
{
	u32 zero=52;
	DMA3COPY((void*)&zero, (u16*)&VideoBuffer[0x7C00], 0x400 | DMA16 | DMA_IMMEDIATE | DMA_SRC_FIXED | DMA_DST_INC);
}

#endif //_HELPERS_H_
