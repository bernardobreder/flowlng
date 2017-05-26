#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "js.h"

#define ENTRY_EQUAL(A,B) (A)->hash == (B)->hash && (A)->length == (B)->length && (A)->word[0] == (B)->word[0] && !strcmp((A)->word, (B)->word)

void js_value_obj_new(struct js_context_t* context) {
    struct js_value_obj_t* self = flow_memory_alloc_typed(context->memory, struct js_value_obj_t);
    self->ref_counter = 0;
    self->type = JS_VALUE_OBJ;
    self->next = 0;
    self->parent = 0;
    self->field = 0;
    js_context_push_typed(context, self);
}

void js_value_obj_free(struct js_value_obj_t* self) {
    if (self->parent) js_value_release(self->parent);
    struct js_value_obj_entry_t* field = self->field;
    while (field) {
        free(field->word);
        js_value_release(field->value);
        struct js_value_obj_entry_t* next = field->next;
        flow_memory_item_free(field);
        field = next;
    }
    if (self->next) {
        js_value_release(self->next);
    }
    flow_memory_item_free(self);
}

void js_value_obj_get(struct js_context_t* context, struct js_value_obj_t* self, struct js_node_id_t* name) {
    struct js_value_obj_t* obj = self;
    while (obj) {
        struct js_value_obj_entry_t* entry = obj->field;
        struct js_value_obj_entry_t* prev = 0;
        while (entry) {
            if (ENTRY_EQUAL(entry, name)) {
                if (entry != obj->field) {
                    prev->next = entry->next;
                    entry->next = obj->field;
                    obj->field = entry;
                }
                js_context_push(context, entry->value);
                return;
            }
            prev = entry;
            entry = entry->next;
        }
        obj = obj->parent;
    }
    js_value_null_new(context);
}

void js_value_obj_set(struct js_context_t* context, struct js_value_obj_t* self, struct js_node_id_t* name, struct js_value_t* value) {
    struct js_value_obj_entry_t* entry = self->field;
    struct js_value_obj_entry_t* prev = 0;
    while (entry) {
        if (ENTRY_EQUAL(entry, name)) {
            if (entry != self->field) {
                prev->next = entry->next;
                entry->next = self->field;
                self->field = entry;
            }
            js_value_release(entry->value);
            js_value_retain(value);
            entry->value = value;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
    entry = flow_memory_alloc_typed(context->memory, struct js_value_obj_entry_t);
    entry->word = strdup(name->word);
    entry->length = name->length;
    entry->hash = name->hash;
    entry->value = value;
    entry->next = self->field;
    self->field = entry;
    js_value_retain(value);
}
