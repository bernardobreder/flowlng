#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "js.h"
#include "xstring.h"

// http://hepunx.rl.ac.uk/~adye/jsspec11/llr.htm

#define js_parser_empty_new() (struct js_node_t*) js_node_empty_new(self->memory)
#define js_node_list_def(HEAD, TAIL) \
        struct js_node_t* HEAD = 0; \
        struct js_node_t* TAIL = 0;
#define js_node_def(NAME, VALUE) struct js_node_t* NAME = VALUE
#define js_node_def_token(NAME) struct js_token_t* NAME = self->token;
#define js_node_def_end() if (!head) { head = js_parser_empty_new(); }
#define js_node_add(HEAD, TAIL, NODE) \
    if (TAIL) { \
        TAIL->next = (struct js_node_t*) NODE; \
    } else { \
        HEAD = (struct js_node_t*) NODE; \
    } \
    TAIL = (struct js_node_t*) NODE; \
    while (TAIL->next) { \
        TAIL = TAIL->next; \
    } \

#define js_parser_word() self->token->word
#define js_parser_word_length() strlen(self->token->word)
#define js_parser_word_hash() js_hash_perform(self->token->word, js_parser_word_length())
#define js_parser_step() self->token = self->token->next;
#define js_parser_skip_comment() while (js_parser_type(JS_TOKEN_COMMENT)) { js_parser_step(); }
#define js_parser_next() js_parser_step(); js_parser_skip_comment();
#define js_parser_token_get()
#define js_parser_type(TYPE) (self->token->type == TYPE)
#define js_parser_type_not(TYPE) (self->token->type != TYPE)
#define js_parser_next_type(TYPE) (self->token->next->type == TYPE)
#define js_parser_next_type_not(TYPE) (self->token->next->type != TYPE)
#define js_parser_error(MEMORY, MSG, TOKEN, NEXT) (struct js_node_t*) js_node_error_new(MEMORY, strdup(MSG), strdup(TOKEN->word), TOKEN->line, TOKEN->column, (struct js_node_error_t*) NEXT)
#define js_parser_error_new(MEMORY, MSG, TOKEN, NEXT) js_node_error_new(MEMORY, strdup(MSG), strdup(TOKEN->word), TOKEN->line, TOKEN->column, (struct js_node_error_t*) NEXT)
#define js_parser_node_id() js_node_id_new(self->memory, strdup(self->token->word), js_parser_word_length(), js_parser_word_hash())
#define js_parser_node_id_free(NODE) js_node_free((struct js_node_t*) NODE)
#define js_parser_func_def(NAME, FUNC) struct js_node_t* NAME = FUNC(self);
#define js_parser_exp_def(NAME) js_parser_func_def(NAME, js_parser_expression)
#define js_parser_stmt_def(NAME) js_parser_func_def(NAME, js_parser_statement)
#define js_parser_id_def(NAME) struct js_node_id_t* js_parser_id_assign(NAME)
#define js_parser_id_def_null(NAME) struct js_node_id_t* NAME = 0;
#define js_parser_id_assign(NAME) NAME = js_parser_node_id();

#define js_parser_def_error() \
        struct js_node_error_t* error = 0;

#define js_parser_def_token() \
        struct js_token_t* token = 0;

#define js_parser_read_id(NODE, MSG) \
        token = self->token; \
        if (js_parser_type_not(JS_TOKEN_ID)) { \
            js_parser_error_goto(token, MSG, 0); \
        } \
        NODE = js_parser_node_id(); \
        js_parser_next();

#define js_parser_read_func(NODE, FUNC, MSG) \
        { token = self->token; \
        NODE = FUNC(self); \
        if (NODE && js_node_error_is(NODE)) { \
        js_node_free_typed(NODE); NODE = 0; \
        js_parser_error_goto(token, MSG, 0); \
        } }

#define js_parser_read_exp(NODE, MSG) js_parser_read_func(NODE, js_parser_expression, MSG)

#define js_parser_read_stmt(NODE, MSG) js_parser_read_func(NODE, js_parser_statement, MSG)

#define js_parser_def_list(HEAD, TAIL) js_node_list_def(HEAD, TAIL)

#define js_parser_def_id(NODE) \
        struct js_node_id_t* NODE = 0;

#define js_parser_def_node(NODE) \
        struct js_node_t* NODE = 0;

#define js_parser_read_token(TOKEN_TYPE, MSG) \
        if (js_parser_type_not(TOKEN_TYPE)) { js_parser_error_goto(self->token, MSG, 0); } \
        js_parser_next();

#define js_parser_error_goto(TOKEN, MSG, PARENT) \
        error = js_parser_error_new(self->memory, MSG, TOKEN, PARENT); \
        goto error;

#define js_parser_error_on() \
        error:

#define js_parser_error_free(NAME) \
        if (NAME) js_node_free_typed(NAME);

#define js_parser_error_return() \
        return js_node_cast(error);

struct js_parser_t* js_parser_new(struct flow_memory_t* memory, struct js_token_t* tokens) {
    struct js_parser_t* self = flow_memory_alloc_typed(memory, struct js_parser_t);
    self->memory = memory;
    self->tokens = tokens;
    self->token = tokens;
    return self;
}

void js_parser_free(struct js_parser_t* self) {
    flow_memory_item_free(self);
}

struct js_node_t* js_parser_statement(struct js_parser_t* self);

struct js_node_t* js_parser_expression(struct js_parser_t* self);

struct js_node_t* js_parser_statement_block(struct js_parser_t* self);

struct js_node_t* js_parser_expression_object_entry(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_node(exp);
    
    js_parser_read_id(id, "<exp obj item>: '<id>' <':'> <exp>");
    
    js_parser_read_token(':', "<exp obj item>: <id> '<':'>' <exp>");
    
    js_parser_read_exp(exp, "<exp obj item>: <id> <':'> '<exp>'");
    
    return js_node_cast(js_node_obj_entry_new(self->memory, id, exp));
    
    js_parser_error_on();
    js_parser_error_free(id);
    js_parser_error_free(exp);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_object(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(entry);
    js_parser_def_list(head, tail);
    
    js_parser_read_token('{', "<exp obj>: '<'{'>' [<exp obj item> {<','> <exp obj item>}] <'}'>");
    
    if (js_parser_type_not('}')) {
        
        js_parser_read_func(entry, js_parser_expression_object_entry, "<exp obj>: <'{'> ['<exp obj item>' {<','> <exp obj item>}] <'}'>");
        js_node_add(head, tail, entry);
        
        while (js_parser_type(',')) {
            js_parser_next();
            
            js_parser_read_func(entry, js_parser_expression_object_entry, "<exp obj>: <'{'> [<exp obj item> {<','> '<exp obj item>'}] <'}'>");
            js_node_add(head, tail, entry);
        }
    }
    
    js_parser_read_token('}', "<exp obj>: <'{'> [<exp obj item> {<','> <exp obj item>}] '<'}'>'");
    
    return js_node_cast(js_node_obj_new(self->memory, (struct js_node_obj_entry_t*) head));
    
    js_parser_error_on();
    js_parser_error_free(head);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_array(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_node(exp);
    js_parser_def_list(head, tail);
    
    js_parser_read_token('[', "<exp array>: '<'['>' [<exp> {<','> <exp>}] <']'>");
    
    if (js_parser_type_not(']')) {
        
        js_parser_read_exp(exp, "<exp array>: <'['> ['<exp>' {<','> <exp>}] <']'>");
        js_node_add(head, tail, exp);
        
        while (js_parser_type(',')) {
            js_parser_next();
            
            js_parser_read_exp(exp, "<exp array>: <'['> [<exp> {<','> '<exp>'}] <']'>");
            js_node_add(head, tail, exp);
            
        }
        
    }
    
    js_parser_read_token(']', "<exp array>: <'['> [<exp> {<','> <exp>}] '<']'>'");
    
    return js_node_cast(js_node_array_new(self->memory, head));
    
    js_parser_error_on();
    js_parser_error_free(id);
    js_parser_error_free(head);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_new(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_node(exp);
    js_parser_def_list(head, tail);
    
    js_parser_read_token(JS_TOKEN_NEW, "<exp new>: '<new>' <id> <'('> [<exp> {<','> <exp>}] <')'>");
    
    js_parser_read_id(id, "<exp new>: <new> '<id>' <'('> [<exp> {<','> <exp>}] <')'>");
    
    js_parser_read_token('(', "<exp new>: <new> <id> '('");
    
    if (js_parser_type_not(')')) {
        
        js_parser_read_exp(exp, "<exp new>: <new> <id> <'('> ['<exp>' {<','> <exp>}] <')'>");
        js_node_add(head, tail, exp);
        
        while (js_parser_type(',')) {
            js_parser_next();
            
            js_parser_read_exp(exp, "<exp new>: <new> <id> <'('> [<exp> {<','> '<exp>'}] <')'>");
            js_node_add(head, tail, exp);
            
        }
        
    }
    
    js_parser_read_token(')', "<exp new>: <new> <id> <'('> [<exp> {<','> <exp>}] '<')'>'");
    
    return js_node_cast(js_node_new_new(self->memory, id, head));
    
    js_parser_error_on();
    js_parser_error_free(id);
    js_parser_error_free(head);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_single(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(exp);
    
    js_parser_read_token('(', "<exp single>: '<'('>' <exp> <')'>");
    
    js_parser_read_exp(exp, "<exp single>: <'('> '<exp>' <')'>");
    
    js_parser_read_token('(', "<exp single>: <'('> <exp> '<')'>'");
    
    return js_node_cast(exp);
    
    js_parser_error_on();
    js_parser_error_free(exp);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_identifier(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_node(left);
    js_parser_def_node(this);
    js_parser_def_node(exp);
    
    js_parser_read_id(id, "<exp id>: <id> [<'='> <exp>]");
    left = js_node_cast(id);
    
    if (js_parser_type('=') && js_parser_next_type_not('=')) {
        js_parser_next();
        
        this = js_node_cast(js_node_this_new(self->memory));
        js_parser_read_exp(exp, "<exp id>: <id> [<'='> <exp>]");
        
        left = js_node_cast(js_node_assignment_new(self->memory, this, id, exp));
    }
    
    return js_node_cast(left);
    
    js_parser_error_on();
    js_parser_error_free(this);
    js_parser_error_free(exp);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_true(struct js_parser_t* self) {
    js_parser_next();
    return js_node_cast(js_node_true_new(self->memory));
}

struct js_node_t* js_parser_expression_false(struct js_parser_t* self) {
    js_parser_next();
    return js_node_cast(js_node_false_new(self->memory));
}

struct js_node_t* js_parser_expression_this(struct js_parser_t* self) {
    js_parser_next();
    return js_node_cast(js_node_this_new(self->memory));
}

struct js_node_t* js_parser_expression_super(struct js_parser_t* self) {
    js_parser_next();
    return js_node_cast(js_node_super_new(self->memory));
}

struct js_node_t* js_parser_expression_null(struct js_parser_t* self) {
    js_parser_next();
    return js_node_cast(js_node_null_new(self->memory));
}

struct js_node_t* js_parser_expression_string(struct js_parser_t* self) {
    struct js_node_t* node = js_node_cast(js_node_string_new(self->memory, strdup(js_parser_word()), strlen(js_parser_word())));
    js_parser_next();
    return node;
}

struct js_node_t* js_parser_expression_number(struct js_parser_t* self) {
    if (strchr(js_parser_word(), '.')) {
        double value;
        sscanf(js_parser_word(), "%lf", &value);
        struct js_node_t* node = js_node_cast(js_node_num_new(self->memory, value));
        js_parser_next();
        return node;
    } else {
        js_int value;
        sscanf(js_parser_word(), "%ld", &value);
        struct js_node_t* node = js_node_cast(js_node_int_new(self->memory, value));
        js_parser_next();
        return node;
    }
}

struct js_node_t* js_parser_expression_literal(struct js_parser_t* self) {
    js_node_def_token(token);
    if (js_parser_type('(')) {
        return js_parser_expression_single(self);
    } else if (js_parser_type('{')) {
        return js_parser_expression_object(self);
    } else if (js_parser_type('[')) {
        return js_parser_expression_array(self);
    } else if (js_parser_type(JS_TOKEN_ID)) {
        return js_parser_expression_identifier(self);
    } else if (js_parser_type(JS_TOKEN_TRUE)) {
        return js_parser_expression_true(self);
    } else if (js_parser_type(JS_TOKEN_FALSE)) {
        return js_parser_expression_false(self);
    } else if (js_parser_type(JS_TOKEN_THIS)) {
        return js_parser_expression_this(self);
    } else if (js_parser_type(JS_TOKEN_SUPER)) {
        return js_parser_expression_super(self);
    } else if (js_parser_type(JS_TOKEN_NULL)) {
        return js_parser_expression_null(self);
    } else if (js_parser_type(JS_TOKEN_STRING)) {
        return js_parser_expression_string(self);
    } else if (js_parser_type(JS_TOKEN_NUMBER)) {
        return js_parser_expression_number(self);
    } else if (js_parser_type(JS_TOKEN_NEW)) {
        return js_parser_expression_new(self);
    } else {
        return js_parser_error(self->memory, "exp primitive: ['(', '{', '[', <id>, <new>, <string>, <number>, <true>, <false>, <this>, <super>, <null>]", token, 0);
    }
}

struct js_node_t* js_parser_expression_member(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    js_parser_def_id(id);
    js_parser_def_node(exp);
    js_parser_def_list(head, tail);
    
    js_parser_read_func(left, js_parser_expression_literal, "<exp member>: <exp literal>");
    
    while (js_parser_type('.') || js_parser_type('[') || js_parser_type('(')) {
        
        if (js_parser_type('.')) {
            js_parser_next();
            
            js_parser_read_id(id, "<exp member>: <exp literal> <'.'> '<id>'");
            
            left = js_node_cast(js_node_get_new(self->memory, left, id));
        } else if (js_parser_type('(')) {
            js_parser_next();
            
            if (js_parser_type_not(')')) {
                
                js_parser_read_exp(exp, "<exp member>: <exp literal> <'('> ['<exp>' {<','> <exp>}] <')'>");
                js_node_add(head, tail, exp);
                
                while (js_parser_type(',')) {
                    js_parser_next();
                    
                    js_parser_read_exp(exp, "<exp member>: <exp literal> <'('> [<exp> {<','> '<exp>'}] <')'>");
                    js_node_add(head, tail, exp);
                }
            }
            
            js_parser_read_token(')', "<exp member>: <exp literal> <'('> [<exp> {<','> <exp>}] '<')'>'");
            
            left = js_node_cast(js_node_call_new(self->memory, left, head));
        }
        
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_free(exp);
    js_parser_error_free(head);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_unary(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    if (js_parser_type('-') && js_parser_next_type('-')) {
        js_parser_next();
        js_parser_next();
        
        js_parser_read_func(left, js_parser_expression_member, "<exp unary>: <'-'> <'-'> <exp member>");
        
        left = js_node_cast(js_node_pre_dec_new(self->memory, left));
    } else if (js_parser_type('+') && js_parser_next_type('+')) {
        js_parser_next();
        js_parser_next();
        
        js_parser_read_func(left, js_parser_expression_member, "<exp unary>: <'+'> <'+'> <exp member>");
        
        left = js_node_cast(js_node_pre_inc_new(self->memory, left));
    } else if (js_parser_type('-')) {
        js_parser_next();
        
        js_parser_read_func(left, js_parser_expression_member, "<exp unary>: <'-'> <exp member>");
        
        left = js_node_cast(js_node_neg_new(self->memory, left));
    } else if (js_parser_type('!')) {
        js_parser_next();
        
        js_parser_read_func(left, js_parser_expression_member, "<exp unary>: <'!'> <exp member>");
        
        left = js_node_cast(js_node_not_new(self->memory, left));
    } else {
        js_parser_read_func(left, js_parser_expression_member, "<exp unary>: <exp member>");
    }
    
    if (js_parser_type('-') && js_parser_next_type('-')) {
        js_parser_next();
        js_parser_next();
        
        left = js_node_cast(js_node_pos_dec_new(self->memory, left));
    } else if (js_parser_type('+') && js_parser_next_type('+')) {
        js_parser_next();
        js_parser_next();
        
        left = js_node_cast(js_node_pos_inc_new(self->memory, left));
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_multiplicative(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_unary, "<exp mult>: <exp unary>");
    
    while (js_parser_type('*') || js_parser_type('/')) {
        js_parser_def_node(right);
        if (js_parser_type('*')) {
            js_parser_next();
            
            js_parser_read_func(right, js_parser_expression_unary, "<exp mult>: <exp unary> <'*'> '<exp unary>'");
            
            left = js_node_cast(js_node_mul_new(self->memory, left, right));
        } else if (js_parser_type('/')) {
            js_parser_next();
            
            js_parser_read_func(right, js_parser_expression_unary, "<exp mult>: <exp unary> <'/'> '<exp unary>'");
            
            left = js_node_cast(js_node_div_new(self->memory, left, right));
        }
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_additive(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_multiplicative, "<exp add>: <exp mult>");
    
    while (js_parser_type('+') || js_parser_type('-')) {
        js_parser_def_node(right);
        if (js_parser_type('+')) {
            js_parser_next();
            
            js_parser_read_func(right, js_parser_expression_multiplicative, "<exp add>: <exp mult> <'+'> '<exp mult>'");
            
            left = js_node_cast(js_node_sum_new(self->memory, left, right));
        } else if (js_parser_type('-')) {
            js_parser_next();
            
            js_parser_read_func(right, js_parser_expression_multiplicative, "<exp add>: <exp mult> <'-'> '<exp mult>'");
            
            left = js_node_cast(js_node_sub_new(self->memory, left, right));
        }
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_shift(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_additive, "<exp shift>: <exp add>");
    
    while ((js_parser_type('>') && js_parser_next_type('>')) || (js_parser_type('<') && js_parser_next_type('<'))) {
        js_parser_def_node(right);
        if (js_parser_type('>') && js_parser_next_type('>')) {
            js_parser_next();
            js_parser_next();
            
            js_parser_read_func(right, js_parser_expression_additive, "<exp shift>: <exp add> <'>'> <'>'> '<exp add>'");
            
            left = js_node_cast(js_node_shift_right_new(self->memory, left, right));
        } else if (js_parser_type('<') && js_parser_next_type('<')) {
            js_parser_next();
            js_parser_next();
            
            js_parser_read_func(right, js_parser_expression_additive, "<exp shift>: <exp add> <'<'> <'<'> '<exp add>'");
            
            left = js_node_cast(js_node_shift_left_new(self->memory, left, right));
        }
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_equality(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    js_parser_def_node(right);
    
    js_parser_read_func(left, js_parser_expression_shift, "<exp compare>: <exp shift>");
    
    if (js_parser_type('=') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_shift, "<exp compare>: <exp shift> <'='> <'='> '<exp shift>'");
        
        left = js_node_cast(js_node_equal_new(self->memory, left, right));
    } else if (js_parser_type('!') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_shift, "<exp compare>: <exp shift> <'!'> <'='> '<exp shift>'");
        
        left = js_node_cast(js_node_not_equal_new(self->memory, left, right));
    } else if (js_parser_type('<') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_shift, "<exp compare>: <exp shift> <'<'> <'='> '<exp shift>'");
        
        left = js_node_cast(js_node_lower_equal_new(self->memory, left, right));
    } else if (js_parser_type('>') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_shift, "<exp compare>: <exp shift> <'>'> <'='> '<exp shift>'");
        
        left = js_node_cast(js_node_greater_equal_new(self->memory, left, right));
    } else if (js_parser_type('<')) {
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_shift, "<exp compare>: <exp shift> <'<'> '<exp shift>'");
        
        left = js_node_cast(js_node_lower_than_new(self->memory, left, right));
    } else if (js_parser_type('>')) {
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_shift, "<exp compare>: <exp shift> <'>'> '<exp shift>'");
        
        left = js_node_cast(js_node_greater_than_new(self->memory, left, right));
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_bitwise_and(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_equality, "<exp bit and>: <exp compare>");
    
    while (js_parser_type(JS_TOKEN_BITAND)) {
        js_parser_def_node(right);
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_equality, "<exp bit and>: <exp compare> <bitand> '<exp compare>'");
        
        left = js_node_cast(js_node_bitwise_and_new(self->memory, left, right));
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_bitwise_xor(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_bitwise_and, "<exp xor>: <exp compare>");
    
    while (js_parser_type(JS_TOKEN_XOR)) {
        js_parser_def_node(right);
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_bitwise_and, "<exp xor>: <exp bit and> <xor> '<exp bit and>'");
        
        left = js_node_cast(js_node_bitwise_xor_new(self->memory, left, right));
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_bitwise_or(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_bitwise_xor, "<exp bit or>: <exp xor>");
    
    while (js_parser_type(JS_TOKEN_BITOR)) {
        js_parser_def_node(right);
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_bitwise_xor, "<exp bit or>: <exp xor> <bitor> '<exp bit or>'");
        
        left = js_node_cast(js_node_bitwise_or_new(self->memory, left, right));
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_and(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_bitwise_or, "<exp and>: <exp bit or>");
    
    while (js_parser_type(JS_TOKEN_AND)) {
        js_parser_def_node(right);
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_bitwise_or, "<exp and>: <exp bit or> <and> '<exp bit or>'");
        
        left = js_node_cast(js_node_and_new(self->memory, left, right));
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_or(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_and, "<exp or>: <exp and>");
    
    while (js_parser_type(JS_TOKEN_OR)) {
        js_parser_def_node(right);
        js_parser_next();
        
        js_parser_read_func(right, js_parser_expression_and, "<exp or>: <exp and> <or> '<exp and>'");
        
        left = js_node_cast(js_node_or_new(self->memory, left, right));
    }
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression_conditional(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(center);
    js_parser_def_node(left);
    js_parser_def_node(right);
    
    js_parser_read_func(center, js_parser_expression_or, "<exp if>: <exp or>");
    
    if (js_parser_type('?')) {
        js_parser_next();
        
        js_parser_read_func(left, js_parser_expression_or, "<exp if>: <exp or> <'?'> '<exp>' <':'> <exp>");
        
        js_parser_read_token(':', "<exp if>: <exp or> <'?'> <exp> '<':'>' <exp>")
        
        js_parser_read_func(right, js_parser_expression_or, "<exp if>: <exp or> <'?'> <exp> <':'> '<exp>'");
        
        center = js_node_cast(js_node_ternary_new(self->memory, center, left, right));
    }
    
    return center;
    
    js_parser_error_on();
    js_parser_error_free(right);
    js_parser_error_free(left);
    js_parser_error_free(center);
    js_parser_error_return();
}

struct js_node_t* js_parser_expression(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(left);
    
    js_parser_read_func(left, js_parser_expression_conditional, "<exp>: <exp if>");
    
    return left;
    
    js_parser_error_on();
    js_parser_error_free(left);
    js_parser_error_return();
}

struct js_node_t* js_parser_statement_if(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(exp);
    js_parser_def_node(stmt_true);
    js_parser_def_node(stmt_false);
    
    js_parser_read_token(JS_TOKEN_IF, "<stmt if>: '<if>' <exp> <stmt>");
    
    js_parser_read_exp(exp, "<stmt if>: <if> '<exp>' <stmt>");
    
    js_parser_read_stmt(stmt_true, "<stmt if>: <if> <exp> '<stmt>'");
    
    if (js_parser_type(JS_TOKEN_ELSE)) {
        js_parser_next();
        
        js_parser_read_stmt(stmt_false, "<stmt if>: <if> <exp> <stmt> <else> '<stmt>'");
    }
    
    return js_node_cast(js_node_if_new(self->memory, exp, stmt_true, stmt_false));
    
    js_parser_error_on();
    js_parser_error_free(stmt_false);
    js_parser_error_free(stmt_true);
    js_parser_error_free(exp);
    js_parser_error_return();
}

struct js_node_t* js_parser_statement_while(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(exp);
    js_parser_def_node(stmt);
    
    js_parser_read_token(JS_TOKEN_WHILE, "<stmt while>: '<while>' <exp> <stmt>");
    
    js_parser_read_exp(exp, "<stmt while>: <while> '<exp>' <stmt>");
    
    js_parser_read_stmt(stmt, "<stmt while>: <while> <exp> '<stmt>'");
    
    return js_node_cast(js_node_while_new(self->memory, exp, stmt));
    
    js_parser_error_on();
    js_parser_error_free(stmt);
    js_parser_error_free(exp);
    js_parser_error_return();
}

struct js_node_t* js_parser_statement_return(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(exp);
    
    js_parser_read_token(JS_TOKEN_RETURN, "<stmt return>: '<return>' <exp>");
    
    js_parser_read_exp(exp, "<stmt return>: <return> '<exp>'");
    
    return js_node_cast(js_node_return_new(self->memory, exp));
    
    js_parser_error_on();
    js_parser_error_free(exp);
    js_parser_error_return();
}

struct js_node_t* js_parser_statement_variable_item(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_node(exp);
    
    js_parser_read_id(id, "<stmt var item>: '<id>'");
    
    if (js_parser_type('=')) {
        js_parser_next();
    
        js_parser_read_exp(exp, "<stmt var item>: <id> <'='> '<exp>'");
    }
    
    return js_node_cast(js_node_var_item_new(self->memory, id, exp));
    
    js_parser_error_on();
    js_parser_error_free(id);
    js_parser_error_free(exp);
    js_parser_error_return();
}

struct js_node_t* js_parser_statement_variable(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(item);
    js_parser_def_list(head, tail);
    
    js_parser_read_token(JS_TOKEN_VAR, "<stmt var>: '<var>' <stmt var item>");
    
    js_parser_read_func(item, js_parser_statement_variable_item, "<stmt var>: <var> '<stmt var item>'")
    js_node_add(head, tail, item);
    
    while (js_parser_type(',')) {
        js_parser_next();
        
        js_parser_read_func(item, js_parser_statement_variable_item, "<stmt var>: <var> <stmt var item> { <','> '<stmt var item>'}");
        js_node_add(head, tail, item);
    }
    
    return js_node_cast(head);
    
    js_parser_error_on();
    js_parser_error_free(head);
    js_parser_error_return();
}

struct js_node_t* js_parser_statement_expression(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(exp);
    
    js_parser_read_exp(exp, "<stmt exp>: '<exp>'");
    
    return js_node_cast(js_node_stmtexp_new(self->memory, exp));
    
    js_parser_error_on();
    js_parser_error_free(exp);
    js_parser_error_return();
}

struct js_node_t* js_parser_statement_block(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(stmt);
    js_parser_def_list(head, tail);
    
    js_parser_read_token(JS_TOKEN_DO, "<stmt block>: '<do>' { <stmt> } <end>");
    
    while (js_parser_type_not(JS_TOKEN_END)) {
        
        js_parser_read_stmt(stmt, "<stmt block>: <do> { '<stmt>' } <end>")
        js_node_add(head, tail, stmt);
        
    }
    
    js_parser_read_token(JS_TOKEN_END, "<stmt block>: <do> { <stmt> } '<end>'");
    
    return js_node_cast(head);
    
    js_parser_error_on();
    js_parser_error_free(head);
    js_parser_error_return();
}

struct js_node_t* js_parser_parameter_list(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_list(head, tail);
    
    js_parser_read_token('(', "<param list>: '<'('>'");
    
    if (js_parser_type(')')) {
        js_parser_next();
        return head;
    }
    
    js_parser_read_id(id, "<param list>: '<id>'");
    js_node_add(head, tail, id);
    
    while (js_parser_type(',')) {
        
        js_parser_read_id(id, "<param list>: <id> { <','> '<exp>' }");
        js_node_add(head, tail, id);
        
    }
    
    js_parser_read_token(')', "<param list>: <'('> { <','> <exp> } '<')'>'");
    
    return js_node_cast(head);
    
    js_parser_error_on();
    js_parser_error_free(head);
    js_parser_error_return();
}

struct js_node_t* js_parser_function(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_node(param);
    js_parser_def_node(stmt);
    
    js_parser_read_token(JS_TOKEN_FUNCTION, "<function>: '<'function'>' <id> <param list> <stmt>");
    
    js_parser_read_id(id, "<function>: <'function'> '<id>' <param list> <stmt>");
    
    js_parser_read_func(param, js_parser_parameter_list, "<function>: <'function'> <id> '<param list>' <stmt>");
    
    js_parser_read_stmt(stmt, "<function>: <'function'> <id> <param list> '<stmt>'");
    
    return js_node_cast(js_node_function_new(self->memory, id, (struct js_node_param_t*) param, stmt));
    
    js_parser_error_on();
    js_parser_error_free(stmt);
    js_parser_error_free(param);
    js_parser_error_free(id);
    js_parser_error_return();
}

struct js_node_t* js_parser_init(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_node(param);
    js_parser_def_node(stmt);
    
    js_parser_read_token(JS_TOKEN_CONSTRUCTOR, "<init>: '<constructor>' <param list> <stmt>");
    
    js_parser_read_func(param, js_parser_parameter_list, "<init>: <constructor> '<param list>' <stmt>");
    
    js_parser_read_stmt(stmt, "<init>: <constructor> <param list> '<stmt>'");
    
    return js_node_cast(js_node_constructor_new(self->memory, param, stmt));
    
    js_parser_error_on();
    js_parser_error_free(stmt);
    js_parser_error_free(param);
    js_parser_error_return();
}

struct js_node_t* js_parser_field(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_node(param);
    js_parser_def_node(stmt);
    
    js_parser_read_token(JS_TOKEN_VAR, "<class field>: '<'var'>' <id>");
    
    js_parser_read_id(id, "<class field>: <'var'> '<id>'");
    
    return js_node_cast(js_node_field_new(self->memory, id));
    
    js_parser_error_on();
    js_parser_error_free(stmt);
    js_parser_error_free(param);
    js_parser_error_free(id);
    js_parser_error_return();
}

struct js_node_t* js_parser_class(struct js_parser_t* self) {
    js_parser_def_error();
    js_parser_def_token();
    js_parser_def_id(id);
    js_parser_def_id(extend);
    js_parser_def_node(node);
    js_parser_def_list(field_head, field_tail);
    js_parser_def_list(init_head, init_tail);
    js_parser_def_list(func_head, func_tail);
    
    js_parser_read_token(JS_TOKEN_CLASS, "<class>: '<class>' <id> <do> <end>");
    
    js_parser_read_id(id, "<class>: <class> '<id>' <do> <end>");
    
    if (js_parser_type(JS_TOKEN_EXTENDS)) {
        js_parser_next();
        
        js_parser_read_id(extend, "<class>: <class> <id> <extends> '<id>' <do> <end>");
    }
    
    js_parser_read_token(JS_TOKEN_DO, "<class>: '<class>' <id> <do> <end>");
    
    while (js_parser_type_not(JS_TOKEN_END)) {
        
        if (js_parser_type(JS_TOKEN_FUNCTION)) {
            
            js_parser_read_func(node, js_parser_function, "<class>: '<class>' <id> <do> { '<function>' | <init> | <field> } <end>");
            js_node_add(func_head, func_tail, node);
            
        } else if (js_parser_type(JS_TOKEN_CONSTRUCTOR)) {
            
            js_parser_read_func(node, js_parser_init, "<class>: '<class>' <id> <do> { <function> | '<init>' | <field> } <end>");
            js_node_add(init_head, init_tail, node);
            
        } else if (js_parser_type(JS_TOKEN_VAR)) {
            
            js_parser_read_func(node, js_parser_field, "<class>: '<class>' <id> <do> { <function> | <init> | '<field>' } <end>");
            js_node_add(field_head, field_tail, node);
            
        }
        
    }
    
    js_parser_read_token(JS_TOKEN_END, "<class>: <class> <id> <do> '<end>'");
    
    return js_node_cast(js_node_class_new(self->memory, id, extend, init_head, field_head, func_head));
    
    js_parser_error_on();
    js_parser_error_free(field_head);
    js_parser_error_free(init_head);
    js_parser_error_free(func_head);
    js_parser_error_free(extend);
    js_parser_error_free(id);
    js_parser_error_return();
}

struct js_node_t* js_parser_statement(struct js_parser_t* self) {
    if (js_parser_type(';')) {
        js_parser_next();
        return js_parser_empty_new();
    } else if (js_parser_type(JS_TOKEN_BREAK)) {
        js_parser_next();
        return (struct js_node_t*) js_node_break_new(self->memory);
    } else if (js_parser_type(JS_TOKEN_CONTINUE)) {
        js_parser_next();
        return (struct js_node_t*) js_node_continue_new(self->memory);
    } else if (js_parser_type(JS_TOKEN_DO)) {
        return js_parser_statement_block(self);
    } else if (js_parser_type(JS_TOKEN_IF)) {
        return js_parser_statement_if(self);
    } else if (js_parser_type(JS_TOKEN_WHILE)) {
        return js_parser_statement_while(self);
    } else if (js_parser_type(JS_TOKEN_RETURN)) {
        return js_parser_statement_return(self);
    } else if (js_parser_type(JS_TOKEN_VAR)) {
        return js_parser_statement_variable(self);
    } else if (js_parser_type(JS_TOKEN_FUNCTION)) {
        return js_parser_function(self);
    } else if (js_parser_type(JS_TOKEN_CLASS)) {
        return js_parser_class(self);
    } else {
        return js_parser_statement_expression(self);
    }
}

struct js_node_t* js_parser(struct js_parser_t* self) {
    js_node_list_def(head, tail);
    js_parser_skip_comment();
    
    while (js_parser_type_not(JS_TOKEN_EOF)) {
        js_node_def_token(elem_token);
        struct js_node_t* element = js_parser_statement(self);
        if (js_node_error_is(element)) {
            return js_parser_error(self->memory, "parser: ('<element>')*", elem_token, element);
        }
        js_node_add(head, tail, element);
    }
 
    js_node_def_end();
    return head;
}
