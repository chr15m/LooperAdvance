/*****************************************************

	looper advance
	(c) chris mccormick, 2004
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx
	http://looper.mccormick.cx/
	
	$Id$

******************************************************/

#ifndef GBA_DPRINT_H_
#define GBA_DPRINT_H_

#define DEBUG_CHAR_BUFFER_SIZE	512

/* NOTES:
 *
 * The mappy version works in VBA v0.9 (SDL) too,
 * the differences: mappy version smashes three registers,
 *                  rather than 1
 *             but: produces no error messages for "unhandled swi"
 *                  in non-comforming emulators.
 *                  (like the vba macro does for vba, non-SDL)
 */

#if defined(NO_DPRINT)
# define dprint(s)
#endif

#ifdef USE_MAPPY

#define dprint(s) ({      \
  const char *sz = s;     \
  __asm__ __volatile__ (  \
    "mov r2, %0;"         \
    "ldr r0, =0xc0ded00d;"\
    "mov r1, #0;"         \
    "and r0, r0, r0;"     \
  : /* No output */       \
  : "r" (sz)              \
  : "r0", "r1", "r2");    \
})


#elif defined(USE_VBA)

#if defined(INTERWORK) || defined(THUMB)
# define SWILINE "swi 0xff;"
#elif defined(ARM)
# define SWILINE "swi 0x00ff0000;"
#else
# error CPU must be defined as arm, thumb, or interwork
#endif

#define dprint(s) ({  \
 const char *str = s; \
 __asm__ __volatile__(\
	"mov r0, %0;"  \
   SWILINE        \
   : /*no ouput*/ \
   : "r" (str)    \
   : "r0");       \
})

#define dprintf(format, args...)  ({	\
	char buffer[DEBUG_CHAR_BUFFER_SIZE];					\
	snprintf( buffer, DEBUG_CHAR_BUFFER_SIZE, format , ## args); \
	dprint(buffer); \
})


#else

#define dprintf(format, args...)

#endif

#ifdef USE_VBA

#define dprint(s) ({  \
 const char *str = s; \
 __asm__ __volatile__(\
	"mov r0, %0;"  \
   SWILINE        \
   : /*no ouput*/ \
   : "r" (str)    \
   : "r0");       \
})

#define dprintf(format, args...)  ({	\
	char buffer[DEBUG_CHAR_BUFFER_SIZE];					\
	snprintf( buffer, DEBUG_CHAR_BUFFER_SIZE, format , ## args); \
	dprint(buffer); \
})

#endif

#endif
