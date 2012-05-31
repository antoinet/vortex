#include "hexdump.h"

void hexdump (const char* buf, char* dest, int offset, int length) {
	static char hex_digits[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	int row, col, i;
	int num_rows = (length + NUM_COLS - 1)/NUM_COLS;

	buf = buf + offset;
	for (row = 0; row < num_rows; row++) {
		for (col = 0; col < NUM_COLS; col++) {
			if (row*NUM_COLS + col >= length) {
				for (i = 0; i < 3; i++) *(dest++) = 0x20;
				continue;
			}
			*(dest++) = hex_digits[*(buf+col)>>4&0x0f];
			*(dest++) = hex_digits[*(buf+col)&0x0f];
			*(dest++) = 0x20;
		}
		for (col = 0; col < NUM_COLS; col++) {
			if (row*NUM_COLS + col>= length) {
				*(dest++) = 0x20;
				continue;
			}
			*(dest++) = (*(buf+col) >= 0x20 && *(buf+col) <= 0x7E) ? *(buf+col) : '.';
		}
		buf += NUM_COLS;
		*(dest++) = 0x0A;
	}
	*dest = 0x00;
}

