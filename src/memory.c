#include <stdlib.h>
#include "memory.h"

struct flow_memory_t* flow_memory_new() {
    struct flow_memory_t* self = (struct flow_memory_t*) malloc(sizeof(struct flow_memory_t));
    self->count = 0;
    return self;
}

void flow_memory_free(struct flow_memory_t* self) {
    free(self);
}

void* flow_memory_alloc(struct flow_memory_t* self, size_t size) {
    struct flow_memory_item_t* data = (struct flow_memory_item_t*) malloc(sizeof(struct flow_memory_item_t) + size);
    data->parent = self;
    self->count++;
    return data + 1;
}

void* flow_memory_realloc(struct flow_memory_t* self, void* data, size_t size) {
    struct flow_memory_item_t* old_data = ((struct flow_memory_item_t*)data) - 1;
    struct flow_memory_item_t* new_data = (struct flow_memory_item_t*) realloc(old_data, sizeof(struct flow_memory_item_t) + size);
    return new_data + 1;
}

void flow_memory_item_free(void* self) {
    struct flow_memory_item_t* data = ((struct flow_memory_item_t*)self) - 1;
    data->parent->count--;
    free(data);
}
