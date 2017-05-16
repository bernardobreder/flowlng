#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"
#include "js.h"

void js_value_free(struct js_value_t* self) {
    if (!self) return;
    if (self->next) js_value_free(self->next);
    free(self);
}

struct js_value_null_t* js_value_null() {
    static struct js_value_null_t value = {JS_VALUE_NULL, 0};
    return &value;
}

struct js_value_boolean_t* js_value_true() {
    static struct js_value_boolean_t value = {JS_VALUE_BOOL, 0, 1};
    return &value;
}

struct js_value_boolean_t* js_value_false() {
    static struct js_value_boolean_t value = {JS_VALUE_BOOL, 0, 0};
    return &value;
}

struct js_value_int_t* js_value_int_new(struct flow_memory_t* memory, int32 value) {
    struct js_value_int_t* self = flow_memory_alloc_typed(memory, struct js_value_int_t);
    self->type = JS_VALUE_INT;
    self->next = 0;
    self->value = value;
    return self;
}

struct js_value_num_t* js_value_num_new(struct flow_memory_t* memory, double value) {
    struct js_value_num_t* self = flow_memory_alloc_typed(memory, struct js_value_num_t);
    self->type = JS_VALUE_NUM;
    self->next = 0;
    self->value = value;
    return self;
}

char* js_value_object_string_ansi(struct js_value_t* self) {
    switch (self->type) {
        case JS_VALUE_NULL: return strdup("null");
        case JS_VALUE_BOOL: return strdup(((struct js_value_boolean_t*)self)->value ? "true" : "false");
        case JS_VALUE_INT: {
            char chars[32];
            sprintf(chars, "%d", ((struct js_value_int_t*)self)->value);
            return strdup(chars);
        }
        case JS_VALUE_NUM: {
            char chars[64];
            sprintf(chars, "%lf", ((struct js_value_num_t*)self)->value);
            return strdup(chars);
        }
        default: return strdup("<object>");
    }
}
