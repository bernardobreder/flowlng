#include <assert.h>
#include <string.h>
#include "js.h"

void test_js_lexer() {
    {
        struct js_token_t* tokens = js_lexer("");
        assert(tokens == 0);
    }
    {
        struct js_token_t* tokens = js_lexer("a");
        assert(!strcmp(tokens->word, "a"));
        assert(tokens->type == JS_TOKEN_TYPE_ID);
        js_tokens_free(tokens);
    }
    {
        struct js_token_t* tokens = js_lexer("a b");
        struct js_token_t* token = tokens;
        
        assert(!strcmp(token->word, "a"));
        assert(token->type == JS_TOKEN_TYPE_ID);
        token = token->next;

        assert(!strcmp(token->word, "b"));
        assert(token->type == JS_TOKEN_TYPE_ID);
        token = token->next;
        
        js_tokens_free(tokens);
    }
    {
        struct js_token_t* tokens = js_lexer("a b c");
        struct js_token_t* token = tokens;
        
        assert(!strcmp(token->word, "a"));
        assert(token->type == JS_TOKEN_TYPE_ID);
        token = token->next;

        assert(!strcmp(token->word, "b"));
        assert(token->type == JS_TOKEN_TYPE_ID);
        token = token->next;
        
        assert(!strcmp(token->word, "c"));
        assert(token->type == JS_TOKEN_TYPE_ID);
        token = token->next;
        
        js_tokens_free(tokens);
    }
    {
        struct js_token_t* tokens = js_lexer("for 1 -");
        struct js_token_t* token = tokens;
        
        assert(!token->word);
        assert(token->type == JS_TOKEN_TYPE_FOR_WORD);
        token = token->next;

        assert(!strcmp(token->word, "1"));
        assert(token->type == JS_TOKEN_TYPE_NUMBER);
        token = token->next;
        
        assert(!token->word);
        assert(token->type == '-');
        token = token->next;
        
        assert(token == 0);
        js_tokens_free(tokens);
    }
    {
        struct js_token_t* tokens = js_lexer("for(a==2){\n}");
        struct js_token_t* token = tokens;
        
        assert(!token->word);
        assert(token->type == JS_TOKEN_TYPE_FOR_WORD);
        token = token->next;
        
        assert(!token->word);
        assert(token->type == '(');
        token = token->next;

        assert(!strcmp(token->word, "a"));
        assert(token->type == JS_TOKEN_TYPE_ID);
        token = token->next;

        assert(!token->word);
        assert(token->type == '=');
        token = token->next;

        assert(!token->word);
        assert(token->type == '=');
        token = token->next;

        assert(!strcmp(token->word, "2"));
        assert(token->type == JS_TOKEN_TYPE_NUMBER);
        token = token->next;

        assert(!token->word);
        assert(token->type == ')');
        token = token->next;

        assert(!token->word);
        assert(token->type == '{');
        token = token->next;

        assert(!token->word);
        assert(token->type == '}');
        token = token->next;

        assert(token == 0);
        js_tokens_free(tokens);
    }
}
