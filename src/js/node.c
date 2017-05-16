#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "js.h"

void js_node_free(struct js_node_t* self) {
    if (!self) return;
    switch(self->type) {
        case JS_NODE_ID: {
            return js_node_id_free((struct js_node_id_t*) self);
        }
        case JS_NODE_STRING: {
            struct js_node_string_t* node = (struct js_node_string_t*) self;
            free(node->value);
            break;
        }
        case JS_NODE_ERROR: {
            return js_node_error_free((struct js_node_error_t*) self);
        }
        case JS_NODE_FUNCTION: {
            return js_node_function_free((struct js_node_function_t*) self);
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
        case JS_NODE_VAR_ITEM: {
            return js_node_var_item_free((struct js_node_var_item_t*) self);
        }
    }
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_head(struct js_node_t* self) {
    if (!self) return;
    switch(self->type) {
//        case JS_NODE_FUNCTION: {
//            return js_node_function_head((struct js_node_function_t*) self);
//        }
        case JS_NODE_IF: {
            return js_node_if_head((struct js_node_if_t*) self);
        }
//        case JS_NODE_WHILE: {
//            return js_node_while_head((struct js_node_while_t*) self);
//        }
//        case JS_NODE_VAR_ITEM: {
//            return js_node_var_item_head((struct js_node_var_item_t*) self);
//        }
    }
}

void js_node_body(struct js_node_t* self) {
    if (!self) return;
    switch(self->type) {
//        case JS_NODE_FUNCTION: {
//            return js_node_function_body((struct js_node_function_t*) self);
//        }
        case JS_NODE_IF: {
            return js_node_if_body((struct js_node_if_t*) self);
        }
//        case JS_NODE_WHILE: {
//            return js_node_while_body((struct js_node_while_t*) self);
//        }
//        case JS_NODE_VAR_ITEM: {
//            return js_node_var_item_body((struct js_node_var_item_t*) self);
//        }
    }
}

struct js_node_error_t* js_node_error_new(char* message, char* word, size_t line, size_t column, struct js_node_error_t* next) {
    struct js_node_error_t* self = (struct js_node_error_t*) malloc(sizeof(struct js_node_error_t));
    self->type = JS_NODE_ERROR;
    self->next = next;
    self->message = message;
    self->word = word;
    self->line = line;
    self->column = column;
    return self;
}

void js_node_error_free(struct js_node_error_t* self) {
    free(self->message);
    free(self->word);
    if (self->next) js_node_error_free(self->next);
    free(self);
}

struct js_node_error_t* js_node_error_revert(struct js_node_error_t* self) {
    struct js_node_error_t* node = self;
    struct js_node_error_t* aux = self->next;
    node->next = 0;
    while (aux) {
        struct js_node_error_t* next = aux->next;
        aux->next = node;
        node = aux;
        aux = next;
    }
    return node;
}

void js_node_error_print(struct js_node_error_t* self) {
    struct js_node_error_t* node = self;
    while (node) {
        if (node->word) {
            printf("{state: \"%s\", word: \"%s\", line: %ld, column: %ld}\n", node->message, node->word, node->line, node->column);
        }
        node = node->next;
    }
    printf("\n");
}

struct js_node_function_t* js_node_function_new(struct js_node_id_t* name, struct js_node_t* params, struct js_node_t* statement) {
    struct js_node_function_t* self = (struct js_node_function_t*) malloc(sizeof(struct js_node_function_t));
    self->type = JS_NODE_FUNCTION;
    self->next = 0;
    self->name = name;
    self->params = params;
    self->statement = statement;
    return self;
}

void js_node_function_free(struct js_node_function_t* self) {
    js_node_id_free(self->name);
    js_node_free(self->params);
    js_node_free(self->statement);
    if (self->next) js_node_free(self->next);
    free(self);
}

struct js_node_var_item_t* js_node_var_item_new(struct js_node_id_t* name, struct js_node_t* value) {
    struct js_node_var_item_t* self = (struct js_node_var_item_t*) malloc(sizeof(struct js_node_var_item_t));
    self->type = JS_NODE_VAR_ITEM;
    self->next = 0;
    self->name = name;
    self->value = value;
    return self;
}

void js_node_var_item_free(struct js_node_var_item_t* self) {
    js_node_id_free(self->name);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

struct js_node_empty_t* js_node_empty_new() {
    struct js_node_empty_t* self = (struct js_node_empty_t*) malloc(sizeof(struct js_node_empty_t));
    self->type = JS_NODE_EMPTY;
    self->next = 0;
    return self;
}

void js_node_empty_body(struct js_node_empty_t* self) {
}

void js_node_empty_exec(struct js_node_empty_t* self) {
}

struct js_node_break_t* js_node_break_new() {
    struct js_node_break_t* self = (struct js_node_break_t*) malloc(sizeof(struct js_node_break_t));
    self->type = JS_NODE_BREAK;
    self->next = 0;
    return self;
}

void js_node_break_body(struct js_node_break_t* self) {
}

void js_node_break_exec(struct js_node_break_t* self) {
}

struct js_node_continue_t* js_node_continue_new() {
    struct js_node_continue_t* self = (struct js_node_continue_t*) malloc(sizeof(struct js_node_continue_t));
    self->type = JS_NODE_CONTINUE;
    self->next = 0;
    return self;
}

void js_node_continue_body(struct js_node_continue_t* self) {
}

void js_node_continue_exec(struct js_node_continue_t* self) {
}

struct js_node_if_t* js_node_if_new(struct js_node_t* expression, struct js_node_t* statement, struct js_node_t* else_statement) {
    struct js_node_if_t* self = (struct js_node_if_t*) malloc(sizeof(struct js_node_if_t));
    self->type = JS_NODE_IF;
    self->next = 0;
    self->expression = expression;
    self->statement = statement;
    self->else_statement = else_statement;
    return self;
}

void js_node_if_head(struct js_node_if_t* self) {
    js_node_head(self->expression);
    js_node_head(self->statement);
    js_node_head(self->else_statement);
}

void js_node_if_body(struct js_node_if_t* self) {
    js_node_body(self->expression);
    js_node_body(self->statement);
    js_node_body(self->else_statement);
}

void js_node_if_exec(struct js_node_if_t* self) {
}

struct js_node_while_t* js_node_while_new(struct js_node_t* expression, struct js_node_t* statement) {
    struct js_node_while_t* self = (struct js_node_while_t*) malloc(sizeof(struct js_node_while_t));
    self->type = JS_NODE_WHILE;
    self->next = 0;
    self->expression = expression;
    self->statement = statement;
    return self;
}

struct js_node_return_t* js_node_return_new(struct js_node_t* expression) {
    struct js_node_return_t* self = (struct js_node_return_t*) malloc(sizeof(struct js_node_return_t));
    self->type = JS_NODE_RETURN;
    self->next = 0;
    self->expression = expression;
    return self;
}

struct js_node_id_t* js_node_id_new(char* word) {
    struct js_node_id_t* self = (struct js_node_id_t*) malloc(sizeof(struct js_node_id_t));
    self->type = JS_NODE_ID;
    self->next = 0;
    self->word = word;
    return self;
}

void js_node_id_free(struct js_node_id_t* self) {
    free(self->word);
    if (self->next) js_node_free(self->next);
    free(self);
}

struct js_node_string_t* js_node_string_new(char* value) {
    struct js_node_string_t* self = (struct js_node_string_t*) malloc(sizeof(struct js_node_string_t));
    self->type = JS_NODE_STRING;
    self->next = 0;
    self->value = value;
    return self;
}

struct js_node_num_t* js_node_num_new(double value) {
    struct js_node_num_t* self = (struct js_node_num_t*) malloc(sizeof(struct js_node_num_t));
    self->type = JS_NODE_NUM;
    self->next = 0;
    self->value = value;
    return self;
}

struct js_node_int_t* js_node_int_new(int value) {
    struct js_node_int_t* self = (struct js_node_int_t*) malloc(sizeof(struct js_node_int_t));
    self->type = JS_NODE_INT;
    self->next = 0;
    self->value = value;
    return self;
}

struct js_node_true_t* js_node_true_new() {
    struct js_node_true_t* self = (struct js_node_true_t*) malloc(sizeof(struct js_node_true_t));
    self->type = JS_NODE_TRUE;
    self->next = 0;
    return self;
}

struct js_node_false_t* js_node_false_new() {
    struct js_node_false_t* self = (struct js_node_false_t*) malloc(sizeof(struct js_node_false_t));
    self->type = JS_NODE_FALSE;
    self->next = 0;
    return self;
}

struct js_node_null_t* js_node_null_new() {
    struct js_node_null_t* self = (struct js_node_null_t*) malloc(sizeof(struct js_node_null_t));
    self->type = JS_NODE_NULL;
    self->next = 0;
    return self;
}

struct js_node_this_t* js_node_this_new() {
    struct js_node_this_t* self = (struct js_node_this_t*) malloc(sizeof(struct js_node_this_t));
    self->type = JS_NODE_THIS;
    self->next = 0;
    return self;
}

struct js_node_super_t* js_node_super_new() {
    struct js_node_super_t* self = (struct js_node_super_t*) malloc(sizeof(struct js_node_super_t));
    self->type = JS_NODE_SUPER;
    self->next = 0;
    return self;
}
