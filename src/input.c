
#include <stdio.h>
#include "input.h"

void flow_input_perform(char* path, flow_watch_event_t type) {
    printf("%s: %d\n", path, type);
}