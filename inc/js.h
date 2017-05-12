
#include <stdint.h>

#define JS_TOKEN_EOF 0
#define JS_TOKEN_ID 1
#define JS_TOKEN_STRING 2
#define JS_TOKEN_NUMBER 3
#define JS_TOKEN_COMMENT 4

#define JS_TOKEN_FOR 271
#define JS_TOKEN_FALSE 272
#define JS_TOKEN_REQUIRE 273
#define JS_TOKEN_IF 274
#define JS_TOKEN_OR 275
#define JS_TOKEN_TRUE 276
#define JS_TOKEN_WHILE 277
#define JS_TOKEN_RETURN 278
#define JS_TOKEN_DO 279
#define JS_TOKEN_ELSE 280
#define JS_TOKEN_FUNCTION 281
#define JS_TOKEN_BREAK 282
#define JS_TOKEN_CONTINUE 283

#define JS_NODE_ERROR 1
#define JS_NODE_ID 2
#define JS_NODE_EMPTY 3
#define JS_NODE_FUNCTION 4
#define JS_NODE_IF 5
#define JS_NODE_WHILE 6
#define JS_NODE_BREAK 7
#define JS_NODE_CONTINUE 8

struct js_token_t {
    unsigned short type;
    char* word;
    int line;
    int column;
    struct js_token_t* next;
};

struct js_parser_t {
    struct js_token_t* tokens;
    struct js_token_t* token;
};

struct js_node_t {
    unsigned char type;
    struct js_node_t* next;
};

struct js_node_error_t {
    unsigned char type;
    struct js_node_t* next;
    char* message;
    char* word;
    size_t line_begin;
    size_t line_end;
    size_t column_begin;
    size_t column_end;
};

struct js_node_id_t {
    unsigned char type;
    struct js_next_t* next;
    char* word;
};

struct js_node_empty_t {
    unsigned char type;
    struct js_node_t* next;
};

struct js_node_break_t {
    unsigned char type;
    struct js_node_t* next;
};

struct js_node_continue_t {
    unsigned char type;
    struct js_node_t* next;
};

struct js_node_function_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* name;
    struct js_node_t* params;
    struct js_node_t* statements;
};

struct js_node_if_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* expression;
    struct js_node_t* statement;
};

struct js_node_while_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* expression;
    struct js_node_t* statement;
};

void js_token_free(struct js_token_t* self);

void js_tokens_free(struct js_token_t* self);

struct js_token_t* js_lexer(const char* text);

struct js_parser_t* js_parser_new(struct js_token_t* tokens);

void js_parser_free(struct js_parser_t* self);

struct js_node_t* js_parser(struct js_parser_t* self, struct js_token_t* tokens);

void js_node_free(struct js_node_t* self);

void js_nodes_free(struct js_node_t* self);

unsigned char js_node_has_error(struct js_node_t* self);

struct js_node_error_t* js_node_error_new(char* message, char* word, size_t line_begin, size_t column_begin, size_t line_end, size_t column_end);

struct js_node_id_t* js_node_id_new(char* word);

struct js_node_empty_t* js_node_empty_new();

struct js_node_break_t* js_node_break_new();

struct js_node_continue_t* js_node_continue_new();

struct js_node_function_t* js_node_function_new(struct js_node_t* name, struct js_node_t* params, struct js_node_t* statements);

struct js_node_if_t* js_node_if_new(struct js_node_t* expression, struct js_node_t* statement);

struct js_node_while_t* js_node_while_new(struct js_node_t* expression, struct js_node_t* statement);
