
// Description: This class handles keys

#include "Keys.hh"

Keys::Keys()
{
	u16 i=0;

	// set up and initialize key handler
	keymap[0] = KEY_A;
	keymap[1] = KEY_B;
	keymap[2] = KEY_SELECT;
	keymap[3] = KEY_START;
	keymap[4] = KEY_RIGHT;
	keymap[5] = KEY_LEFT;
	keymap[6] = KEY_UP;
	keymap[7] = KEY_DOWN;
	keymap[8] = KEY_R;
	keymap[9] = KEY_L;
	keymap[10] = KEY_BUTTONS;
	keymap[11] = KEY_PADDLE;
	keymap[12] = KEY_ANY;
			
	for (i=0;i<13;i++)
	{
		currentstate[i] = released;
		oldstate[i] = released;
	}
}
