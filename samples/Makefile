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

WAVS	= $(shell ls *.wav)
RAWS	= $(WAVS:.wav=.raw)
	
%.raw : %.wav
	@echo converting $<
	@sox $< -r 22050 -c 1 -s -b $@

samples.cc samples.hh : $(RAWS) buildsamplebank.py
	@./buildsamplebank.py $(RAWS)

clean:
	rm -rf *.raw
	rm -rf samples.cc samples.hh samples.o

