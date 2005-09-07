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
	
	# create the header bits
	htext.append("// This file is produced automatically by " + sys.argv[0])
	htext.append("")
	htext.append("#define NUMSAMPLES " + str(numsamples));
	htext.append('#include "typedefs.h"')
	htext.append('#include "SampleData.hh"')
	htext.append("")
	
	ctext.append("// This file is produced automatically by " + sys.argv[0])
	ctext.append("")
	ctext.append('#include"samples.hh"')
	ctext.append("")
	
	# for every sample file
	for filename in sys.argv[1:]:
		print "processing " + filename
		
		shortname = filename[:-4]
		
		# try and open the file
		binfile = 0
		try:
			binfile = file(filename, "rb")
		except:
			print "Couldn't open " + filename
		
		# do we have a real file
		if (binfile):	
			filesize = 0
			
			byte = 1
			adata = []
			
			# for every byte in the file
			while(byte):
				byte = binfile.read(1)
				if (byte):
					# add this byte to our data array
					adata.append(ord(byte))
					filesize = filesize + 1
			binfile.close()
			
			# create entries in the .cc and .hh data file for this particular sample
			htext.append('extern const s8 ' + shortname + "[" + str(filesize) + "];")
			ctext.append('const s8 ' + shortname + "[" + str(filesize) + "] = {")
			
			# now loop through all of our data writing it to the .cc file
			datachunks = []
			for seek in range(len(adata)):
				# datachunks.append("0x%02x" % adata[seek])
				datachunks.append("0x%02x" % adata[seek])
				# put a carriage return to make the file neater
				if ((seek + 1) % 32 == 0):
					ctext.append(string.join(datachunks, ", ") + ", ")
					datachunks = []
			
			ctext.append("};\n")
		
		samples.append([shortname, filesize])

	# write an entry in the samples data table for every sample we've added
	ctext.append("const SampleData samples[NUMSAMPLES] = {")
	for bits in samples:
		ctext.append("{(void *)" + bits[0] + ", " + str(bits[1]) + ', "' + bits[0] + '"},')
	ctext.append("};")
	
	htext.append("extern const SampleData samples[NUMSAMPLES];")

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
