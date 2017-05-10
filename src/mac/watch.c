#include <CoreServices/CoreServices.h>
#include "watch.h"

struct watch_t* watch_new() {
    struct watch_t* self = (struct watch_t*) malloc(sizeof(struct watch_t));
    self->dirs = 0;
    return self;
}

void watch_dir(struct watch_t* self, const char* path) {
    self->dir_count++;
    struct watch_dir_t* dir = (struct watch_dir_t*) malloc(sizeof(struct watch_dir_t));
    dir->next = self->dirs;
    dir->path = strdup(path);
    self->dirs = dir;
}

void watch_free(struct watch_t* self) {
    struct watch_dir_t* dir = self->dirs;
    while (dir) {
        struct watch_dir_t* next = dir->next;
        free(dir);
        dir = next;
    }
    free(self);
}

static void watch_loop_callback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]) {
    struct watch_t* self = clientCallBackInfo;
    int i;
    char** paths = eventPaths;
    for (i = 0 ; i < numEvents ; i++) {
        if (strstr(paths[i], "/.")) continue;
        FSEventStreamEventFlags flag = eventFlags[i];
        if (!(flag & kFSEventStreamEventFlagItemIsFile)) continue;
        char* path = paths[i];
        if (flag & kFSEventStreamEventFlagItemCreated) {
            self->callback(path, FLOW_WATCH_CREATED);
        } else if (flag & kFSEventStreamEventFlagItemModified) {
            self->callback(path, FLOW_WATCH_MODIFIED);
        } else if (flag & kFSEventStreamEventFlagItemRenamed) {
            if (access(path, F_OK) != -1) {
                self->callback(path, FLOW_WATCH_CREATED);
            } else {
                self->callback(path, FLOW_WATCH_REMOVED);
            }
        } else if (flag & kFSEventStreamEventFlagItemRemoved) {
            self->callback(path, FLOW_WATCH_REMOVED);
        } 
    }
}

void watch_loop(struct watch_t* self, void (*callback)(char* path, unsigned char type)) {
    self->callback = callback;
    int i;
    CFStringRef dir_array[self->dir_count];
    struct watch_dir_t* dir = self->dirs; i = 0;
    while (dir) {
        dir_array[i++] = CFStringCreateWithCString(NULL, dir->path, kCFStringEncodingUTF8);
        dir = dir->next;
    }
    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (void*)dir_array, self->dir_count, &kCFTypeArrayCallBacks);
    
    FSEventStreamContext* context = (FSEventStreamContext*) malloc(sizeof(FSEventStreamContext));
    context->version = 0;
    context->info = self;
    context->retain = NULL;
    context->release = NULL;
    context->copyDescription = NULL;
    
    CFAbsoluteTime latency = 1.0;
    
    FSEventStreamRef stream = FSEventStreamCreate(NULL, watch_loop_callback, context, pathsToWatch, kFSEventStreamEventIdSinceNow, latency, kFSEventStreamCreateFlagFileEvents);
    if (!stream) return;

    CFRunLoopRef run_loop = CFRunLoopGetCurrent();
    FSEventStreamScheduleWithRunLoop(stream, run_loop, kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);
    CFRunLoopRun();
}