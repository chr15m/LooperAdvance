#include "gba.h"
#include "stdio.h"
#include "string.h"
#include "typedefs.h"
#include "emudp.h"
#include "screenmode.h"
#include "helpers.h"

// IntrTable for crt0
void (*IntrTable[])() =
{
	0, // v-blank
	0, // h-blank
	0, // vcount
	0, // timer0
	0, // timer1
	0, // timer2
	0, // timer3
	0, // serial
	0, // dma0
	0, // dma1
	0, // dma2
	0, // dma3
	0, // key
	0  // cart
};

int main()
{
	u32 offset=0;
	char string1[255];
	char string2[255];
	u32 myint;
	u16 hex;
	u16 magic = 0xABCD;
	u32 i=1234567;
	
	// write a bunch
	bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));
	offset += sizeof(u16);
	
	// write an odd number of letters
	bcopy("hello", (char *)(SRAM + offset), sizeof("hello"));
	offset += sizeof("hello");
	if (offset % 2) offset++;
	
	// write a bunch
	bcopy((const char*)&i, (char *)(SRAM + offset), sizeof(u32));
	offset += sizeof(u32);
	
	// write an odd number of letters
	bcopy("pants", (char *)(SRAM + offset), sizeof("pants"));
	offset += sizeof("pants");
	if (offset % 2) offset++;
	
	// write a bunch
	bcopy((const char*)&magic, (char *)(SRAM + offset), sizeof(u16));
	offset += sizeof(u16);
	
	// read it back out again
	// reset our pointer
	offset = 0;
	dprintf("Results:\n");
	
	// read the start number
	bcopy((const char*)(SRAM + offset), (char *)&hex, sizeof(u16));
	offset += sizeof(u16);
	dprintf("0x%x\n", hex);
	
	// read in a string
	strncpy(string1, (const char*)(SRAM + offset), 255);
	dprintf("String size: %d\n", strlen(string1));
	offset += sizeof(strlen(string1)) + 1;
	if (offset % 2) offset++;
	dprintf("%s\n", string1);
	
	// read the start number
	bcopy((const char*)(SRAM + offset), (char *)&myint, sizeof(u32));
	offset += sizeof(u32);
	dprintf("%d\n", myint);
	
	// read in a string
	strncpy(string2, (const char*)(SRAM + offset), 255);
	dprintf("String size: %d\n", strlen(string2));
	offset += sizeof(strlen(string2)) + 1;
	if (offset % 2) offset++;
	dprintf("%s\n", string2);
	
	// read the start number
	bcopy((const char*)(SRAM + offset), (char *)&hex, sizeof(u16));
	offset += sizeof(u16);
	dprintf("0x%x\n", hex);
	
	dprintf("\n");
	while(1);
}
