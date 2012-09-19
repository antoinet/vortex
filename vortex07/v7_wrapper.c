/*
 * Taken from:
 *
 * Reversing CRC - Theory and Practice.
 * HU Berlin Public Report
 * SAR-PR-2006-05
 *
 * Authors:
 *  Martin Stigge
 *  Henryk Ploetz
 *  Wolf Mueller
 *  Jens-Peter Redlich
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crc32_table.h"

#define CRCPOLY		0xEDB88320
#define CRCINV		0x5B358FD3 // inverse poly of (x^N) mod CRCPOLY
//#define INITXOR	0xFFFFFFFF
#define INITXOR		0x00000000
//#define FINALXOR	0xFFFFFFFF
#define FINALXOR	0x00000000
#define FIXCRC		0xe1ca95ee
typedef unsigned int uint32;

/**
 * Creates the CRC table with 256 32-bit entries. CAUTION: Assumes that
 * enough space for the resulting table has already been allocated.
 */
void make_crc_table(uint32 *table) {
	uint32 c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = n;
		for (k = 0; k < 8; k++) {
			if ((c & 1) != 0) {
				c = CRCPOLY ^ (c >> 1);
			} else {
				c = c >> 1;
			}
		}
		table[n] = c;
	}
}

/**
 * Computes the CRC32 of the buffer of the given length
 * using the supplied crc_table.
 */
int crc32_tabledriven(unsigned char *buffer, int length, uint32 *crc_table) {
	int i;
	uint32 crcreg = INITXOR;

	for (i = 0; i < length; ++i) {
		crcreg = (crcreg >> 8) ^ crc_table[((crcreg ^ buffer[i]) & 0xFF)];
	}

	return crcreg ^ FINALXOR;
}

/**
 * Changes the last 4 bytes of the given buffer so that it afterwards will
 * compute to the given tcrcreg using the given crc_table
 *
 * This function uses the method of the multiplication with (x^N)^-1.
 */
void fix_crc_end(unsigned char *buffer, int length, uint32 tcrcreg, uint32 *crc_table) {
	int i;
	tcrcreg ^= FINALXOR;
	
	// calculate crc except for the last 4 bytes; this is essentially crc32()
	uint32 crcreg = INITXOR;
	for (i = 0; i < length - 4; ++i) {
		crcreg = (crcreg >> 8) ^ crc_table[((crcreg ^ buffer[i]) & 0xFF)];
	}

	// calculate new content bits
	// new_content = tcrcreg * CRCINV mod CRCPOLY
	uint32 new_content = 0;
	for (i = 0; i < 32; ++i) {
		// reduce modulo CRCPOLY
		if (new_content & 1) {
			new_content = (new_content >> 1) ^ CRCPOLY;
		} else {
			new_content >>= 1;
		}
		// add CRCINV if corresponding bit of operand is set
		if (tcrcreg & 1) {
			new_content ^= CRCINV;
		}
		tcrcreg >>= 1;
	}
	// finally add old crc
	new_content ^= crcreg;

	// inject new content
	for (i = 0; i < 4; ++i) {
		buffer[length - 4 + i] = (new_content >> i*8) & 0xFF;
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("usage: %s <arg>\n", argv[0]);
		return 0;
	}
	int length = strlen(argv[1]);

	// generate CRC32 table
	uint32 crc_table_dynamic[256];
	make_crc_table(crc_table_dynamic);

	// use reference to static CRC32 table
	uint32 *crc_table = (uint32*)crc_table_static;
//	uint32 *crc_table = crc_table_dynamic;

	// compute CRC32
	int crc32;
	crc32 = crc32_tabledriven(argv[1], length, crc_table);
	fprintf(stderr, "Original CRC:\t0x%08X\n", crc32);

	// adjust buffer
	char* buffer = malloc(length + 4);
	memcpy(buffer, argv[1], length);
	fix_crc_end(buffer, length + 4, FIXCRC, crc_table);
	
	// recompute CRC32
	crc32 = crc32_tabledriven(buffer, length + 4, crc_table);
	fprintf(stderr, "Adjusted CRC:\t0x%08X\n", crc32);

	// output adjustment
	uint32 adjustment = *((uint32*)(buffer + length));
	fprintf(stderr, "Adjustment:\t0x%08X\n", adjustment);

	fprintf(stdout, "%s", buffer);
	return 0;
}
