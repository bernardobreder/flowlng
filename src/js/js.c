#include <stdlib.h>
#include "js.h"

void js_token_free(struct js_token_t* self) {
    if (self->word) free(self->word);
    free(self);
}

void js_tokens_free(struct js_token_t* self) {
    struct js_token_t* aux = self;
    while (aux) {
        struct js_token_t* next = aux->next;
        js_token_free(aux);
        aux = next;
    }
}
