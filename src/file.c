
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

unsigned char* flow_io_file(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) {
		return 0;
	}
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* data = (unsigned char*) malloc(size + 1);
	if (!data) {
		fclose(file);
		return 0;
	}
	size_t readed = fread(data, 1, size, file);
	if (readed != size) {
		free(data);
		fclose(file);
		return 0;
	}
	data[size] = 0;
	fclose(file);
	return data;
}

void flow_io_int_write(unsigned char* bytes, int value) {
	unsigned char neg = value < 0;
	unsigned int uvalue = neg ? abs(value) : value;
	*bytes++ = (neg ? 0x80 : 0) + ((uvalue >> 24) & 0x7F);
	*bytes++ = (uvalue >> 16) & 0xFF;
	*bytes++ = (uvalue >> 8) & 0xFF;
	*bytes++ = (uvalue) & 0xFF;
}

void flow_io_uint32_write(unsigned char* bytes, unsigned int value) {
	*(bytes++) = (value >> 24) & 0xFF;
	*(bytes++) = (value >> 16) & 0xFF;
	*(bytes++) = (value >> 8) & 0xFF;
	*(bytes++) = (value) & 0xFF;
}

unsigned int flow_io_uint_read(const unsigned char* bytes) {
	unsigned char i1 = *(bytes++);
	unsigned char i2 = *(bytes++);
	unsigned char i3 = *(bytes++);
	unsigned char i4 = *(bytes++);
	return (i1 << 24) + (i2 << 16) + (i3 << 8) + i4;
}

void flow_io_uint64_write(unsigned char* bytes, uint64_t value) {
	*bytes++ = (value >> 56) & 0xFF;
	*bytes++ = (value >> 48) & 0xFF;
	*bytes++ = (value >> 40) & 0xFF;
	*bytes++ = (value >> 32) & 0xFF;
	*bytes++ = (value >> 24) & 0xFF;
	*bytes++ = (value >> 16) & 0xFF;
	*bytes++ = (value >> 8) & 0xFF;
	*bytes++ = (value) & 0xFF;
}

uint64_t flow_io_ulong_read(unsigned char* bytes) {
	uint64_t a = *bytes++;
	uint64_t b = *bytes++;
	uint64_t c = *bytes++;
	uint64_t d = *bytes++;
	uint32_t e = *bytes++;
	uint32_t f = *bytes++;
	uint32_t g = *bytes++;
	uint32_t h = *bytes++;
	return (a << 56) + (b << 48) + (c << 40) + (d << 32) + (e << 24) + (f << 16) + (g << 8) + h;
}