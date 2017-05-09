
void watch();

struct watch_t {
    struct watch_node_t* node;
};

struct watch_node_t {
    char* path;
    size_t length;
    char* md5;
    struct watch_node_t* next;
};

struct watch_t* watch_new(char* path);

void watch_free(struct watch_t* self);

struct watch_node_t* watch_changed(struct watch_t* self);

void watch_node_free(struct watch_node_t* self, unsigned char deep);