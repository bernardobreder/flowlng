
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
#define JS_TOKEN_CLASS 297
#define JS_TOKEN_EXTENDS 298
#define JS_TOKEN_CONSTRUCTOR 299

#define JS_NODE_ERROR 1
#define JS_NODE_ID 2
#define JS_NODE_EMPTY 3
#define JS_NODE_FUNCTION 4
#define JS_NODE_CLASS 47
#define JS_NODE_CONSTRUCTOR 48
#define JS_NODE_FIELD 49
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
#define JS_NODE_GET 44
#define JS_NODE_ARRAY 45
#define JS_NODE_CALL 46
#define JS_NODE_PARAM 47
#define JS_NODE_STMT_EXP 50

#define JS_VALUE_NULL 0
#define JS_VALUE_BOOL 1
#define JS_VALUE_NUM 2
#define JS_VALUE_INT 3
#define JS_VALUE_STR 4
#define JS_VALUE_OBJ 5
#define JS_VALUE_CLASS 6
#define JS_VALUE_FUNC 7
#define JS_VALUE_COBJ 8

typedef uint8 js_bool;
typedef double js_num;
typedef long js_int;
typedef size_t js_size;
typedef char js_char;
typedef js_char* js_str;
typedef void* js_obj;

struct js_token_t {
    unsigned short type;
    char* word;
    size_t length;
    js_hash hash;
    int line;
    int column;
    struct js_token_t* next;
};

struct js_parser_t {
    struct flow_memory_t* memory;
    struct js_token_t* tokens;
    struct js_token_t* token;
};

struct js_compiler_t {
    struct flow_memory_t* memory;
    struct js_compiler_error_t* error;
};

struct js_compiler_error_t {
    struct js_compiler_error_t* next;
    char* message;
};

struct js_context_t {
    struct flow_memory_t* memory;
    struct js_value_t* value;
};

struct js_value_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_t* next;
};

struct js_value_null_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_t* next;
};

struct js_value_bool_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_t* next;
    js_bool value;
};

struct js_value_int_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_t* next;
    js_int value;
    js_str str_value;
    js_size str_size_value;
};

struct js_value_num_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_t* next;
    js_num value;
    js_str str_value;
    js_size str_size_value;
};

struct js_value_str_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_t* next;
    js_str value;
    size_t length;
    js_hash hash;
};

struct js_value_func_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_t* next;
    struct js_node_function_t* function;
};

struct js_value_class_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_class_t* next;
    struct js_value_str_t* class_str;
    struct js_value_obj_entry_t* constructor;
    struct js_value_obj_entry_t* desctructor;
    struct js_value_obj_entry_t* field;
    struct js_value_obj_entry_t* function;
    struct js_value_func_t* to_str_func;
    struct js_value_func_t* to_int_func;
    struct js_value_func_t* clone_func;
    struct js_value_func_t* equal_func;
};

struct js_value_obj_entry_t {
    char* word;
    size_t length;
    js_hash hash;
    struct js_value_t* value;
    struct js_value_obj_entry_t* next;
};

struct js_value_obj_t {
    size_t ref_counter;
    unsigned char type;
    struct js_value_t* next;
    struct js_value_obj_t* parent;
    struct js_value_obj_entry_t* field;
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
    size_t length;
    js_hash hash;
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
    struct js_node_id_t* name;
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

struct js_node_get_t {
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
    struct js_node_t* param;
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

struct js_node_param_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_id_t* name;
};

struct js_node_function_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_id_t* name;
    struct js_node_param_t* param;
    struct js_node_t* statement;
};

struct js_node_constructor_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* params;
    struct js_node_t* statement;
};

struct js_node_field_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_id_t* name;
};

struct js_node_class_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_id_t* name;
    struct js_node_id_t* extends;
    struct js_node_t* constructor;
    struct js_node_t* field;
    struct js_node_t* method;
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

struct js_node_stmtexp_t {
    unsigned char type;
    struct js_node_t* next;
    struct js_node_t* expression;
};


#define js_free(OBJ) free(OBJ)
#define js_free_nullable(OBJ) if (OBJ) { free(OBJ); }
#define js_str_hash_prime 13
#define js_num_precision 0.0000001

struct js_compiler_error_t* js_compiler_error_new(struct flow_memory_t* memory, char* message);
void js_compiler_error_free(struct js_compiler_t* self);

struct js_compiler_t* js_compiler_new(struct flow_memory_t* memory);
void js_compiler_free(struct js_compiler_t* self);
struct js_node_t* js_compiler_exec(struct js_compiler_t* self, char * source);
js_bool js_compiler_error_empty(struct js_compiler_t* self);
void js_compiler_error_push(struct js_compiler_t* self, struct js_compiler_error_t* error);
struct js_compiler_error_t* js_compiler_error_pop(struct js_compiler_t* self);

void js_token_free(struct js_token_t* self);

void js_tokens_free(struct js_token_t* self);

struct js_token_t* js_lexer(struct flow_memory_t* memory, const char* text);

struct js_parser_t* js_parser_new(struct flow_memory_t* memory, struct js_token_t* tokens);
void js_parser_free(struct js_parser_t* self);
struct js_node_t* js_parser(struct js_parser_t* self);

struct js_context_t* js_context_new(struct flow_memory_t* memory);
void js_context_free(struct js_context_t* self);
js_bool js_context_empty(struct js_context_t* self);
void js_context_push(struct js_context_t* self, struct js_value_t* value);
struct js_value_t* js_context_pop(struct js_context_t* self);
struct js_value_t* js_context_peek_index(struct js_context_t* self, js_size index);
#define js_context_push_typed(CONTEXT, VALUE) js_context_push(CONTEXT, (struct js_value_t*) VALUE)
#define js_context_pop_def(CONTEXT, NAME) struct js_value_t* NAME = js_context_pop(CONTEXT);
#define js_context_memory(CONTEXT) CONTEXT->memory
#define js_context_peek_def(CONTEXT, NAME) \
        struct js_value_t* NAME = (struct js_value_t*) CONTEXT->value;
#define js_context_peek_index_def(CONTEXT, NAME, INDEX) \
        struct js_value_t* NAME = js_context_peek_index(CONTEXT, INDEX);
#define js_context_peek_is_obj(CONTEXT) js_value_is_obj(CONTEXT->value)
#define js_context_peek_obj_def(CONTEXT, NAME) \
        struct js_value_obj_t* NAME = (struct js_value_obj_t*) CONTEXT->value;
#define js_context_peek_func_def(CONTEXT, NAME) \
        struct js_value_func_t* NAME = (struct js_value_func_t*) CONTEXT->value;
#define js_context_peek_obj_or_push_null(CONTEXT, NAME) \
        struct js_value_obj_t* NAME = (struct js_value_obj_t*) CONTEXT->value; \
        if (!js_value_is_obj(NAME)) { js_context_push_typed(context, js_value_null()); return; }

void js_value_free(struct js_value_t* self);
void js_value_null_new(struct js_context_t* context);
void js_value_true_new(struct js_context_t* context);
void js_value_false_new(struct js_context_t* context);
js_bool js_value_is_equal(struct js_context_t* context, struct js_value_t* left, struct js_value_t* right);
js_int js_value_is_compare(struct js_context_t* context, struct js_value_t* left, struct js_value_t* right);
#define js_value_def(NAME, VALUE) struct js_value_t* NAME = VALUE;
//#define js_value_bool(FLAG) ((FLAG) ? js_value_true() : js_value_false())
#define js_value_exec_def(NAME, NODE) js_node_exec_typed(NODE, context); js_context_peek_def(context, NAME);
#define js_value_free_typed(VALUE) js_value_free((struct js_value_t*)VALUE)
#define js_value_release(VALUE) \
        if (--(VALUE)->ref_counter == 0) { js_value_free_typed(VALUE); }
#define js_value_retain(VALUE) \
        (VALUE)->ref_counter++;
#define js_value_is_bool_true(VALUE) js_value_is_bool(VALUE) && js_value_bool_value(VALUE)
#define js_value_is_bool_false(VALUE) js_value_is_bool(VALUE) && !js_value_bool_value(VALUE)
#define js_value_bool_value(VALUE) ((struct js_value_bool_t*)VALUE)->value
#define js_value_bool_def(NAME, VALUE) js_bool NAME = js_value_is_bool_true(VALUE);
//#define js_value_func_def_new(NAME, CONTEXT, FUNCTION_NODE) js_value_def(NAME, js_value_func_new(context, FUNCTION_NODE));
#define js_value_class_def_new(CONTEXT, NAME, CLASS) js_value_def(NAME, js_value_class_new(context, CLASS));
#define js_value_is_true(VALUE) js_value_is_bool_true(VALUE)
#define js_value_is_false(VALUE) js_value_is_bool_false(VALUE)
#define js_value_is_null(VALUE) (VALUE->type == JS_VALUE_NULL)
#define js_value_is_bool(VALUE) (VALUE->type == JS_VALUE_BOOL)
#define js_value_is_func(VALUE) (VALUE->type == JS_VALUE_FUNC)
#define js_value_is_int(VALUE) (VALUE->type == JS_VALUE_INT)
#define js_value_is_obj(VALUE) (VALUE->type == JS_VALUE_OBJ || VALUE->type == JS_VALUE_COBJ)
#define js_value_is_num(VALUE) (VALUE->type == JS_VALUE_NUM)
#define js_value_is_numint(VALUE) (VALUE->type == JS_VALUE_INT || VALUE->type == JS_VALUE_NUM)
#define js_value_is_str(VALUE) (VALUE->type == JS_VALUE_STR)
#define js_value_cast(VALUE) ((struct js_value_t*)(VALUE))
#define js_value_obj_cast(VALUE) ((struct js_value_obj_t*)(VALUE))
#define js_value_func_cast(VALUE) ((struct js_value_func_t*)(VALUE))
#define js_value_int_def(NAME, VALUE) js_int NAME = js_value_int_value(VALUE);
#define js_value_int_value(VALUE) ((struct js_value_int_t*)VALUE)->value
//#define js_value_int_new_def(CONTEXT, NAME, VALUE) struct js_value_t* NAME = (struct js_value_t*) js_value_int_new(CONTEXT, VALUE);
#define js_value_num_def(NAME, VALUE) js_num NAME = js_value_num_value(VALUE);
#define js_value_num_value(VALUE) ((struct js_value_num_t*)VALUE)->value
#define js_value_num_new_def(CONTEXT, NAME, VALUE) struct js_value_t* NAME = (struct js_value_t*) js_value_num_new(CONTEXT, VALUE);
#define js_value_numint_def(NAME, VALUE) js_num NAME = js_value_is_num(VALUE) ? js_value_num_value(VALUE) : (js_num)js_value_int_value(VALUE);
#define js_value_str_def(NAME, VALUE) js_str NAME = js_value_str_value(VALUE);
#define js_value_str_value(VALUE) ((struct js_value_str_t*)VALUE)->value
#define js_value_str_len_def(NAME, VALUE) js_size NAME = js_value_str_len(VALUE);
#define js_value_str_hash_def(NAME, VALUE) js_size NAME = js_value_str_hash(VALUE);
#define js_value_str_new_def(CONTEXT, NAME, VALUE, LENGTH, HASH) \
        struct js_value_t* NAME = js_value_str_new(CONTEXT, VALUE, LENGTH, HASH);
#define js_value_to_str_def(NAME, VALUE) \
        const js_str NAME = js_value_is_str(VALUE) ? js_value_str_value(VALUE) : js_value_str_ansi(VALUE);

void js_value_int_new(struct js_context_t* context, js_int value);
void js_value_int_free(struct js_value_int_t* self);
void js_value_int_sum(struct js_context_t* context);

void js_value_num_new(struct js_context_t* context, js_num value);
void js_value_num_free(struct js_value_num_t* self);
void js_value_num_sum(struct js_context_t* context);

void js_value_str_new(struct js_context_t* context, char* value, size_t length, js_hash hash);
void js_value_str_free(struct js_value_str_t* self);
void js_value_obj_new(struct js_context_t* context);
#define js_value_obj_new_def(CONTEXT, NAME) struct js_value_obj_t* NAME = js_value_obj_new(CONTEXT);
#define js_value_obj_def(NAME, VALUE) struct js_value_obj_t* NAME = (struct js_value_obj_t*) VALUE;
void js_value_obj_free(struct js_value_obj_t* self);
void js_value_obj_get(struct js_context_t* context, struct js_value_obj_t* self, struct js_node_id_t* name);
void js_value_obj_set(struct js_context_t* context, struct js_value_obj_t* self, struct js_node_id_t* name, struct js_value_t* value);
void js_value_func_new(struct js_context_t* context, struct js_node_function_t* function);
void js_value_func_free(struct js_value_func_t* self);
void js_value_class_new(struct js_context_t* context, struct js_value_str_t* class_str);
void js_value_class_free(struct js_value_class_t* self);
const js_str js_value_str_ansi(struct js_value_t* self);
js_size js_value_str_len(struct js_value_t* self);
js_hash js_value_str_hash(struct js_value_t* self);
js_hash js_value_str_hash_concat(struct js_value_t* left, struct js_value_t* right);

void js_value_str_concat(struct js_context_t* context);

#define js_node_cast(VALUE) ((struct js_node_t*)(VALUE))
#define js_node_free_self(NODE) \
        if (self->next) js_node_free_typed(self->next); \
        flow_memory_item_free(self);
#define js_node_free_typed(NODE) js_node_free((struct js_node_t*)NODE)
#define js_node_head_typed(NODE, COMPILER) js_node_head((struct js_node_t*)NODE, COMPILER)
#define js_node_body_typed(NODE, COMPILER) js_node_body((struct js_node_t*)NODE, COMPILER)
#define js_node_exec_typed(NODE, CONTEXT) js_node_exec((struct js_node_t*)NODE, CONTEXT)
//void js_node_compile(struct js_node_t* self);
void js_node_free(struct js_node_t* self);
void js_node_head(struct js_node_t* self, struct js_compiler_t* compiler);
void js_node_body(struct js_node_t* self, struct js_compiler_t* compiler);
void js_node_exec(struct js_node_t* self, struct js_context_t* context);

struct js_node_error_t* js_node_error_new(struct flow_memory_t* memory, char* message, char* word, size_t line, size_t column, struct js_node_error_t* next);
void js_node_error_free(struct js_node_error_t* self);
struct js_node_error_t* js_nodes_error_is(struct js_node_t* self);
struct js_node_error_t* js_node_error_revert(struct js_node_error_t* self);
void js_node_error_print(struct js_node_error_t* self);
#define js_node_error_type(SELF) ((struct js_node_error_t*)SELF)
#define js_node_error_is(SELF) (SELF)->type == JS_NODE_ERROR
#define js_node_error_is_not(SELF) (SELF)->type != JS_NODE_ERROR

struct js_node_id_t* js_node_id_new(struct flow_memory_t* memory, char* word, size_t length, js_hash hash);
void js_node_id_free(struct js_node_id_t* self);
void js_node_id_head(struct js_node_id_t* self, struct js_compiler_t* compiler);
void js_node_id_body(struct js_node_id_t* self, struct js_compiler_t* compiler);
void js_node_id_exec(struct js_node_id_t* self, struct js_context_t* context);

struct js_node_string_t* js_node_string_new(struct flow_memory_t* memory, char* word, size_t length);
void js_node_string_free(struct js_node_string_t* self);
void js_node_string_head(struct js_node_string_t* self, struct js_compiler_t* compiler);
void js_node_string_body(struct js_node_string_t* self, struct js_compiler_t* compiler);
void js_node_string_exec(struct js_node_string_t* self, struct js_context_t* context);

struct js_node_num_t* js_node_num_new(struct flow_memory_t* memory, double value);
void js_node_num_free(struct js_node_num_t* self);
void js_node_num_head(struct js_node_num_t* self, struct js_compiler_t* compiler);
void js_node_num_body(struct js_node_num_t* self, struct js_compiler_t* compiler);
void js_node_num_exec(struct js_node_num_t* self, struct js_context_t* context);

struct js_node_int_t* js_node_int_new(struct flow_memory_t* memory, int value);
void js_node_int_free(struct js_node_int_t* self);
void js_node_int_head(struct js_node_int_t* self, struct js_compiler_t* compiler);
void js_node_int_body(struct js_node_int_t* self, struct js_compiler_t* compiler);
void js_node_int_exec(struct js_node_int_t* self, struct js_context_t* context);

struct js_node_true_t* js_node_true_new(struct flow_memory_t* memory);
void js_node_true_free(struct js_node_true_t* self);
void js_node_true_head(struct js_node_true_t* self, struct js_compiler_t* compiler);
void js_node_true_body(struct js_node_true_t* self, struct js_compiler_t* compiler);
void js_node_true_exec(struct js_node_true_t* self, struct js_context_t* context);

struct js_node_false_t* js_node_false_new(struct flow_memory_t* memory);
void js_node_false_free(struct js_node_false_t* self);
void js_node_false_head(struct js_node_false_t* self, struct js_compiler_t* compiler);
void js_node_false_body(struct js_node_false_t* self, struct js_compiler_t* compiler);
void js_node_false_exec(struct js_node_false_t* self, struct js_context_t* context);

struct js_node_null_t* js_node_null_new(struct flow_memory_t* memory);
void js_node_null_free(struct js_node_null_t* self);
void js_node_null_head(struct js_node_null_t* self, struct js_compiler_t* compiler);
void js_node_null_body(struct js_node_null_t* self, struct js_compiler_t* compiler);
void js_node_null_exec(struct js_node_null_t* self, struct js_context_t* context);

struct js_node_this_t* js_node_this_new(struct flow_memory_t* memory);
void js_node_this_free(struct js_node_this_t* self);
void js_node_this_head(struct js_node_this_t* self, struct js_compiler_t* compiler);
void js_node_this_body(struct js_node_this_t* self, struct js_compiler_t* compiler);
void js_node_this_exec(struct js_node_this_t* self, struct js_context_t* context);

struct js_node_super_t* js_node_super_new(struct flow_memory_t* memory);
void js_node_super_free(struct js_node_super_t* self);
void js_node_super_head(struct js_node_super_t* self, struct js_compiler_t* compiler);
void js_node_super_body(struct js_node_super_t* self, struct js_compiler_t* compiler);
void js_node_super_exec(struct js_node_super_t* self, struct js_context_t* context);

struct js_node_assignment_t* js_node_assignment_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_id_t* name, struct js_node_t* value);
void js_node_assignment_free(struct js_node_assignment_t* self);
void js_node_assignment_head(struct js_node_assignment_t* self, struct js_compiler_t* compiler);
void js_node_assignment_body(struct js_node_assignment_t* self, struct js_compiler_t* compiler);
void js_node_assignment_exec(struct js_node_assignment_t* self, struct js_context_t* context);

struct js_node_ternary_t* js_node_ternary_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* true_value, struct js_node_t* false_value);
void js_node_ternary_free(struct js_node_ternary_t* self);
void js_node_ternary_head(struct js_node_ternary_t* self, struct js_compiler_t* compiler);
void js_node_ternary_body(struct js_node_ternary_t* self, struct js_compiler_t* compiler);
void js_node_ternary_exec(struct js_node_ternary_t* self, struct js_context_t* context);

struct js_node_or_t* js_node_or_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_or_free(struct js_node_or_t* self);
void js_node_or_head(struct js_node_or_t* self, struct js_compiler_t* compiler);
void js_node_or_body(struct js_node_or_t* self, struct js_compiler_t* compiler);
void js_node_or_exec(struct js_node_or_t* self, struct js_context_t* context);

struct js_node_and_t* js_node_and_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_and_free(struct js_node_and_t* self);
void js_node_and_head(struct js_node_and_t* self, struct js_compiler_t* compiler);
void js_node_and_body(struct js_node_and_t* self, struct js_compiler_t* compiler);
void js_node_and_exec(struct js_node_and_t* self, struct js_context_t* context);

struct js_node_bitwise_or_t* js_node_bitwise_or_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_bitwise_or_free(struct js_node_bitwise_or_t* self);
void js_node_bitwise_or_head(struct js_node_bitwise_or_t* self, struct js_compiler_t* compiler);
void js_node_bitwise_or_body(struct js_node_bitwise_or_t* self, struct js_compiler_t* compiler);
void js_node_bitwise_or_exec(struct js_node_bitwise_or_t* self, struct js_context_t* context);

struct js_node_bitwise_xor_t* js_node_bitwise_xor_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_bitwise_xor_free(struct js_node_bitwise_xor_t* self);
void js_node_bitwise_xor_head(struct js_node_bitwise_xor_t* self, struct js_compiler_t* compiler);
void js_node_bitwise_xor_body(struct js_node_bitwise_xor_t* self, struct js_compiler_t* compiler);
void js_node_bitwise_xor_exec(struct js_node_bitwise_xor_t* self, struct js_context_t* context);

struct js_node_bitwise_and_t* js_node_bitwise_and_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_bitwise_and_free(struct js_node_bitwise_and_t* self);
void js_node_bitwise_and_head(struct js_node_bitwise_and_t* self, struct js_compiler_t* compiler);
void js_node_bitwise_and_body(struct js_node_bitwise_and_t* self, struct js_compiler_t* compiler);
void js_node_bitwise_and_exec(struct js_node_bitwise_and_t* self, struct js_context_t* context);

struct js_node_equal_t* js_node_equal_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_equal_free(struct js_node_equal_t* self);
void js_node_equal_head(struct js_node_equal_t* self, struct js_compiler_t* compiler);
void js_node_equal_body(struct js_node_equal_t* self, struct js_compiler_t* compiler);
void js_node_equal_exec(struct js_node_equal_t* self, struct js_context_t* context);

struct js_node_not_equal_t* js_node_not_equal_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_not_equal_free(struct js_node_not_equal_t* self);
void js_node_not_equal_head(struct js_node_not_equal_t* self, struct js_compiler_t* compiler);
void js_node_not_equal_body(struct js_node_not_equal_t* self, struct js_compiler_t* compiler);
void js_node_not_equal_exec(struct js_node_not_equal_t* self, struct js_context_t* context);

struct js_node_lower_equal_t* js_node_lower_equal_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_lower_equal_free(struct js_node_lower_equal_t* self);
void js_node_lower_equal_head(struct js_node_lower_equal_t* self, struct js_compiler_t* compiler);
void js_node_lower_equal_body(struct js_node_lower_equal_t* self, struct js_compiler_t* compiler);
void js_node_lower_equal_exec(struct js_node_lower_equal_t* self, struct js_context_t* context);

struct js_node_greater_equal_t* js_node_greater_equal_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_greater_equal_free(struct js_node_greater_equal_t* self);
void js_node_greater_equal_head(struct js_node_greater_equal_t* self, struct js_compiler_t* compiler);
void js_node_greater_equal_body(struct js_node_greater_equal_t* self, struct js_compiler_t* compiler);
void js_node_greater_equal_exec(struct js_node_greater_equal_t* self, struct js_context_t* context);

struct js_node_lower_than_t* js_node_lower_than_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_lower_than_free(struct js_node_lower_than_t* self);
void js_node_lower_than_head(struct js_node_lower_than_t* self, struct js_compiler_t* compiler);
void js_node_lower_than_body(struct js_node_lower_than_t* self, struct js_compiler_t* compiler);
void js_node_lower_than_exec(struct js_node_lower_than_t* self, struct js_context_t* context);

struct js_node_greater_than_t* js_node_greater_than_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_greater_than_free(struct js_node_greater_than_t* self);
void js_node_greater_than_head(struct js_node_greater_than_t* self, struct js_compiler_t* compiler);
void js_node_greater_than_body(struct js_node_greater_than_t* self, struct js_compiler_t* compiler);
void js_node_greater_than_exec(struct js_node_greater_than_t* self, struct js_context_t* context);

struct js_node_shift_left_t* js_node_shift_left_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_shift_left_free(struct js_node_shift_left_t* self);
void js_node_shift_left_head(struct js_node_shift_left_t* self, struct js_compiler_t* compiler);
void js_node_shift_left_body(struct js_node_shift_left_t* self, struct js_compiler_t* compiler);
void js_node_shift_left_exec(struct js_node_shift_left_t* self, struct js_context_t* context);

struct js_node_shift_right_t* js_node_shift_right_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_shift_right_free(struct js_node_shift_right_t* self);
void js_node_shift_right_head(struct js_node_shift_right_t* self, struct js_compiler_t* compiler);
void js_node_shift_right_body(struct js_node_shift_right_t* self, struct js_compiler_t* compiler);
void js_node_shift_right_exec(struct js_node_shift_right_t* self, struct js_context_t* context);

struct js_node_sum_t* js_node_sum_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_sum_free(struct js_node_sum_t* self);
void js_node_sum_head(struct js_node_sum_t* self, struct js_compiler_t* compiler);
void js_node_sum_body(struct js_node_sum_t* self, struct js_compiler_t* compiler);
void js_node_sum_exec(struct js_node_sum_t* self, struct js_context_t* context);

struct js_node_sub_t* js_node_sub_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_sub_free(struct js_node_sub_t* self);
void js_node_sub_head(struct js_node_sub_t* self, struct js_compiler_t* compiler);
void js_node_sub_body(struct js_node_sub_t* self, struct js_compiler_t* compiler);
void js_node_sub_exec(struct js_node_sub_t* self, struct js_context_t* context);

struct js_node_mul_t* js_node_mul_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_mul_free(struct js_node_mul_t* self);
void js_node_mul_head(struct js_node_mul_t* self, struct js_compiler_t* compiler);
void js_node_mul_body(struct js_node_mul_t* self, struct js_compiler_t* compiler);
void js_node_mul_exec(struct js_node_mul_t* self, struct js_context_t* context);

struct js_node_div_t* js_node_div_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_div_free(struct js_node_div_t* self);
void js_node_div_head(struct js_node_div_t* self, struct js_compiler_t* compiler);
void js_node_div_body(struct js_node_div_t* self, struct js_compiler_t* compiler);
void js_node_div_exec(struct js_node_div_t* self, struct js_context_t* context);

struct js_node_not_t* js_node_not_new(struct flow_memory_t* memory, struct js_node_t* node);
void js_node_not_free(struct js_node_not_t* self);
void js_node_not_head(struct js_node_not_t* self, struct js_compiler_t* compiler);
void js_node_not_body(struct js_node_not_t* self, struct js_compiler_t* compiler);
void js_node_not_exec(struct js_node_not_t* self, struct js_context_t* context);

struct js_node_neg_t* js_node_neg_new(struct flow_memory_t* memory, struct js_node_t* node);
void js_node_neg_free(struct js_node_neg_t* self);
void js_node_neg_head(struct js_node_neg_t* self, struct js_compiler_t* compiler);
void js_node_neg_body(struct js_node_neg_t* self, struct js_compiler_t* compiler);
void js_node_neg_exec(struct js_node_neg_t* self, struct js_context_t* context);

struct js_node_pre_inc_t* js_node_pre_inc_new(struct flow_memory_t* memory, struct js_node_t* node);
void js_node_pre_inc_free(struct js_node_pre_inc_t* self);
void js_node_pre_inc_head(struct js_node_pre_inc_t* self, struct js_compiler_t* compiler);
void js_node_pre_inc_body(struct js_node_pre_inc_t* self, struct js_compiler_t* compiler);
void js_node_pre_inc_exec(struct js_node_pre_inc_t* self, struct js_context_t* context);

struct js_node_pre_dec_t* js_node_pre_dec_new(struct flow_memory_t* memory, struct js_node_t* node);
void js_node_pre_dec_free(struct js_node_pre_dec_t* self);
void js_node_pre_dec_head(struct js_node_pre_dec_t* self, struct js_compiler_t* compiler);
void js_node_pre_dec_body(struct js_node_pre_dec_t* self, struct js_compiler_t* compiler);
void js_node_pre_dec_exec(struct js_node_pre_dec_t* self, struct js_context_t* context);

struct js_node_pos_inc_t* js_node_pos_inc_new(struct flow_memory_t* memory, struct js_node_t* node);
void js_node_pos_inc_free(struct js_node_pos_inc_t* self);
void js_node_pos_inc_head(struct js_node_pos_inc_t* self, struct js_compiler_t* compiler);
void js_node_pos_inc_body(struct js_node_pos_inc_t* self, struct js_compiler_t* compiler);
void js_node_pos_inc_exec(struct js_node_pos_inc_t* self, struct js_context_t* context);

struct js_node_pos_dec_t* js_node_pos_dec_new(struct flow_memory_t* memory, struct js_node_t* node);
void js_node_pos_dec_free(struct js_node_pos_dec_t* self);
void js_node_pos_dec_head(struct js_node_pos_dec_t* self, struct js_compiler_t* compiler);
void js_node_pos_dec_body(struct js_node_pos_dec_t* self, struct js_compiler_t* compiler);
void js_node_pos_dec_exec(struct js_node_pos_dec_t* self, struct js_context_t* context);

struct js_node_get_t* js_node_get_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_id_t* value);
void js_node_get_free(struct js_node_get_t* self);
void js_node_get_head(struct js_node_get_t* self, struct js_compiler_t* compiler);
void js_node_get_body(struct js_node_get_t* self, struct js_compiler_t* compiler);
void js_node_get_exec(struct js_node_get_t* self, struct js_context_t* context);

struct js_node_array_t* js_node_array_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value);
void js_node_array_free(struct js_node_array_t* self);
void js_node_array_head(struct js_node_array_t* self, struct js_compiler_t* compiler);
void js_node_array_body(struct js_node_array_t* self, struct js_compiler_t* compiler);
void js_node_array_exec(struct js_node_array_t* self, struct js_context_t* context);

struct js_node_call_t* js_node_call_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* param);
void js_node_call_free(struct js_node_call_t* self);
void js_node_call_head(struct js_node_call_t* self, struct js_compiler_t* compiler);
void js_node_call_body(struct js_node_call_t* self, struct js_compiler_t* compiler);
void js_node_call_exec(struct js_node_call_t* self, struct js_context_t* context);

struct js_node_return_t* js_node_return_new(struct flow_memory_t* memory, struct js_node_t* expression);
void js_node_return_free(struct js_node_return_t* self);
void js_node_return_head(struct js_node_return_t* self, struct js_compiler_t* compiler);
void js_node_return_body(struct js_node_return_t* self, struct js_compiler_t* compiler);
void js_node_return_exec(struct js_node_return_t* self, struct js_context_t* context);

struct js_node_empty_t* js_node_empty_new(struct flow_memory_t* memory);
void js_node_empty_free(struct js_node_empty_t* self);
void js_node_empty_head(struct js_node_empty_t* self, struct js_compiler_t* compiler);
void js_node_empty_body(struct js_node_empty_t* self, struct js_compiler_t* compiler);
void js_node_empty_exec(struct js_node_empty_t* self, struct js_context_t* context);

struct js_node_break_t* js_node_break_new(struct flow_memory_t* memory);
void js_node_break_free(struct js_node_break_t* self);
void js_node_break_head(struct js_node_break_t* self, struct js_compiler_t* compiler);
void js_node_break_body(struct js_node_break_t* self, struct js_compiler_t* compiler);
void js_node_break_exec(struct js_node_break_t* self, struct js_context_t* context);

struct js_node_continue_t* js_node_continue_new(struct flow_memory_t* memory);
void js_node_continue_free(struct js_node_continue_t* self);
void js_node_continue_head(struct js_node_continue_t* self, struct js_compiler_t* compiler);
void js_node_continue_body(struct js_node_continue_t* self, struct js_compiler_t* compiler);
void js_node_continue_exec(struct js_node_continue_t* self, struct js_context_t* context);

struct js_node_stmtexp_t* js_node_stmtexp_new(struct flow_memory_t* memory, struct js_node_t* expression);
void js_node_stmtexp_free(struct js_node_stmtexp_t* self);
void js_node_stmtexp_head(struct js_node_stmtexp_t* self, struct js_compiler_t* compiler);
void js_node_stmtexp_body(struct js_node_stmtexp_t* self, struct js_compiler_t* compiler);
void js_node_stmtexp_exec(struct js_node_stmtexp_t* self, struct js_context_t* context);

struct js_node_var_item_t* js_node_var_item_new(struct flow_memory_t* memory, struct js_node_id_t* name, struct js_node_t* value);
void js_node_var_item_free(struct js_node_var_item_t* self);
void js_node_var_item_head(struct js_node_var_item_t* self, struct js_compiler_t* compiler);
void js_node_var_item_body(struct js_node_var_item_t* self, struct js_compiler_t* compiler);
void js_node_var_item_exec(struct js_node_var_item_t* self, struct js_context_t* context);

struct js_node_class_t* js_node_class_new(struct flow_memory_t* memory, struct js_node_id_t* name, struct js_node_id_t* extends, struct js_node_t* constructor, struct js_node_t* field, struct js_node_t* method);
void js_node_class_free(struct js_node_class_t* self);
void js_node_class_head(struct js_node_class_t* self, struct js_compiler_t* compiler);
void js_node_class_body(struct js_node_class_t* self, struct js_compiler_t* compiler);
void js_node_class_exec(struct js_node_class_t* self, struct js_context_t* context);

struct js_node_param_t* js_node_param_new(struct flow_memory_t* memory, struct js_node_id_t* name);
void js_node_param_free(struct js_node_param_t* self);
void js_node_param_head(struct js_node_param_t* self, struct js_compiler_t* compiler);
void js_node_param_body(struct js_node_param_t* self, struct js_compiler_t* compiler);
void js_node_param_exec(struct js_node_param_t* self, struct js_context_t* context);

struct js_node_function_t* js_node_function_new(struct flow_memory_t* memory, struct js_node_id_t* name, struct js_node_param_t* param, struct js_node_t* statement);
void js_node_function_free(struct js_node_function_t* self);
void js_node_function_head(struct js_node_function_t* self, struct js_compiler_t* compiler);
void js_node_function_body(struct js_node_function_t* self, struct js_compiler_t* compiler);
void js_node_function_exec(struct js_node_function_t* self, struct js_context_t* context);

struct js_node_constructor_t* js_node_constructor_new(struct flow_memory_t* memory, struct js_node_t* params, struct js_node_t* statement);
void js_node_constructor_free(struct js_node_constructor_t* self);
void js_node_constructor_head(struct js_node_constructor_t* self, struct js_compiler_t* compiler);
void js_node_constructor_body(struct js_node_constructor_t* self, struct js_compiler_t* compiler);
void js_node_constructor_exec(struct js_node_constructor_t* self, struct js_context_t* context);

struct js_node_field_t* js_node_field_new(struct flow_memory_t* memory, struct js_node_id_t* name);
void js_node_field_free(struct js_node_field_t* self);
void js_node_field_head(struct js_node_field_t* self, struct js_compiler_t* compiler);
void js_node_field_body(struct js_node_field_t* self, struct js_compiler_t* compiler);
void js_node_field_exec(struct js_node_field_t* self, struct js_context_t* context);

struct js_node_stmtexp_t* js_node_stmtexp_new(struct flow_memory_t* memory, struct js_node_t* expression);
void js_node_stmtexp_free(struct js_node_stmtexp_t* self);
void js_node_stmtexp_head(struct js_node_stmtexp_t* self, struct js_compiler_t* compiler);
void js_node_stmtexp_body(struct js_node_stmtexp_t* self, struct js_compiler_t* compiler);
void js_node_stmtexp_exec(struct js_node_stmtexp_t* self, struct js_context_t* context);

struct js_node_if_t* js_node_if_new(struct flow_memory_t* memory, struct js_node_t* expression, struct js_node_t* statement, struct js_node_t* else_statement);
void js_node_if_free(struct js_node_if_t* self);
void js_node_if_head(struct js_node_if_t* self, struct js_compiler_t* compiler);
void js_node_if_body(struct js_node_if_t* self, struct js_compiler_t* compiler);
void js_node_if_exec(struct js_node_if_t* self, struct js_context_t* context);

struct js_node_while_t* js_node_while_new(struct flow_memory_t* memory, struct js_node_t* expression, struct js_node_t* statement);
void js_node_while_free(struct js_node_while_t* self);
void js_node_while_head(struct js_node_while_t* self, struct js_compiler_t* compiler);
void js_node_while_body(struct js_node_while_t* self, struct js_compiler_t* compiler);
void js_node_while_exec(struct js_node_while_t* self, struct js_context_t* context);
