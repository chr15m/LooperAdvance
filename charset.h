/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: charset.hh,v 1.2 2004/04/08 06:09:42 chrism Exp $

******************************************************/

#include <gba.h>

namespace font
{
	extern const u8 charlayout[95];
	extern const u8 charoffset[127];
	extern const u16 tiles[3232];
	extern const u16 tiles_color[3232];
}
