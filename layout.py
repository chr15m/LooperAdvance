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

#layout = "abcdefghijklmnopqrstuvwxyz0123456789,./<>?;':\"[]\{}|!@#$%^&*()_+-="
layout = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ   1234567890!@#$%^&*()-=_+[]{}\|;':\",.<>/?`~"

print str(len(layout)) + " letters"
letters = []

for i in xrange(127):
	letters.append(0xFF)
	
for i in xrange(len(layout)):
	letters[ord(layout[i])] = i
	letters[ord(layout[i].upper())] = i
	#print str(i) + " = " + str(ord(layout[i]))
	
for i in letters:
	print str(i) + ",",
