// This file is part of the looping program for GBA which Chrism&Fenris use in their live show

// Represents a single loop-page
#include "looper.h"
#include "Keys.h"

#ifndef _PAGE_HH_
#define _PAGE_HH_

class Page
{
private:
	Keys *keys;

public:
	// these are the links to the next page if they hit right and left on the shoulder buttons
	Page *right;
	Page *left;

	Page();
	virtual void Draw()=0;
	virtual void Process()=0;

	//! this sets what managers to use
	inline void UseKeys(Keys *inkeys)
	{
		keys = inkeys;
	}
	
	//! this checks to see if they're trying to swap pages
	inline Page *PageSwap()
	{
		if (keys->TestKey(keyL) == pressed)
			if (left)
				return left;
			
		if (keys->TestKey(keyR) == pressed)
			if (right)
				return right;
		
		return NULL;
	}
	
	//! this does a bunch of default stuff on the page
	inline Page *Cycle()
	{
		Page *newPage=NULL;
	
		newPage = PageSwap();
		
		if (!newPage)
			newPage = this;
//		else
//			dprintf("Page: %ld\n", (u32)newPage);
		
		return newPage;
	}
};

#endif
