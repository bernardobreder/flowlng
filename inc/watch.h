#ifndef _FLOW_WATCH_H_
#define _FLOW_WATCH_H_

#include <stdint.h>

typedef int flow_watch_event_t;
#define FLOW_WATCH_CREATED 1
#define FLOW_WATCH_REMOVED 2
#define FLOW_WATCH_MODIFIED 3

struct watch_t {
    int dir_count;
    struct watch_dir_t* dirs;
    void (*callback)(char* path, flow_watch_event_t type);
};

struct watch_dir_t {
    char* path;
    struct watch_dir_t* next;
};

struct watch_t* watch_new();

void watch_dir(struct watch_t* self, const char* path);

void watch_free(struct watch_t* self);

void watch_loop(struct watch_t* self, void (*callback)(char* path, flow_watch_event_t type));

#endif
