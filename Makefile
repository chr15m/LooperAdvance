#!/usr/bin/make -f
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

ifndef TARGET
	TARGET = looper
endif

SRCS	= $(shell ls *.c)
CXXSRCS = $(shell ls *.cc)
ASM	= $(shell ls *.s)
OBJS	= $(SRCS:.c=.o) $(CXXSRCS:.cc=.o) $(ASM:.s=.o) 
GAMEOBJECTS = Keys.o Page.o First.o Loop.o Widget.o Label.o EditBox.o SelectBox.o NumberBox.o GlobalData.o instruments.o samples.o krawall.lib charset.o samplenames.o
AUDIOOBJECTS = modules.h samples.h instruments.h samples.s instruments.s
SCREENOBJECTS = scr_bg.o scr_krawall_splash.o scr_splash.o

ifdef RELEASE
	#release build
	CFLAGS 	= -O -Wall -ffreestanding -fomit-frame-pointer -funroll-loops -mcpu=arm7tdmi  $(INCLUDE) -DRAN_SEED=322187 -D_FIXED_NO_FLOATINGPOINT_ -D_VECTOR_NO_FLOATINGPOINT_ -DNO_DPRINT -DNO_COLOUR_PROFILING -DINTERWORK ${GNUDEBUG} -mthumb-interwork -msoft-float -mno-long-calls -DFIXED_USE_BIOS_DIVIDE ${MMFLAGS}
	CXXFLAGS = -fno-rtti -fno-exceptions
else
	# debug build
	CFLAGS 	= -Wall -O -ffreestanding -fomit-frame-pointer -funroll-loops -mcpu=arm7tdmi  $(INCLUDE) -DRAN_SEED=322187 -DUSE_VGBA -mthumb-interwork -msoft-float -mno-long-calls -DUSE_VBA -DINTERWORK -DFIXED_USE_BIOS_DIVIDE ${GNUDEBUG} ${BMDEBUG} ${TMDEBUG} ${SEDEBUG} ${WORLDDEBUG} ${OBJECTDEBUG} ${MMFLAGS}
	CXXFLAGS = -fno-rtti
endif

ARCHIVE=$(shell echo `basename ${PWD}``date '+%Y%m%d'`)
DOC= doc/

export HPATH = ./
export LANGUAGE=en

include Makefile.inc

%.text.iwram.o : %.text.iwram.c
	$(CC) $(CFLAGS) -marm -c $< -o $@

%.o : %.c samplenames.hh
	$(CC) $(CFLAGS) -mthumb -c $< -o $@

%.o : %.s samplenames.hh
	$(AS) $< -o $@

%.text.iwram.o : %.text.iwram.cc
	$(CXX) $(CFLAGS) $(CXXFLAGS) -marm -c $< -o $@
	
%.o : %.cc samplenames.hh
	$(CXX) $(CFLAGS) $(CXXFLAGS) -mthumb -c $< -o $@

all: $(TARGET).gba

# main game stuff

$(TARGET).gba: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).gba

$(TARGET).elf: $(RESOURCES) $(TARGET).cc $(TARGET).o $(CRT) $(GAMEOBJECTS) $(SCREENOBJECTS) $(LDSCRIPT) Makefile
	$(LD) $(LFLAGS) crt0.o $(TARGET).o $(GAMEOBJECTS) $(SCREENOBJECTS) -o $(TARGET).elf $(LIBS)

# links to lib
krawall.lib: ../krawall/gcclib/krawall-32k-60-medium-sf.lib
	ln -s ../krawall/gcclib/krawall-32k-60-medium-sf.lib krawall.lib

samples.xm: ../samples.xm
	ln -s ../samples.xm
	
# audio stuff

$(AUDIOOBJECTS): samples.xm
	../krawall/converter/converter.linux samples.xm

samplenames.cc samplenames.hh: samples.h
	./samplenames.py
	
clean:
	rm -f *.elf *.gba *~ *.log *.bak *.o
	rm -f `find -name \*.o -print -or -name \*~ -print`  
	rm -f .depend .hdepend .sdepend
	# audio stuff:
	rm -f $(AUDIOOBJECTS)
	rm -r samplenames.hh samplenames.cc
	# @touch `find`

#build dependencies
dep: depend
.hdepend: depend
.sdepend: depend
depend: $(MKDEP)
	@echo Build sources\(c,c++\) dependencies...
	-$(CC) -MM *.cc >.sdepend

run: $(TARGET).gba
	@$(EMULATOR) $(TARGET).gba

###
-include .depend
-include .sdepend
-include .hdepend
