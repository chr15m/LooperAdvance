/*****************************\
* 	screenmode.h
*	Original by dovoto
*	Modified by staringmonkey
*	Last modified on 8/8/01
\*****************************/

#ifndef SCREENMODE_H
#define SCREENMODE_H

///////////////////Enumerations////////////////////
#define MODE_0 0x0
#define MODE_1 0x1
#define MODE_2 0x2
#define MODE_3 0x3
#define MODE_4 0x4
#define MODE_5 0x5

// these are for compatibility becuase we used them by accident
#define SCREENMODE0    0x0
#define SCREENMODE1    0x1
#define SCREENMODE2    0x2
#define SCREENMODE3    0x3
#define SCREENMODE4    0x4
#define SCREENMODE5    0x5

#define BACKBUFFER 0x10
#define H_BLANK_OAM 0x20 

// compatibility
#define HBLANKOAM      0x20                 //Update OAM during HBlank?

#define OBJ_MAP_2D 0x0
#define OBJ_MAP_1D 0x40
#define FORCE_BLANK 0x80

// compatibility
#define OBJMAP2D       0x0                  //2D object (sprite) mapping
#define OBJMAP1D       0x40                 //1D object(sprite) mapping
#define FORCEBLANK     0x80                 //Force a blank

#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200 
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800
#define OBJ_ENABLE 0x1000 

// compatibility
#define BG0ENABLE      0x100                //Enable background 0
#define BG1ENABLE      0x200                //Enable background 1
#define BG2ENABLE      0x400                //Enable background 2
#define BG3ENABLE      0x800                //Enable background 3
#define OBJENABLE      0x1000               //Enable sprites

#define WIN1_ENABLE 0x2000 
#define WIN2_ENABLE 0x4000
#define WINOBJ_ENABLE 0x8000

// compatibility
#define WIN1ENABLE     0x2000               //Enable window 1
#define WIN2ENABLE     0x4000               //Enable window 2
#define WINOBJENABLE   0x8000               //Enable object window

//////////////////////Macros/////////////////////
#define SetMode(mode) REG_DISPCNT = (mode) 


// memory locations
#define BG0	0x7000
#define BG1	0x7400
#define BG2	0x7800
#define BG3	0x7C00

#define TEXT_BG	BG1

#define FONT_TILES_BASE 0x3000
#endif
