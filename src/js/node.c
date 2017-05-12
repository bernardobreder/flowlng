#include "js.h"

struct js_node_id_t* js_node_id_new(char* word) {
    struct js_node_id_t* self = (struct js_node_id_t*) malloc(sizeof(struct js_node_id_t));
    self->type = JS_NODE_ID;
    self->next = 0;
    self->word = word;
    return self;
}

struct js_node_error_t* js_node_error_new(char* message, char* word, int line_begin, int column_begin, int line_end, int column_end) {
    struct js_node_error_t* self = (struct js_node_error_t*) malloc(sizeof(struct js_node_t));
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
    switch(self->type) {
        case JS_NODE_ID:
            free(((struct js_node_id_t*)self)->word); 
            break;
        case JS_NODE_ERROR:
            free(((struct js_node_error_t*)self)->message); 
            free(((struct js_node_error_t*)self)->word); 
            break;
    }
    free(self);
}

void js_nodes_free(struct js_node_t* self) {
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