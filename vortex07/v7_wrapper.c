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

#define CRCPOLY  0xEDB88320
#define CRCINV   0x5B358FD3 // inverse poly of (x^N) mod CRCPOLY
#define INITXOR  0xFFFFFFFF
#define FINALXOR 0xFFFFFFFF
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


int main(int argc, char* argv[]) {
	uint32 crc_table[256];
	int crc32;

	make_crc_table(crc_table);
	crc32 = crc32_tabledriven(argv[1], strlen(argv[1]), crc_table);
	printf("0x%08X\n", crc32);

	return 0;
}
