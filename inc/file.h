
#include <stdint.h>

unsigned char* flow_io_file(const char* filename);

void flow_io_int_write(unsigned char* bytes, int value);

void flow_io_uint32_write(unsigned char* bytes, unsigned int value);

unsigned int flow_io_uint_read(const unsigned char* bytes);

void flow_io_uint64_write(unsigned char* bytes, uint64_t value);

uint64_t flow_io_ulong_read(unsigned char* bytes);