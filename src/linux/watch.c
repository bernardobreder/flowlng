#include <CoreServices/CoreServices.h>
#include "linux/watch.h"

int flow_watch_node_cmp(const void* pleft, const void* pright) {
    const struct watch_node_t* left = (struct watch_node_t*) pleft;
    const struct watch_node_t* right = (struct watch_node_t*) pright;
    if (left->parent_hash != right->parent_hash) {
        return left->parent_hash - right->parent_hash;
    }
    if (left->name_hash != right->name_hash) {
        return left->name_hash - right->name_hash;
    }
    if (left->path_hash != right->path_hash) {
        return left->path_hash - right->path_hash;
    }
    int cmp = strcmp(left->parent, right->parent);
    if (cmp) return cmp;
    return strcmp(left->name, right->name);
}

static int flow_watch_node_parent_hash(char* parent, size_t parentLength) {
    return (int) parentLength + 3 * parent[parentLength - 1] + 5 * parent[parentLength - 2];
}

static int flow_watch_node_name_hash(char* name, size_t nameLength) {
    return (int) nameLength + 3 * name[0];
}

static int flow_watch_node_path_hash(char* parent, size_t parentLength, int parent_hash, char* name, size_t nameLength, int name_hash) {
    return (int) parent_hash + 3 * parent_hash;
}

static struct watch_node_t* watch_node_new(char* parent, size_t parent_length, char* name, size_t name_length, char* path, time_t modified) {
    struct watch_node_t* self = (struct watch_node_t*) malloc(sizeof(struct watch_node_t));
    self->parent = parent;
    self->parent_hash = flow_watch_node_parent_hash(parent, parent_length);
    self->name = name;
    self->name_hash = flow_watch_node_name_hash(name, name_length);
    self->path = path;
    self->path_hash = flow_watch_node_path_hash(parent, parent_length, self->parent_hash, name, name_length, self->name_hash);
    self->modified = modified;
    self->mark = 0;
    return self;
}

void watch_node_free(struct watch_node_t* self, unsigned char deep) {
    free(self->parent);
    free(self->name);
    free(self->path);
    free(self);
}

static unsigned char flow_watch_accept(struct dirent* entry) {
    if (entry->d_name[0] == '.') return 0;
    if (entry->d_type != DT_DIR && entry->d_type != DT_REG) return 0;
    return 1;
}

static void flow_watch_list(struct watch_t* self, char* parent, size_t parentLength) {
    DIR *dir;
    struct dirent* entry;
    
    if (!(dir = opendir(parent))) return;
    if (!(entry = readdir(dir))) return;
    
    do {
        if (!flow_watch_accept(entry)) continue;
        
        size_t pathLength = parentLength + 1 + entry->d_namlen;
        char* path = (char*) alloca(pathLength + 1);
        strcpy(path, parent);
        path[parentLength] = '/';
        strcpy(path + parentLength + 1, entry->d_name);
        path[pathLength] = 0;
        
        switch (entry->d_type) {
            case DT_DIR: {
                flow_watch_list(self, path, pathLength);
                break;
            }
            case DT_REG: {
                tsearch(watch_node_new(strdup(parent), parentLength, strdup(entry->d_name), entry->d_namlen, strdup(path), flow_file_modif(path)), (void**) &self->node, flow_watch_node_cmp);
                break;
            }
        }
    } while ((entry = readdir(dir)));
    
    closedir(dir);
}

struct watch_t* watch_new(char* path) {
    struct watch_t* self = (struct watch_t*) malloc(sizeof(struct watch_t));
    if (!self) return 0;
    self->path = strdup(path);
    self->node = 0;
    flow_watch_list(self, path, strlen(path));
    return self;
}

void watch_free(struct watch_t* self) {
    free(self->path);
//    tdestroy(self->node, watch_node_free);
    free(self);
}

static void flow_watch_check(struct watch_t* self, char* parent, size_t parentLength) {
    DIR *dir;
    struct dirent* entry;
    
    if (!(dir = opendir(parent))) return;
    if (!(entry = readdir(dir))) return;
    
    do {
        if (!flow_watch_accept(entry)) continue;
        
        size_t pathLength = parentLength + 1 + entry->d_namlen;
        char* path = (char*) alloca(pathLength + 1);
        strcpy(path, parent);
        path[parentLength] = '/';
        strcpy(path + parentLength + 1, entry->d_name);
        path[pathLength] = 0;
        
        switch (entry->d_type) {
            case DT_DIR: {
                flow_watch_check(self, path, pathLength);
                break;
            }
            case DT_REG: {
                struct watch_node_t* wnode = (struct watch_node_t*) alloca(sizeof(struct watch_node_t));
                wnode->parent = parent;
                wnode->parent_hash = flow_watch_node_parent_hash(parent, parentLength);
                wnode->name = entry->d_name;
                wnode->name_hash = flow_watch_node_name_hash(entry->d_name, entry->d_namlen);
                wnode->path = path;
                wnode->path_hash = flow_watch_node_path_hash(parent, parentLength, wnode->parent_hash, entry->d_name, entry->d_namlen, wnode->name_hash);
                wnode->modified = flow_file_modif(path);
                
                struct watch_node_t** pfnode = (struct watch_node_t**) tfind(wnode, (void**) &self->node, flow_watch_node_cmp);
                if (pfnode) {
                    struct watch_node_t* fnode = *pfnode;
                    fnode->mark = 1;
                    if (fnode->modified != wnode->modified) {
                        fnode->modified = wnode->modified;
                        printf("changed file: %s\n", path);
                    }
                } else {
                    printf("new file: %s\n", path);
                    tsearch(watch_node_new(strdup(parent), parentLength, strdup(entry->d_name), entry->d_namlen, strdup(path), wnode->modified), (void**) &self->node, flow_watch_node_cmp);
                }
                break;
            }
        }
    } while ((entry = readdir(dir)));
    
    closedir(dir);
}

struct watch_node_t* watch_changed(struct watch_t* self) {
    return 0;
}