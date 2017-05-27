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
    self->memory = memory;
    return self;
}

void js_compiler_free(struct js_compiler_t* self) {
    self->memory = 0;
    flow_memory_item_free(self);
}

struct js_node_t* js_compiler_exec(struct js_compiler_t* self, char * source) {
    struct js_token_t* tokens = js_lexer(self->memory, source);
    struct js_parser_t* parser = js_parser_new(self->memory, tokens);
    struct js_node_t* node = js_parser(parser);
    js_parser_free(parser);
    js_tokens_free(tokens);
    if (!js_nodes_error_is(node)) {
        js_node_head(node, self);
        js_node_body(node, self);
    }
    return node;
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
