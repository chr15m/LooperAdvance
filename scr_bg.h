/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: scr_bg.hh,v 1.2 2004/04/08 06:09:43 chrism Exp $

******************************************************/

#include <gba.h>

#define BG_TILESIZE 5984
#define BG_PALSIZE 24

extern const u16 bg_tiles[BG_TILESIZE];
extern const u16 bg_palette[BG_PALSIZE];
extern const u16 bg_map[640];
