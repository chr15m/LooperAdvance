#/*****************************************************
#
#	looper advance
#	(c) chris mccormick, 2004
#	
#	licensed under the terms of the GPL
#	see the file gpl.txt for details
#	
#	chris@mccormick.cx
#	http://looper.mccormick.cx/
#	
#	$Id$
#
#******************************************************/

GNUDEBUG = -g -ggdb

ifdef DEBUGLOOPS
	DBLOOPS=-DDEBUGLOOPS
endif

ifdef DEBUG
	# debug build
	CFLAGS 	= -Wall -O -ffreestanding -fomit-frame-pointer -funroll-loops -mcpu=arm7tdmi  $(INCLUDE) -DRAN_SEED=322187 -DUSE_VGBA -mthumb-interwork -msoft-float -mno-long-calls -DUSE_VBA -DINTERWORK -DFIXED_USE_BIOS_DIVIDE ${GNUDEBUG} ${DBLOOPS}
	CXXFLAGS = -fno-rtti
else
	#release build
	CFLAGS 	= -O -Wall -ffreestanding -fomit-frame-pointer -funroll-loops -mcpu=arm7tdmi  $(INCLUDE) -DRAN_SEED=322187 -D_FIXED_NO_FLOATINGPOINT_ -D_VECTOR_NO_FLOATINGPOINT_ -DNO_DPRINT -DNO_COLOUR_PROFILING -DINTERWORK ${GNUDEBUG} -mthumb-interwork -msoft-float -mno-long-calls -DFIXED_USE_BIOS_DIVIDE
	CXXFLAGS = -fno-rtti -fno-exceptions
endif

PLATFORM= arm-thumb-elf-
CC		= ${PLATFORM}gcc
CXX		= ${PLATFORM}g++
#AS		= ${PLATFORM}as -mcpu=arm7tdmi -mthumb-interwork -mfpu=softfpa 
AS		= ${PLATFORM}gcc -c $(CFLAGS)
LD		= ${PLATFORM}g++ 
OBJCOPY 	= ${PLATFORM}objcopy
EMULATOR	= `which VisualBoyAdvance` --auto-frameskip 

CRT			= crt0.o
LDSCRIPT		= lnkscript

LFLAGS = -mthumb -nostartfiles -static -Wl,-T,$(LDSCRIPT) $(CFLAGS) $(LIBS)
