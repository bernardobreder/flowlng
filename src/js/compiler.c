#include <stdlib.h>
#include "memory.h"
#include "js.h"

struct js_compiler_error_t* js_compiler_error_new(struct flow_memory_t* memory, char* message) {
    struct js_compiler_error_t* self = flow_memory_alloc_typed(memory, struct js_compiler_error_t);
    self->next = 0;
    self->message = message;
    return self;
}

void js_compiler_error_free(struct js_compiler_t* self) {
    flow_memory_item_free(self);
}

struct js_compiler_t* js_compiler_new(struct flow_memory_t* memory) {
    struct js_compiler_t* self = flow_memory_alloc_typed(memory, struct js_compiler_t);
    return self;
}

void js_compiler_free(struct js_compiler_t* self) {
    flow_memory_item_free(self);
}

js_bool js_compiler_error_empty(struct js_compiler_t* self) {
    return self->error == 0;
}

void js_compiler_error_push(struct js_compiler_t* self, struct js_compiler_error_t* error) {
    error->next = self->error;
    self->error = error;
}

struct js_compiler_error_t* js_compiler_error_pop(struct js_compiler_t* self) {
    struct js_compiler_error_t* error = self->error;
    self->error = error->next;
    error->next = 0;
    return error;
}
