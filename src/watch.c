#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "watch.h"
#include "file.h"
#include "md5.h"
#include "sort.h"

int flow_resource_node_cmp(struct flow_resource_node_t* left, struct flow_resource_node_t* right) {
    return strcmp(left->path, right->path);
}

int flow_resource_node_cmp_inv(struct flow_resource_node_t* left, struct flow_resource_node_t* right) {
    return strcmp(right->path, left->path);
}

struct flow_resource_node_t* flow_resource_node_next(struct flow_resource_node_t* node) {
    return node->next;
}

void flow_resource_node_setnext(struct flow_resource_node_t* node, struct flow_resource_node_t* value) {
    node->next = value;
}

static struct watch_node_t* watch_node_new(char* path, char* name, size_t length, time_t modifed, char* md5, struct watch_node_t* next) {
    struct watch_node_t* self = (struct watch_node_t*) malloc(sizeof(struct watch_node_t));
    self->path = path;
    self->name = name;
    self->length = length;
    self->modifed = modifed;
    self->md5 = md5;
    self->next = next;
    return self;
}

void watch_node_free(struct watch_node_t* self, unsigned char deep) {
    free(self->path);
    free(self->name);
    free(self->md5);
    if (deep && self->next) watch_node_free(self->next, deep);
    free(self);
}

struct watch_t* watch_new(char* path) {
    struct watch_t* self = (struct watch_t*) malloc(sizeof(struct watch_t));
    if (!self) return 0;
    self->path = strdup(path);
    struct watch_node_t* wnode = 0;
    struct flow_resource_node_t* rnode = flow_file_list_deep(path);
    flow_linked_list_sort(&rnode, flow_resource_node_cmp_inv, flow_resource_node_next, flow_resource_node_setnext);
    while (rnode) {
        if (rnode->is_file) {
            char* path = strdup(rnode->path);
            // printf("%s\n", path);
            char* name = strdup(rnode->name);
            time_t modifed = flow_file_modif(path);
            // size_t length = flow_file_length(path);
            size_t length = 0;
            // unsigned char* bytes = flow_io_file(path);
            // char* md5 = flow_md5(bytes, length);
            // free(bytes);
            char* md5 = "Xxx";
            wnode = watch_node_new(path, name, length, modifed, md5, wnode);
            // printf("%s %ld %s\n", path, length, md5);
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
    free(self->path);
    while (node) {
        struct watch_node_t* next = node->next;
        free(node);
        node = next;
    }
    free(self);
}

struct watch_node_t* watch_changed(struct watch_t* self) {
    struct flow_resource_node_t* rnode = flow_file_list_deep(self->path);
    flow_linked_list_sort(&rnode, flow_resource_node_cmp, flow_resource_node_next, flow_resource_node_setnext);

    int count = 0;
    struct watch_node_t* prev = 0;
    struct watch_node_t* wit = self->node;
    struct flow_resource_node_t* rit = rnode;
    while (wit && rit) {
        while (rit && !rit->is_file) {
            struct flow_resource_node_t* rnext = rit->next;
            flow_resource_node_free(rit, 0);
            rit = rnext;
        }
        if (!rit) break;

        // printf("%s %s\n", wit->path, rit->path);
        if (!strcmp(wit->path, rit->path)) {
            // printf("%s\n", wit->path);
            time_t modifed = flow_file_modif(rit->path);
            if (wit->modifed != modifed) {
                wit->modifed = modifed;
                printf("changed\n");
            }
        }
        
        struct watch_node_t* wnext = wit->next;
        struct flow_resource_node_t* rnext = rit->next;
        free(rit);
        wit = wnext;
        rit = rnext;
        // count++;
    }

    // printf("Count: %d\n", count);
    // flow_resource_node_free(rnode, 1);
    return prev;
}
