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
#include "looper.hh"

#ifndef _PAGE_HH_
#define _PAGE_HH_

#include "Keys.hh"
#include "Widgets.hh"

class Page
{
typedef struct structWidgetList
{
	Widget *widget;
	structWidgetList *next;
};
	
private:
protected:
	Widget *selected;
	Keys *keys;

public:
	// these are the links to the next page if they hit right and left on the shoulder buttons
	Page *right;
	Page *left;
	structWidgetList *first;
	structWidgetList *last;
	
	Page();
	virtual ~Page();
	virtual void DoSwap()=0;
	virtual void DoProcess()=0;
	
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
		structWidgetList *traverse = first;
		Page *newPage = NULL;
	
		newPage = PageSwap();
		
		if (!newPage)
			newPage = this;
		else
			newPage->DoSwap();
		
		// go through our widgets drawing them
		while (traverse)
		{
			traverse->widget->Draw();
			traverse = traverse->next;
		}
		
		return newPage;
	}

	inline void Process()
	{
		DoProcess();
		// tell our next page to do it's processing
		if (right)
		{
			right->Process();
		}
	}
	
	inline void Draw()
	{
		selected = selected->Process();
	}
	
	//! add a widget to our list
	inline void AddWidget(Widget *incoming)
	{
		// if we don't have a first yet
		if (!first)
		{
			// create a starting last and first
			last = new structWidgetList;
			first = last;
		}
		else
		{
			// use the existing last one
			last->next = new structWidgetList;
			last = last->next;
		}
		last->widget = incoming;
		last->next = NULL;
	}
};

#endif
