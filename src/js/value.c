#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "memory.h"
#include "js.h"

void js_value_free(struct js_value_t* self) {
    if (!self) return;
    switch (self->type) {
        case JS_VALUE_NULL:
        case JS_VALUE_BOOL:
            return;
        case JS_VALUE_STR: return js_value_str_free((struct js_value_str_t*) self);
        case JS_VALUE_OBJ: return js_value_obj_free((struct js_value_obj_t*) self);
    }
    if (self->next) js_value_free(self->next);
    flow_memory_item_free(self);
}

struct js_value_t* js_value_null() {
    static struct js_value_null_t value = {JS_VALUE_NULL, 0};
    return (struct js_value_t*) &value;
}

struct js_value_t* js_value_true() {
    static struct js_value_bool_t value = {JS_VALUE_BOOL, 0, 1};
    return (struct js_value_t*) &value;
}

struct js_value_t* js_value_false() {
    static struct js_value_bool_t value = {JS_VALUE_BOOL, 0, 0};
    return (struct js_value_t*) &value;
}

struct js_value_t* js_value_int_new(struct js_context_t* context, js_int value) {
    struct js_value_int_t* self = flow_memory_alloc_typed(context->memory, struct js_value_int_t);
    self->type = JS_VALUE_INT;
    self->next = 0;
    self->value = value;
    return (struct js_value_t*) self;
}

struct js_value_t* js_value_num_new(struct js_context_t* context, js_num value) {
    struct js_value_num_t* self = flow_memory_alloc_typed(context->memory, struct js_value_num_t);
    self->type = JS_VALUE_NUM;
    self->next = 0;
    self->value = value;
    return (struct js_value_t*) self;
}

struct js_value_t* js_value_str_new(struct js_context_t* context, char* value, size_t length, uint64 hash) {
    struct js_value_str_t* self = flow_memory_alloc_typed(context->memory, struct js_value_str_t);
    self->type = JS_VALUE_STR;
    self->next = 0;
    self->value = strdup(value);
    self->length = length;
    self->hash = hash;
    return (struct js_value_t*) self;
}

void js_value_str_free(struct js_value_str_t* self) {
    free(self->value);
    if (self->next) js_value_free(self->next);
    flow_memory_item_free(self);
}

struct js_value_t* js_value_class_new(struct js_context_t* context, struct js_value_str_t* class_str) {
    struct js_value_class_t* self = flow_memory_alloc_typed(context->memory, struct js_value_class_t);
    self->type = JS_VALUE_CLASS;
    self->next = 0;
    self->constructor = 0;
    self->desctructor = 0;
    self->field = 0;
    self->function = 0;
    self->to_str_func = 0;
    self->to_int_func = 0;
    self->clone_func = 0;
    self->equal_func = 0;
    self->class_str = class_str;
    return (struct js_value_t*) self;
}

void js_value_class_free(struct js_value_str_t* self) {
    if (self->next) js_value_free(self->next);
    
    flow_memory_item_free(self);
}

struct js_value_t* js_value_func_new(struct js_context_t* context, struct js_node_function_t* function) {
    struct js_value_func_t* self = flow_memory_alloc_typed(context->memory, struct js_value_func_t);
    self->type = JS_VALUE_FUNC;
    self->next = 0;
    self->function = function;
    flow_memory_retain(function);
    return (struct js_value_t*) self;
}

void js_value_func_free(struct js_value_str_t* self) {
    if (self->next) js_value_free(self->next);
    flow_memory_release(self->function);
    flow_memory_item_free(self);
}

struct js_value_obj_t* js_value_obj_new(struct js_context_t* context) {
    struct js_value_obj_t* self = flow_memory_alloc_typed(context->memory, struct js_value_obj_t);
    self->type = JS_VALUE_OBJ;
    self->next = 0;
    self->field = 0;
    return self;
}

void js_value_obj_free(struct js_value_obj_t* self) {
    if (self->next) js_value_obj_free(self->next);
    while (self->field) {
        free(self->field->name);
        js_value_free(self->field->value);
        self->field = self->field->next;
    }
    flow_memory_item_free(self);
}

struct js_value_t* js_value_obj_field_get(struct js_value_obj_t* self, struct js_node_id_t* name) {
    struct js_value_obj_t* obj = self;
    while (obj) {
        struct js_value_obj_entry_t* entry = obj->field;
        while (entry) {
            if (entry->hash == name->hash && entry->length == name->length && !strcmp(entry->name, name->word)) {
                if (entry != obj->field) {
                    entry->next = obj->field;
                    obj->field = entry;
                }
                return entry->value;
            }
            entry = entry->next;
        }
        obj = obj->next;
    }
    return js_value_null();
}

void js_value_obj_field_set(struct js_context_t* context, struct js_value_obj_t* self, struct js_node_id_t* name, struct js_value_t* value) {
    struct js_value_obj_entry_t* entry = self->field;
    while (entry) {
        if (entry->hash == name->hash && entry->length == name->length && !strcmp(entry->name, name->word)) {
            if (entry != self->field) {
                entry->next = self->field;
                self->field = entry;
            }
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    entry = flow_memory_alloc_typed(context->memory, struct js_value_obj_entry_t);
    entry->name = strdup(name->word);
    entry->length = name->length;
    entry->hash = name->hash;
    entry->value = value;
    entry->next = self->field;
    self->field = entry;
}

char* js_value_object_string_ansi(struct js_value_t* self) {
    switch (self->type) {
        case JS_VALUE_NULL: return strdup("null");
        case JS_VALUE_FUNC: return strdup("function");
        case JS_VALUE_CLASS: return strdup("class");
        case JS_VALUE_OBJ: return strdup("<object>");
        case JS_VALUE_BOOL: return strdup(js_value_bool_value(self) ? "true" : "false");
        case JS_VALUE_STR: return strdup(((struct js_value_str_t*)self)->value );
        case JS_VALUE_INT: {
            char chars[32];
            sprintf(chars, "%ld", ((struct js_value_int_t*)self)->value);
            return strdup(chars);
        }
        case JS_VALUE_NUM: {
            char chars[64];
            sprintf(chars, "%.6g", ((struct js_value_num_t*)self)->value);
            return strdup(chars);
        }
        default: return strdup("unknown value type");
    }
}

js_bool js_value_is_equal(struct js_context_t* context, struct js_value_t* left, struct js_value_t* right) {
    if (left->type != right->type) return 0;
    switch (left->type) {
        case JS_VALUE_NULL:
            return 1;
        case JS_VALUE_BOOL: {
            js_value_bool_def(bool_left, left);
            js_value_bool_def(bool_right, right);
            return bool_left == bool_right;
        }
        case JS_VALUE_INT: {
            js_value_int_def(int_left, left);
            js_value_int_def(int_right, right);
            return int_left == int_right;
        }
        case JS_VALUE_NUM: {
            js_value_num_def(num_left, left);
            js_value_num_def(num_right, right);
            js_num delta = num_left - num_right;
            if (delta < 0) delta = -delta;
            return delta < js_num_precision;
        }
        case JS_VALUE_STR: {
            js_value_str_def(str_left, left);
            js_value_str_def(str_right, right);
            return !strcmp(str_left, str_right);
        }
        default: return left == right;
    }
}

js_int js_value_is_compare(struct js_context_t* context, struct js_value_t* left, struct js_value_t* right) {
    if (left->type != right->type) return left->type - right->type;
    switch (left->type) {
        case JS_VALUE_NULL: {
            return 0;
        }
        case JS_VALUE_BOOL: {
            js_value_bool_def(bool_left, left);
            js_value_bool_def(bool_right, right);
            return (bool_left ? 1 : 0) - (bool_right ? 1 : 0);
        }
        case JS_VALUE_INT: {
            js_value_int_def(int_left, left);
            js_value_int_def(int_right, right);
            return int_left - int_right;
        }
        case JS_VALUE_NUM: {
            js_value_num_def(num_left, left);
            js_value_num_def(num_right, right);
            js_num delta = num_left - num_right;
            if (delta < 0) delta = -delta;
            return delta < js_num_precision ? 0 : num_left - num_right;
        }
        case JS_VALUE_STR: {
            js_value_str_def(str_left, left);
            js_value_str_def(str_right, right);
            return strcmp(str_left, str_right);
        }
        default: return (void*)left - (void*)right;
    }
}
