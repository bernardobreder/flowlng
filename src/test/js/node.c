#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "js.h"

static void test_js_node_exec(char* code, char* expected) {
    struct flow_memory_t* memory = flow_memory_new();
    struct js_token_t* tokens = js_lexer(code);
    struct js_parser_t* parser = js_parser_new(memory, tokens);
    struct js_node_t* node = js_parser(parser, tokens);
    js_parser_free(parser);
    js_tokens_free(tokens);
    if (js_node_error_is(node)) {
        js_node_error_print(js_node_error_revert(js_node_error_type(node)));
        assert(0);
    } else {
        struct js_compiler_t* compiler = js_compiler_new(memory);
        js_node_head(node, compiler);
        js_node_body(node, compiler);
        js_compiler_free(compiler);
        struct js_context_t* context = js_context_new(memory);
        js_value_obj_new(context);
        js_node_exec_typed(node, context);
        if (!js_context_empty(context)) {
            js_context_peek_def(context, value);
            const char* chars = js_value_str_ansi((struct js_value_t*) value);
            if (strcmp(expected, chars)) {
                printf("%s\nExpected: %s\nActual: %s\n\n", code, expected, chars);
            }
            js_context_pop(context);
        }
        while (!js_context_empty(context)) {
            js_context_pop(context);
        }
        js_context_free(context);
        js_node_free_typed(node);
        if (memory->count != 0) {
            printf("%s\n%zu objects alive\n\n", code, memory->count);
        }
        flow_memory_free(memory);
    }
}

void test_js_node() {
    test_js_node_exec("class a do end", "<object>");
    test_js_node_exec("class a do var a end", "<object>");
    test_js_node_exec("class a do function a() do end end", "<object>");
    test_js_node_exec("class a do constructor() do end end", "<object>");
    
    test_js_node_exec("var a = 1 return a", "1");
    test_js_node_exec("var a = 1, b = 2 return a + b", "3");
    test_js_node_exec("var a = 1 a = 2 return a", "2");
    
    test_js_node_exec("return 2+3", "5");
    test_js_node_exec("return \"a\"+1", "a1");
    test_js_node_exec("return \"a\"+\"b\"", "ab");
    test_js_node_exec("return 2-3", "-1");
    test_js_node_exec("return -2+3", "1");
    test_js_node_exec("return 2*3", "6");
    test_js_node_exec("return 4/2", "2");
    test_js_node_exec("return 4/-2", "-2");
    
    test_js_node_exec("function a() do end", "<object>");
    test_js_node_exec("function a() do return 1 end a()", "<object>");
    test_js_node_exec("function a() do return 1 end return a()", "1");
    test_js_node_exec("function a() do return 1 end function b() do return a() end return b()", "1");
    test_js_node_exec("function a() do return 1 end function b() do return a()+1 end return b()", "2");
    test_js_node_exec("function a() do return 1 end function b() do return 1+a() end return b()", "2");
    test_js_node_exec("function a() do return 1 end function b() do return a()+a()+a() end return b()", "3");

    test_js_node_exec("function a() do function b() do return 1 end return b() end return a()", "1");
    
    test_js_node_exec("return 1", "1");
    test_js_node_exec("return 1.234", "1.234");
    test_js_node_exec("return true", "true");
    test_js_node_exec("return false", "false");
    test_js_node_exec("return null", "null");
    test_js_node_exec("return a", "null");
    test_js_node_exec("return \"abc\"", "abc");

    test_js_node_exec("return a=1", "1");

    test_js_node_exec("return 1 ? 2 : 3", "3");
    test_js_node_exec("return false ? 2 : 3", "3");
    test_js_node_exec("return true ? 2 : 3", "2");

    test_js_node_exec("return true or true", "true");
    test_js_node_exec("return true or false", "true");
    test_js_node_exec("return false or true", "true");
    test_js_node_exec("return false or false", "false");
    
    test_js_node_exec("return true and true", "true");
    test_js_node_exec("return true and false", "false");
    test_js_node_exec("return false and true", "false");
    test_js_node_exec("return false and false", "false");

    test_js_node_exec("return true or true and true", "true");
    test_js_node_exec("return true or true and false", "true");
    test_js_node_exec("return true or false and true", "true");
    test_js_node_exec("return true or false and false", "true");
    test_js_node_exec("return false or true and true", "true");
    test_js_node_exec("return false or true and false", "false");
    test_js_node_exec("return false or false and true", "false");
    test_js_node_exec("return false or false and false", "false");
    
    test_js_node_exec("return 1==1", "true");
    test_js_node_exec("return 1==2", "false");
    test_js_node_exec("return true==1", "false");
    test_js_node_exec("return 1==true", "false");
    test_js_node_exec("return 1.1==1", "false");
    test_js_node_exec("return 1==1.1", "false");
    test_js_node_exec("return \"a\"==1", "false");
    test_js_node_exec("return 1==\"a\"", "false");
    
    test_js_node_exec("return 1.1==1.1", "true");
    test_js_node_exec("return 0.3333334==0.333333333333", "true");
    test_js_node_exec("return 0.333334==0.333333333333", "false");
    test_js_node_exec("return 1.1==1.2", "false");
    test_js_node_exec("return \"a\"==1.1", "false");
    test_js_node_exec("return 1.1==\"a\"", "false");

    test_js_node_exec("return true==true", "true");
    test_js_node_exec("return true==false", "false");
    test_js_node_exec("return true==1.1", "false");
    test_js_node_exec("return 1.1==true", "false");
    test_js_node_exec("return true==\"a\"", "false");
    test_js_node_exec("return \"a\"==true", "false");
    
    test_js_node_exec("return 1!=1", "false");
    test_js_node_exec("return 1!=2", "true");
    test_js_node_exec("return true!=1", "true");
    test_js_node_exec("return 1!=true", "true");
    test_js_node_exec("return 1.1!=1", "true");
    test_js_node_exec("return 1!=1.1", "true");
    test_js_node_exec("return \"a\"!=1", "true");
    test_js_node_exec("return 1!=\"a\"", "true");

    test_js_node_exec("return 1<=1", "true");
    test_js_node_exec("return 1<=2", "true");
    test_js_node_exec("return 2<=1", "false");
    test_js_node_exec("return 1.1<=1.1", "true");
    test_js_node_exec("return 1.1<=2.1", "true");
    test_js_node_exec("return 2.1<=1.1", "false");
    test_js_node_exec("return true<=true", "true");
    test_js_node_exec("return true<=false", "false");
    test_js_node_exec("return false<=true", "true");
    test_js_node_exec("return false<=false", "true");
    test_js_node_exec("return \"abc\"<=\"abc\"", "true");
    test_js_node_exec("return \"abd\"<=\"abc\"", "false");
    test_js_node_exec("return \"abc\"<=\"abd\"", "true");
    
    test_js_node_exec("return 1>=1", "true");
    test_js_node_exec("return 1>=2", "false");
    test_js_node_exec("return 2>=1", "true");
    test_js_node_exec("return 1.1>=1.1", "true");
    test_js_node_exec("return 1.1>=2.1", "false");
    test_js_node_exec("return 2.1>=1.1", "true");
    test_js_node_exec("return true>=true", "true");
    test_js_node_exec("return true>=false", "true");
    test_js_node_exec("return false>=true", "false");
    test_js_node_exec("return false>=false", "true");
    test_js_node_exec("return \"abc\">=\"abc\"", "true");
    test_js_node_exec("return \"abd\">=\"abc\"", "true");
    test_js_node_exec("return \"abc\">=\"abd\"", "false");
    
    test_js_node_exec("return 1<1", "false");
    test_js_node_exec("return 1<2", "true");
    test_js_node_exec("return 2<1", "false");
    test_js_node_exec("return 1.1<1.1", "false");
    test_js_node_exec("return 1.1<2.1", "true");
    test_js_node_exec("return 2.1<1.1", "false");
    test_js_node_exec("return true<true", "false");
    test_js_node_exec("return true<false", "false");
    test_js_node_exec("return false<true", "true");
    test_js_node_exec("return false<false", "false");
    test_js_node_exec("return \"abc\"<\"abc\"", "false");
    test_js_node_exec("return \"abd\"<\"abc\"", "false");
    test_js_node_exec("return \"abc\"<\"abd\"", "true");
    
    test_js_node_exec("return 1>1", "false");
    test_js_node_exec("return 1>2", "false");
    test_js_node_exec("return 2>1", "true");
    test_js_node_exec("return 1.1>1.1", "false");
    test_js_node_exec("return 1.1>2.1", "false");
    test_js_node_exec("return 2.1>1.1", "true");
    test_js_node_exec("return true>true", "false");
    test_js_node_exec("return true>false", "true");
    test_js_node_exec("return false>true", "false");
    test_js_node_exec("return false>false", "false");
    test_js_node_exec("return \"abc\">\"abc\"", "false");
    test_js_node_exec("return \"abd\">\"abc\"", "true");
    test_js_node_exec("return \"abc\">\"abd\"", "false");
}
