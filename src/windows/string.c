#include <stdlib.h>
#include <string.h>

char* strndup (const char *s, size_t size) {
    char *end = memchr(s, 0, size);
    if (end) size = end - s + 1;
    char* r = malloc(size);
    if (size) {
        memcpy(r, s, size-1);
        r[size-1] = '\0';
    }
    return r;
}