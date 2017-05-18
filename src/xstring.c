
#include <stdint.h>
#include <stddef.h>
#include "platform.h"
#include "xstring.h"

js_hash js_hash_perform(char* text, size_t length) {
    js_hash hash = 1;
    int n; for (n = 0 ; n < length ; n++) {
        hash += 13 * text[n];
    }
    return hash;
}
