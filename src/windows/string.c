#include <stdlib.h>
#include <string.h>

char* strndup (const char *s, size_t size) {
    //char *end = memchr(s, 0, size);
    //if (end) size = end - s + 1;
    char* r = malloc(size + 1);
    if (size) {
        memcpy(r, s, size);
        r[size] = '\0';
    }
    return r;
}