#!/usr/bin/env python

import fileinput
import re

codestring = ""
numsamples = 0
ctext = ""
htext = ""

for line in fileinput.input("samples.h"):
	for matches in re.findall('\#define SAMPLE_(.*?)\ (\d+)', line):
		print "adding " + matches[0] + " at position " + str(numsamples)
		codestring += '"' + matches[0] + '",' + "\n"
		numsamples += 1
ctext += "#include \"samplenames.h\"\n\n"		
ctext += "const char *samplenames[NUMSAMPLES] = \n"
ctext += "{\n"
ctext += codestring + "};\n"

htext += "#define NUMSAMPLES " + str(numsamples) + "\n"
htext += "extern const char *samplenames[NUMSAMPLES];\n"

cfile=open("samplenames.cpp", "w")
hfile=open("samplenames.h", "w")

# load in and write a header for each file
cfile.write(ctext)
hfile.write(htext)

# close filehandles
cfile.close()
hfile.close()
