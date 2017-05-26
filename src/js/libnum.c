#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "js.h"

void js_value_num_new(struct js_context_t* context, js_num value) {
    struct js_value_num_t* self = flow_memory_alloc_typed(context->memory, struct js_value_num_t);
    self->ref_counter = 0;
    self->type = JS_VALUE_NUM;
    self->next = 0;
    self->value = value;
    self->str_value = 0;
    self->str_size_value = 0;
    js_context_push_typed(context, self);
}

void js_value_num_free(struct js_value_num_t* self) {
    js_free_nullable(self->str_value);
    if (self->next) js_value_free(self->next);
    flow_memory_item_free(self);
}

void js_value_num_sum(struct js_context_t* context) {
    js_context_peek_def(context, left);
    js_context_peek_index_def(context, right, 1);
    if (js_value_is_int(right)) {
        js_value_num_def(num_left, left);
        js_value_int_def(int_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_num_new(context, num_left + int_right);
    } else if (js_value_is_num(right)) {
        js_value_num_def(num_left, left);
        js_value_num_def(num_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_num_new(context, num_left + num_right);
    } else if (js_value_is_str(right)) {
        js_value_to_str_def(str_left, left);
        js_value_to_str_def(str_right, right);
        js_value_str_len_def(str_len_left, left);
        js_value_str_len_def(str_len_right, right);
        js_context_pop(context);
        js_context_pop(context);
        size_t length = str_len_left + str_len_right;
        js_char buffer[length + 1];
        strncpy(buffer, str_left, str_len_left);
        strncpy(buffer + str_len_left, str_right, str_len_right);
        buffer[length] = 0;
        js_hash hash = js_value_str_hash_concat(left, right);
        js_value_str_new(context, buffer, length, hash);
    } else {
        js_context_pop(context);
    }
}
