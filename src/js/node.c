#include <stdlib.h>
#include "js.h"

struct js_node_empty_t* js_node_empty_new() {
    struct js_node_empty_t* self = (struct js_node_empty_t*) malloc(sizeof(struct js_node_empty_t));
    self->type = JS_NODE_EMPTY;
    self->next = 0;
    return self;
}

struct js_node_break_t* js_node_break_new() {
    struct js_node_break_t* self = (struct js_node_break_t*) malloc(sizeof(struct js_node_break_t));
    self->type = JS_NODE_BREAK;
    self->next = 0;
    return self;
}

struct js_node_continue_t* js_node_continue_new() {
    struct js_node_continue_t* self = (struct js_node_continue_t*) malloc(sizeof(struct js_node_continue_t));
    self->type = JS_NODE_CONTINUE;
    self->next = 0;
    return self;
}

struct js_node_function_t* js_node_function_new(struct js_node_t* name, struct js_node_t* params, struct js_node_t* statements) {
    struct js_node_function_t* self = (struct js_node_function_t*) malloc(sizeof(struct js_node_function_t));
    self->type = JS_NODE_FUNCTION;
    self->next = 0;
    self->name = name;
    self->params = params;
    self->statements = statements;
    return self;
}

struct js_node_if_t* js_node_if_new(struct js_node_t* expression, struct js_node_t* statement) {
    struct js_node_if_t* self = (struct js_node_if_t*) malloc(sizeof(struct js_node_if_t));
    self->type = JS_NODE_IF;
    self->next = 0;
    self->expression = expression;
    self->statement = statement;
    return self;
}

struct js_node_while_t* js_node_while_new(struct js_node_t* expression, struct js_node_t* statement) {
    struct js_node_while_t* self = (struct js_node_while_t*) malloc(sizeof(struct js_node_while_t));
    self->type = JS_NODE_WHILE;
    self->next = 0;
    self->expression = expression;
    self->statement = statement;
    return self;
}

struct js_node_id_t* js_node_id_new(char* word) {
    struct js_node_id_t* self = (struct js_node_id_t*) malloc(sizeof(struct js_node_id_t));
    self->type = JS_NODE_ID;
    self->next = 0;
    self->word = word;
    return self;
}

struct js_node_error_t* js_node_error_new(char* message, char* word, size_t line_begin, size_t column_begin, size_t line_end, size_t column_end) {
    struct js_node_error_t* self = (struct js_node_error_t*) malloc(sizeof(struct js_node_error_t));
    self->type = JS_NODE_ERROR;
    self->next = 0;
    self->message = message;
    self->word = word;
    self->line_begin = line_begin;
    self->column_begin = column_begin;
    self->line_end = line_end;
    self->column_end = column_end;
    return self;
}

void js_node_free(struct js_node_t* self) {
    if (!self) return;
    switch(self->type) {
        case JS_NODE_ID: {
            struct js_node_id_t* node = (struct js_node_id_t*) self;
            free(node->word);
            break;
        }
        case JS_NODE_ERROR: {
            struct js_node_error_t* node = (struct js_node_error_t*) self;
            free(node->message);
            free(node->word);
            break;
        }
        case JS_NODE_FUNCTION: {
            struct js_node_function_t* node = (struct js_node_function_t*) self;
            js_node_free(node->name);
            js_nodes_free(node->params);
            js_nodes_free(node->statements);
            break;
        }
        case JS_NODE_IF: {
            struct js_node_if_t* node = (struct js_node_if_t*) self;
            js_node_free(node->expression);
            js_node_free(node->statement);
            break;
        }
        case JS_NODE_WHILE: {
            struct js_node_while_t* node = (struct js_node_while_t*) self;
            js_node_free(node->expression);
            js_node_free(node->statement);
            break;
        }
    }
    free(self);
}

void js_nodes_free(struct js_node_t* self) {
    if (!self) return;
    struct js_node_t* aux = self;
    while (aux) {
        struct js_node_t* next = aux->next;
        js_node_free(aux);
        aux = next;
    }
}

unsigned char js_node_has_error(struct js_node_t* self) {
    return self->type == JS_NODE_ERROR;
}
