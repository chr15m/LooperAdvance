// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

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
