#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned long vu32;

typedef const unsigned char const_u8;
typedef const unsigned short const_u16;
typedef const unsigned long const_u32;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;

typedef unsigned char byte;
typedef unsigned short hword;
typedef unsigned long word;
typedef void (*fp)(void);   //this is the definition you will find in gba.h.  It is just declaring fp to represent a pointer to a function

#endif
