#!/usr/bin/make -f

TARGET = looper.gba
ELF    = looper.elf

SRCS	= $(shell ls *.c)
CXXSRCS = $(shell ls *.cc)
ASM	= $(shell ls *.s)
OBJS	= $(SRCS:.c=.o) $(CXXSRCS:.cc=.o) $(ASM:.s=.o) 
GAMEOBJECTS = crt0.o main.o Keys.o Page.o First.o Loop.o Widget.o SelectBox.o NumberBox.o instruments.o samples.o krawall.lib charset.o songdata.o samplenames.o

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

# default stuff

%.text.iwram.o : %.text.iwram.c
	$(CC) $(CFLAGS) -marm -c $< -o $@

%.o : %.c samples.h 
	$(CC) $(CFLAGS) -mthumb -c $< -o $@

%.o : %.s samples.h
	$(AS) $< -o $@

%.text.iwram.o : %.text.iwram.cc
	$(CXX) $(CFLAGS) $(CXXFLAGS) -marm -c $< -o $@
	
%.o : %.cc samples.h
	$(CXX) $(CFLAGS) $(CXXFLAGS) -mthumb -c $< -o $@

all: $(TARGET)

# main game stuff

$(TARGET): $(ELF)
	$(OBJCOPY) -O binary $(ELF) $(TARGET)

$(ELF): $(RESOURCES) main.cc $(GAMEOBJECTS) $(CRT) $(LDSCRIPT) Makefile
	$(LD) $(LFLAGS) $(GAMEOBJECTS) -o $(ELF) $(LIBS)

# audio stuff

samplenames.cc modules.h samples.h instruments.h samples.s instruments.s: samples.xm
	../krawall/converter/converter.linux samples.xm
	./samplenames.py

samples.xm: ../samples.xm
	ln -s ../samples.xm

clean:
	rm -f *.elf *.gba *~ *.log *.bak *.o
	rm -f `find -name \*.o -print -or -name \*~ -print`  
	rm -f .depend .hdepend .sdepend
	@touch `find`

#build dependencies
dep: depend
.hdepend: depend
.sdepend: depend
depend: $(MKDEP)
	@echo Build sources\(c,c++\) dependencies...
	-$(CC) -MM *.cc >.sdepend

run: $(TARGET)
	@$(EMULATOR) $(TARGET)

###
-include .depend
-include .sdepend
-include .hdepend
