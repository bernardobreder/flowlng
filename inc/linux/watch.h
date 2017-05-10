
#include <time.h> 

struct watch_t {
    char* path;
    struct watch_node_t* node;
};

struct watch_node_t {
    char* parent;
    int parent_hash;
    char* name;
    int name_hash;
    char* path;
    int path_hash;
    time_t modified;
    unsigned char mark;
};

struct watch_t* watch_new(char* path);

void watch_free(struct watch_t* self);

struct watch_node_t* watch_changed(struct watch_t* self);

void watch_node_free(struct watch_node_t* self, unsigned char deep);
