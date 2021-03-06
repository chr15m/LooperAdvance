/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: Page.cc,v 1.4 2004/04/08 06:09:42 chrism Exp $

******************************************************/

// Represents a single loop-page

#include "Page.h"

Page::Page()
{
	right = NULL;
	left = NULL;
	first = NULL;
	last = NULL;
}

Page::~Page()
{
	debug("Destroying Page()");
}
