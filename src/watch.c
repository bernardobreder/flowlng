#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "watch.h"
#include "file.h"
#include "md5.h"

static struct watch_node_t* watch_node_new(char* path, size_t length, char* md5, struct watch_node_t* next) {
    struct watch_node_t* self = (struct watch_node_t*) malloc(sizeof(struct watch_node_t));
    self->path = path;
    self->length = length;
    self->md5 = md5;
    self->next = next;
    return self;
}

void watch_node_free(struct watch_node_t* self, unsigned char deep) {
    free(self->path);
    free(self->md5);
    if (deep && self->next) watch_node_free(self->next, deep);
    free(self);
}

struct watch_t* watch_new(char* path) {
    struct watch_t* self = (struct watch_t*) malloc(sizeof(struct watch_t));
    if (!self) return 0;
    
    struct watch_node_t* wnode = 0;
    struct flow_resource_node_t* rnode = flow_file_list_deep(path);
    while (rnode) {
        if (rnode->is_file) {
            char* path = strdup(rnode->path);
            size_t length = flow_file_length(path);
            unsigned char* bytes = flow_io_file(path);
            char* md5 = flow_md5(bytes, length);
            wnode = watch_node_new(path, length, md5, wnode);
            printf("%s %ld %s\n", wnode->path, wnode->length, wnode->md5);
        } 
        struct flow_resource_node_t* next = rnode->next;
        flow_resource_node_free(rnode, 0);
        rnode = next;
    }
    self->node = wnode;

    return self;
}

void watch_free(struct watch_t* self) {
    struct watch_node_t* node = self->node;
    while (node) {
        struct watch_node_t* next = node->next;
        free(node);
        node = next;
    }
    free(self);
}

struct watch_node_t* watch_changed(struct watch_t* self) {
    struct watch_node_t* prev = 0;
    struct watch_node_t* node = self->node;
    while (node) {
        struct watch_node_t* next = node->next;
        free(node);
        node = next;
    }
    return prev;
}