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

# default theme is 'basic'
ifndef THEME
	THEME = basic
endif

GNUDEBUG = -g -ggdb

PLATFORM= arm-thumb-elf-
CC		= ${PLATFORM}gcc
CXX		= ${PLATFORM}g++
AS		= ${PLATFORM}gcc -c $(CFLAGS)
LD		= ${PLATFORM}g++ 
OBJCOPY 	= ${PLATFORM}objcopy
EMULATOR	= `which VisualBoyAdvance` --auto-frameskip 

CRT			= crt0.o
LDSCRIPT		= lnkscript

LFLAGS = -mthumb -nostartfiles -static -Wl,-T,$(LDSCRIPT) $(CFLAGS) $(LIBS)

SRCS	= $(shell ls *.c)
CXXSRCS = $(shell ls *.cc)
ASM	= $(shell ls *.s)
OBJS	= $(SRCS:.c=.o) $(CXXSRCS:.cc=.o) $(ASM:.s=.o) 
INCLUDE	= -I$(shell pwd)

# what to build if we're building the whole program
ifeq ($(TARGET),looper)
	GAMEOBJECTS = Keys.o Page.o First.o Loop.o Widget.o Label.o EditBox.o SelectBox.o NumberBox.o GlobalData.o Sample.text.iwram.o ClarkMix.text.iwram.o charset.o samples/samples.o splash.o screens.o
endif

ifeq ($(TARGET),audiotest)
	GAMEOBJECTS = samples/samples.o
endif

ifeq ($(TARGET),audiobuffertest)
	GAMEOBJECTS = samples/samples.o
endif

ifeq ($(TARGET),sampletest)
	GAMEOBJECTS = samples/samples.o Sample.o
endif

ifeq ($(TARGET),audiolayertest)
	GAMEOBJECTS = samples/samples.o Sample.o AudioLayer.o
endif

ifeq ($(TARGET),clarkinoutest)
	GAMEOBJECTS = samples/samples.o Sample.o ClarkMix.o Keys.o
endif

ifdef RELEASE
	#release build
	#CFLAGS 	= -O2 -Wall -ffreestanding -fomit-frame-pointer -funroll-loops -finline-functions -mcpu=arm7tdmi $(INCLUDE) -DNO_DPRINT -DINTERWORK ${GNUDEBUG} -mthumb-interwork -msoft-float -mlong-calls -DFIXED_USE_BIOS_DIVIDE ${MMFLAGS}
	CXXFLAGS = -fno-exceptions 
	CFLAGS = -mthumb-interwork -O3 -mlong-calls -Wall $(INCLUDE) -DNO_DPRINT -msoft-float
else
	# debug build
	#CFLAGS 	= -Wall -O2 -ffreestanding -fomit-frame-pointer -funroll-loops -finline-functions -mcpu=arm7tdmi  $(INCLUDE) -DUSE_VGBA -mthumb-interwork -msoft-float -mlong-calls -DUSE_VBA -DINTERWORK -DFIXED_USE_BIOS_DIVIDE ${GNUDEBUG} ${MMFLAGS}
	CXXFLAGS = 
	CFLAGS = -mthumb-interwork -O3 -mlong-calls -Wall $(INCLUDE) -DUSE_VBA -DINTERWORK -msoft-float 
endif

ARCHIVE=$(shell echo `basename ${PWD}``date '+%Y%m%d'`)
DOC= doc/

export HPATH = ./
export LANGUAGE=en

%.text.iwram.o : %.text.iwram.c
	$(CC) $(CFLAGS) -marm -c $< -o $@

%.o : %.c
	$(CC) $(CFLAGS) -mthumb -c $< -o $@

%.o : %.s
	$(AS) $< -o $@

%.text.iwram.o : %.text.iwram.cc
	$(CXX) $(CFLAGS) $(CXXFLAGS) -marm -c $< -o $@
	
%.o : %.cc
	$(CXX) $(CFLAGS) $(CXXFLAGS) -mthumb -c $< -o $@

all: $(TARGET).gba

$(TARGET).gba: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).gba

$(TARGET).elf: $(RESOURCES) $(TARGET).cc $(TARGET).o $(CRT) $(GAMEOBJECTS) $(LDSCRIPT) Makefile
	$(LD) $(LFLAGS) crt0.o $(TARGET).o $(GAMEOBJECTS) -o $(TARGET).elf $(LIBS)

# audio stuff
samples/samples.o: samples/samples.hh samples/samples.cc

samples/samples.hh samples/samples.cc: samples/Makefile
	make -C samples/

# screens data
splash.hh splash.cc: screens/$(THEME)/*.png
	./createscreendata.py -b`cat screens/$(THEME)/transparent` screens/$(THEME)/splash.png

screens.hh screens.cc: screens/$(THEME)/*.png
	./createscreendata.py -b`cat screens/$(THEME)/transparent` -oscreens -nscreens/$(THEME)/font.png -nscreens/$(THEME)/altfont.png screens/$(THEME)/loop.png screens/$(THEME)/first.png

# clean
clean:
	rm -f *.elf *.gba *~ *.log *.bak *.o
	rm -f `find -name \*.o -print -or -name \*~ -print`  
	rm -f .depend .hdepend .sdepend
	rm screens.* splash.*
	make clean -C samples/

run: $(TARGET).gba
	@$(EMULATOR) $(TARGET).gba

# build dependencies
#dep: depend
#.hdepend: depend
#.sdepend: depend
#depend: $(MKDEP)
#	@echo Build sources\(c,c++\) dependencies...
#	-$(CC) -MM *.cc >.sdepend

###
#-include .depend
#-include .sdepend
#-include .hdepend
