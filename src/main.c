#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <signal.h>
#include <string.h>
#include "main.h"
#include "watch.h"
#include "file.h"
#include "input.h"
#include "linkedlist.h"
#include "js.h"

struct flow_argument_t {
    char* value;
    struct flow_argument_t* next;
};

// --signal SIGKILL \
    
// void registerSignal() {
//     struct sigaction action;
//     action.sa_handler = close_handler;
//     sigemptyset(&action.sa_mask);
//     action.sa_flags = 0;

//     if (sigaction(SIGTERM, &action, nullptr) == 0)
//     {
//     FSW_ELOG(_("SIGTERM handler registered.\n"));
//     }
//     else
//     {
//     cerr << _("SIGTERM handler registration failed.") << endl;
//     }

//     if (sigaction(SIGABRT, &action, nullptr) == 0)
//     {
//     FSW_ELOG(_("SIGABRT handler registered.\n"));
//     }
//     else
//     {
//     cerr << _("SIGABRT handler registration failed.") << endl;
//     }

//     if (sigaction(SIGINT, &action, nullptr) == 0)
//     {
//     FSW_ELOG(_("SIGINT handler registered.\n"));
//     }
//     else
//     {
//     cerr << _("SIGINT handler registration failed") << endl;
//     }
// }

int help_func() {
    printf("Usage:\n");
    printf("  flow [options] <file.js>\n");
    printf("  flow [mode]\n");
    printf("\n");
    printf("Example:\n");
    printf("  flow index.js\n");
    printf("  flow -t\n");
    printf("  flow -p\n");
    printf("  flow -e \"console.log('test')\"\n");
    printf("  flow -e \"console.mylog('test')\" index.js\n");
    printf("  flow -w\n");
    printf("  flow -w src\n");
    printf("  flow -c .\n");
    printf("  flow -c index.js\n");
    printf("\n");
    printf("Mode:\n");
    printf("  -l, --loop      Execute in loop the flow executable\n");
    printf("  -t, --test      Execute all tests of flow executable\n");
    printf("  -r, --repl      Enter the playground for test command\n");
    printf("  -e, --eval      Execute one command with start source\n");
    printf("  -w, --watch     Compile all sources of project\n");
    printf("  -c, --compile   Enter the watch mode compiling all changes\n");
    printf("\n");
    printf("Options:\n");
    printf("\n");
    return 0;
}

int eval_func(struct flow_argument_t* arg_node) {
    struct flow_memory_t* memory = flow_memory_new();
    struct js_compiler_t* compiler = js_compiler_new(memory);
    struct flow_argument_t* arg = arg_node;
    while (arg) {
        struct js_node_t* node = js_compiler_exec(compiler, arg->value);
        struct js_node_error_t* error = js_nodes_error_is(node);
        if (error) {
            js_node_error_print(js_node_error_revert(error));
        } else {
            struct js_context_t* context = js_context_new(memory);
            js_value_obj_new(context);
            js_node_exec_typed(node, context);
            if (!js_context_empty(context)) {
                js_context_peek_def(context, value);
                const char* chars = js_value_str_ansi((struct js_value_t*) value);
                printf("%s\n", chars);
                js_context_pop(context);
            }
            while (!js_context_empty(context)) {
                js_context_pop(context);
            }
            js_context_free(context);
            js_node_free(node);
        }
        arg = arg->next;
    }
    js_compiler_free(compiler);
    flow_memory_free(memory);
    return 0;
}

int exec_func(int test_mode, int eval_mode, int help_mode, struct flow_argument_t* arg_node) {
    if (test_mode) {
        test();
    } else if (eval_mode) {
        eval_func(arg_node);
    } else if (help_mode) {
        help_func();
    } else {
        struct flow_memory_t* memory = flow_memory_new();
        struct flow_argument_t* arg = arg_node;
        struct js_compiler_t* compiler = js_compiler_new(memory);
        while (arg) {
            char* source = (char*) flow_io_file(arg->value);
            if (source) {
                struct js_node_t* node = js_compiler_exec(compiler, source);
                struct js_node_error_t* error = js_nodes_error_is(node);
                if (error) {
                    js_node_error_print(js_node_error_revert(error));
                } else {
                    struct js_context_t* context = js_context_new(memory);
                    js_value_obj_new(context);
                    js_node_exec_typed(node, context);
                    if (!js_context_empty(context)) {
                        js_context_peek_def(context, value);
                        const char* chars = js_value_str_ansi((struct js_value_t*) value);
                        printf("%s\n", chars);
                        js_context_pop(context);
                    }
                    while (!js_context_empty(context)) {
                        js_context_pop(context);
                    }
                    js_context_free(context);
                    js_node_free(node);
                }
            } else {
                printf("[fatal]: file not found: %s\n", arg->value);
            }
            
            struct flow_argument_t* arg_next = arg_node->next;
            free(arg->value);
            free(arg);
            arg = arg_next;
        }
        js_compiler_free(compiler);
        flow_memory_free(memory);

        //printf("Creating watch\n");
        //struct watch_t* watch = watch_new();
        //watch_dir(watch, "/Users/bernardobreder/git/flowlng/src");
        //watch_loop(watch, (void (*)(char*, flow_watch_event_t))flow_input_perform);
        //watch_free(watch);
    }
    return 0;
}

int main(int argc, char **argv) {
    int test_mode = 0, help_mode = 0, eval_mode = 0, loop_mode = 0;
    flow_linkedlist_inline_new(arg_head, arg_tail, struct flow_argument_t);
    int i;
    for (i = 1 ; i < argc ; i++) {
        char *arg = argv[i];
        if (!strcmp(arg, "-l") || !strcmp(arg, "--loop")) { loop_mode = 1; }
        else if (!strcmp(arg, "-t") || !strcmp(arg, "--test")) { test_mode = 1; }
        else if (!strcmp(arg, "-e") || !strcmp(arg, "--eval")) { eval_mode = 1; }
        else if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) { help_mode = 1; }
        else if (arg[0] == '-') {}
        else {
            struct flow_argument_t* arg_node = (struct flow_argument_t*) malloc(sizeof(struct flow_argument_t));
            arg_node->value = strdup(arg);
            arg_node->next = 0;
            flow_linkedlist_inline_add(arg_head, arg_tail, arg_node);
        }
    }
    
    if (loop_mode) {
        int n; for (n = 0 ;; n++) {
            exec_func(test_mode, eval_mode, help_mode, arg_head);
        }
    } else return exec_func(test_mode, eval_mode, help_mode, arg_head);
}
