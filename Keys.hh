// Description: This class handles key presses

#include "looper.hh"

#ifndef _KEYS_HH_
#define _KEYS_HH_

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
	inline void Keys::Jiffie()
	{
		u16 i=NUM_KEY_STATES;
		
		while(i)
		{
			i--;
			
			oldstate[i] = currentstate[i];
	
			if (!(REG_JP & keymap[i]))
				currentstate[i] = pressed;
			else
				currentstate[i] = released;
		}
	}
	
	//! test if an individual key has changed state
	inline KeyState Keys::TestKey(u16 key)
	{
		if (currentstate[key] == oldstate[key])
			return unchanged;
		else
			return currentstate[key];
	}
	
	//! test if a key is currently down
	inline bool Keys::IsPressed(u16 key)
	{
		return !(REG_JP & keymap[key]);
	}	
};

#endif //_KEYS_HH_
