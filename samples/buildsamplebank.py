#!/usr/bin/env python
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

# This program reads in 8bit unsigned raw audio data and writes out a .cc and .hh file containing that data

import sys, string

# check they're using correctly
if (len(sys.argv) > 1):
	# how many samples are we dealing with?
	numsamples = len(sys.argv[1:])
	
	# these variables hold the text to go into our .cc and .hh files
	ctext = []
	htext = []
	
	# actual hexdecimal values of our binary data
	adata = []
	
	# the names of all our samples
	samples = []
	
	# for every sample file
	for filename in sys.argv[1:]:
		print "processing " + filename
		
		shortname = filename[:-4]
		samples.append(shortname)
		
		# try and open the file
		binfile = 0
		try:
			binfile = file(filename, "rb")
		except:
			print "Couldn't open " + filename
		
		htext.append("// This file is produced automatically by " + sys.argv[0])
		htext.append("")
		htext.append("#define NUMSAMPLES " + str(numsamples));
		htext.append('#include "typedefs.h"')
		htext.append("")
		
		ctext.append("// This file is produced automatically by " + sys.argv[0])
		ctext.append("")
		ctext.append('#include"samples.hh"')
		ctext.append("")

		if (binfile):	
			filesize = 0
			
			byte = 1
			adata = []
			
			# for every byte in the file
			while(byte):
				byte = binfile.read(1)
				if (byte):
					# add this byte to our data array
					adata.append(str(hex(ord(byte))))
					filesize = filesize + 1
			binfile.close()
			
			htext.append('extern const u8 ' + shortname + "[" + str(filesize) + "];")
			ctext.append('const u8 ' + shortname + "[" + str(filesize) + "] = {")
			
			for seek in range(len(adata)):
				ctext.append(adata[seek] + ", ")
				if ((seek + 1) % 8 == 0):
					ctext.append("")
			
			ctext.append("};\n")
	
	ctext.append("const u8* samples[NUMSAMPLES] = {" + string.join(samples, ", ") + "};")
	ctext.append("const char* samplenames[NUMSAMPLES] = {" + string.join(map(lambda x: '"' + x + '"', samples), ", ") + "};")
	
	htext.append("extern const u8 *samples[NUMSAMPLES];")
	htext.append("extern const char *samplenames[NUMSAMPLES];")

	# open our c and h files for writing
	cfile=open("samples.cc", "w")
	hfile=open("samples.hh", "w")
	
	# load in and write a header for each file
	cfile.write(string.join(ctext, "\n") + "\n")
	hfile.write(string.join(htext, "\n") + "\n")
	
	# close filehandles
	cfile.close()
	hfile.close()
else:
	print sys.argv[0] + " file1.raw [file2.raw file3.raw ...]"
