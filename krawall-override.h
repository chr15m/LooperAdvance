#ifndef __KRAWALL_OVERRIDE_H__
#define __KRAWALL_OVERRIDE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*typedef __attribute__ ((packed)) struct {
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
} Sample;*/


typedef struct {
	unsigned long	loopLength;
	unsigned char*	end;
	// neutral frequency
	unsigned long	c2Freq;
	signed char		fineTune;
	signed char		relativeNote;
	// default volume
	unsigned char	volDefault;
	// default pan
	signed char		panDefault;
	unsigned char  	loop;
	unsigned char	hq;
	signed char	data[1];
} SampleOverride;


#ifdef __cplusplus
}
#endif

#endif

