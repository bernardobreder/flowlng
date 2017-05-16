#include <assert.h>
#include <string.h>
#include "js.h"

void test_js_parser_compile(char* code) {
    struct js_token_t* tokens = js_lexer(code);
    struct js_parser_t* parser = js_parser_new(tokens);
    struct js_node_t* nodes = js_parser(parser, tokens);
    js_parser_free(parser);
    js_tokens_free(tokens);
    js_node_free(nodes);
}

void test_js_parser() {
    {
        test_js_parser_compile("function");
        test_js_parser_compile("function a");
        test_js_parser_compile("function a(");
        test_js_parser_compile("function a()");
        test_js_parser_compile("function a(){");
        test_js_parser_compile("function a(){}");
        test_js_parser_compile("function a(a){}");
        test_js_parser_compile("function a(a,){}");
        test_js_parser_compile("function a(a,b){}");
        test_js_parser_compile("function a(a,b,){}");
        test_js_parser_compile("function a(a,b,c){}");
        test_js_parser_compile("function a(a,b,c){;}");
        test_js_parser_compile("function a(a,b,c){;}function");
        test_js_parser_compile("function a(a,b,c){;}function b");
        test_js_parser_compile("function a(a,b,c){;}function b(");
        test_js_parser_compile("function a(a,b,c){;}function b()");
        test_js_parser_compile("function a(a,b,c){;}function b(){");
        test_js_parser_compile("function a(a,b,c){;}function b(){}");
        test_js_parser_compile("function a(a,b,c){;}function b(){;}");
    }
    {
        test_js_parser_compile("function a(){break}");
        test_js_parser_compile("function a(){break;}");
        test_js_parser_compile("function a(){continue}");
        test_js_parser_compile("function a(){continue;}");
        test_js_parser_compile("function a(){break;continue;}");
        test_js_parser_compile("function a(){break;continue;break;}");
        test_js_parser_compile("function a(){break;continue;break;continue}");
        test_js_parser_compile("function a(){break;continue;break;continue;}");
    }
    {
        test_js_parser_compile("a");
        test_js_parser_compile("1");
        test_js_parser_compile("1.2");
        test_js_parser_compile("true");
        test_js_parser_compile("false");
        test_js_parser_compile("null");
        test_js_parser_compile("this");
        test_js_parser_compile("super");
        test_js_parser_compile("(a)");
    }
    {
        test_js_parser_compile("function a(){if}");
        test_js_parser_compile("function a(){if(false){}}");
    }
    {
        test_js_parser_compile("function a(){while}");
        test_js_parser_compile("function a(){while(false){}}");
    }
    {
        struct js_token_t* tokens = js_lexer("function a(a,b,c){;}");
        struct js_parser_t* parser = js_parser_new(tokens);
        struct js_node_t* nodes = js_parser(parser, tokens);
        js_parser_free(parser);
        js_tokens_free(tokens);
        js_node_free(nodes);
    }
}
