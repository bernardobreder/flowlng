#include <stdlib.h>
#include "memory.h"
#include "js.h"

struct js_context_t* js_context_new(struct flow_memory_t* memory) {
    struct js_context_t* self = (struct js_context_t*) malloc(sizeof(struct js_context_t));
    self->value = 0;
    self->memory = memory;
    return self;
}

void js_context_free(struct js_context_t* self) {
    js_value_free_typed(self->value);
    free(self);
}

uint8 js_context_empty(struct js_context_t* self) {
    return self->value == 0;
}

void js_context_push(struct js_context_t* self, struct js_value_t* value) {
    value->next = self->value;
    self->value = value;
    js_value_retain(value);
}

struct js_value_t* js_context_pop(struct js_context_t* self) {
    struct js_value_t* value = self->value;
    self->value = value->next;
    value->next = 0;
    js_value_release(value);
    return value;
}

struct js_value_t* js_context_peek_index(struct js_context_t* self, js_size index) {
    struct js_value_t* value = self->value;
    while (index--) value = value->next;
    return value;
}
