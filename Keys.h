/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id: Keys.hh,v 1.3 2004/04/08 06:09:42 chrism Exp $

******************************************************/

// Description: This class handles key presses

#include "looper.h"

#ifndef _KEYS_HH_
#define _KEYS_HH_

#define KEY_BUTTONS (KEY_A|KEY_B|KEY_SELECT|KEY_START|KEY_R|KEY_L)
#define KEY_PADDLE  (KEY_RIGHT|KEY_LEFT|KEY_UP|KEY_DOWN)
#define KEY_ANY     (KEY_PADDLE | KEY_BUTTONS)

enum KeyState { unchanged, pressed, released };
enum KeyNames { keyA, keyB, keySelect, keyStart, keyRight, keyLeft, keyUp, keyDown, keyR, keyL};

#define NUM_KEY_STATES 13

class Keys
{
private:
	u16 keymap[NUM_KEY_STATES];
	KeyState currentstate[NUM_KEY_STATES];
	KeyState oldstate[NUM_KEY_STATES];
	
public:
	Keys();

	//! detect any changes to key states etc
	inline void Jiffie()
	{
		u16 i=NUM_KEY_STATES;
		
		while(i)
		{
			i--;
			
			oldstate[i] = currentstate[i];
	
			if (!(REG_KEYINPUT & keymap[i]))
				currentstate[i] = pressed;
			else
				currentstate[i] = released;
		}
	}
	
	//! test if an individual key has changed state
	inline KeyState TestKey(u16 key)
	{
		if (currentstate[key] == oldstate[key])
			return unchanged;
		else
			return currentstate[key];
	}
	
	//! test if a key is currently down
	inline bool IsPressed(u16 key)
	{
		return !(REG_KEYINPUT & keymap[key]);
	}	
};

#endif //_KEYS_HH_
