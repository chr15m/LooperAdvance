/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

// Represents a single loop-page

#include "Page.hh"

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
