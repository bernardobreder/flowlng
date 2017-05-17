
#include <stdint.h>
#include <stddef.h>
#include "platform.h"
#include "memory.h"

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
#define JS_TOKEN_AND 290
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
#define JS_TOKEN_XOR 291
#define JS_TOKEN_POW 292
#define JS_TOKEN_BITOR 293
#define JS_TOKEN_BITAND 294
#define JS_TOKEN_LSHIFT 295
#define JS_TOKEN_RSHIFT 296

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
#define JS_NODE_ASSIGNMENT 19
#define JS_NODE_TERNARY 20
#define JS_NODE_OR 21
#define JS_NODE_AND 22
#define JS_NODE_BITWISE_OR 23
#define JS_NODE_BITWISE_XOR 24
#define JS_NODE_BITWISE_AND 25
#define JS_NODE_EQUAL 26
#define JS_NODE_NOT_EQUAL 27
#define JS_NODE_LOWER_EQUAL 28
#define JS_NODE_GREATER_EQUAL 29
#define JS_NODE_LOWER_THAN 30
#define JS_NODE_GREATER_THAN 31
#define JS_NODE_SHIFT_LEFT 32
#define JS_NODE_SHIFT_RIGHT 33
#define JS_NODE_SUM 34
#define JS_NODE_SUB 35
#define JS_NODE_MUL 36
#define JS_NODE_DIV 37
#define JS_NODE_NOT 38
#define JS_NODE_NEG 39
#define JS_NODE_PRE_INC 40
#define JS_NODE_PRE_DEC 41
#define JS_NODE_POS_INC 42
#define JS_NODE_POS_DEC 43
#define JS_NODE_FIELD 44
#define JS_NODE_ARRAY 45
#define JS_NODE_CALL 46


#define JS_VALUE_NULL 0
#define JS_VALUE_BOOL 1
#define JS_VALUE_NUM 2
#define JS_VALUE_INT 3
#define JS_VALUE_STR 4
#define JS_VALUE_OBJ 5
#define JS_VALUE_FUNC 6

typedef unsigned char js_bool;
typedef double js_num;
typedef long js_int;
typedef char js_char;
typedef js_char* js_str;
typedef uint64 js_hash;
typedef void* js_obj;

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

struct js_context_t {
    struct flow_memory_t* memory;
    struct js_value_t* value;
};

struct js_value_t {
    unsigned char type;
    struct js_value_t* next;
};

struct js_value_null_t {
    unsigned char type;
    struct js_value_t* next;
};

struct js_value_bool_t {
    unsigned char type;
    struct js_value_t* next;
    js_bool value;
};

struct js_value_int_t {
    unsigned char type;
    struct js_value_t* next;
    js_int value;
};

struct js_value_num_t {
    unsigned char type;
    struct js_value_t* next;
    js_num value;
};

struct js_value_str_t {
    unsigned char type;
    struct js_value_t* next;
    js_str value;
    size_t length;
    js_hash hash;
};

struct js_value_func_t {
    unsigned char type;
    struct js_value_t* next;
};

struct js_value_obj_entry_t {
    char* name;
    size_t length;
    js_hash hash;
    struct js_value_t* value;
};

struct js_value_obj_t {
    unsigned char type;
    struct js_value_obj_t* next;
    struct js_value_obj_entry_t* constructor;
    struct js_value_obj_entry_t* desctructor;
    struct js_value_obj_entry_t* field;
    struct js_value_obj_entry_t* function;
    struct js_value_func_t* to_str_func;
    struct js_value_func_t* to_int_func;
    struct js_value_func_t* clone_func;
    struct js_value_func_t* equal_func;
    struct js_value_str_t* class_str;
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
    js_str value;
    size_t length;
    js_hash hash;
};

struct js_node_num_t {
    unsigned char type;
    struct js_node_t* next;
    double value;
};

struct js_node_int_t {
    unsigned char type;
    struct js_node_t* next;
    int32 value;
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

struct js_node_assignment_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_ternary_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* true_value;
    struct js_node_t* false_value;
};

struct js_node_or_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_and_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_bitwise_or_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_bitwise_xor_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_bitwise_and_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_equal_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_not_equal_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_lower_equal_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_greater_equal_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_lower_than_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_greater_than_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_shift_left_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_shift_right_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_sum_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_sub_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_mul_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_div_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_not_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
};

struct js_node_neg_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
};

struct js_node_pre_inc_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
};

struct js_node_pre_dec_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
};

struct js_node_pos_inc_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
};

struct js_node_pos_dec_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
};

struct js_node_field_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_id_t* value;
};

struct js_node_array_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
};

struct js_node_call_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* node;
    struct js_node_t* value;
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

#define js_str_hash_prime 13

void js_token_free(struct js_token_t* self);

void js_tokens_free(struct js_token_t* self);

struct js_token_t* js_lexer(const char* text);

struct js_parser_t* js_parser_new(struct js_token_t* tokens);
void js_parser_free(struct js_parser_t* self);
struct js_node_t* js_parser(struct js_parser_t* self, struct js_token_t* tokens);

struct js_context_t* js_context_new(struct flow_memory_t* memory);
void js_context_free(struct js_context_t* self);
uint8 js_context_empty(struct js_context_t* self);
void js_context_push(struct js_context_t* self, struct js_value_t* value);
#define js_context_push_typed(CONTEXT, VALUE) js_context_push(CONTEXT, (struct js_value_t*) VALUE)
struct js_value_t* js_context_pop(struct js_context_t* self);

void js_value_free(struct js_value_t* self);
struct js_value_t* js_value_null();
struct js_value_t* js_value_true();
struct js_value_t* js_value_false();
js_bool js_value_is_equal(struct js_context_t* context, struct js_value_t* left, struct js_value_t* right);
js_int js_value_is_compare(struct js_context_t* context, struct js_value_t* left, struct js_value_t* right);
#define js_value_def(NAME, VALUE) struct js_value_t* NAME = VALUE;
#define js_value_bool(FLAG) ((FLAG) ? js_value_true() : js_value_false())
#define js_value_exec_def(NAME, NODE) js_node_exec(NODE, context); struct js_value_t* NAME = js_context_pop(context);
#define js_value_release(VALUE) js_value_free(VALUE);
#define js_value_bool_value(VALUE) ((struct js_value_bool_t*)VALUE)->value
#define js_value_bool_def(NAME, VALUE) js_bool NAME = js_value_is_bool(VALUE) ? js_value_bool_value(VALUE) : 0;
#define js_value_is_true(VALUE) (VALUE) == js_value_true()
#define js_value_is_false(VALUE) (VALUE) != js_value_true()
#define js_value_is_null(VALUE) (VALUE->type == JS_VALUE_NULL)
#define js_value_is_bool(VALUE) (VALUE->type == JS_VALUE_BOOL)
#define js_value_is_int(VALUE) (VALUE->type == JS_VALUE_INT)
#define js_value_is_num(VALUE) (VALUE->type == JS_VALUE_NUM)
#define js_value_is_numint(VALUE) (VALUE->type == JS_VALUE_INT || VALUE->type == JS_VALUE_NUM)
#define js_value_is_str(VALUE) (VALUE->type == JS_VALUE_STR)
#define js_value_int_def(NAME, VALUE) js_int NAME = js_value_int_value(VALUE);
#define js_value_int_value(VALUE) ((struct js_value_int_t*)VALUE)->value
#define js_value_int_new_def(CONTEXT, NAME, VALUE) struct js_value_t* NAME = js_value_int_new(CONTEXT->memory, VALUE);
#define js_value_num_def(NAME, VALUE) js_num NAME = js_value_num_value(VALUE);
#define js_value_num_value(VALUE) ((struct js_value_num_t*)VALUE)->value
#define js_value_num_new_def(CONTEXT, NAME, VALUE) struct js_value_t* NAME = js_value_num_new(CONTEXT->memory, VALUE);
#define js_value_numint_def(NAME, VALUE) js_num NAME = js_value_is_num(VALUE) ? js_value_num_value(VALUE) : (js_num)js_value_int_value(VALUE);
#define js_value_str_def(NAME, VALUE) js_str NAME = js_value_str_value(VALUE);
#define js_value_str_value(VALUE) ((struct js_value_str_t*)VALUE)->value
#define js_value_str_len_def(NAME, VALUE) size_t NAME = strlen(js_value_str_value(VALUE));
#define js_value_str_new_def(CONTEXT, NAME, VALUE, LENGTH, HASH) \
        struct js_value_t* NAME = js_value_str_new(CONTEXT->memory, VALUE, LENGTH, HASH);
#define js_value_obj_to_str_def(NAME, VALUE) \
        js_str NAME = js_value_is_str(VALUE) ? js_value_str_value(VALUE) : js_value_object_string_ansi(VALUE);
struct js_value_t* js_value_int_new(struct flow_memory_t* memory, js_int value);
struct js_value_t* js_value_num_new(struct flow_memory_t* memory, js_num value);
struct js_value_t* js_value_str_new(struct flow_memory_t* memory, char* value, size_t length, js_hash hash);
void js_value_str_free(struct js_value_str_t* self);
struct js_value_t* js_value_obj_new(struct flow_memory_t* memory, struct js_value_str_t* class_str);
void js_value_obj_free(struct js_value_str_t* self);
struct js_value_t* js_value_func_new(struct flow_memory_t* memory);
void js_value_func_free(struct js_value_str_t* self);
char* js_value_object_string_ansi(struct js_value_t* self);

void js_node_free(struct js_node_t* self);
void js_node_compile(struct js_node_t* self);
void js_node_head(struct js_node_t* self);
void js_node_body(struct js_node_t* self);
void js_node_exec(struct js_node_t* self, struct js_context_t* context);

struct js_node_error_t* js_node_error_new(char* message, char* word, size_t line, size_t column, struct js_node_error_t* next);
void js_node_error_free(struct js_node_error_t* self);
struct js_node_error_t* js_node_error_revert(struct js_node_error_t* self);
void js_node_error_print(struct js_node_error_t* self);
#define js_node_error_type(SELF) ((struct js_node_error_t*)SELF)
#define js_node_error_is(SELF) (SELF)->type == JS_NODE_ERROR
#define js_node_error_is_not(SELF) (SELF)->type != JS_NODE_ERROR

struct js_node_id_t* js_node_id_new(char* word);
void js_node_id_free(struct js_node_id_t* self);
void js_node_id_head(struct js_node_id_t* self);
void js_node_id_body(struct js_node_id_t* self);
void js_node_id_exec(struct js_node_id_t* self, struct js_context_t* context);

struct js_node_string_t* js_node_string_new(char* word, size_t length);
void js_node_string_free(struct js_node_string_t* self);
void js_node_string_head(struct js_node_string_t* self);
void js_node_string_body(struct js_node_string_t* self);
void js_node_string_exec(struct js_node_string_t* self, struct js_context_t* context);

struct js_node_num_t* js_node_num_new(double value);
void js_node_num_free(struct js_node_num_t* self);
void js_node_num_head(struct js_node_num_t* self);
void js_node_num_body(struct js_node_num_t* self);
void js_node_num_exec(struct js_node_num_t* self, struct js_context_t* context);

struct js_node_int_t* js_node_int_new(int value);
void js_node_int_free(struct js_node_int_t* self);
void js_node_int_head(struct js_node_int_t* self);
void js_node_int_body(struct js_node_int_t* self);
void js_node_int_exec(struct js_node_int_t* self, struct js_context_t* context);

struct js_node_true_t* js_node_true_new();
void js_node_true_free(struct js_node_true_t* self);
void js_node_true_head(struct js_node_true_t* self);
void js_node_true_body(struct js_node_true_t* self);
void js_node_true_exec(struct js_node_true_t* self, struct js_context_t* context);

struct js_node_false_t* js_node_false_new();
void js_node_false_free(struct js_node_false_t* self);
void js_node_false_head(struct js_node_false_t* self);
void js_node_false_body(struct js_node_false_t* self);
void js_node_false_exec(struct js_node_false_t* self, struct js_context_t* context);

struct js_node_null_t* js_node_null_new();
void js_node_null_free(struct js_node_null_t* self);
void js_node_null_head(struct js_node_null_t* self);
void js_node_null_body(struct js_node_null_t* self);
void js_node_null_exec(struct js_node_null_t* self, struct js_context_t* context);

struct js_node_this_t* js_node_this_new();
void js_node_this_free(struct js_node_this_t* self);
void js_node_this_head(struct js_node_this_t* self);
void js_node_this_body(struct js_node_this_t* self);
void js_node_this_exec(struct js_node_this_t* self, struct js_context_t* context);

struct js_node_super_t* js_node_super_new();
void js_node_super_free(struct js_node_super_t* self);
void js_node_super_head(struct js_node_super_t* self);
void js_node_super_body(struct js_node_super_t* self);
void js_node_super_exec(struct js_node_super_t* self, struct js_context_t* context);

struct js_node_assignment_t* js_node_assignment_new(struct js_node_t* node, struct js_node_t* value);
void js_node_assignment_free(struct js_node_assignment_t* self);
void js_node_assignment_head(struct js_node_assignment_t* self);
void js_node_assignment_body(struct js_node_assignment_t* self);
void js_node_assignment_exec(struct js_node_assignment_t* self, struct js_context_t* context);

struct js_node_ternary_t* js_node_ternary_new(struct js_node_t* node, struct js_node_t* true_value, struct js_node_t* false_value);
void js_node_ternary_free(struct js_node_ternary_t* self);
void js_node_ternary_head(struct js_node_ternary_t* self);
void js_node_ternary_body(struct js_node_ternary_t* self);
void js_node_ternary_exec(struct js_node_ternary_t* self, struct js_context_t* context);

struct js_node_or_t* js_node_or_new(struct js_node_t* node, struct js_node_t* value);
void js_node_or_free(struct js_node_or_t* self);
void js_node_or_head(struct js_node_or_t* self);
void js_node_or_body(struct js_node_or_t* self);
void js_node_or_exec(struct js_node_or_t* self, struct js_context_t* context);

struct js_node_and_t* js_node_and_new(struct js_node_t* node, struct js_node_t* value);
void js_node_and_free(struct js_node_and_t* self);
void js_node_and_head(struct js_node_and_t* self);
void js_node_and_body(struct js_node_and_t* self);
void js_node_and_exec(struct js_node_and_t* self, struct js_context_t* context);

struct js_node_bitwise_or_t* js_node_bitwise_or_new(struct js_node_t* node, struct js_node_t* value);
void js_node_bitwise_or_free(struct js_node_bitwise_or_t* self);
void js_node_bitwise_or_head(struct js_node_bitwise_or_t* self);
void js_node_bitwise_or_body(struct js_node_bitwise_or_t* self);
void js_node_bitwise_or_exec(struct js_node_bitwise_or_t* self, struct js_context_t* context);

struct js_node_bitwise_xor_t* js_node_bitwise_xor_new(struct js_node_t* node, struct js_node_t* value);
void js_node_bitwise_xor_free(struct js_node_bitwise_xor_t* self);
void js_node_bitwise_xor_head(struct js_node_bitwise_xor_t* self);
void js_node_bitwise_xor_body(struct js_node_bitwise_xor_t* self);
void js_node_bitwise_xor_exec(struct js_node_bitwise_xor_t* self, struct js_context_t* context);

struct js_node_bitwise_and_t* js_node_bitwise_and_new(struct js_node_t* node, struct js_node_t* value);
void js_node_bitwise_and_free(struct js_node_bitwise_and_t* self);
void js_node_bitwise_and_head(struct js_node_bitwise_and_t* self);
void js_node_bitwise_and_body(struct js_node_bitwise_and_t* self);
void js_node_bitwise_and_exec(struct js_node_bitwise_and_t* self, struct js_context_t* context);

struct js_node_equal_t* js_node_equal_new(struct js_node_t* node, struct js_node_t* value);
void js_node_equal_free(struct js_node_equal_t* self);
void js_node_equal_head(struct js_node_equal_t* self);
void js_node_equal_body(struct js_node_equal_t* self);
void js_node_equal_exec(struct js_node_equal_t* self, struct js_context_t* context);

struct js_node_not_equal_t* js_node_not_equal_new(struct js_node_t* node, struct js_node_t* value);
void js_node_not_equal_free(struct js_node_not_equal_t* self);
void js_node_not_equal_head(struct js_node_not_equal_t* self);
void js_node_not_equal_body(struct js_node_not_equal_t* self);
void js_node_not_equal_exec(struct js_node_not_equal_t* self, struct js_context_t* context);

struct js_node_lower_equal_t* js_node_lower_equal_new(struct js_node_t* node, struct js_node_t* value);
void js_node_lower_equal_free(struct js_node_lower_equal_t* self);
void js_node_lower_equal_head(struct js_node_lower_equal_t* self);
void js_node_lower_equal_body(struct js_node_lower_equal_t* self);
void js_node_lower_equal_exec(struct js_node_lower_equal_t* self, struct js_context_t* context);

struct js_node_greater_equal_t* js_node_greater_equal_new(struct js_node_t* node, struct js_node_t* value);
void js_node_greater_equal_free(struct js_node_greater_equal_t* self);
void js_node_greater_equal_head(struct js_node_greater_equal_t* self);
void js_node_greater_equal_body(struct js_node_greater_equal_t* self);
void js_node_greater_equal_exec(struct js_node_greater_equal_t* self, struct js_context_t* context);

struct js_node_lower_than_t* js_node_lower_than_new(struct js_node_t* node, struct js_node_t* value);
void js_node_lower_than_free(struct js_node_lower_than_t* self);
void js_node_lower_than_head(struct js_node_lower_than_t* self);
void js_node_lower_than_body(struct js_node_lower_than_t* self);
void js_node_lower_than_exec(struct js_node_lower_than_t* self, struct js_context_t* context);

struct js_node_greater_than_t* js_node_greater_than_new(struct js_node_t* node, struct js_node_t* value);
void js_node_greater_than_free(struct js_node_greater_than_t* self);
void js_node_greater_than_head(struct js_node_greater_than_t* self);
void js_node_greater_than_body(struct js_node_greater_than_t* self);
void js_node_greater_than_exec(struct js_node_greater_than_t* self, struct js_context_t* context);

struct js_node_shift_left_t* js_node_shift_left_new(struct js_node_t* node, struct js_node_t* value);
void js_node_shift_left_free(struct js_node_shift_left_t* self);
void js_node_shift_left_head(struct js_node_shift_left_t* self);
void js_node_shift_left_body(struct js_node_shift_left_t* self);
void js_node_shift_left_exec(struct js_node_shift_left_t* self, struct js_context_t* context);

struct js_node_shift_right_t* js_node_shift_right_new(struct js_node_t* node, struct js_node_t* value);
void js_node_shift_right_free(struct js_node_shift_right_t* self);
void js_node_shift_right_head(struct js_node_shift_right_t* self);
void js_node_shift_right_body(struct js_node_shift_right_t* self);
void js_node_shift_right_exec(struct js_node_shift_right_t* self, struct js_context_t* context);

struct js_node_sum_t* js_node_sum_new(struct js_node_t* node, struct js_node_t* value);
void js_node_sum_free(struct js_node_sum_t* self);
void js_node_sum_head(struct js_node_sum_t* self);
void js_node_sum_body(struct js_node_sum_t* self);
void js_node_sum_exec(struct js_node_sum_t* self, struct js_context_t* context);

struct js_node_sub_t* js_node_sub_new(struct js_node_t* node, struct js_node_t* value);
void js_node_sub_free(struct js_node_sub_t* self);
void js_node_sub_head(struct js_node_sub_t* self);
void js_node_sub_body(struct js_node_sub_t* self);
void js_node_sub_exec(struct js_node_sub_t* self, struct js_context_t* context);

struct js_node_mul_t* js_node_mul_new(struct js_node_t* node, struct js_node_t* value);
void js_node_mul_free(struct js_node_mul_t* self);
void js_node_mul_head(struct js_node_mul_t* self);
void js_node_mul_body(struct js_node_mul_t* self);
void js_node_mul_exec(struct js_node_mul_t* self, struct js_context_t* context);

struct js_node_div_t* js_node_div_new(struct js_node_t* node, struct js_node_t* value);
void js_node_div_free(struct js_node_div_t* self);
void js_node_div_head(struct js_node_div_t* self);
void js_node_div_body(struct js_node_div_t* self);
void js_node_div_exec(struct js_node_div_t* self, struct js_context_t* context);

struct js_node_not_t* js_node_not_new(struct js_node_t* node);
void js_node_not_free(struct js_node_not_t* self);
void js_node_not_head(struct js_node_not_t* self);
void js_node_not_body(struct js_node_not_t* self);
void js_node_not_exec(struct js_node_not_t* self, struct js_context_t* context);

struct js_node_neg_t* js_node_neg_new(struct js_node_t* node);
void js_node_neg_free(struct js_node_neg_t* self);
void js_node_neg_head(struct js_node_neg_t* self);
void js_node_neg_body(struct js_node_neg_t* self);
void js_node_neg_exec(struct js_node_neg_t* self, struct js_context_t* context);

struct js_node_pre_inc_t* js_node_pre_inc_new(struct js_node_t* node);
void js_node_pre_inc_free(struct js_node_pre_inc_t* self);
void js_node_pre_inc_head(struct js_node_pre_inc_t* self);
void js_node_pre_inc_body(struct js_node_pre_inc_t* self);
void js_node_pre_inc_exec(struct js_node_pre_inc_t* self, struct js_context_t* context);

struct js_node_pre_dec_t* js_node_pre_dec_new(struct js_node_t* node);
void js_node_pre_dec_free(struct js_node_pre_dec_t* self);
void js_node_pre_dec_head(struct js_node_pre_dec_t* self);
void js_node_pre_dec_body(struct js_node_pre_dec_t* self);
void js_node_pre_dec_exec(struct js_node_pre_dec_t* self, struct js_context_t* context);

struct js_node_pos_inc_t* js_node_pos_inc_new(struct js_node_t* node);
void js_node_pos_inc_free(struct js_node_pos_inc_t* self);
void js_node_pos_inc_head(struct js_node_pos_inc_t* self);
void js_node_pos_inc_body(struct js_node_pos_inc_t* self);
void js_node_pos_inc_exec(struct js_node_pos_inc_t* self, struct js_context_t* context);

struct js_node_pos_dec_t* js_node_pos_dec_new(struct js_node_t* node);
void js_node_pos_dec_free(struct js_node_pos_dec_t* self);
void js_node_pos_dec_head(struct js_node_pos_dec_t* self);
void js_node_pos_dec_body(struct js_node_pos_dec_t* self);
void js_node_pos_dec_exec(struct js_node_pos_dec_t* self, struct js_context_t* context);

struct js_node_field_t* js_node_field_new(struct js_node_t* node, struct js_node_id_t* value);
void js_node_field_free(struct js_node_field_t* self);
void js_node_field_head(struct js_node_field_t* self);
void js_node_field_body(struct js_node_field_t* self);
void js_node_field_exec(struct js_node_field_t* self, struct js_context_t* context);

struct js_node_array_t* js_node_array_new(struct js_node_t* node, struct js_node_t* value);
void js_node_array_free(struct js_node_array_t* self);
void js_node_array_head(struct js_node_array_t* self);
void js_node_array_body(struct js_node_array_t* self);
void js_node_array_exec(struct js_node_array_t* self, struct js_context_t* context);

struct js_node_call_t* js_node_call_new(struct js_node_t* node, struct js_node_t* value);
void js_node_call_free(struct js_node_call_t* self);
void js_node_call_head(struct js_node_call_t* self);
void js_node_call_body(struct js_node_call_t* self);
void js_node_call_exec(struct js_node_call_t* self, struct js_context_t* context);

struct js_node_return_t* js_node_return_new(struct js_node_t* expression);
void js_node_return_free(struct js_node_return_t* self);
void js_node_return_head(struct js_node_return_t* self);
void js_node_return_body(struct js_node_return_t* self);
void js_node_return_exec(struct js_node_return_t* self, struct js_context_t* context);

struct js_node_empty_t* js_node_empty_new();
void js_node_empty_head(struct js_node_empty_t* self);
void js_node_empty_body(struct js_node_empty_t* self);
void js_node_empty_exec(struct js_node_empty_t* self, struct js_context_t* context);

struct js_node_break_t* js_node_break_new();
void js_node_break_head(struct js_node_break_t* self);
void js_node_break_body(struct js_node_break_t* self);
void js_node_break_exec(struct js_node_break_t* self, struct js_context_t* context);

struct js_node_continue_t* js_node_continue_new();
void js_node_continue_head(struct js_node_continue_t* self);
void js_node_continue_body(struct js_node_continue_t* self);
void js_node_continue_exec(struct js_node_continue_t* self, struct js_context_t* context);

struct js_node_var_item_t* js_node_var_item_new(struct js_node_id_t* name, struct js_node_t* value);
void js_node_var_item_free(struct js_node_var_item_t* self);

struct js_node_function_t* js_node_function_new(struct js_node_id_t* name, struct js_node_t* params, struct js_node_t* statement);
void js_node_function_free(struct js_node_function_t* self);

struct js_node_if_t* js_node_if_new(struct js_node_t* expression, struct js_node_t* statement, struct js_node_t* else_statement);
void js_node_if_free(struct js_node_if_t* self);
void js_node_if_head(struct js_node_if_t* self);
void js_node_if_body(struct js_node_if_t* self);
void js_node_if_exec(struct js_node_if_t* self, struct js_context_t* context);

struct js_node_while_t* js_node_while_new(struct js_node_t* expression, struct js_node_t* statement);
void js_node_while_free(struct js_node_while_t* self);
void js_node_while_head(struct js_node_while_t* self);
void js_node_while_body(struct js_node_while_t* self);
void js_node_while_exec(struct js_node_while_t* self, struct js_context_t* context);
