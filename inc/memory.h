
#ifndef MEMORY_T
#define MEMORY_T

struct flow_memory_t {
    size_t count;
};

struct flow_memory_item_t {
    struct flow_memory_t* parent;
};

struct flow_memory_t* flow_memory_new();

void flow_memory_free(struct flow_memory_t* self);

void* flow_memory_alloc(struct flow_memory_t* self, size_t size);
void* flow_memory_realloc(struct flow_memory_t* self, void* data, size_t size);

#define flow_memory_alloc_typed(SELF, TYPE) (TYPE*) flow_memory_alloc(SELF, sizeof(TYPE))
#define flow_memory_retain(VALUE) VALUE = VALUE;
#define flow_memory_release(VALUE) VALUE = VALUE;

void flow_memory_item_free(void* self);

#endif
