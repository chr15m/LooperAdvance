/***********************************************************
* KRAWALL Soundsystem
* Copyright (c) 2001,2002 Sebastian Kienzl <seb@riot.org>
************************************************************
* $Id$
************************************************************/

#ifndef __MTYPES_H__
#define __MTYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef __attribute__ ((packed)) struct {
	unsigned long 	loopLength;
	unsigned char*	end;
	unsigned long	c2Freq;
	signed char		fineTune;
	signed char		relativeNote;
	unsigned char  	volDefault;
	signed char		panDefault;
	unsigned char  	loop;
	unsigned char	hq;
	signed char  	data[1];
} Sample;

typedef __attribute__ ((packed)) struct {
	unsigned short	coord, inc;
} EnvNode;

typedef __attribute__ ((packed)) struct {
	EnvNode			nodes[ 12 ];
	unsigned char	max;
	unsigned char	sus;
	unsigned char	loopStart;
	unsigned char	flags;
} Envelope;


typedef __attribute__ ((packed)) struct {
	unsigned short	samples[ 96 ];

	Envelope		envVol;
	Envelope		envPan;
	unsigned short	volFade;

	unsigned char	vibType;
	unsigned char	vibSweep;
	unsigned char	vibDepth;
	unsigned char	vibRate;
} Instrument;

typedef __attribute__ ((packed)) struct {
	unsigned short 	index[ 16 ];
	unsigned char	rows;
	unsigned char 	data[1];
} Pattern;

typedef __attribute__ ((packed)) struct {
	unsigned char 	channels;
	unsigned char 	numOrders;
	unsigned char	songRestart;
	unsigned char 	order[ 256 ];

	signed char 	channelPan[ 32 ];

	unsigned char 	songIndex[ 64 ];

	unsigned char 	volGlobal;

	unsigned char 	initSpeed;
	unsigned char 	initBPM;

	unsigned char	flagInstrumentBased;
	unsigned char	flagLinearSlides;
	unsigned char 	flagVolSlides;
	unsigned char 	flagVolOpt;
	unsigned char 	flagAmigaLimits;
	unsigned char	___padding;

	const Pattern* 	patterns[1];
} Module;

extern const Sample* const samples[];
extern const Instrument* const instruments[];

#ifdef __cplusplus
}
#endif

#endif

