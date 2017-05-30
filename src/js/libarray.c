#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "js.h"

void js_value_array_new(struct js_context_t* context) {
    struct js_value_array_t* self = flow_memory_alloc_typed(context->memory, struct js_value_array_t);
    self->ref_counter = 0;
    self->type = JS_VALUE_ARRAY;
    self->next = 0;
    self->max = 8;
    self->len = 0;
    self->array = 0;
    js_context_push_typed(context, self);
}

void js_value_array_free(struct js_value_array_t* self) {
    js_int n; for (n = 0 ; n < self->len ; n++) {
        js_value_release(self->array[n]);
    }
    if (self->array) flow_memory_item_free(self->array);
    if (self->next) js_value_free(self->next);
    flow_memory_item_free(self);
}

void js_value_array_length(struct js_context_t* context) {
    js_context_peek_typed_def(context, value, struct js_value_array_t*);
    js_value_retain(value);
    js_context_pop(context);
    js_value_int_new(context, value->len);
}

void js_value_array_add(struct js_context_t* context) {
    js_context_peek_def(context, value);
    js_value_retain(value);
    js_context_pop(context);
    
    js_context_peek_typed_def(context, self, struct js_value_array_t*);
    js_value_retain(self);
    js_context_pop(context);
    
    if (!self->array) {
        self->array = (struct js_value_t**) flow_memory_alloc(context->memory, self->max * sizeof(struct js_value_t*));
    }
    
    if (self->len == self->max) {
        self->max = self->max + self->max / 2;
        self->array = (struct js_value_t**) flow_memory_realloc(context->memory, self->array, self->max * sizeof(struct js_value_t*));
    }
    
    js_value_retain(value);
    self->array[self->len++] = value;
}

void js_value_array_last(struct js_context_t* context) {
    js_context_peek_typed_def(context, self, struct js_value_array_t*);
    js_value_retain(self);
    js_context_pop(context);
    
    if (self->len == 0) {
        js_value_null_new(context);
    } else {
        js_value_def(value, self->array[self->len-1]);
        js_context_push_typed(context, value);
    }
}

void js_value_array_first(struct js_context_t* context) {
    js_context_peek_typed_def(context, self, struct js_value_array_t*);
    js_value_retain(self);
    js_context_pop(context);
    
    if (self->len == 0) {
        js_value_null_new(context);
    } else {
        js_value_def(value, self->array[0]);
        js_context_push_typed(context, value);
    }
}

void js_value_array_get(struct js_context_t* context) {
    js_context_peek_int_def(context, index);
    js_value_retain(index);
    js_context_pop(context);
    
    js_context_peek_typed_def(context, self, struct js_value_array_t*);
    js_value_retain(self);
    js_context_pop(context);
    
    if (self->len == 0) {
        js_value_null_new(context);
    } else if (index < 0 || index->value >= self->len) {
        js_value_null_new(context);
    } else {
        js_value_def(value, self->array[index->value]);
        js_context_push_typed(context, value);
    }
}
