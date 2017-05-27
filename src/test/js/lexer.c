#include <assert.h>
#include <string.h>
#include "js.h"

void test_js_lexer() {
    {
        struct flow_memory_t* memory = flow_memory_new();
        struct js_token_t* tokens = js_lexer(memory, "");
        struct js_token_t* token = tokens;
        assert(token->type == JS_TOKEN_EOF);
        token = token->next;
        js_tokens_free(tokens);
        flow_memory_free(memory);
    }
    {
        struct flow_memory_t* memory = flow_memory_new();
        struct js_token_t* tokens = js_lexer(memory, "a");
        assert(!strcmp(tokens->word, "a"));
        assert(tokens->type == JS_TOKEN_ID);
        js_tokens_free(tokens);
        flow_memory_free(memory);
    }
    {
        struct flow_memory_t* memory = flow_memory_new();
        struct js_token_t* tokens = js_lexer(memory, "a b");
        struct js_token_t* token = tokens;
        
        assert(!strcmp(token->word, "a"));
        assert(token->type == JS_TOKEN_ID);
        token = token->next;

        assert(!strcmp(token->word, "b"));
        assert(token->type == JS_TOKEN_ID);
        token = token->next;
        
        assert(token->type == JS_TOKEN_EOF);
        token = token->next;
        
        js_tokens_free(tokens);
        flow_memory_free(memory);
    }
    {
        struct flow_memory_t* memory = flow_memory_new();
        struct js_token_t* tokens = js_lexer(memory, "a b c");
        struct js_token_t* token = tokens;
        
        assert(!strcmp(token->word, "a"));
        assert(token->type == JS_TOKEN_ID);
        token = token->next;
        
        assert(!strcmp(token->word, "b"));
        assert(token->type == JS_TOKEN_ID);
        token = token->next;
        
        assert(!strcmp(token->word, "c"));
        assert(token->type == JS_TOKEN_ID);
        token = token->next;
        
        assert(token->type == JS_TOKEN_EOF);
        token = token->next;
        
        js_tokens_free(tokens);
        flow_memory_free(memory);
    }
    {
        struct flow_memory_t* memory = flow_memory_new();
        struct js_token_t* tokens = js_lexer(memory, "1");
        struct js_token_t* token = tokens;
        
        assert(!strcmp(token->word, "1"));
        assert(token->type == JS_TOKEN_NUMBER);
        token = token->next;
        
        assert(token->type == JS_TOKEN_EOF);
        token = token->next;
        
        js_tokens_free(tokens);
        flow_memory_free(memory);
    }
    {
        struct flow_memory_t* memory = flow_memory_new();
        struct js_token_t* tokens = js_lexer(memory, "-");
        struct js_token_t* token = tokens;
        
        assert(!strcmp(token->word, "-"));
        assert(token->type == '-');
        token = token->next;
        
        assert(token->type == JS_TOKEN_EOF);
        token = token->next;
        
        js_tokens_free(tokens);
        flow_memory_free(memory);
    }
    {
        struct flow_memory_t* memory = flow_memory_new();
        struct js_token_t* tokens = js_lexer(memory, "for 1 -");
        struct js_token_t* token = tokens;
        
        assert(!strcmp(token->word, "for"));
        assert(token->type == JS_TOKEN_FOR);
        token = token->next;

        assert(!strcmp(token->word, "1"));
        assert(token->type == JS_TOKEN_NUMBER);
        token = token->next;
        
        assert(!strcmp(token->word, "-"));
        assert(token->type == '-');
        token = token->next;
        
        assert(token->type == JS_TOKEN_EOF);
        token = token->next;
        
        js_tokens_free(tokens);
        flow_memory_free(memory);
    }
    {
        struct flow_memory_t* memory = flow_memory_new();
        struct js_token_t* tokens = js_lexer(memory, "for(a==2){\n}");
        struct js_token_t* token = tokens;
        
        assert(!strcmp(token->word, "for"));
        assert(token->type == JS_TOKEN_FOR);
        token = token->next;
        
        assert(!strcmp(token->word, "("));
        assert(token->type == '(');
        token = token->next;

        assert(!strcmp(token->word, "a"));
        assert(token->type == JS_TOKEN_ID);
        token = token->next;

        assert(!strcmp(token->word, "="));
        assert(token->type == '=');
        token = token->next;

        assert(!strcmp(token->word, "="));
        assert(token->type == '=');
        token = token->next;

        assert(!strcmp(token->word, "2"));
        assert(token->type == JS_TOKEN_NUMBER);
        token = token->next;

        assert(!strcmp(token->word, ")"));
        assert(token->type == ')');
        token = token->next;

        assert(!strcmp(token->word, "{"));
        assert(token->type == '{');
        token = token->next;

        assert(!strcmp(token->word, "}"));
        assert(token->type == '}');
        token = token->next;
        
        assert(token->type == JS_TOKEN_EOF);
        token = token->next;

        assert(token == 0);
        js_tokens_free(tokens);
        flow_memory_free(memory);
    }
}
