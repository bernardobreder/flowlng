
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
#define JS_TOKEN_THIS 284
#define JS_TOKEN_SUPER 285
#define JS_TOKEN_NULL 286
#define JS_TOKEN_VAR 287
#define JS_TOKEN_END 289

#define JS_NODE_ERROR 1
#define JS_NODE_ID 2
#define JS_NODE_EMPTY 3
#define JS_NODE_FUNCTION 4
#define JS_NODE_IF 5
#define JS_NODE_WHILE 6
#define JS_NODE_BREAK 7
#define JS_NODE_CONTINUE 8
#define JS_NODE_TRUE 9
#define JS_NODE_FALSE 10
#define JS_NODE_INT 11
#define JS_NODE_NUM 12
#define JS_NODE_STRING 13
#define JS_NODE_NULL 14
#define JS_NODE_THIS 15
#define JS_NODE_SUPER 16
#define JS_NODE_RETURN 17
#define JS_NODE_VAR_ITEM 18

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
    struct js_node_error_t* next;
    char* message;
    char* word;
    size_t line;
    size_t column;
};

struct js_node_string_t {
    unsigned char type;
    struct js_node_t* next;
    char* value;
};

struct js_node_num_t {
    unsigned char type;
    struct js_node_t* next;
    double value;
};

struct js_node_int_t {
    unsigned char type;
    struct js_node_t* next;
    int value;
};

struct js_node_id_t {
    unsigned char type;
    struct js_node_t* next;
    char* word;
};

struct js_node_true_t {
    unsigned char type;
    struct js_node_t* next;
};

struct js_node_false_t {
    unsigned char type;
    struct js_node_t* next;
};

struct js_node_null_t {
    unsigned char type;
    struct js_node_t* next;
};

struct js_node_this_t {
    unsigned char type;
    struct js_node_t* next;
};

struct js_node_super_t {
    unsigned char type;
    struct js_node_t* next;
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

struct js_node_var_item_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_id_t* name;
    struct js_node_t* value;
};

struct js_node_function_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_id_t* name;
    struct js_node_t* params;
    struct js_node_t* statement;
};

struct js_node_return_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* expression;
};

struct js_node_if_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* expression;
    struct js_node_t* statement;
    struct js_node_t* else_statement;
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
void js_node_head(struct js_node_t* self);
void js_node_body(struct js_node_t* self);

struct js_node_error_t* js_node_error_new(char* message, char* word, size_t line, size_t column, struct js_node_error_t* next);
void js_node_error_free(struct js_node_error_t* self);
struct js_node_error_t* js_node_error_revert(struct js_node_error_t* self);
void js_node_error_print(struct js_node_error_t* self);
#define js_node_error_type(SELF) ((struct js_node_error_t*)SELF)
#define js_node_error_is(SELF) (SELF)->type == JS_NODE_ERROR

struct js_node_id_t* js_node_id_new(char* word);
void js_node_id_free(struct js_node_id_t* self);

struct js_node_string_t* js_node_string_new(char* word);

struct js_node_num_t* js_node_num_new(double value);

struct js_node_int_t* js_node_int_new(int value);

struct js_node_true_t* js_node_true_new();

struct js_node_false_t* js_node_false_new();

struct js_node_null_t* js_node_null_new();

struct js_node_this_t* js_node_this_new();

struct js_node_super_t* js_node_super_new();

struct js_node_return_t* js_node_return_new(struct js_node_t* expression);

struct js_node_empty_t* js_node_empty_new();
void js_node_empty_body(struct js_node_empty_t* self);
void js_node_empty_exec(struct js_node_empty_t* self);

struct js_node_break_t* js_node_break_new();
void js_node_break_body(struct js_node_break_t* self);
void js_node_break_exec(struct js_node_break_t* self);

struct js_node_continue_t* js_node_continue_new();
void js_node_continue_body(struct js_node_continue_t* self);
void js_node_continue_exec(struct js_node_continue_t* self);

struct js_node_var_item_t* js_node_var_item_new(struct js_node_id_t* name, struct js_node_t* value);
void js_node_var_item_free(struct js_node_var_item_t* self);

struct js_node_function_t* js_node_function_new(struct js_node_id_t* name, struct js_node_t* params, struct js_node_t* statement);
void js_node_function_free(struct js_node_function_t* self);

struct js_node_if_t* js_node_if_new(struct js_node_t* expression, struct js_node_t* statement, struct js_node_t* else_statement);
void js_node_if_head(struct js_node_if_t* self);
void js_node_if_body(struct js_node_if_t* self);
void js_node_if_exec(struct js_node_if_t* self);

struct js_node_while_t* js_node_while_new(struct js_node_t* expression, struct js_node_t* statement);
