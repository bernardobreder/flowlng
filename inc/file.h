
#include <stdint.h>
#include <time.h>

struct flow_resource_t {
    unsigned char is_file;
    char* path;
    char* name;
};

struct flow_resource_node_t {
    unsigned char is_file;
    char* path;
    char* name;
    struct flow_resource_node_t* next;
};

struct flow_resource_node_t* flow_file_list(char* path);

struct flow_resource_node_t* flow_file_list_deep(char* path);

int flow_resource_node_cmp(struct flow_resource_node_t* left, struct flow_resource_node_t* right);

int flow_resource_node_cmp_inv(struct flow_resource_node_t* left, struct flow_resource_node_t* right);

struct flow_resource_node_t* flow_resource_node_next(struct flow_resource_node_t* node);

void flow_resource_node_setnext(struct flow_resource_node_t* node, struct flow_resource_node_t* value);

void flow_resource_node_free(struct flow_resource_node_t* self, unsigned char deep);

size_t flow_file_length(const char* filename);

time_t flow_file_modif(const char* filename);

unsigned char* flow_io_file(const char* filename);

void flow_io_int_write(unsigned char* bytes, int value);

void flow_io_uint32_write(unsigned char* bytes, unsigned int value);

unsigned int flow_io_uint_read(const unsigned char* bytes);

void flow_io_uint64_write(unsigned char* bytes, uint64_t value);

uint64_t flow_io_ulong_read(unsigned char* bytes);