#include <assert.h>
#include <string.h>
#include "js.h"

void test_js_parser() {
    struct js_token_t* tokens = js_lexer("a b");
    struct js_node_t* node = js_parser(tokens);
    js_tokens_free(tokens);
}