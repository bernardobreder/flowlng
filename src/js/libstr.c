#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "js.h"

void js_value_str_concat(struct js_context_t* context) {
    js_context_pop_def(context, left);
    js_context_pop_def(context, right);
    js_value_to_str_def(str_left, left);
    js_value_to_str_def(str_right, right);
    js_value_str_len_def(str_len_left, left);
    js_value_str_len_def(str_len_right, right);
    size_t length = str_len_left + str_len_right;
    js_char buffer[length + 1];
    strncpy(buffer, str_left, str_len_left);
    strncpy(buffer + str_len_left, str_right, str_len_right);
    buffer[length] = 0;
    js_hash hash = 1;
    int n; for (n = 0 ; n < length ; n++) { hash += js_str_hash_prime * buffer[n]; }
    js_value_str_new_def(context, value, buffer, length, hash);
    js_value_release(left);
    js_value_release(right);
    js_context_push(context, value);
}

const js_str js_value_str_ansi(struct js_value_t* self) {
    switch (self->type) {
        case JS_VALUE_NULL: return "null";
        case JS_VALUE_FUNC: return "function";
        case JS_VALUE_CLASS: return "class";
        case JS_VALUE_OBJ: return "<object>";
        case JS_VALUE_BOOL: return js_value_bool_value(self) ? "true" : "false";
        case JS_VALUE_STR: return ((struct js_value_str_t*)self)->value;
        case JS_VALUE_INT: {
            struct js_value_int_t* int_value = (struct js_value_int_t*) self;
            if (!int_value->str_value) {
                char chars[32];
                sprintf(chars, "%ld", int_value->value);
                int_value->str_value = strdup(chars);
            }
            return int_value->str_value;
        }
        case JS_VALUE_NUM: {
            struct js_value_num_t* num_value = (struct js_value_num_t*) self;
            if (!num_value->str_value) {
                char chars[64];
                sprintf(chars, "%.6g", ((struct js_value_num_t*)self)->value);
                num_value->str_value = strdup(chars);
            }
            return num_value->str_value;
        }
        default: return "<unknown>";
    }
}

js_size js_value_str_len(struct js_value_t* self) {
    switch (self->type) {
        case JS_VALUE_NULL: return 4;
        case JS_VALUE_FUNC: return 8;
        case JS_VALUE_CLASS: return 5;
        case JS_VALUE_OBJ: return 8;
        case JS_VALUE_BOOL: return js_value_bool_value(self) ? 4 : 5;
        case JS_VALUE_STR: return ((struct js_value_str_t*)self)->length;
        case JS_VALUE_INT: {
            struct js_value_int_t* int_value = (struct js_value_int_t*) self;
            if (!int_value->str_size_value) {
                int_value->str_size_value = strlen(js_value_str_ansi(self));
            }
            return int_value->str_size_value;
        }
        case JS_VALUE_NUM: {
            struct js_value_num_t* num_value = (struct js_value_num_t*) self;
            if (!num_value->str_size_value) {
                num_value->str_size_value = strlen(js_value_str_ansi(self));
            }
            return num_value->str_size_value;
        }
        default: return 9;
    }
}
