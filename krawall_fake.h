// backside 180 heelflip fakie krawall

#ifndef __FAKIE_KRAWALL__
#define __FAKIE_KRAWALL__

#define kramGetFreq(...) 0
#define kramGetPan(...) 0
#define kramGetPos(...) 0
#define kramHandleValid(...) 0
#define kramPlay(...) 0
#define kramPlayExt(...) 0
#define kramStop(...) 0
#define kramSetFreq(...) 0
#define kramSetPan(...) 0
#define kramSetPos(...) 0
#define kramSetSFXVol(...) 0
#define kramSetVol(...) 0

/*
unsigned int kramGetFreq (chandle c)
int kramGetPan (chandle c)
unsigned int kramGetPos (chandle c)
int kramHandleValid (chandle c)
chandle kramPlay (const Sample  s, int sfx, chandle c)
chandle kramPlayExt (const Sample  s, int sfx, chandle c, unsigned int freq, unsigned int vol, int pan)
int kramSetFreq (chandle c, unsigned int freq)
int kramSetPan (chandle c, int pan)
int kramSetPos (chandle c, unsigned int pos)
void kramSetSFXVol (unsigned int vol)   (128 = loud)
int kramSetVol (chandle c, unsigned int vol)
*/

#endif
