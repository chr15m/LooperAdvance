//////////////////////////////////////////////////////////////////////////
// File: 	gba.h                                             	//
// Description: Contains all the GBA register definitions	 	//
// Author:	Eloist						  	//
// Date: 	9th January 2002                                      	//
//////////////////////////////////////////////////////////////////////////
#ifndef GBA_HEADER
#define GBA_HEADER

#include "directsound.h"

#define OAMData  		((vu32*)0x7000000)
#define VideoBuffer 		((vu16*)0x6000000)
#define OAMmem			((vu16*)0x6010000)
#define BGPaletteMem 		((vu16*)0x5000000)
#define OBJPaletteMem 		((vu16*)0x5000200)

#define SRAM ((vu8*)0x0E000000)

//#define FIXED s32
#define PI 3.14159
#define RADIAN(n) 		(((float)n)/(float)180*PI)

#define BIT00 1
#define BIT01 2
#define BIT02 4
#define BIT03 8
#define BIT04 16
#define BIT05 32
#define BIT06 64
#define BIT07 128
#define BIT08 256
#define BIT09 512
#define BIT10 1024
#define BIT11 2048
#define BIT12 4096
#define BIT13 8192
#define BIT14 16384
#define BIT15 32768

#define KEY_A      BIT00
#define KEY_B      BIT01
#define KEY_SELECT BIT02
#define KEY_START  BIT03
#define KEY_RIGHT  BIT04
#define KEY_LEFT   BIT05
#define KEY_UP     BIT06
#define KEY_DOWN   BIT07
#define KEY_R      BIT08
#define KEY_L      BIT09
#define KEY_BUTTONS (KEY_A|KEY_B|KEY_SELECT|KEY_START|KEY_R|KEY_L)
#define KEY_PADDLE  (KEY_RIGHT|KEY_LEFT|KEY_UP|KEY_DOWN)
#define KEY_ANY     (KEY_PADDLE | KEY_BUTTONS)

#define REG_INTERUPT   *(volatile u32*)0x3007FFC
#define REG_DISPCNT    *(volatile u32*)0x4000000
#define REG_DISPCNT_L  *(volatile u16*)0x4000000
#define REG_DISPCNT_H  *(volatile u16*)0x4000002
#define REG_DISPSTAT   *(volatile u16*)0x4000004
#define REG_VCOUNT     *(volatile u16*)0x4000006
#define REG_BG0CNT     *(volatile u16*)0x4000008
#define REG_BG1CNT     *(volatile u16*)0x400000A
#define REG_BG2CNT     *(volatile u16*)0x400000C
#define REG_BG3CNT     *(volatile u16*)0x400000E
#define REG_BG0HOFS    *(volatile u16*)0x4000010
#define REG_BG0VOFS    *(volatile u16*)0x4000012
#define REG_BG1HOFS    *(volatile u16*)0x4000014
#define REG_BG1VOFS    *(volatile u16*)0x4000016
#define REG_BG2HOFS    *(volatile u16*)0x4000018
#define REG_BG2VOFS    *(volatile u16*)0x400001A
#define REG_BG3HOFS    *(volatile u16*)0x400001C
#define REG_BG3VOFS    *(volatile u16*)0x400001E
#define REG_BG2PA      *(volatile u16*)0x4000020
#define REG_BG2PB      *(volatile u16*)0x4000022
#define REG_BG2PC      *(volatile u16*)0x4000024
#define REG_BG2PD      *(volatile u16*)0x4000026
#define REG_BG2X       *(volatile u32*)0x4000028
#define REG_BG2X_L     *(volatile u16*)0x4000028
#define REG_BG2X_H     *(volatile u16*)0x400002A
#define REG_BG2Y       *(volatile u32*)0x400002C
#define REG_BG2Y_L     *(volatile u16*)0x400002C
#define REG_BG2Y_H     *(volatile u16*)0x400002E
#define REG_BG3PA      *(volatile u16*)0x4000030
#define REG_BG3PB      *(volatile u16*)0x4000032
#define REG_BG3PC      *(volatile u16*)0x4000034
#define REG_BG3PD      *(volatile u16*)0x4000036
#define REG_BG3X       *(volatile u32*)0x4000038
#define REG_BG3X_L     *(volatile u16*)0x4000038
#define REG_BG3X_H     *(volatile u16*)0x400003A
#define REG_BG3Y       *(volatile u32*)0x400003C
#define REG_BG3Y_L     *(volatile u16*)0x400003C
#define REG_BG3Y_H     *(volatile u16*)0x400003E
#define REG_WIN0H      *(volatile u16*)0x4000040
#define REG_WIN1H      *(volatile u16*)0x4000042
#define REG_WIN0V      *(volatile u16*)0x4000044
#define REG_WIN1V      *(volatile u16*)0x4000046
#define REG_WININ      *(volatile u16*)0x4000048
#define REG_WINOUT     *(volatile u16*)0x400004A
#define REG_MOSAIC     *(volatile u32*)0x400004C
#define REG_MOSAIC_L   *(volatile u32*)0x400004C
#define REG_MOSAIC_H   *(volatile u32*)0x400004E
#define REG_BLDMOD     *(volatile u16*)0x4000050
#define REG_COLEV      *(volatile u16*)0x4000052
#define REG_COLEY      *(volatile u16*)0x4000054

#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200 
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800
#define OBJ_ENABLE 0x1000 

#define REG_SG10       *(volatile u32*)0x4000060
#define REG_SG10_L     *(volatile u16*)0x4000060
#define REG_SG10_H     *(volatile u16*)0x4000062
#define REG_SG11       *(volatile u16*)0x4000064
#define REG_SG20       *(volatile u16*)0x4000068
#define REG_SG21       *(volatile u16*)0x400006C
#define REG_SG30       *(volatile u32*)0x4000070
#define REG_SG30_L     *(volatile u16*)0x4000070
#define REG_SG30_H     *(volatile u16*)0x4000072
#define REG_SG31       *(volatile u16*)0x4000074
#define REG_SG40       *(volatile u16*)0x4000078
#define REG_SG41       *(volatile u16*)0x400007C
#define REG_SGCNT0     *(volatile u32*)0x4000080
#define REG_SGCNT0_L   *(volatile u16*)0x4000080
#define REG_SGCNT0_H   *(volatile u16*)0x4000082
#define REG_SGCNT1     *(volatile u16*)0x4000084
#define REG_SGBIAS     *(volatile u16*)0x4000088
#define REG_SGWR0      *(volatile u32*)0x4000090
#define REG_SGWR0_L    *(volatile u16*)0x4000090
#define REG_SGWR0_H    *(volatile u16*)0x4000092
#define REG_SGWR1      *(volatile u32*)0x4000094
#define REG_SGWR1_L    *(volatile u16*)0x4000094
#define REG_SGWR1_H    *(volatile u16*)0x4000096
#define REG_SGWR2      *(volatile u32*)0x4000098
#define REG_SGWR2_L    *(volatile u16*)0x4000098
#define REG_SGWR2_H    *(volatile u16*)0x400009A
#define REG_SGWR3      *(volatile u32*)0x400009C
#define REG_SGWR3_L    *(volatile u16*)0x400009C
#define REG_SGWR3_H    *(volatile u16*)0x400009E
#define REG_SGFIFOA    *(volatile u32*)0x40000A0
#define REG_SGFIFOA_L  *(volatile u16*)0x40000A0
#define REG_SGFIFOA_H  *(volatile u16*)0x40000A2
#define REG_SGFIFOB    *(volatile u32*)0x40000A4
#define REG_SGFIFOB_L  *(volatile u16*)0x40000A4
#define REG_SGFIFOB_H  *(volatile u16*)0x40000A6
#define REG_DM0SAD     *(volatile u32*)0x40000B0
#define REG_DM0SAD_L   *(volatile u16*)0x40000B0
#define REG_DM0SAD_H   *(volatile u16*)0x40000B2
#define REG_DM0DAD     *(volatile u32*)0x40000B4
#define REG_DM0DAD_L   *(volatile u16*)0x40000B4
#define REG_DM0DAD_H   *(volatile u16*)0x40000B6
#define REG_DM0CNT     *(volatile u32*)0x40000B8
#define REG_DM0CNT_L   *(volatile u16*)0x40000B8
#define REG_DM0CNT_H   *(volatile u16*)0x40000BA
#define REG_DM1SAD     *(volatile u32*)0x40000BC
#define REG_DM1SAD_L   *(volatile u16*)0x40000BC
#define REG_DM1SAD_H   *(volatile u16*)0x40000BE
#define REG_DM1DAD     *(volatile u32*)0x40000C0
#define REG_DM1DAD_L   *(volatile u16*)0x40000C0
#define REG_DM1DAD_H   *(volatile u16*)0x40000C2
#define REG_DM1CNT     *(volatile u32*)0x40000C4
#define REG_DM1CNT_L   *(volatile u16*)0x40000C4
#define REG_DM1CNT_H   *(volatile u16*)0x40000C6
#define REG_DM2SAD     *(volatile u32*)0x40000C8
#define REG_DM2SAD_L   *(volatile u16*)0x40000C8
#define REG_DM2SAD_H   *(volatile u16*)0x40000CA
#define REG_DM2DAD     *(volatile u32*)0x40000CC
#define REG_DM2DAD_L   *(volatile u16*)0x40000CC
#define REG_DM2DAD_H   *(volatile u16*)0x40000CE
#define REG_DM2CNT     *(volatile u32*)0x40000D0
#define REG_DM2CNT_L   *(volatile u16*)0x40000D0
#define REG_DM2CNT_H   *(volatile u16*)0x40000D2
#define REG_DM3SAD     *(volatile u32*)0x40000D4
#define REG_DM3SAD_L   *(volatile u16*)0x40000D4
#define REG_DM3SAD_H   *(volatile u16*)0x40000D6
#define REG_DM3DAD     *(volatile u32*)0x40000D8
#define REG_DM3DAD_L   *(volatile u16*)0x40000D8
#define REG_DM3DAD_H   *(volatile u16*)0x40000DA
#define REG_DM3CNT     *(volatile u32*)0x40000DC
#define REG_DM3CNT_L   *(volatile u16*)0x40000DC
#define REG_DM3CNT_H   *(volatile u16*)0x40000DE
#define REG_TM0D       *(volatile u16*)0x4000100
#define REG_TM0CNT     *(volatile u16*)0x4000102
#define REG_TM1D       *(volatile u16*)0x4000104
#define REG_TM1CNT     *(volatile u16*)0x4000106
#define REG_TM2D       *(volatile u16*)0x4000108
#define REG_TM2CNT     *(volatile u16*)0x400010A
#define REG_TM3D       *(volatile u16*)0x400010C
#define REG_TM3CNT     *(volatile u16*)0x400010E
#define REG_SCD0       *(volatile u16*)0x4000120
#define REG_SCD1       *(volatile u16*)0x4000122
#define REG_SCD2       *(volatile u16*)0x4000124
#define REG_SCD3       *(volatile u16*)0x4000126
#define REG_SCCNT      *(volatile u32*)0x4000128
#define REG_SCCNT_L    *(volatile u16*)0x4000128
#define REG_SCCNT_H    *(volatile u16*)0x400012A
#define REG_P1         *(volatile u16*)0x4000130
#define REG_JP         REG_P1
#define REG_P1CNT      *(volatile u16*)0x4000132
#define REG_R          *(volatile u16*)0x4000134
#define REG_HS_CTRL    *(volatile u16*)0x4000140
#define REG_JOYRE      *(volatile u32*)0x4000150
#define REG_JOYRE_L    *(volatile u16*)0x4000150
#define REG_JOYRE_H    *(volatile u16*)0x4000152
#define REG_JOYTR      *(volatile u32*)0x4000154
#define REG_JOYTR_L    *(volatile u16*)0x4000154
#define REG_JOYTR_H    *(volatile u16*)0x4000156
#define REG_JSTAT      *(volatile u32*)0x4000158
#define REG_JSTAT_L    *(volatile u16*)0x4000158
#define REG_JSTAT_H    *(volatile u16*)0x400015A
#define REG_IE         *(volatile u16*)0x4000200
#define REG_IF         *(volatile u16*)0x4000202
#define REG_WSCNT      *(volatile u16*)0x4000204
#define REG_IME        *(volatile u16*)0x4000208
#define REG_PAUSE      *(volatile u16*)0x4000300


// sound hardware defines
#define REG_SOUND1CNT_L *(volatile u16*)0x04000060       // sound 1 sweep control  
#define REG_SOUND1CNT_H *(volatile u16*)0x04000062       // sound 1 length, wave duty and envelope control 
#define REG_SOUND1CNT_X *(volatile u16*)0x04000064       // sound 1 frequency, reset and loop control 
#define REG_SOUND2CNT_L *(volatile u16*)0x04000068       // sound 2 length, wave duty and envelope control 
#define REG_SOUND2CNT_H *(volatile u16*)0x0400006C       // sound 2 frequency, reset and loop control 
#define REG_SOUND3CNT_L *(volatile u16*)0x04000070       // sound 3 enable and wave ram bank control 
#define REG_SOUND3CNT_H *(volatile u16*)0x04000072       // sound 3 sound length and output level control 
#define REG_SOUND3CNT_X *(volatile u16*)0x04000074       // sound 3 frequency, reset and loop control 
#define REG_SOUND4CNT_L *(volatile u16*)0x04000078       // sound 4 length, output level and envelope control 
#define REG_SOUND4CNT_H *(volatile u16*)0x0400007C       // sound 4 noise parameters, reset and loop control 
#define REG_SOUNDCNT_L  *(volatile u16*)0x04000080       // sound 1-4 output level and stereo control 
#define REG_SOUNDCNT_H  *(volatile u16*)0x04000082       // direct sound control and sound 1-4 output ratio 
#define REG_SOUNDCNT_X  *(volatile u32*)0x04000084       // master sound enable and sound 1-4 play status 
#define REG_SOUNDBIAS   *(volatile u32*)0x04000088       // sound bias and amplitude resolution control 
#define REG_WAVE_RAM0_L *(volatile u16*)0x04000090       // sound 3 samples 0-3 
#define REG_WAVE_RAM0_H *(volatile u16*)0x04000092       // sound 3 samples 4-7 
#define REG_WAVE_RAM1_L *(volatile u16*)0x04000094       // sound 3 samples 8-11 
#define REG_WAVE_RAM1_H *(volatile u16*)0x04000096       // sound 3 samples 12-15 
#define REG_WAVE_RAM2_L *(volatile u16*)0x04000098       // sound 3 samples 16-19 
#define REG_WAVE_RAM2_H *(volatile u16*)0x0400009A       // sound 3 samples 20-23 
#define REG_WAVE_RAM3_L *(volatile u16*)0x0400009C       // sound 3 samples 23-27 
#define REG_WAVE_RAM3_H *(volatile u16*)0x0400009E       // sound 3 samples 28-31 
#define REG_FIFO_A      0x040000A0              // direct sound channel A samples 0-3 
#define REG_FIFO_A_L    0x040000A0              // direct sound channel A samples 0-1 
#define REG_FIFO_A_H    0x040000A2              // direct sound channel A samples 2-3 
#define REG_FIFO_B      0x040000A4              // direct sound channel B samples 0-3 
#define REG_FIFO_B_L    0x040000A4              // direct sound channel B samples 0-1 
#define REG_FIFO_B_H    0x040000A6              // direct sound channel B samples 2-3 

// timer defines
#define REG_TM0D        *(volatile u16*)0x4000100
#define REG_TM0CNT      *(volatile u16*)0x4000102
#define REG_TM1D        *(volatile u16*)0x4000104
#define REG_TM1CNT      *(volatile u16*)0x4000106
#define REG_TM2D        *(volatile u16*)0x4000108
#define REG_TM2CNT      *(volatile u16*)0x400010A
#define REG_TM3D        *(volatile u16*)0x400010C
#define REG_TM3CNT      *(volatile u16*)0x400010E

#define TIMER_FREQ_SYSTEM		0x0
#define TIMER_FREQ_64			0x1
#define TIMER_FREQ_256			0x2
#define TIMER_FREQ_1024			0x3
#define TIMER_CASCADE			0x4
#define TIMER_IRQ_ENABLE		0x40
#define TIMER_ENABLE			0x80

// DMA defines
#define REG_DMA0SAD          *(volatile u32*)0x40000B0
#define REG_DMA0DAD          *(volatile u32*)0x40000B4
#define REG_DMA0CNT          *(volatile u32*)0x40000B8
#define REG_DMA1SAD          *(volatile u32*)0x40000BC
#define REG_DMA1DAD          *(volatile u32*)0x40000C0
#define REG_DMA1CNT          *(volatile u32*)0x40000C4
#define REG_DMA2SAD          *(volatile u32*)0x40000C8
#define REG_DMA2DAD          *(volatile u32*)0x40000CC
#define REG_DMA2CNT          *(volatile u32*)0x40000D0
#define REG_DMA3SAD          *(volatile u32*)0x40000D4
#define REG_DMA3DAD          *(volatile u32*)0x40000D8
#define REG_DMA3CNT          *(volatile u32*)0x40000DC
#define WORD_DMA             0x04000000
#define HALF_WORD_DMA        0x00000000
#define DMA_32	             0x04000000
#define DMA_16		         0x00000000
#define ENABLE_DMA           0x80000000
#define START_ON_FIFO_EMPTY  0x30000000
#define DMA_REPEAT           0x02000000
#define DEST_REG_SAME        0x00400000
#define DMA_ENABLE           0x00008000

#define DMA_TIMING_SYNC_TO_DISPLAY 0x3000
#define DMA_TIMING_HBLANK 0x2000
#define DMA_TIMING_VBLANK 0x1000
#define DMA_TIMING_IMMEDIATE 0x0000
#define DMA_SOURCE_INCREMENT 0x0000
#define DMA_SOURCE_DECREMENT 0x0080
#define DMA_SOURCE_FIXED 0x0100
#define DMA_DEST_INCREMENT 0x0000
#define DMA_DEST_DECREMENT 0x0020
#define DMA_DEST_FIXED 0x0040

#define DMA32NOW DMA_DEST_INCREMENT | DMA_SOURCE_INCREMENT | DMA_TIMING_IMMEDIATE |  DMA_32
#define DMA16NOW DMA_DEST_INCREMENT | DMA_SOURCE_INCREMENT | DMA_TIMING_IMMEDIATE |  DMA_16


#define DMA_CHANNEL_0        0x0
#define DMA_CHANNEL_1        0x1
#define DMA_CHANNEL_2        0x2
#define DMA_CHANNEL_3        0x3
#define DMA_CHANNEL_DEFAULT  0x3


// Key interrupt generation values.
#define KEY_IRQ		(0x4000)	// IRQ enable.
#define KEY_AND		(0x8000)	// AND the keys.
#define KEY_OR		(0x0000)	// OR the keys.
#define REG_KEYCNT		(*(volatile u16 *)0x04000132)

// Interrupt mask numbers.
#define	INT_V		(0x0001)
#define	INT_H		(0x0002)
#define	INT_VC		(0x0004)
#define	INT_TM0		(0x0008)
#define	INT_TM1		(0x0010)
#define	INT_TM2		(0x0020)
#define	INT_TM3		(0x0040)
#define	INT_SER		(0x0080)
#define	INT_DMA0	(0x0100)
#define	INT_DMA1	(0x0200)
#define	INT_DMA2	(0x0400)
#define	INT_DMA3	(0x0800)
#define	INT_KEY		(0x1000)
#define	INT_CAR		(0x2000)

/* some flags for attribute 0 */
#define SPRITE_ROTATION					0x100
#define SPRITE_SIZE_DOUBLE				0x200
#define SPRITE_ALPHA_NORMAL			0x0
#define SPRITE_ALPHA_TRANSPARENT		0x400
#define SPRITE_ALPHA_WINDOWED			0x800
#define SPRITE_MOSAIC					0x1000
#define SPRITE_COLOUR16					0x0
#define SPRITE_COLOUR256				0x2000
#define SPRITE_SHAPE_SQUARE			0x0
#define SPRITE_SHAPE_WIDE				0x4000
#define SPRITE_SHAPE_TALL				0x8000

/* some flags for attribute 1 */
#define SPRITE_HORIZONTAL_FLIP		0x1000
#define SPRITE_VERTICAL_FLIP			0x2000
#define SPRITE_SIZE8						0x0
#define SPRITE_SIZE16					0x4000
#define SPRITE_SIZE32					0x8000
#define SPRITE_SIZE64					0xC000

#define SPRITE_FLIP_MASK	0xcfff

/* shape / size table 
** ==================
**            SIZE8        SIZE16          SIZE32        SIZE64
**   SQUARE    8x8         16x16           32x32         64x64     
**   WIDE      16x8        32x8            32x16         64x32     
**   TALL      8x16        8x32            16x32         32x64       
*/

//Attribute2 stuff
#define SPRITE_PRIORITY(n)		((n) << 10)
#define SPRITE_PALETTE(n)		((n) << 12)

#define CODE_IN_IWRAM __attribute__ ((section (".iwram"), long_call))
#define CODE_IN_ROM __attribute__ ((section (".text"), long_call))
#define IN_IWRAM __attribute__ ((section (".iwram")))
#define IN_EWRAM __attribute__ ((section (".ewram")))

#define SCREEN_HEIGHT_PIXELS		160
#define HALF_SCREEN_HEIGHT_PIXELS	80

#define SCREEN_WIDTH_PIXELS			240
#define HALF_SCREEN_WIDTH_PIXELS	120

#endif
