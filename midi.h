/*****************************************************

	looper advance
	(c) chris mccormick, 2004-2014
	
	licensed under the terms of the GPL
	see the file gpl.txt for details
	
	chris@mccormick.cx

******************************************************/

// midi functions - from http://gba.atlantes.org/index_en.html#midioutadvance

#ifndef _MIDI_H_
#define _MIDI_H_

int __gba_multiboot;

struct {
	u8 *data;
	u8 data_size;
	s8 next_bit;
	u8 byte;
} midi_send_struct;

// SO = 3.3 v   (always 1)  (SO is SI in the slave end of the cable)
// SC = MIDI stream         (SC is SC too in the slave end of the cable)
// #define  REG_R_0_VALUE  0x80F8
// #define  REG_R_1_VALUE  0x80F9

#define REG_R_MIDI (R_GPIO | GPIO_SC_IO | GPIO_SD_IO | GPIO_SI_IO | GPIO_SO_IO | GPIO_SO)

#define  MIDI_CLOCKS_PER_MINUTE  ((u32)31242L * (u32)60L)
#define  MIDI_CLOCKS_BY_BPM(b)   (MIDI_CLOCKS_PER_MINUTE / (((u32) (b)) * 4))

inline void midiInit()
{
	midi_send_struct.data_size = 0;
	midi_send_struct.byte = -1;
	// turn off timer 0
	REG_TM3CNT_H = 0;
	// setup timer 0
	// 65536 - 64999 = 537
	// (16 * 1024 * 1024) / 537 = 31242 bps
	// MIDI is 31250 bsp at +/-1% tolerance (-1% => 30937, +1% => 31562)
	// so 31242 bps is a valid bps value
	REG_TM3CNT_L = 0xFDE7;   // 64999
	// use 0x3 below = slow enough divisor to watch (1024)
	REG_TM3CNT_H = TIMER_START | TIMER_IRQ | 0x0;   // start + irq + f (16MHz) 
}

inline void midiStop() {
	// stop timer 0
	REG_TM3CNT_H = 0;
}

// #define IWRAM_CODE	__attribute__((section(".iwram"), long_call))
// void InterruptProcess(void) __attribute__ ((section(".iwram")));   // the ISR must be declared in this way
inline void midiInterrupt(void) IWRAM_CODE;

static u8 midi_bit;
inline void midiInterrupt(void) {
	// dprintf("midiInterrupt\n");
	
	if (midi_send_struct.byte >= midi_send_struct.data_size) {
		midi_bit = 1;
		// dprintf("midiInterrupt OFF!\n");
		midiStop();
	} else if (midi_send_struct.next_bit == -1) {
		midi_send_struct.next_bit = 0;
		midi_bit = 0;
	} else if (midi_send_struct.next_bit > 7) {
		midi_send_struct.byte++;
		midi_send_struct.next_bit = -1;
		midi_bit = 1;
	} else {
		midi_bit = 1 & (midi_send_struct.data[midi_send_struct.byte] >> midi_send_struct.next_bit);
		midi_send_struct.next_bit++;
	}
	
	// dprintf("midiInterrupt: %d\n", midi_bit);
	
	// perform the cable send
	REG_RCNT = (midi_bit) ? REG_R_MIDI | GPIO_SC : REG_R_MIDI;
	
	// clear interrupt flag
	// REG_IF |= REG_IF;
	REG_IF = IRQ_TIMER3;
}

inline u8 midiSend(u8 *data, u8 size) {
	if (midi_send_struct.byte < midi_send_struct.data_size)
		return 0;
	midiInit();
	midi_send_struct.data = data;
	midi_send_struct.data_size = size;
	midi_send_struct.next_bit = -1;
	midi_send_struct.byte = 0;
	return 1;
}

// u8 note_on_message[] = {0x90, 0x3C, 0x40};
// u8 note_off_message[] = {0x90, 0x3C, 0x00};
// midi_send(note_on_message, 3);
// midi_send(note_off_message, 3);

// ugh - can't sent two notes in quick succession
static u8 midi_message_note[] = {0x90, 0x00, 0x00};
inline u8 note(u8 note, u8 volume) {
	midi_message_note[1] = note;
	midi_message_note[2] = volume;
	midiSend(midi_message_note, 3);
}

// song position pointer message
static u8 midi_message_spp[] = {0xF2, 0x00, 0x00};
static u32 lastbeat = 0;
inline void midiUpdateBeat(u32 beat) {
	if (lastbeat != beat) {
		midi_message_spp[2] = beat & 0x7F;
		midi_message_spp[1] = (beat >> 7) & 0x7F;
		lastbeat = beat;
		midiSend(midi_message_spp, 3);
	}
}

#endif // _MIDI_H_
