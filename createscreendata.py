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

import os
import sys
import md5
import string
import pygame.image

# this class holds a simple structure for an 8x8 tile of palette references
class Tile:
	def __init__(self):
		# set up an initial tile of the correct size
		self.pixels = []
		for y in xrange(0,8):
			row = []
			for x in xrange(0,8):
				row.append(0)
			self.pixels.append(row)
		self.md5 = ""
	
	def SetPixel(self, x, y, value):
		self.pixels[y][x] = value
	
	def GetPixel(self, x, y):
		return self.pixels[y][x]
	
	# this creates an md5 sum of the current tile
	# this is so that later we can quickly determine whether we have a matching tile in our tileset
	def MakeMd5(self):
		self.md5 = md5.new(str(self.pixels)).digest()
	
	def GetMd5(self):
		return self.md5

# This class holds a simple structure for a map of tile references
class Map:
	def __init__(self, width, height):
		# set up the intial map of the correct size
		self.tileref = []
		self.width = width
		self.height = height
		
		for y in xrange(0, height):
			row = []
			for x in xrange(0, width):
				row.append(0)
			self.tileref.append(row)
	
	def SetTileRef(self, x, y, value):
		self.tileref[y][x] = value
	
	def GetTileRef(self, x, y):
		return self.tileref[y][x]
	
	def GetWidth(self):
		return self.width
	
	def GetHeight(self):
		return self.height
	
	def GetSize(self):
		return self.width * self.height
	
palette = []
bgcolor = "000000"
tilesets = {}
maps = {}
outputname = ""
alltiles = []

if (len(sys.argv) < 2):
	print "Usage: " + sys.argv[0] + " -oOutputname -bBackgroundcolour filename1 filename2 -nFilename3 ..."
	print
	print "\tRun this program for each set of images that should share a palette."
	print
	print "\t-nFilename"
	print "\t\tDon't compress matching tiles - useful for font sets where you might have blank letters."
	print
	print "\t-bBackgroundcolour"
	print "\t\tThis colour (in xxxxxx hex format) will be used for the transparency and background colour."
	print "\t\tDefaults to black 000000."
	print
	print "\t-oOutputname"
	print "\t\tThe name for the file and palettes that you want to output."
	print
else:
	# previous option
	prev = ""
	
	# list of files to be parsed
	files = []
	nocompress = {}
	
	# loop through the arguments to find files
	for arg in sys.argv[1:]:
		# is it a special option?
		if (arg[:2] == '-n'):
			filename = arg[2:]
			if os.access(filename, os.F_OK):
				files.append(filename)
				nocompress[filename] = 1
			else:
				print filename + " does not exist"
		elif (arg[:2] == '-b'):
			bgcolor = arg[2:]
		elif (arg[:2] == '-o'):
			outputname = arg[2:]
		else:
			# does this file exist?
			if os.access(arg, os.F_OK):
				files.append(arg)
			else:
				print arg + " does not exist"
	
	# figure out what our background colour is
	r, g, b = int(bgcolor[0:2], 16), int(bgcolor[2:4], 16), int(bgcolor[4:], 16)
	bgtupple = (b>>3) * 1024 + (g>>3) * 32 + (r>>3)
	
	for filename in files:
		print "Processing " + filename
		
		# get the tileset details
		(basename, extension) = os.path.splitext(os.path.basename(filename))
		
		# load file
		image = pygame.image.load(filename)
		
		# is this image size divisible by 8?
		width, height = image.get_width(), image.get_height()
		
		if (width % 8) or (height % 8):
			print filename + " size is not divisible by 8 (eg 240x160)"
		else:
			if (outputname == ""):
				outputname = basename
			
			# start the palette with our see-through color
			palette.append(bgtupple)
			
			if (not nocompress.has_key(filename)):
				map = Map(width/8, height/8)
			else:
				# make a new set of tiles
				tiles = []
			
			# loop through the tiles in our image
			for mapy in range(height/8):
				for mapx in range(width/8):
					# make a new tile
					newtile = Tile()
					# loop through the pixels in each tile
					for y in xrange(0,8):
						for x in xrange(0,8):
							# for every pixel we encounter, let's add it to our tile
							(r, g, b, a) = image.get_at((mapx * 8 + x, mapy * 8 + y))
							
							thispixel = 0
							palval = (b>>3) * 1024 + (g>>3) * 32 + (r>>3)
							
							try:
								thispixel = palette.index(palval)
							except:
								palette.append(palval)
								thispixel = len(palette) - 1
							
							newtile.SetPixel(x, y, thispixel)
					
					# make a map, and reduce tiles
					if (not nocompress.has_key(filename)):
						# create a hashsum for our new tile
						newtile.MakeMd5()
						
						# this flag tells us whether to add this tile to our tile set
						addtile = 1
						
						# try and find our new tile in our set of old ones
						for tileref in xrange(0, len(alltiles)):
							if (newtile.GetMd5() == alltiles[tileref].GetMd5()):
								# we have found a match
								map.SetTileRef(mapx, mapy, tileref)
								addtile = 0
						
						if (addtile):
							alltiles.append(newtile)
							map.SetTileRef(mapx, mapy, len(alltiles) - 1)
					else:
						# add our tile to our set of tiles
						tiles.append(newtile)
			
			if (not nocompress.has_key(filename)):
				maps[basename] = map
			else:
				tilesets[basename] = tiles
	
	# put our master tileset (for maps with shared tiles) in
	tilesets[outputname] = alltiles
	
	print "Done reading data."
	
	print "Outputting " + outputname + ".cc and " + outputname + ".hh"
	
	# now for the file output bit
	# open up the .hh and .cc file we want to use
	hfile = file(outputname + ".hh", "w")
	cfile = file(outputname + ".cc", "w")
	
	# write some headers
	message = "/* This file is automatically generated by " + sys.argv[0] + " */"
	
	hfile.write("%s\n\n" % message);
	cfile.write("%s\n\n" % message);
	
	hfile.write('#include "typedefs.h"\n\n')
	cfile.write('#include "%s.hh"\n\n' % outputname)
	
	# first write the palette
	paletteLength = len(palette) - 1
	
	print "Writing the palette to the file"
	
	hfile.write("#define " + string.upper(outputname) + "_PALETTE_SIZE " + str(paletteLength))
	hfile.write("extern const u16 " + outputname + "_palette[" + string.upper(outputname) + "_PALETTE_SIZE];\n")
	cfile.write("const u16 " + outputname + "_palette[" + string.upper(outputname) + "_PALETTE_SIZE] = {\n")
	
	for palref in xrange(0, paletteLength):
		cfile.write("0x%04x, " % palette[palref])
		if not ((palref + 1) % 8):
			cfile.write("\n")
	
	cfile.write("};\n\n")
	
	# now it's time to write out the maps
	
	print "Writing the maps to the file"
	
	for map in maps:
		print "Map: " + map
		mapobject = maps[map]
		
		mapLength = mapobject.GetWidth() * mapobject.GetHeight()
		
		hfile.write("#define " + string.upper(map) + "_MAP_SIZE " + str(mapLength))
		hfile.write("extern const u16 " + map + "_map[" + string.upper(map) + "_MAP_SIZE];\n")
		cfile.write("const u16 " + map + "_map[" + string.upper(map) + "_MAP_SIZE] = {\n")
		
		for y in xrange(0, mapobject.GetHeight()):
			for x in xrange(0, mapobject.GetWidth()):
				cfile.write("0x%04x, " % mapobject.GetTileRef(x, y))
			cfile.write("\n")
		
		cfile.write("};\n\n")
	
	# now it's time to write out the tiles
	
	print "Writing the tiles out to the file"

	for tileset in tilesets:
		print "Tileset: " + tileset
		
		tilesLength = len(tilesets[tileset]) * 32
		
		hfile.write("#define " + string.upper(tileset) + "_MAP_SIZE " + str(tilesLength))
		hfile.write("extern const u16 " + tileset + "_tiles[" + string.upper(tileset) + "_MAP_SIZE];\n")
		cfile.write("const u16 " + tileset + "_tiles[" + string.upper(tileset) + "_MAP_SIZE] = {\n")
		
		# look at every tile in the tileset
		for tile in tilesets[tileset]:
			# let's encode our pixels how the GBA wants them
			for y in xrange(0,8):
				for x in xrange(0,4):
					thispixel = tile.GetPixel(x * 2, y)
					nextpixel = tile.GetPixel(x * 2 + 1, y)
					
					thispixel = (nextpixel) * 256 + thispixel
					
					cfile.write("0x%04x, " % thispixel)
				cfile.write("\n")
			cfile.write("\n")
		
		cfile.write("};\n\n")
		
	# close off the files
	hfile.close()
	cfile.close()
