#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "js.h"
#include "xstring.h"

// http://hepunx.rl.ac.uk/~adye/jsspec11/llr.htm

#define js_parser_empty_new() (struct js_node_t*) js_node_empty_new(self->memory)
#define js_node_list_def(HEAD, TAIL) struct js_node_t *HEAD = 0, *TAIL = 0
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
#define js_parser_node_id() js_node_id_new(self->memory, strdup(self->token->word), js_parser_word_length(), js_parser_word_hash())
#define js_parser_node_id_free(NODE) js_node_free((struct js_node_t*) NODE)
#define js_parser_func_def(NAME, FUNC) struct js_node_t* NAME = FUNC(self);
#define js_parser_exp_def(NAME) js_parser_func_def(NAME, js_parser_expression)
#define js_parser_stmt_def(NAME) js_parser_func_def(NAME, js_parser_statement)
#define js_parser_id_def(NAME) struct js_node_id_t* js_parser_id_assign(NAME)
#define js_parser_id_def_null(NAME) struct js_node_id_t* NAME = 0;
#define js_parser_id_assign(NAME) NAME = js_parser_node_id();

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
    js_node_def_token(id_token);
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error(self->memory, "exp object entry: '<id>'", id_token, 0);
    }
    js_parser_id_def(id);
    js_parser_next();
    
    if (js_parser_type(JS_TOKEN_EOF)) {
        return js_parser_error(self->memory, "exp object entry: <id> ':'", self->token, 0);
    }
    
    js_node_def_token(dot_token);
    if (js_parser_type_not(':')) {
        return js_parser_error(self->memory, "exp object entry: <id> ':'", dot_token, 0);
    }
    js_parser_next();
    
    js_node_def_token(exp_token);
    js_parser_exp_def(exp);
    if (js_node_error_is(exp)) {
        return js_parser_error(self->memory, "exp object entry: <id> : <exp>", exp_token, exp);
    }
    
    return js_node_cast(js_node_obj_entry_new(self->memory, id, exp));
}

struct js_node_t* js_parser_expression_object(struct js_parser_t* self) {
    js_node_list_def(head, tail);
    js_parser_next();
    
    if (js_parser_type(JS_TOKEN_EOF)) {
        return js_parser_error(self->memory, "exp primitive: { '<id> : <exp>'", self->token, 0);
    }
    
    while (js_parser_type_not('}')) {
        if (js_parser_type(JS_TOKEN_EOF)) {
            return js_parser_error(self->memory, "exp primitive: { '<id> : <exp>'", self->token, 0);
        }
        
        js_node_def_token(entry_token);
        js_parser_func_def(entry_node, js_parser_expression_object_entry);
        if (js_node_error_is(entry_node)) {
            return js_parser_error(self->memory, "exp primitive: { '<id> : <exp>'", entry_token, entry_node);
        }
        js_node_add(head, tail, entry_node);
    }
    
    if (js_parser_type(JS_TOKEN_EOF)) {
        return js_parser_error(self->memory, "exp primitive: { <id> : <exp> '}'", self->token, 0);
    }
    
    js_parser_next();
    return js_node_cast(js_node_obj_new(self->memory, (struct js_node_obj_entry_t*) head));
}

struct js_node_t* js_parser_expression_primary(struct js_parser_t* self) {
    js_node_def_token(token);
    if (js_parser_type('(')) {
        js_parser_next();
        
        js_node_def_token(exp_token);
        js_parser_exp_def(expression);
        if (js_node_error_is(expression)) {
            return js_parser_error(self->memory, "exp primitive:", exp_token, expression);
        }
        
        js_node_def_token(close_token);
        if (js_parser_type_not(')')) {
            js_node_free(expression);
            return js_parser_error(self->memory, "exp primitive: ( <exp> ')'", close_token, 0);
        }
        js_parser_next();
        
        return expression;
    } else if (js_parser_type('{')) {
        return js_parser_expression_object(self);
    } else if (js_parser_type(JS_TOKEN_ID)) {
        js_parser_id_def(id);
        js_parser_next();
        
        if (js_parser_type('=') && js_parser_next_type_not('=')) {
            js_parser_next();
            
            js_node_def_token(value_token);
            js_parser_exp_def(value);
            if (js_node_error_is(value)) {
                js_node_id_free(id);
                return js_parser_error(self->memory, "exp primitive: <id> = '<exp>'", value_token, value);
            }
            js_node_def(this, js_node_cast(js_node_this_new(self->memory)));
            return js_node_cast(js_node_assignment_new(self->memory, this, id, value));
        } else {
            return (struct js_node_t*) id;
        }
    } else if (js_parser_type(JS_TOKEN_TRUE)) {
        struct js_node_t* node = (struct js_node_t*) js_node_true_new(self->memory);
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_FALSE)) {
        struct js_node_t* node = (struct js_node_t*) js_node_false_new(self->memory);
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_THIS)) {
        struct js_node_t* node = (struct js_node_t*) js_node_this_new(self->memory);
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_SUPER)) {
        struct js_node_t* node = (struct js_node_t*) js_node_super_new(self->memory);
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_NULL)) {
        struct js_node_t* node = (struct js_node_t*) js_node_null_new(self->memory);
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_STRING)) {
        struct js_node_t* node = (struct js_node_t*) js_node_string_new(self->memory, strdup(js_parser_word()), strlen(js_parser_word()));
        js_parser_next();
        return node;
    } else if (js_parser_type(JS_TOKEN_NUMBER)) {
        if (strchr(js_parser_word(), '.')) {
            double value;
            sscanf(js_parser_word(), "%lf", &value);
            struct js_node_t* node = (struct js_node_t*) js_node_num_new(self->memory, value);
            js_parser_next();
            return node;
        } else {
            int value;
            sscanf(js_parser_word(), "%d", &value);
            struct js_node_t* node = (struct js_node_t*) js_node_int_new(self->memory, value);
            js_parser_next();
            return node;
        }
    } else {
        return js_parser_error(self->memory, "exp primitive: ['(', '{', <id>, <string>, <number>, true, false, this, super, null]", token, 0);
    }
}

struct js_node_t* js_parser_expression_member(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_primary);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp member:", exp_token, node);
    }
    
    while (js_parser_type('.') || js_parser_type('[') || js_parser_type('(')) {
        if (js_parser_type('.')) {
            js_parser_next();
            
            js_node_def_token(id_token);
            if (js_parser_type_not(JS_TOKEN_ID)) {
                js_node_free(node);
                return js_parser_error(self->memory, "exp member:", id_token, 0);
            }
            js_parser_id_def(id);
            js_parser_next();
            
            node = js_node_cast(js_node_get_new(self->memory, node, id));
        } else if (js_parser_type('(')) {
            js_parser_next();

            js_node_list_def(head, tail);
            if (js_parser_type_not(')')) {
                for (;;) {
                    js_node_def_token(param_token);
                    js_parser_exp_def(param);
                    if (js_node_error_is(param)) {
                        js_node_free(node);
                        js_node_free(head);
                        return js_parser_error(self->memory, "exp member:", param_token, param);
                    }
                    js_node_add(head, tail, param)
                    
                    if (js_parser_type_not(',')) {
                        js_parser_next();
                    } else break;
                }
            }
            
            js_node_def_token(close_token);
            if (js_parser_type_not(')')) {
                js_node_free(node);
                js_node_free(head);
                return js_parser_error(self->memory, "exp member:", close_token, 0);
            }
            js_parser_next();
            
            node = js_node_cast(js_node_call_new(self->memory, node, head));
        }
    }
    return node;
}

struct js_node_t* js_parser_expression_constructor_call(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_member);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp constructor call:", exp_token, node);
    }
    return node;
}

struct js_node_t* js_parser_expression_constructor(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_constructor_call);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp constructor:", exp_token, node);
    }
    return node;
}

struct js_node_t* js_parser_expression_unary(struct js_parser_t* self) {
    struct js_node_t* node;
    if (js_parser_type('-') && js_parser_next_type('-')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        node = js_node_cast(js_parser_expression_constructor(self));
        if (js_node_error_is(node)) {
            return js_parser_error(self->memory, "exp unary: -- '<expression constructor>'", token, node);
        }
        
        node = js_node_cast(js_node_pre_dec_new(self->memory, node));
    } else if (js_parser_type('+') && js_parser_next_type('+')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        node = js_node_cast(js_parser_expression_constructor(self));
        if (js_node_error_is(node)) {
            return js_parser_error(self->memory, "exp unary: ++ '<expression constructor>'", token, node);
        }
        
        node = js_node_cast(js_node_pre_inc_new(self->memory, node));
    } else if (js_parser_type('-')) {
        js_parser_next();
        
        js_node_def_token(token);
        node = js_node_cast(js_parser_expression_constructor(self));
        if (js_node_error_is(node)) {
            return js_parser_error(self->memory, "exp unary: - '<expression constructor>'", token, node);
        }
        
        node = (struct js_node_t*) js_node_neg_new(self->memory, node);
    } else if (js_parser_type('!')) {
        js_parser_next();
        
        js_node_def_token(token);
        node = js_node_cast(js_parser_expression_constructor(self));
        if (js_node_error_is(node)) {
            return js_parser_error(self->memory, "exp unary: ! '<expression constructor>'", token, node);
        }
        
        node = (struct js_node_t*) js_node_not_new(self->memory, node);
    } else {
        js_node_def_token(exp_token);
        node = js_node_cast(js_parser_expression_constructor(self));
        if (js_node_error_is(node)) {
            return js_parser_error(self->memory, "exp unary: '<expression constructor>'", exp_token, node);
        }
    }
    
    if (js_parser_type('-') && js_parser_next_type('-')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        node = js_node_cast(js_parser_expression_constructor(self));
        if (js_node_error_is(node)) {
            return js_parser_error(self->memory, "exp unary: -- '<expression constructor>'", token, node);
        }
        
        node = js_node_cast(js_node_pos_dec_new(self->memory, node));
    } else if (js_parser_type('+') && js_parser_next_type('+')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        node = js_node_cast(js_parser_expression_constructor(self));
        if (js_node_error_is(node)) {
            return js_parser_error(self->memory, "exp unary: ++ '<expression constructor>'", token, node);
        }
        
        node = js_node_cast(js_node_pos_inc_new(self->memory, node));
    }
    
    return node;
}

struct js_node_t* js_parser_expression_multiplicative(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_unary);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp multiplicative:", exp_token, node);
    }
    
    while (js_parser_type('*') || js_parser_type('/')) {
        if (js_parser_type('*')) {
            js_parser_next();
            
            js_node_def_token(token);
            js_parser_func_def(value, js_parser_expression_unary);
            if (js_node_error_is(value)) {
                js_node_free(node);
                return js_parser_error(self->memory, "exp multiplicative: <expression unary> * '<expression unary>'", token, value);
            }
            
            node = js_node_cast(js_node_mul_new(self->memory, node, value));
        } else if (js_parser_type('/')) {
            js_parser_next();
            
            js_node_def_token(token);
            js_parser_func_def(value, js_parser_expression_unary);
            if (js_node_error_is(value)) {
                js_node_free(node);
                return js_parser_error(self->memory, "exp multiplicative: <expression unary> * '<expression unary>'", token, value);
            }
            
            node = js_node_cast(js_node_div_new(self->memory, node, value));
        }
    }
    
    return node;
}

struct js_node_t* js_parser_expression_additive(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_multiplicative);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp additive:", exp_token, node);
    }
    
    while (js_parser_type('+') || js_parser_type('-')) {
        if (js_parser_type('+')) {
            js_parser_next();
            
            js_node_def_token(token);
            js_parser_func_def(value, js_parser_expression_multiplicative);
            if (js_node_error_is(value)) {
                js_node_free(node);
                return js_parser_error(self->memory, "exp additive: <expression multiplicative> + '<expression multiplicative>'", token, value);
            }
            
            node = js_node_cast(js_node_sum_new(self->memory, node, value));
        } else if (js_parser_type('-')) {
            js_parser_next();
            
            js_node_def_token(token);
            js_parser_func_def(value, js_parser_expression_multiplicative);
            if (js_node_error_is(value)) {
                js_node_free(node);
                return js_parser_error(self->memory, "exp additive: <expression multiplicative> - '<expression multiplicative>'", token, value);
            }
            
            node = js_node_cast(js_node_sub_new(self->memory, node, value));
        }
    }
    
    return node;
}

struct js_node_t* js_parser_expression_shift(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_additive);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp shift:", exp_token, node);
    }
    
    while ((js_parser_type('>') && js_parser_next_type('>')) || (js_parser_type('<') && js_parser_next_type('<'))) {
        if (js_parser_type('>') && js_parser_next_type('>')) {
            js_parser_next();
            js_parser_next();
            
            js_node_def_token(token);
            js_parser_func_def(value, js_parser_expression_additive);
            if (js_node_error_is(value)) {
                js_node_free(node);
                return js_parser_error(self->memory, "exp shift: <expression additive> >> '<expression additive>'", token, value);
            }
            
            node = js_node_cast(js_node_shift_right_new(self->memory, node, value));
        } else if (js_parser_type('<') && js_parser_next_type('<')) {
            js_parser_next();
            js_parser_next();
            
            js_node_def_token(token);
            js_parser_func_def(value, js_parser_expression_additive);
            if (js_node_error_is(value)) {
                js_node_free(node);
                return js_parser_error(self->memory, "exp shift: <expression additive> << '<expression additive>'", token, value);
            }
            
            node = js_node_cast(js_node_shift_left_new(self->memory, node, value));
        }
    }
    
    return node;
}

struct js_node_t* js_parser_expression_equality(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_shift);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp equality:", exp_token, node);
    }
    
    if (js_parser_type('=') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp equality: <expression shift> == '<expression equality>'", token, value);
        }
        
        node = js_node_cast(js_node_equal_new(self->memory, node, value));
    } else if (js_parser_type('!') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp equality: <expression shift> != '<expression equality>'", token, value);
        }
        
        node = js_node_cast(js_node_not_equal_new(self->memory, node, value));
    } else if (js_parser_type('<') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp equality: <expression shift> <= '<expression equality>'", token, value);
        }
        
        node = js_node_cast(js_node_lower_equal_new(self->memory, node, value));
    } else if (js_parser_type('>') && js_parser_next_type('=')) {
        js_parser_next();
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp equality: <expression shift> >= '<expression equality>'", token, value);
        }
        
        node = js_node_cast(js_node_greater_equal_new(self->memory, node, value));
    } else if (js_parser_type('<')) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp equality: <expression shift> < '<expression equality>'", token, value);
        }
        
        node = js_node_cast(js_node_lower_than_new(self->memory, node, value));
    } else if (js_parser_type('>')) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_equality);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp equality: <expression shift> > '<expression equality>'", token, value);
        }
        
        node = js_node_cast(js_node_greater_than_new(self->memory, node, value));
    }
    
    return node;
}

struct js_node_t* js_parser_expression_bitwise_and(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_equality);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp bitwise and:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_BITAND)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_bitwise_and);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp bitwise and: <expression bitwise equality> bitand '<expression bitwise and>'", token, value);
        }
        
        node = js_node_cast(js_node_bitwise_and_new(self->memory, node, value));
    }
    
    return node;
}

struct js_node_t* js_parser_expression_bitwise_xor(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_bitwise_and);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp bitwise xor:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_XOR)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_bitwise_xor);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp bitwise or: <expression bitwise and> xor '<expression bitwise xor>'", token, value);
        }
        
        node = js_node_cast(js_node_bitwise_xor_new(self->memory, node, value));
    }
    
    return node;
}

struct js_node_t* js_parser_expression_bitwise_or(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_bitwise_xor);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp bitwise or:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_BITOR)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_bitwise_or);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp bitwise or: <expression bitwise xor> bitor '<expression bitwise or>'", token, value);
        }
        
        node = js_node_cast(js_node_bitwise_or_new(self->memory, node, value));
    }
    
    return node;
}

struct js_node_t* js_parser_expression_and(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_bitwise_or);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp and:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_AND)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_and);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp and: <expression bitwise or> and '<expression and>'", token, value);
        }
        
        node = js_node_cast(js_node_and_new(self->memory, node, value));
    }
    
    return node;
}

struct js_node_t* js_parser_expression_or(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_and);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp or:", exp_token, node);
    }
    
    if (js_parser_type(JS_TOKEN_OR)) {
        js_parser_next();
        
        js_node_def_token(token);
        js_parser_func_def(value, js_parser_expression_or);
        if (js_node_error_is(value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp or: <expression and> or '<expression or>'", token, value);
        }
        
        node = js_node_cast(js_node_or_new(self->memory, node, value));
    }
    
    return node;
}

struct js_node_t* js_parser_expression_conditional(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_or);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp conditional:", exp_token, node);
    }
    
    if (js_parser_type('?')) {
        js_parser_next();
        
        js_node_def_token(true_token);
        js_parser_func_def(true_value, js_parser_expression_conditional);
        if (js_node_error_is(true_value)) {
            js_node_free(node);
            return js_parser_error(self->memory, "exp conditional: <expression or> ? '<expression conditional>'", true_token, true_value);
        }
        
        js_node_def_token(dot_token);
        if (js_parser_type_not(':')) {
            js_node_free(node);
            js_node_free(true_value);
            return js_parser_error(self->memory, "exp conditional: <expression or> ? <expression conditional> ':'", dot_token, 0);
        }
        js_parser_next();
        
        js_node_def_token(false_token);
        js_parser_func_def(false_value, js_parser_expression_conditional);
        if (js_node_error_is(false_value)) {
            js_node_free(node);
            js_node_free(true_value);
            return js_parser_error(self->memory, "exp conditional: <expression or> ? <expression conditional> : '<expression conditional>'", false_token, false_value);
        }
        
        node = js_node_cast(js_node_ternary_new(self->memory, node, true_value, false_value));
    }
    
    return node;
}

struct js_node_t* js_parser_expression(struct js_parser_t* self) {
    js_node_def_token(exp_token);
    js_parser_func_def(node, js_parser_expression_conditional);
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "exp:", exp_token, node);
    }
    return node;
}

struct js_node_t* js_parser_statement_if(struct js_parser_t* self) {
    js_parser_next();

    js_node_def_token(exp_token);
    js_parser_exp_def(expression);
    if (js_node_error_is(expression)) {
        return js_parser_error(self->memory, "statement if: if '<exp>'", exp_token, expression);
    }
    
    js_node_def_token(stmt_token);
    js_parser_stmt_def(statement);
    if (js_node_error_is(statement)) {
        js_node_free(expression);
        return js_parser_error(self->memory, "statement if: if <exp> '<statement>'", stmt_token, statement);
    }
    
    if (js_parser_type(JS_TOKEN_ELSE)) {
        js_parser_next();
        
        js_node_def_token(else_exp_token);
        js_parser_exp_def(else_expression);
        if (js_node_error_is(else_expression)) {
            js_node_free(expression);
            js_node_free(statement);
            return js_parser_error(self->memory, "statement if: if <exp> <statement> else '<exp>'", else_exp_token, else_expression);
        }
        return js_node_cast(js_node_if_new(self->memory, expression, statement, else_expression));
    } else {
        return js_node_cast(js_node_if_new(self->memory, expression, statement, 0));
    }    
}

struct js_node_t* js_parser_statement_while(struct js_parser_t* self) {
    js_parser_next();
    
    js_node_def_token(exp_token);
    js_parser_exp_def(expression);
    if (js_node_error_is(expression)) {
        return js_parser_error(self->memory, "statement while: while '<exp>'", exp_token, expression);
    }
    
    js_node_def_token(stmt_token);
    js_parser_stmt_def(statement);
    if (js_node_error_is(statement)) {
        js_node_free(expression);
        return js_parser_error(self->memory, "statement while: while <exp> '<statement>'", stmt_token, statement);
    }
    
    return js_node_cast(js_node_while_new(self->memory, expression, statement));
}

struct js_node_t* js_parser_statement_return(struct js_parser_t* self) {
    js_parser_next();
    
    js_node_def_token(exp_token);
    js_parser_exp_def(expression);
    if (js_node_error_is(expression)) {
        return js_parser_error(self->memory, "statement return: return '<exp>'", exp_token, expression);
    }
    
    return js_node_cast(js_node_return_new(self->memory, expression));
}

struct js_node_t* js_parser_statement_variable_item(struct js_parser_t* self) {
    js_node_def_token(id_token);
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error(self->memory, "statement variable item: '<id>'", id_token, 0);
    }
    
    js_parser_id_def(id);
    js_parser_next();
    
    if (js_parser_type('=')) {
        js_parser_next();
        
        js_node_def_token(exp_token);
        js_parser_exp_def(value);
        if (js_node_error_is(value)) {
            js_node_free((struct js_node_t*) id);
            return js_parser_error(self->memory, "statement variable item: <id> = '<exp>'", exp_token, value);
        }
        return js_node_cast(js_node_var_item_new(self->memory, (struct js_node_id_t*) id, value));
    } else {
        return js_node_cast(js_node_var_item_new(self->memory, (struct js_node_id_t*) id, 0));
    }
}

struct js_node_t* js_parser_statement_variable(struct js_parser_t* self) {
    js_node_list_def(head, tail);
    js_parser_next();
    
    js_node_def_token(var_item_token);
    js_node_def(node, js_parser_statement_variable_item(self));
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "statement variable: var '<id> [= <exp>]?'", var_item_token, node);
    }
    js_node_add(head, tail, node)
    
    while (js_parser_type(',')) {
        js_parser_next();
        
        js_node_def_token(var_item_token);
        js_node_def(node, js_parser_statement_variable_item(self));
        if (js_node_error_is(node)) {
            return js_parser_error(self->memory, "statement variable: var '<id> [= <exp>]?'", var_item_token, node);
        }
        js_node_add(head, tail, node);
    }
    
    js_node_def_end();
    return head;
}

struct js_node_t* js_parser_statement_expression(struct js_parser_t* self) {
    js_node_def_token(token);
    js_node_def(node, js_parser_expression(self));
    if (js_node_error_is(node)) {
        return js_parser_error(self->memory, "statement expression:", token, node);
    }
    return js_node_cast(js_node_stmtexp_new(self->memory, node));
}

struct js_node_t* js_parser_statement_block(struct js_parser_t* self) {
    js_node_list_def(head, tail);
    js_parser_next();
    
    js_node_def_token(statement_token);
    while (js_parser_type_not(JS_TOKEN_END)) {
        js_parser_stmt_def(node);
        if (js_node_error_is(node)) {
            js_node_free(head);
            return js_parser_error(self->memory, "statement block: do '(<statement>)*' end", statement_token, node);
        }
        js_node_add(head, tail, node);
    }
    
    js_node_def_token(end_token);
    if (js_parser_type_not(JS_TOKEN_END)) {
        js_node_free(head);
        return js_parser_error(self->memory, "statement block: do (<statement)* 'end'", end_token, 0);
    }
    js_parser_next();
    
    js_node_def_end();
    return head;
}

struct js_node_t* js_parser_parameter_list(struct js_parser_t* self) {
    js_node_list_def(head, tail);
    
    js_node_def_token(id_first_token);
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error(self->memory, "parameter list: '<id>'", id_first_token, 0);
    }
    js_parser_id_def(id);
    
    js_node_add(head, tail, id);
    js_parser_next();

    while (js_parser_type(',')) {
        js_parser_next();
        
        js_node_def_token(id_next_token);
        if (js_parser_type_not(JS_TOKEN_ID)) {
            js_node_free(head);
            return js_parser_error(self->memory, "parameter list: '<id>'", id_next_token, 0);
        }
        js_parser_id_def(id);
        js_node_add(head, tail, id);
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
    
    js_node_def_token(id_token);
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error(self->memory, "element function: function '<id>'", id_token, 0);
    }
    js_parser_id_def(id);
    js_parser_next();
    
    js_node_def_token(arg_open_token);
    if (js_parser_type_not('(')) {
        js_parser_node_id_free(id);
        return js_parser_error(self->memory, "element function: function <id> '('", arg_open_token, 0);
    }
    js_parser_next();
    
    js_node_def_token(arg_token);
    struct js_node_param_t* param = (struct js_node_param_t*) js_parser_parameter_list_opt(self);
    if (param && js_node_error_is(param)) {
        js_parser_node_id_free(id);
        return js_parser_error(self->memory, "element function: function <id> ( '<parameter list>'", arg_token, param);
    }
    
    js_node_def_token(arg_close_token);
    if (js_parser_type_not(')')) {
        js_parser_node_id_free(id);
        js_node_free_typed(param);
        return js_parser_error(self->memory, "element function: function <id> ( <parameter list> ')'", arg_close_token, 0);
    }
    js_parser_next();
    
    js_node_def_token(stmt_token);
    js_node_def(statement, js_parser_statement(self));
    if (js_node_error_is(statement)) {
        js_parser_node_id_free(id);
        js_node_free_typed(param);
        return js_parser_error(self->memory, "element function: function <id> ( <parameter list> ) '<statement>'", stmt_token, statement);
    }
    
    return js_node_cast(js_node_function_new(self->memory, id, param, statement));
}

struct js_node_t* js_parser_element_constructor(struct js_parser_t* self) {
    js_parser_next();
    
    js_node_def_token(arg_open_token);
    if (js_parser_type_not('(')) {
        return js_parser_error(self->memory, "element constructor: constructor '('", arg_open_token, 0);
    }
    js_parser_next();
    
    js_node_def_token(arg_token);
    js_node_def(parameters, js_parser_parameter_list_opt(self));
    if (parameters && js_node_error_is(parameters)) {
        return js_parser_error(self->memory, "element constructor: constructor ( '<parameter list>'", arg_token, parameters);
    }
    
    js_node_def_token(arg_close_token);
    if (js_parser_type_not(')')) {
        js_node_free(parameters);
        return js_parser_error(self->memory, "element constructor: constructor ( <parameter list> ')'", arg_close_token, 0);
    }
    js_parser_next();
    
    js_node_def_token(stmt_token);
    js_node_def(statement, js_parser_statement(self));
    if (js_node_error_is(statement)) {
        js_node_free(parameters);
        return js_parser_error(self->memory, "element constructor: constructor ( <parameter list> ) '<statement>'", stmt_token, statement);
    }
    
    return js_node_cast(js_node_constructor_new(self->memory, parameters, statement));
}

struct js_node_t* js_parser_element_field(struct js_parser_t* self) {
    js_parser_next();
    
    js_node_def_token(id_token);
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error(self->memory, "element class field: var '<id>'", id_token, 0);
    }
    js_parser_id_def(id);
    js_parser_next();
    
    return js_node_cast(js_node_field_new(self->memory, id));
}

struct js_node_t* js_parser_element_class(struct js_parser_t* self) {
    js_parser_next();
    
    js_node_def_token(id_token);
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error(self->memory, "element class: class '<id>'", id_token, 0);
    }
    js_parser_id_def(id);
    js_parser_next();
    
    js_parser_id_def_null(extends);
    if (js_parser_type(JS_TOKEN_EXTENDS)) {
        js_parser_next();
        
        js_node_def_token(extends_token);
        if (js_parser_type_not(JS_TOKEN_ID)) {
            js_parser_node_id_free(id);
            return js_parser_error(self->memory, "element class: class <id> extends '<id>'", extends_token, 0);
        }
        js_parser_id_assign(extends);
        js_parser_next();
    }
    
    js_node_def_token(do_token);
    if (js_parser_type_not(JS_TOKEN_DO)) {
        js_parser_node_id_free(id);
        js_parser_node_id_free(extends);
        return js_parser_error(self->memory, "element class: class <id> ... 'do'", do_token, 0);
    }
    js_parser_next();
    
    js_node_list_def(constructor_head, constructor_tail);
    js_node_list_def(field_head, field_tail);
    js_node_list_def(method_head, method_tail);
    
    for (;;) {
        
        js_node_def_token(end_token);
        if (js_parser_type(JS_TOKEN_FUNCTION)) {
            js_node_def_token(func_token);
            js_parser_func_def(node, js_parser_element_function);
            if (js_node_error_is(node)) {
                js_parser_node_id_free(id);
                js_parser_node_id_free(extends);
                js_node_free(constructor_head);
                js_node_free(field_head);
                js_node_free(method_head);
                return js_parser_error(self->memory, "element class: class <id> ... do ... '<function>'", func_token, node);
            }
            js_node_add(method_head, method_tail, node);
        } else if (js_parser_type(JS_TOKEN_CONSTRUCTOR)) {
            js_node_def_token(func_token);
            js_parser_func_def(node, js_parser_element_constructor);
            if (js_node_error_is(node)) {
                js_parser_node_id_free(id);
                js_parser_node_id_free(extends);
                js_node_free(constructor_head);
                js_node_free(field_head);
                js_node_free(method_head);
                return js_parser_error(self->memory, "element class: class <id> ... do ... '<constructor>'", func_token, node);
            }
            js_node_add(constructor_head, constructor_tail, node);
        } else if (js_parser_type(JS_TOKEN_VAR)) {
            js_node_def_token(func_token);
            js_parser_func_def(node, js_parser_element_field);
            if (js_node_error_is(node)) {
                js_parser_node_id_free(id);
                js_parser_node_id_free(extends);
                js_node_free(constructor_head);
                js_node_free(field_head);
                js_node_free(method_head);
                return js_parser_error(self->memory, "element class: class <id> ... do ... '<field>'", func_token, node);
            }
            js_node_add(field_head, field_tail, node);
        } else if (js_parser_type(JS_TOKEN_END)) {
            break;
        } else {
            return js_parser_error(self->memory, "element class: class <id> ... do ... '<constructor> | <function> | <field>'", end_token, 0);
        }
        
    }
    
    js_node_def_token(end_token);
    if (js_parser_type_not(JS_TOKEN_END)) {
        js_parser_node_id_free(id);
        js_parser_node_id_free(extends);
        return js_parser_error(self->memory, "element class: class <id> [extends <id>] do (<field> | <function>)* 'end'", end_token, 0);
    }
    js_parser_next();
    
    return js_node_cast(js_node_class_new(self->memory, id, extends, constructor_head, field_head, method_head));
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
        return js_parser_element_function(self);
    } else if (js_parser_type(JS_TOKEN_CLASS)) {
        return js_parser_element_class(self);
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
