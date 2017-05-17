#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "js.h"

// http://hepunx.rl.ac.uk/~adye/jsspec11/llr.htm

#define js_parser_empty_new() (struct js_node_t*) js_node_empty_new()
#define js_node_def(HEAD, TAIL) struct js_node_t *HEAD = 0, *TAIL = 0
#define js_node_def_token(NAME) struct js_token_t* NAME = self->token;
#define js_node_def_end() if (!head) { head = js_parser_empty_new(); }
#define js_node_add(NODE) { if (tail) { tail->next = (struct js_node_t*) NODE; } else { head = (struct js_node_t*) NODE; } } tail = (struct js_node_t*) NODE;
#define js_parser_word() self->token->word
#define js_parser_step() self->token = self->token->next;
#define js_parser_skip_comment() while (js_parser_type(JS_TOKEN_COMMENT)) { js_parser_step(); }
#define js_parser_next() js_parser_step(); js_parser_skip_comment();
#define js_parser_token_get()
#define js_parser_type(TYPE) (self->token->type == TYPE)
#define js_parser_type_not(TYPE) (self->token->type != TYPE)
#define js_parser_next_type(TYPE) (self->token->next->type == TYPE)
#define js_parser_next_type_not(TYPE) (self->token->next->type != TYPE)
#define js_parser_error(MSG) js_parser_error_next(MSG, 0)
#define js_parser_error_next(MSG, NEXT) (struct js_node_t*) js_node_error_new(strdup(MSG), strdup(self->token->word), self->token->line, self->token->column, (struct js_node_error_t*) NEXT)
#define js_parser_error_token_next(MSG, TOKEN, NEXT) (struct js_node_t*) js_node_error_new(strdup(MSG), strdup(TOKEN->word), TOKEN->line, TOKEN->column, (struct js_node_error_t*) NEXT)
#define js_parser_node_id() js_node_id_new(strdup(self->token->word))
#define js_parser_node_id_free(NODE) js_node_free((struct js_node_t*) NODE)
#define js_parser_func_def(NAME, FUNC) struct js_node_t* NAME = FUNC(self);
#define js_parser_exp_def(NAME) js_parser_func_def(NAME, js_parser_expression)
#define js_parser_stmt_def(NAME) js_parser_func_def(NAME, js_parser_statement)
#define js_parser_id_def(NAME) struct js_node_id_t* NAME = js_parser_node_id();

struct js_parser_t* js_parser_new(struct js_token_t* tokens) {
    struct js_parser_t* self = (struct js_parser_t*) malloc(sizeof(struct js_parser_t));
    self->tokens = tokens;
    self->token = tokens;
    return self;
}

void js_parser_free(struct js_parser_t* self) {
    free(self);
}

unsigned char js_parser_expression_check(struct js_parser_t* self);

unsigned char js_parser_statement_check(struct js_parser_t* self);

struct js_node_t* js_parser_statement(struct js_parser_t* self);

struct js_node_t* js_parser_expression(struct js_parser_t* self);

struct js_node_t* js_parser_statement_block(struct js_parser_t* self);

struct js_node_t* js_parser_expression_primary(struct js_parser_t* self) {
    if (js_parser_type('(')) {
        js_parser_next();
        
        js_parser_exp_def(expression);
        if (js_node_error_is(expression)) {
            return js_parser_error_next("expression primitive:", expression);
        }
        
        if (js_parser_type_not(')')) {
            js_node_free(expression);
            return js_parser_error("expression primitive: ( <expression> ')' expected");
        }
        js_parser_next();
        
        return expression;
    } else if (js_parser_type(JS_TOKEN_ID)) {
        struct js_node_t* node = (struct js_node_t*) js_node_id_new(strdup(js_parser_word()));
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_TRUE)) {
        struct js_node_t* node = (struct js_node_t*) js_node_true_new();
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_FALSE)) {
        struct js_node_t* node = (struct js_node_t*) js_node_false_new();
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_THIS)) {
        struct js_node_t* node = (struct js_node_t*) js_node_this_new();
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_SUPER)) {
        struct js_node_t* node = (struct js_node_t*) js_node_super_new();
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_NULL)) {
        struct js_node_t* node = (struct js_node_t*) js_node_null_new();
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_STRING)) {
        struct js_node_t* node = (struct js_node_t*) js_node_string_new(strdup(js_parser_word()), strlen(js_parser_word()));
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_NUMBER)) {
        if (strchr(js_parser_word(), '.')) {
            double value;
            sscanf(js_parser_word(), "%lf", &value);
            struct js_node_t* node = (struct js_node_t*) js_node_num_new(value);
            js_parser_next();
            return node;
        } else {
            int value;
            sscanf(js_parser_word(), "%d", &value);
            struct js_node_t* node = (struct js_node_t*) js_node_int_new(value);
            js_parser_next();
            return node;
        }
    } else {
        return js_parser_error("expression primitive: ['(', <id>, <string>, <number>, true, false, this, super, null] expected");
    }
}

struct js_node_t* js_parser_expression_member(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_primary();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression member:", exp_token, node);
    }
    
    while (js_parser_type('.') || js_parser_type('[') || js_parser_type('(')) {
        if (js_parser_type('.')) {
            js_parser_next();
            
            js_node_def_token(id_token);
            if (js_parser_type_not(JS_TOKEN_ID)) {
                js_node_free(node);
                return js_parser_error_next("expression member:", id_token);
            }
            js_parser_next();
            js_parser_id_def(id);
            
            node = (struct js_node_t*) js_node_field_new(node, id);
//        } else if (js_parser_type('[')) {
//        } else if (js_parser_type('(')) {
        }
    }
    return node;
}

struct js_node_t* js_parser_expression_constructor_call(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_member();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression constructor call:", exp_token, node);
    }
    return node;
}

struct js_node_t* js_parser_expression_constructor(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_constructor_call();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression constructor:", exp_token, node);
    }
    return node;
}

struct js_node_t* js_parser_expression_unary(struct js_parser_t* self) {
    struct js_node_t* node;
    if (js_parser_type('-') && js_parser_next_type('-')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        node = js_parser_expression_constructor(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("expression unary: -- '<expression constructor>' expected", token, node);
        }
        
        node = (struct js_node_t*) js_node_pre_dec_new(node);
    } else if (js_parser_type('+') && js_parser_next_type('+')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        node = js_parser_expression_constructor(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("expression unary: ++ '<expression constructor>' expected", token, node);
        }
        
        node = (struct js_node_t*) js_node_pre_inc_new(node);
    } else if (js_parser_type('-')) {
        js_parser_next();
        
        js_node_def_token(token);
        node = js_parser_expression_constructor(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("expression unary: - '<expression constructor>' expected", token, node);
        }
        
        node = (struct js_node_t*) js_node_neg_new(node);
    } else if (js_parser_type('!')) {
        js_parser_next();
        
        js_node_def_token(token);
        node = js_parser_expression_constructor(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("expression unary: ! '<expression constructor>' expected", token, node);
        }
        
        node = (struct js_node_t*) js_node_not_new(node);
    } else {
        js_node_def_token(exp_token);
        node = js_parser_expression_constructor(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("expression unary: '<expression constructor>'", exp_token, node);
        }
    }
    
    if (js_parser_type('-') && js_parser_next_type('-')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        node = js_parser_expression_constructor(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("expression unary: -- '<expression constructor>' expected", token, node);
        }
        
        node = (struct js_node_t*) js_node_pos_dec_new(node);
    } else if (js_parser_type('+') && js_parser_next_type('+')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        node = js_parser_expression_constructor(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("expression unary: ++ '<expression constructor>' expected", token, node);
        }
        
        node = (struct js_node_t*) js_node_pos_inc_new(node);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_multiplicative(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_unary();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression multiplicative:", exp_token, node);
    }
    
    if (js_parser_type('*')) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_multiplicative();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression multiplicative: <expression unary> >> '<expression multiplicative>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_mul_new(node, value);
    } else if (js_parser_type('/')) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_multiplicative();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression multiplicative: <expression unary> << '<expression multiplicative>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_div_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_additive(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_multiplicative();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression additive:", exp_token, node);
    }
    
    if (js_parser_type('+')) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_additive();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression additive: <expression multiplicative> >> '<expression additive>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_sum_new(node, value);
    } else if (js_parser_type('-')) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_additive();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression additive: <expression multiplicative> << '<expression additive>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_sub_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_shift(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_additive();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression shift:", exp_token, node);
    }
    
    if (js_parser_type('>') && js_parser_next_type('>')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_shift();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression shift: <expression additive> >> '<expression shift>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_shift_right_new(node, value);
    } else if (js_parser_type('<') && js_parser_next_type('<')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_shift();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression shift: <expression additive> << '<expression shift>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_shift_left_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_equality(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_shift();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression equality:", exp_token, node);
    }
    
    if (js_parser_type('=') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression equality: <expression shift> == '<expression equality>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_equal_new(node, value);
    } else if (js_parser_type('!') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression equality: <expression shift> != '<expression equality>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_not_equal_new(node, value);
    } else if (js_parser_type('<') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression equality: <expression shift> <= '<expression equality>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_lower_equal_new(node, value);
    } else if (js_parser_type('>') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression equality: <expression shift> >= '<expression equality>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_greater_equal_new(node, value);
    } else if (js_parser_type('<')) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression equality: <expression shift> < '<expression equality>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_lower_than_new(node, value);
    } else if (js_parser_type('>')) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression equality: <expression shift> > '<expression equality>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_greater_than_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_bitwise_and(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_equality();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression bitwise and:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_BITAND)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_bitwise_and();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression bitwise and: <expression bitwise equality> bitand '<expression bitwise and>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_bitwise_and_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_bitwise_xor(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_bitwise_and();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression bitwise xor:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_XOR)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_bitwise_xor();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression bitwise or: <expression bitwise and> xor '<expression bitwise xor>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_bitwise_xor_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_bitwise_or(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_bitwise_xor();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression bitwise or:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_BITOR)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_bitwise_or();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression bitwise or: <expression bitwise xor> bitor '<expression bitwise or>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_bitwise_or_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_and(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_bitwise_or();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression and:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_AND)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_and();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression and: <expression bitwise or> and '<expression and>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_and_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_or(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_and();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression or:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_OR)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_or();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression or: <expression and> or '<expression or>' expected", token, value);
        }
        
        node = (struct js_node_t*) js_node_or_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_conditional(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_or();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression conditional:", exp_token, node);
    }
    
    if (js_parser_type('?')) {
        js_parser_next();
        
        js_node_def_token(true_token);
        js_parser_func_def(true_value, js_parser_expression_conditional();
        if (js_node_error_is(true_value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression conditional: <expression or> ? '<expression conditional>' expected", true_token, true_value);
        }
        
        js_node_def_token(dot_token);
        if (js_parser_type_not(':')) {
            js_node_free(node);
            js_node_free(true_value);
            return js_parser_error_token_next("expression conditional: <expression or> ? <expression conditional> ':' expected", dot_token, 0);
        }
        js_parser_next();
        
        js_node_def_token(false_token);
        js_parser_func_def(false_value, js_parser_expression_conditional();
        if (js_node_error_is(false_value)) {
            js_node_free(node);
            js_node_free(true_value);
            return js_parser_error_token_next("expression conditional: <expression or> ? <expression conditional> : '<expression conditional>' expected", false_token, false_value);
        }
        
        node = (struct js_node_t*) js_node_ternary_new(node, true_value, false_value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression_assignment(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_conditional();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression assignment:", exp_token, node);
    }
    
    if (js_parser_type('=') && js_parser_next_type_not('=')) {
        js_parser_next();
        
        js_node_def_token(value_token);
        js_parser_func_def(value, js_parser_expression_assignment();
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error_token_next("expression assignment: <expression conditional> = '<expression assignment>' expected", value_token, value);
        }
        
        node = (struct js_node_t*) js_node_assignment_new(node, value);
    }
    
    return node;
}

struct js_node_t* js_parser_expression(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_assignment();
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("expression:", exp_token, node);
    }
    return node;
}

//unsigned char js_parser_expression_check(struct js_parser_t* self) {
//    return js_parser_type('(') || js_parser_type(JS_TOKEN_ID) || js_parser_type(JS_TOKEN_TRUE) || js_parser_type(JS_TOKEN_FALSE) || js_parser_type(JS_TOKEN_THIS) || js_parser_type(JS_TOKEN_SUPER) || js_parser_type(JS_TOKEN_NULL) || js_parser_type(JS_TOKEN_STRING) || js_parser_type(JS_TOKEN_NUMBER);
//}

struct js_node_t* js_parser_statement_if(struct js_parser_t* self) {
    js_parser_next();

    js_node_def_token(exp_token);
    js_parser_exp_def(expression);
    if (js_node_error_is(expression)) {
        return js_parser_error_token_next("statement if: if '<expression>' expected", exp_token, expression);
    }
    
    js_node_def_token(stmt_token);
    js_parser_stmt_def(statement);
    if (js_node_error_is(statement)) {
        js_node_free(expression);
        return js_parser_error_token_next("statement if: if <expression> '<statement>' expected", stmt_token, statement);
    }
    
    if (js_parser_type(JS_TOKEN_ELSE)) {
        js_parser_next();
        
        js_node_def_token(else_exp_token);
        js_parser_exp_def(else_expression);
        if (js_node_error_is(else_expression)) {
            js_node_free(expression);
            js_node_free(statement);
            return js_parser_error_token_next("statement if: if <expression> <statement> else '<expression>' expected", else_exp_token, else_expression);
        }
        return (struct js_node_t*) js_node_if_new(expression, statement, else_expression);
    } else {
        return (struct js_node_t*) js_node_if_new(expression, statement, 0);
    }    
}

struct js_node_t* js_parser_statement_while(struct js_parser_t* self) {
    js_parser_next();
    
    js_node_def_token(exp_token);
    js_parser_exp_def(expression);
    if (js_node_error_is(expression)) {
        return js_parser_error_token_next("statement while: while '<expression>' expected", exp_token, expression);
    }
    
    js_node_def_token(stmt_token);
    js_parser_stmt_def(statement);
    if (js_node_error_is(statement)) {
        js_node_free(expression);
        return js_parser_error_token_next("statement while: while <expression> '<statement>' expected", stmt_token, statement);
    }
    
    return (struct js_node_t*) js_node_while_new(expression, statement);
}

struct js_node_t* js_parser_statement_return(struct js_parser_t* self) {
    js_parser_next();
    
    js_node_def_token(exp_token);
    js_parser_exp_def(expression);
    if (js_node_error_is(expression)) {
        return js_parser_error_token_next("statement return: return '<expression>' expected", exp_token, expression);
    }
    
    return (struct js_node_t*) js_node_return_new(expression);
}

struct js_node_t* js_parser_statement_variable_item(struct js_parser_t* self) {
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error("statement variable item: '<id>' expected");
    }
    
    js_parser_id_def(id);
    js_parser_next();
    
    struct js_node_t* value = 0;
    if (js_parser_type('=')) {
        js_parser_next();
        
        js_parser_exp_def(value);
        if (js_node_error_is(value)) {
            js_node_free((struct js_node_t*) id);
            return js_parser_error("statement variable item: <id> = '<expression>' expected");
        }
        return (struct js_node_t*) js_node_var_item_new((struct js_node_id_t*) id, value);
    } else {
        return (struct js_node_t*) js_node_var_item_new((struct js_node_id_t*) id, 0);
    }
}

struct js_node_t* js_parser_statement_variable(struct js_parser_t* self) {
    js_node_def(head, tail);
    js_parser_next();
    
    js_node_def_token(var_item_token);
    struct js_node_t* node = js_parser_statement_variable_item(self);
    if (js_node_error_is(node)) {
        return js_parser_error_token_next("statement variable: var '<id> [= <expression>]?' expected", var_item_token, node);
    }
    js_node_add(node)
    
    while (js_parser_type(',')) {
        js_parser_next();
        
        js_node_def_token(var_item_token);
        struct js_node_t* node = js_parser_statement_variable_item(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("statement variable: var '<id> [= <expression>]?' expected", var_item_token, node);
        }
        js_node_add(node);
    }
    
    js_node_def_end();
    return head;
}

struct js_node_t* js_parser_statement_var_exp(struct js_parser_t* self) {
    if (js_parser_type(JS_TOKEN_VAR)) {
        return js_parser_statement_variable(self);
    } else {
        return js_parser_expression(self);
    }
}

struct js_node_t* js_parser_statement(struct js_parser_t* self) {
    if (js_parser_type(';')) {
        js_parser_next();
        return js_parser_empty_new();
    } else if (js_parser_type(JS_TOKEN_BREAK)) {
        js_parser_next();
        return (struct js_node_t*) js_node_break_new();
    } else if (js_parser_type(JS_TOKEN_CONTINUE)) {
        js_parser_next();
        return (struct js_node_t*) js_node_continue_new();
    } else if (js_parser_type(JS_TOKEN_DO)) {
        return js_parser_statement_block(self);
    } else if (js_parser_type(JS_TOKEN_IF)) {
        return js_parser_statement_if(self);
    } else if (js_parser_type(JS_TOKEN_WHILE)) {
        return js_parser_statement_while(self);
    } else if (js_parser_type(JS_TOKEN_RETURN)) {
        return js_parser_statement_return(self);
    } else {
        js_node_def_token(var_exp_token);
        struct js_node_t* node = js_parser_statement_var_exp(self);
        if (js_node_error_is(node)) {
            return js_parser_error_token_next("statement:", var_exp_token, node);
        }
        return node;
    }
}

struct js_node_t* js_parser_statement_block(struct js_parser_t* self) {
    js_node_def(head, tail);
    js_parser_next();
    
    js_node_def_token(statement_token);
    while (js_parser_type_not(JS_TOKEN_END)) {
        js_parser_stmt_def(node);
        if (js_node_error_is(node)) {
            js_node_free(head);
            return js_parser_error_token_next("statement block: do '(<statement>)*' end expected", statement_token, node);
        }
        js_node_add(node);
    }
    
    js_node_def_token(end_token);
    if (js_parser_type_not(JS_TOKEN_END)) {
        js_node_free(head);
        return js_parser_error_token_next("statement block: do (<statement)* 'end' expected", end_token, 0);
    }
    js_parser_next();
    
    js_node_def_end();
    return head;
}

struct js_node_t* js_parser_parameter_list(struct js_parser_t* self) {
    js_node_def(head, tail);
    
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error("parameter list: '<id>' expected");
    }
    js_parser_id_def(id);
    js_node_add(id);
    js_parser_next();
    while (js_parser_type(',')) {
        js_parser_next();
        if (js_parser_type_not(JS_TOKEN_ID)) {
            js_node_free(head);
            return js_parser_error("parameter list: '<id>' expected");
        }
        js_parser_id_def(id);
        js_node_add(id);
        js_parser_next();
    }
    
    js_node_def_end();
    return head;
}

struct js_node_t* js_parser_parameter_list_opt(struct js_parser_t* self) {
    if (js_parser_type_not(JS_TOKEN_ID)) return 0;
    return js_parser_parameter_list(self);
}

struct js_node_t* js_parser_element_function(struct js_parser_t* self) {
    js_parser_next();
    
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error("function: function '<id>' expected");
    }
    struct js_node_id_t* node = js_parser_node_id();
    js_parser_next();
    
    if (js_parser_type_not('(')) {
        js_parser_node_id_free(node);
        return js_parser_error("function: function <id> '(' expected");
    }
    js_parser_next();
    
    struct js_node_t* parameters = js_parser_parameter_list_opt(self);
    if (parameters && js_node_error_is(parameters)) {
        js_parser_node_id_free(node);
        return js_parser_error_next("function: function <id> ( '<parameter list>' expected", parameters);
    }
    
    if (js_parser_type_not(')')) {
        js_parser_node_id_free(node);
        js_node_free(parameters);
        return js_parser_error("function: function <id> ( <parameter list> ')' expected");
    }
    js_parser_next();
    
    struct js_node_t* statement = js_parser_statement(self);
    if (js_node_error_is(statement)) {
        js_parser_node_id_free(node);
        js_node_free(parameters);
        return js_parser_error_next("function: function <id> ( <parameter list> ) '<statement>' expected", statement);
    }
    
    return (struct js_node_t*) js_node_function_new(node, parameters, statement);
}

struct js_node_t* js_parser_element(struct js_parser_t* self) {
    if (js_parser_type(JS_TOKEN_FUNCTION)) {
        return js_parser_element_function(self);
    } else {
        return js_parser_statement(self);
    }
}

struct js_node_t* js_parser(struct js_parser_t* self, struct js_token_t* tokens) {
    js_node_def(head, tail);
    js_parser_skip_comment();
    
    while (js_parser_type_not(JS_TOKEN_EOF)) {
        struct js_node_t* element = js_parser_element(self);
        if (js_node_error_is(element)) {
            return js_parser_error_next("parser: ('<element>')* expected", element);
        }
        js_node_add(element);
    }
 
    js_node_def_end();
    return head;
}
