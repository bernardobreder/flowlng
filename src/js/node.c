#include <stdlib.h>
#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "js.h"

void js_node_free(struct js_node_t* self) {
    if (!self) return;
    switch(self->type) {
        case JS_NODE_ERROR: return js_node_error_free((struct js_node_error_t*) self);
        case JS_NODE_FUNCTION: return js_node_function_free((struct js_node_function_t*) self);
        case JS_NODE_IF: return js_node_if_free((struct js_node_if_t*) self);
        case JS_NODE_WHILE: return js_node_while_free((struct js_node_while_t*) self);
        case JS_NODE_RETURN: return js_node_return_free((struct js_node_return_t*) self);
        case JS_NODE_VAR_ITEM: return js_node_var_item_free((struct js_node_var_item_t*) self);
        case JS_NODE_ID: return js_node_id_free((struct js_node_id_t*) self);
        case JS_NODE_STRING: return js_node_string_free((struct js_node_string_t*) self);
        case JS_NODE_NUM: return js_node_num_free((struct js_node_num_t*) self);
        case JS_NODE_INT: return js_node_int_free((struct js_node_int_t*) self);
        case JS_NODE_TRUE: return js_node_true_free((struct js_node_true_t*) self);
        case JS_NODE_FALSE: return js_node_false_free((struct js_node_false_t*) self);
        case JS_NODE_THIS: return js_node_this_free((struct js_node_this_t*) self);
        case JS_NODE_SUPER: return js_node_super_free((struct js_node_super_t*) self);
        case JS_NODE_ASSIGNMENT: return js_node_assignment_free((struct js_node_assignment_t*) self);
        case JS_NODE_TERNARY: return js_node_ternary_free((struct js_node_ternary_t*) self);
        case JS_NODE_OR: return js_node_or_free((struct js_node_or_t*) self);
        case JS_NODE_AND: return js_node_and_free((struct js_node_and_t*) self);
        case JS_NODE_BITWISE_OR: return js_node_bitwise_or_free((struct js_node_bitwise_or_t*) self);
        case JS_NODE_BITWISE_XOR: return js_node_bitwise_xor_free((struct js_node_bitwise_xor_t*) self);
        case JS_NODE_BITWISE_AND: return js_node_bitwise_and_free((struct js_node_bitwise_and_t*) self);
        case JS_NODE_EQUAL: return js_node_equal_free((struct js_node_equal_t*) self);
        case JS_NODE_NOT_EQUAL: return js_node_not_equal_free((struct js_node_not_equal_t*) self);
        case JS_NODE_LOWER_EQUAL: return js_node_lower_equal_free((struct js_node_lower_equal_t*) self);
        case JS_NODE_GREATER_EQUAL: return js_node_greater_equal_free((struct js_node_greater_equal_t*) self);
        case JS_NODE_LOWER_THAN: return js_node_lower_than_free((struct js_node_lower_than_t*) self);
        case JS_NODE_GREATER_THAN: return js_node_greater_than_free((struct js_node_greater_than_t*) self);
        case JS_NODE_SHIFT_LEFT: return js_node_shift_left_free((struct js_node_shift_left_t*) self);
        case JS_NODE_SHIFT_RIGHT: return js_node_shift_right_free((struct js_node_shift_right_t*) self);
        case JS_NODE_SUM: return js_node_sum_free((struct js_node_sum_t*) self);
        case JS_NODE_SUB: return js_node_sub_free((struct js_node_sub_t*) self);
        case JS_NODE_MUL: return js_node_mul_free((struct js_node_mul_t*) self);
        case JS_NODE_DIV: return js_node_div_free((struct js_node_div_t*) self);
        case JS_NODE_NOT: return js_node_not_free((struct js_node_not_t*) self);
        case JS_NODE_NEG: return js_node_neg_free((struct js_node_neg_t*) self);
        case JS_NODE_PRE_INC: return js_node_pre_inc_free((struct js_node_pre_inc_t*) self);
        case JS_NODE_PRE_DEC: return js_node_pre_dec_free((struct js_node_pre_dec_t*) self);
        case JS_NODE_POS_INC: return js_node_pos_inc_free((struct js_node_pos_inc_t*) self);
        case JS_NODE_POS_DEC: return js_node_pos_dec_free((struct js_node_pos_dec_t*) self);
        case JS_NODE_FIELD: return js_node_field_free((struct js_node_field_t*) self);
        case JS_NODE_ARRAY: return js_node_array_free((struct js_node_array_t*) self);
        case JS_NODE_CALL: return js_node_call_free((struct js_node_call_t*) self);
    }
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_compile(struct js_node_t* self) {
    struct js_node_t* node = self;
    while (node) {
        if (js_node_error_is_not(node)) js_node_head(node);
        node = node->next;
    }
    node = self;
    while (node) {
        if (js_node_error_is_not(node)) js_node_body(node);
        node = node->next;
    }
}

void js_node_head(struct js_node_t* self) {
    if (!self) return;
    switch(self->type) {
//        case JS_NODE_FUNCTION: {
//            return js_node_function_head((struct js_node_function_t*) self);
//        }
        case JS_NODE_IF: return js_node_if_head((struct js_node_if_t*) self);
        case JS_NODE_WHILE: return js_node_while_head((struct js_node_while_t*) self);
        case JS_NODE_RETURN: return js_node_return_head((struct js_node_return_t*) self);
        case JS_NODE_EMPTY: return js_node_empty_head((struct js_node_empty_t*) self);
        case JS_NODE_BREAK: return js_node_break_head((struct js_node_break_t*) self);
        case JS_NODE_CONTINUE: return js_node_continue_head((struct js_node_continue_t*) self);
        case JS_NODE_ID: return js_node_id_head((struct js_node_id_t*) self);
        case JS_NODE_STRING: return js_node_string_head((struct js_node_string_t*) self);
        case JS_NODE_NUM: return js_node_num_head((struct js_node_num_t*) self);
        case JS_NODE_INT: return js_node_int_head((struct js_node_int_t*) self);
        case JS_NODE_TRUE: return js_node_true_head((struct js_node_true_t*) self);
        case JS_NODE_FALSE: return js_node_false_head((struct js_node_false_t*) self);
        case JS_NODE_THIS: return js_node_this_head((struct js_node_this_t*) self);
        case JS_NODE_SUPER: return js_node_super_head((struct js_node_super_t*) self);
        case JS_NODE_ASSIGNMENT: return js_node_assignment_head((struct js_node_assignment_t*) self);
        case JS_NODE_TERNARY: return js_node_ternary_head((struct js_node_ternary_t*) self);
        case JS_NODE_OR: return js_node_or_head((struct js_node_or_t*) self);
        case JS_NODE_AND: return js_node_and_head((struct js_node_and_t*) self);
        case JS_NODE_BITWISE_OR: return js_node_bitwise_or_head((struct js_node_bitwise_or_t*) self);
        case JS_NODE_BITWISE_XOR: return js_node_bitwise_xor_head((struct js_node_bitwise_xor_t*) self);
        case JS_NODE_BITWISE_AND: return js_node_bitwise_and_head((struct js_node_bitwise_and_t*) self);
        case JS_NODE_EQUAL: return js_node_equal_head((struct js_node_equal_t*) self);
        case JS_NODE_NOT_EQUAL: return js_node_not_equal_head((struct js_node_not_equal_t*) self);
        case JS_NODE_LOWER_EQUAL: return js_node_lower_equal_head((struct js_node_lower_equal_t*) self);
        case JS_NODE_GREATER_EQUAL: return js_node_greater_equal_head((struct js_node_greater_equal_t*) self);
        case JS_NODE_LOWER_THAN: return js_node_lower_than_head((struct js_node_lower_than_t*) self);
        case JS_NODE_GREATER_THAN: return js_node_greater_than_head((struct js_node_greater_than_t*) self);
        case JS_NODE_SHIFT_LEFT: return js_node_shift_left_head((struct js_node_shift_left_t*) self);
        case JS_NODE_SHIFT_RIGHT: return js_node_shift_right_head((struct js_node_shift_right_t*) self);
        case JS_NODE_SUM: return js_node_sum_head((struct js_node_sum_t*) self);
        case JS_NODE_SUB: return js_node_sub_head((struct js_node_sub_t*) self);
        case JS_NODE_MUL: return js_node_mul_head((struct js_node_mul_t*) self);
        case JS_NODE_DIV: return js_node_div_head((struct js_node_div_t*) self);
        case JS_NODE_NOT: return js_node_not_head((struct js_node_not_t*) self);
        case JS_NODE_NEG: return js_node_neg_head((struct js_node_neg_t*) self);
        case JS_NODE_PRE_INC: return js_node_pre_inc_head((struct js_node_pre_inc_t*) self);
        case JS_NODE_PRE_DEC: return js_node_pre_dec_head((struct js_node_pre_dec_t*) self);
        case JS_NODE_POS_INC: return js_node_pos_inc_head((struct js_node_pos_inc_t*) self);
        case JS_NODE_POS_DEC: return js_node_pos_dec_head((struct js_node_pos_dec_t*) self);
        case JS_NODE_FIELD: return js_node_field_head((struct js_node_field_t*) self);
        case JS_NODE_ARRAY: return js_node_array_head((struct js_node_array_t*) self);
        case JS_NODE_CALL: return js_node_call_head((struct js_node_call_t*) self);
    }
}

void js_node_body(struct js_node_t* self) {
    if (!self) return;
    switch(self->type) {
            //        case JS_NODE_FUNCTION: {
            //            return js_node_function_body((struct js_node_function_t*) self);
            //        }
        case JS_NODE_IF: return js_node_if_body((struct js_node_if_t*) self);
        case JS_NODE_WHILE: return js_node_while_body((struct js_node_while_t*) self);
        case JS_NODE_RETURN: return js_node_return_body((struct js_node_return_t*) self);
        case JS_NODE_EMPTY: return js_node_empty_body((struct js_node_empty_t*) self);
        case JS_NODE_BREAK: return js_node_break_body((struct js_node_break_t*) self);
        case JS_NODE_CONTINUE: return js_node_continue_body((struct js_node_continue_t*) self);
        case JS_NODE_ID: return js_node_id_body((struct js_node_id_t*) self);
        case JS_NODE_STRING: return js_node_string_body((struct js_node_string_t*) self);
        case JS_NODE_NUM: return js_node_num_body((struct js_node_num_t*) self);
        case JS_NODE_INT: return js_node_int_body((struct js_node_int_t*) self);
        case JS_NODE_TRUE: return js_node_true_body((struct js_node_true_t*) self);
        case JS_NODE_FALSE: return js_node_false_body((struct js_node_false_t*) self);
        case JS_NODE_THIS: return js_node_this_body((struct js_node_this_t*) self);
        case JS_NODE_SUPER: return js_node_super_body((struct js_node_super_t*) self);
        case JS_NODE_ASSIGNMENT: return js_node_assignment_body((struct js_node_assignment_t*) self);
        case JS_NODE_TERNARY: return js_node_ternary_body((struct js_node_ternary_t*) self);
        case JS_NODE_OR: return js_node_or_body((struct js_node_or_t*) self);
        case JS_NODE_AND: return js_node_and_body((struct js_node_and_t*) self);
        case JS_NODE_BITWISE_OR: return js_node_bitwise_or_body((struct js_node_bitwise_or_t*) self);
        case JS_NODE_BITWISE_XOR: return js_node_bitwise_xor_body((struct js_node_bitwise_xor_t*) self);
        case JS_NODE_BITWISE_AND: return js_node_bitwise_and_body((struct js_node_bitwise_and_t*) self);
        case JS_NODE_EQUAL: return js_node_equal_body((struct js_node_equal_t*) self);
        case JS_NODE_NOT_EQUAL: return js_node_not_equal_body((struct js_node_not_equal_t*) self);
        case JS_NODE_LOWER_EQUAL: return js_node_lower_equal_body((struct js_node_lower_equal_t*) self);
        case JS_NODE_GREATER_EQUAL: return js_node_greater_equal_body((struct js_node_greater_equal_t*) self);
        case JS_NODE_LOWER_THAN: return js_node_lower_than_body((struct js_node_lower_than_t*) self);
        case JS_NODE_GREATER_THAN: return js_node_greater_than_body((struct js_node_greater_than_t*) self);
        case JS_NODE_SHIFT_LEFT: return js_node_shift_left_body((struct js_node_shift_left_t*) self);
        case JS_NODE_SHIFT_RIGHT: return js_node_shift_right_body((struct js_node_shift_right_t*) self);
        case JS_NODE_SUM: return js_node_sum_body((struct js_node_sum_t*) self);
        case JS_NODE_SUB: return js_node_sub_body((struct js_node_sub_t*) self);
        case JS_NODE_MUL: return js_node_mul_body((struct js_node_mul_t*) self);
        case JS_NODE_DIV: return js_node_div_body((struct js_node_div_t*) self);
        case JS_NODE_NOT: return js_node_not_body((struct js_node_not_t*) self);
        case JS_NODE_NEG: return js_node_neg_body((struct js_node_neg_t*) self);
        case JS_NODE_PRE_INC: return js_node_pre_inc_body((struct js_node_pre_inc_t*) self);
        case JS_NODE_PRE_DEC: return js_node_pre_dec_body((struct js_node_pre_dec_t*) self);
        case JS_NODE_POS_INC: return js_node_pos_inc_body((struct js_node_pos_inc_t*) self);
        case JS_NODE_POS_DEC: return js_node_pos_dec_body((struct js_node_pos_dec_t*) self);
        case JS_NODE_FIELD: return js_node_field_body((struct js_node_field_t*) self);
        case JS_NODE_ARRAY: return js_node_array_body((struct js_node_array_t*) self);
        case JS_NODE_CALL: return js_node_call_body((struct js_node_call_t*) self);
    }
}

void js_node_exec(struct js_node_t* self, struct js_context_t* context) {
    if (!self) return;
    switch(self->type) {
            //        case JS_NODE_FUNCTION: {
            //            return js_node_function_body((struct js_node_function_t*) self);
            //        }
        case JS_NODE_IF: return js_node_if_exec((struct js_node_if_t*) self, context);
        case JS_NODE_WHILE: return js_node_while_exec((struct js_node_while_t*) self, context);
        case JS_NODE_RETURN: return js_node_return_exec((struct js_node_return_t*) self, context);
        case JS_NODE_EMPTY: return js_node_empty_exec((struct js_node_empty_t*) self, context);
        case JS_NODE_BREAK: return js_node_break_exec((struct js_node_break_t*) self, context);
        case JS_NODE_CONTINUE: return js_node_continue_exec((struct js_node_continue_t*) self, context);
        case JS_NODE_ID: return js_node_id_exec((struct js_node_id_t*) self, context);
        case JS_NODE_STRING: return js_node_string_exec((struct js_node_string_t*) self, context);
        case JS_NODE_NUM: return js_node_num_exec((struct js_node_num_t*) self, context);
        case JS_NODE_INT: return js_node_int_exec((struct js_node_int_t*) self, context);
        case JS_NODE_TRUE: return js_node_true_exec((struct js_node_true_t*) self, context);
        case JS_NODE_FALSE: return js_node_false_exec((struct js_node_false_t*) self, context);
        case JS_NODE_THIS: return js_node_this_exec((struct js_node_this_t*) self, context);
        case JS_NODE_SUPER: return js_node_super_exec((struct js_node_super_t*) self, context);
        case JS_NODE_ASSIGNMENT: return js_node_assignment_exec((struct js_node_assignment_t*) self, context);
        case JS_NODE_TERNARY: return js_node_ternary_exec((struct js_node_ternary_t*) self, context);
        case JS_NODE_OR: return js_node_or_exec((struct js_node_or_t*) self, context);
        case JS_NODE_AND: return js_node_and_exec((struct js_node_and_t*) self, context);
        case JS_NODE_BITWISE_OR: return js_node_bitwise_or_exec((struct js_node_bitwise_or_t*) self, context);
        case JS_NODE_BITWISE_XOR: return js_node_bitwise_xor_exec((struct js_node_bitwise_xor_t*) self, context);
        case JS_NODE_BITWISE_AND: return js_node_bitwise_and_exec((struct js_node_bitwise_and_t*) self, context);
        case JS_NODE_EQUAL: return js_node_equal_exec((struct js_node_equal_t*) self, context);
        case JS_NODE_NOT_EQUAL: return js_node_not_equal_exec((struct js_node_not_equal_t*) self, context);
        case JS_NODE_LOWER_EQUAL: return js_node_lower_equal_exec((struct js_node_lower_equal_t*) self, context);
        case JS_NODE_GREATER_EQUAL: return js_node_greater_equal_exec((struct js_node_greater_equal_t*) self, context);
        case JS_NODE_LOWER_THAN: return js_node_lower_than_exec((struct js_node_lower_than_t*) self, context);
        case JS_NODE_GREATER_THAN: return js_node_greater_than_exec((struct js_node_greater_than_t*) self, context);
        case JS_NODE_SHIFT_LEFT: return js_node_shift_left_exec((struct js_node_shift_left_t*) self, context);
        case JS_NODE_SHIFT_RIGHT: return js_node_shift_right_exec((struct js_node_shift_right_t*) self, context);
        case JS_NODE_SUM: return js_node_sum_exec((struct js_node_sum_t*) self, context);
        case JS_NODE_SUB: return js_node_sub_exec((struct js_node_sub_t*) self, context);
        case JS_NODE_MUL: return js_node_mul_exec((struct js_node_mul_t*) self, context);
        case JS_NODE_DIV: return js_node_div_exec((struct js_node_div_t*) self, context);
        case JS_NODE_NOT: return js_node_not_exec((struct js_node_not_t*) self, context);
        case JS_NODE_NEG: return js_node_neg_exec((struct js_node_neg_t*) self, context);
        case JS_NODE_PRE_INC: return js_node_pre_inc_exec((struct js_node_pre_inc_t*) self, context);
        case JS_NODE_PRE_DEC: return js_node_pre_dec_exec((struct js_node_pre_dec_t*) self, context);
        case JS_NODE_POS_INC: return js_node_pos_inc_exec((struct js_node_pos_inc_t*) self, context);
        case JS_NODE_POS_DEC: return js_node_pos_dec_exec((struct js_node_pos_dec_t*) self, context);
        case JS_NODE_FIELD: return js_node_field_exec((struct js_node_field_t*) self, context);
        case JS_NODE_ARRAY: return js_node_array_exec((struct js_node_array_t*) self, context);
        case JS_NODE_CALL: return js_node_call_exec((struct js_node_call_t*) self, context);
    }
}

struct js_node_error_t* js_node_error_new(char* message, char* word, size_t line, size_t column, struct js_node_error_t* next) {
    struct js_node_error_t* self = (struct js_node_error_t*) malloc(sizeof(struct js_node_error_t));
    self->type = JS_NODE_ERROR;
    self->next = next;
    self->message = message;
    self->word = word;
    self->line = line;
    self->column = column;
    return self;
}

void js_node_error_free(struct js_node_error_t* self) {
    free(self->message);
    free(self->word);
    if (self->next) js_node_error_free(self->next);
    free(self);
}

struct js_node_error_t* js_node_error_revert(struct js_node_error_t* self) {
    struct js_node_error_t* node = self;
    struct js_node_error_t* aux = self->next;
    node->next = 0;
    while (aux) {
        struct js_node_error_t* next = aux->next;
        aux->next = node;
        node = aux;
        aux = next;
    }
    return node;
}

void js_node_error_print(struct js_node_error_t* self) {
    struct js_node_error_t* node = self;
    while (node) {
        if (node->word) {
            printf("{state: \"%s\", word: \"%s\", line: %ld, column: %ld}\n", node->message, node->word, node->line, node->column);
        }
        node = node->next;
    }
    printf("\n");
}

struct js_node_function_t* js_node_function_new(struct js_node_id_t* name, struct js_node_t* params, struct js_node_t* statement) {
    struct js_node_function_t* self = (struct js_node_function_t*) malloc(sizeof(struct js_node_function_t));
    self->type = JS_NODE_FUNCTION;
    self->next = 0;
    self->name = name;
    self->params = params;
    self->statement = statement;
    return self;
}

void js_node_function_free(struct js_node_function_t* self) {
    js_node_id_free(self->name);
    js_node_free(self->params);
    js_node_free(self->statement);
    if (self->next) js_node_free(self->next);
    free(self);
}

struct js_node_var_item_t* js_node_var_item_new(struct js_node_id_t* name, struct js_node_t* value) {
    struct js_node_var_item_t* self = (struct js_node_var_item_t*) malloc(sizeof(struct js_node_var_item_t));
    self->type = JS_NODE_VAR_ITEM;
    self->next = 0;
    self->name = name;
    self->value = value;
    return self;
}

void js_node_var_item_free(struct js_node_var_item_t* self) {
    js_node_id_free(self->name);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

struct js_node_empty_t* js_node_empty_new() {
    struct js_node_empty_t* self = (struct js_node_empty_t*) malloc(sizeof(struct js_node_empty_t));
    self->type = JS_NODE_EMPTY;
    self->next = 0;
    return self;
}

void js_node_empty_head(struct js_node_empty_t* self) {
}

void js_node_empty_body(struct js_node_empty_t* self) {
}

void js_node_empty_exec(struct js_node_empty_t* self, struct js_context_t* context) {
}

struct js_node_break_t* js_node_break_new() {
    struct js_node_break_t* self = (struct js_node_break_t*) malloc(sizeof(struct js_node_break_t));
    self->type = JS_NODE_BREAK;
    self->next = 0;
    return self;
}

void js_node_break_head(struct js_node_break_t* self) {
}

void js_node_break_body(struct js_node_break_t* self) {
}

void js_node_break_exec(struct js_node_break_t* self, struct js_context_t* context) {
}

struct js_node_continue_t* js_node_continue_new() {
    struct js_node_continue_t* self = (struct js_node_continue_t*) malloc(sizeof(struct js_node_continue_t));
    self->type = JS_NODE_CONTINUE;
    self->next = 0;
    return self;
}

void js_node_continue_head(struct js_node_continue_t* self) {
}

void js_node_continue_body(struct js_node_continue_t* self) {
}

void js_node_continue_exec(struct js_node_continue_t* self, struct js_context_t* context) {
}

struct js_node_if_t* js_node_if_new(struct js_node_t* expression, struct js_node_t* statement, struct js_node_t* else_statement) {
    struct js_node_if_t* self = (struct js_node_if_t*) malloc(sizeof(struct js_node_if_t));
    self->type = JS_NODE_IF;
    self->next = 0;
    self->expression = expression;
    self->statement = statement;
    self->else_statement = else_statement;
    return self;
}

void js_node_if_free(struct js_node_if_t* self) {
    js_node_free(self->expression);
    js_node_free(self->statement);
    js_node_free(self->else_statement);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_if_head(struct js_node_if_t* self) {
    js_node_head(self->expression);
    js_node_head(self->statement);
    js_node_head(self->else_statement);
}

void js_node_if_body(struct js_node_if_t* self) {
    js_node_body(self->expression);
    js_node_body(self->statement);
    js_node_body(self->else_statement);
}

void js_node_if_exec(struct js_node_if_t* self, struct js_context_t* context) {
    js_node_exec(self->expression, context);
    struct js_value_t* expression = js_context_pop(context);
    if (js_value_is_bool(expression) && js_value_bool_value(expression)) {
        js_node_exec(self->statement, context);
    } else {
        js_node_exec(self->else_statement, context);
    }
}

struct js_node_while_t* js_node_while_new(struct js_node_t* expression, struct js_node_t* statement) {
    struct js_node_while_t* self = (struct js_node_while_t*) malloc(sizeof(struct js_node_while_t));
    self->type = JS_NODE_WHILE;
    self->next = 0;
    self->expression = expression;
    self->statement = statement;
    return self;
}

void js_node_while_free(struct js_node_while_t* self) {
    js_node_free(self->expression);
    js_node_free(self->statement);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_while_head(struct js_node_while_t* self) {
    js_node_head(self->expression);
    js_node_head(self->statement);
}

void js_node_while_body(struct js_node_while_t* self) {
    js_node_body(self->expression);
    js_node_body(self->statement);
}

void js_node_while_exec(struct js_node_while_t* self, struct js_context_t* context) {
    js_node_exec(self->expression, context);
    struct js_value_t* expression = js_context_pop(context);
    while (js_value_is_bool(expression) && js_value_bool_value(expression)) {
        js_node_exec(self->statement, context);
        js_node_exec(self->expression, context);
        expression = js_context_pop(context);
    }
}

struct js_node_return_t* js_node_return_new(struct js_node_t* expression) {
    struct js_node_return_t* self = (struct js_node_return_t*) malloc(sizeof(struct js_node_return_t));
    self->type = JS_NODE_RETURN;
    self->next = 0;
    self->expression = expression;
    return self;
}

void js_node_return_free(struct js_node_return_t* self) {
    js_node_free(self->expression);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_return_head(struct js_node_return_t* self) {
    js_node_head(self->expression);
}

void js_node_return_body(struct js_node_return_t* self) {
    js_node_body(self->expression);
}

void js_node_return_exec(struct js_node_return_t* self, struct js_context_t* context) {
    js_node_exec(self->expression, context);
}

struct js_node_id_t* js_node_id_new(char* word) {
    struct js_node_id_t* self = (struct js_node_id_t*) malloc(sizeof(struct js_node_id_t));
    self->type = JS_NODE_ID;
    self->next = 0;
    self->word = word;
    return self;
}

void js_node_id_free(struct js_node_id_t* self) {
    free(self->word);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_id_head(struct js_node_id_t* self) {
}

void js_node_id_body(struct js_node_id_t* self) {
}

void js_node_id_exec(struct js_node_id_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_null());
}

struct js_node_string_t* js_node_string_new(char* value, size_t length) {
    struct js_node_string_t* self = (struct js_node_string_t*) malloc(sizeof(struct js_node_string_t));
    self->type = JS_NODE_STRING;
    self->next = 0;
    self->value = value;
    self->length = strlen(value);
    self->hash = 1;
    int n; for (n = 0 ; n < self->length ; n++) { self->hash += js_str_hash_prime * value[n]; }
    return self;
}

void js_node_string_free(struct js_node_string_t* self) {
    free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_string_head(struct js_node_string_t* self) {
}

void js_node_string_body(struct js_node_string_t* self) {
}

void js_node_string_exec(struct js_node_string_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_str_new(context->memory, self->value, self->length, self->hash));
}

struct js_node_num_t* js_node_num_new(double value) {
    struct js_node_num_t* self = (struct js_node_num_t*) malloc(sizeof(struct js_node_num_t));
    self->type = JS_NODE_NUM;
    self->next = 0;
    self->value = value;
    return self;
}

void js_node_num_free(struct js_node_num_t* self) {
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_num_head(struct js_node_num_t* self) {
}

void js_node_num_body(struct js_node_num_t* self) {
}

void js_node_num_exec(struct js_node_num_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_num_new(context->memory, self->value));
}

struct js_node_int_t* js_node_int_new(int value) {
    struct js_node_int_t* self = (struct js_node_int_t*) malloc(sizeof(struct js_node_int_t));
    self->type = JS_NODE_INT;
    self->next = 0;
    self->value = value;
    return self;
}

void js_node_int_free(struct js_node_int_t* self) {
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_int_head(struct js_node_int_t* self) {
}

void js_node_int_body(struct js_node_int_t* self) {
}

void js_node_int_exec(struct js_node_int_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_int_new(context->memory, self->value));
}

struct js_node_true_t* js_node_true_new() {
    struct js_node_true_t* self = (struct js_node_true_t*) malloc(sizeof(struct js_node_true_t));
    self->type = JS_NODE_TRUE;
    self->next = 0;
    return self;
}

void js_node_true_free(struct js_node_true_t* self) {
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_true_head(struct js_node_true_t* self) {
}

void js_node_true_body(struct js_node_true_t* self) {
}

void js_node_true_exec(struct js_node_true_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_true());
}

struct js_node_false_t* js_node_false_new() {
    struct js_node_false_t* self = (struct js_node_false_t*) malloc(sizeof(struct js_node_false_t));
    self->type = JS_NODE_FALSE;
    self->next = 0;
    return self;
}

void js_node_false_free(struct js_node_false_t* self) {
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_false_head(struct js_node_false_t* self) {
}

void js_node_false_body(struct js_node_false_t* self) {
}

void js_node_false_exec(struct js_node_false_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_false());
}

struct js_node_null_t* js_node_null_new() {
    struct js_node_null_t* self = (struct js_node_null_t*) malloc(sizeof(struct js_node_null_t));
    self->type = JS_NODE_NULL;
    self->next = 0;
    return self;
}

void js_node_null_free(struct js_node_null_t* self) {
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_null_head(struct js_node_null_t* self) {
}

void js_node_null_body(struct js_node_null_t* self) {
}

void js_node_null_exec(struct js_node_null_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_null());
}

struct js_node_this_t* js_node_this_new() {
    struct js_node_this_t* self = (struct js_node_this_t*) malloc(sizeof(struct js_node_this_t));
    self->type = JS_NODE_THIS;
    self->next = 0;
    return self;
}

void js_node_this_free(struct js_node_this_t* self) {
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_this_head(struct js_node_this_t* self) {
}

void js_node_this_body(struct js_node_this_t* self) {
}

void js_node_this_exec(struct js_node_this_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_null());
}

struct js_node_super_t* js_node_super_new() {
    struct js_node_super_t* self = (struct js_node_super_t*) malloc(sizeof(struct js_node_super_t));
    self->type = JS_NODE_SUPER;
    self->next = 0;
    return self;
}

void js_node_super_free(struct js_node_super_t* self) {
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_super_head(struct js_node_super_t* self) {
}

void js_node_super_body(struct js_node_super_t* self) {
}

void js_node_super_exec(struct js_node_super_t* self, struct js_context_t* context) {
    js_context_push_typed(context, js_value_null());
}

struct js_node_assignment_t* js_node_assignment_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_assignment_t* self = (struct js_node_assignment_t*) malloc(sizeof(struct js_node_assignment_t));
    self->type = JS_NODE_ASSIGNMENT;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_assignment_free(struct js_node_assignment_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_assignment_head(struct js_node_assignment_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_assignment_body(struct js_node_assignment_t* self) {
    js_node_head(self->node);
    js_node_body(self->value);
}

void js_node_assignment_exec(struct js_node_assignment_t* self, struct js_context_t* context) {
    js_node_exec(self->value, context);
}

struct js_node_ternary_t* js_node_ternary_new(struct js_node_t* node, struct js_node_t* true_value, struct js_node_t* false_value) {
    struct js_node_ternary_t* self = (struct js_node_ternary_t*) malloc(sizeof(struct js_node_ternary_t));
    self->type = JS_NODE_TERNARY;
    self->next = 0;
    self->node = node;
    self->true_value = true_value;
    self->false_value = false_value;
    return self;
}

void js_node_ternary_free(struct js_node_ternary_t* self) {
    js_node_free(self->node);
    js_node_free(self->true_value);
    js_node_free(self->false_value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_ternary_head(struct js_node_ternary_t* self) {
    js_node_head(self->node);
    js_node_head(self->true_value);
    js_node_head(self->false_value);
}

void js_node_ternary_body(struct js_node_ternary_t* self) {
    js_node_body(self->node);
    js_node_body(self->true_value);
    js_node_body(self->false_value);
}

void js_node_ternary_exec(struct js_node_ternary_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
    js_value_bool_def(is_true, value);
    js_value_release(value);
    if (is_true) {
        js_node_exec(self->true_value, context);
    } else {
        js_node_exec(self->false_value, context);
    }
}

struct js_node_or_t* js_node_or_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_or_t* self = (struct js_node_or_t*) malloc(sizeof(struct js_node_or_t));
    self->type = JS_NODE_OR;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_or_free(struct js_node_or_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_or_head(struct js_node_or_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_or_body(struct js_node_or_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_or_exec(struct js_node_or_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_bool_def(is_left_true, left);
    if (is_left_true) {
        js_context_push_typed(context, js_value_true());
    } else {
        js_value_exec_def(right, self->value);
        js_value_bool_def(is_right_true, right);
        if (is_right_true) {
            js_context_push_typed(context, js_value_true());
        } else {
            js_context_push_typed(context, js_value_false());
        }
    }
}

struct js_node_and_t* js_node_and_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_and_t* self = (struct js_node_and_t*) malloc(sizeof(struct js_node_and_t));
    self->type = JS_NODE_AND;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_and_free(struct js_node_and_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_and_head(struct js_node_and_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_and_body(struct js_node_and_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_and_exec(struct js_node_and_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_bool_def(is_left_true, left);
    if (!is_left_true) {
        js_context_push_typed(context, js_value_false());
    } else {
        js_value_exec_def(right, self->value);
        js_value_bool_def(is_right_true, right);
        if (!is_right_true) {
            js_context_push_typed(context, js_value_false());
        } else {
            js_context_push_typed(context, js_value_true());
        }
    }
}

struct js_node_bitwise_or_t* js_node_bitwise_or_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_bitwise_or_t* self = (struct js_node_bitwise_or_t*) malloc(sizeof(struct js_node_bitwise_or_t));
    self->type = JS_NODE_BITWISE_OR;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_bitwise_or_free(struct js_node_bitwise_or_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_bitwise_or_head(struct js_node_bitwise_or_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_bitwise_or_body(struct js_node_bitwise_or_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_bitwise_or_exec(struct js_node_bitwise_or_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    if (js_value_is_int(left)) {
        js_value_int_def(intLeft, left);
        js_value_exec_def(right, self->value);
        if (js_value_is_int(right)) {
            js_value_int_def(intRight, left);
            js_value_release(left);
            js_value_release(right);
            js_context_push_typed(context, js_value_int_new(context->memory, intLeft | intRight));
        } else {
            js_value_release(right);
            js_context_push_typed(context, left);
        }
    } else {
        js_context_push_typed(context, left);
    }
}

struct js_node_bitwise_xor_t* js_node_bitwise_xor_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_bitwise_xor_t* self = (struct js_node_bitwise_xor_t*) malloc(sizeof(struct js_node_bitwise_xor_t));
    self->type = JS_NODE_BITWISE_XOR;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_bitwise_xor_free(struct js_node_bitwise_xor_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_bitwise_xor_head(struct js_node_bitwise_xor_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_bitwise_xor_body(struct js_node_bitwise_xor_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_bitwise_xor_exec(struct js_node_bitwise_xor_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    if (js_value_is_int(left)) {
        js_value_int_def(intLeft, left);
        js_value_exec_def(right, self->value);
        if (js_value_is_int(right)) {
            js_value_int_def(intRight, left);
            js_value_release(left);
            js_value_release(right);
            js_context_push_typed(context, js_value_int_new(context->memory, intLeft ^ intRight));
        } else {
            js_value_release(right);
            js_context_push_typed(context, left);
        }
    } else {
        js_context_push_typed(context, left);
    }
}

struct js_node_bitwise_and_t* js_node_bitwise_and_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_bitwise_and_t* self = (struct js_node_bitwise_and_t*) malloc(sizeof(struct js_node_bitwise_and_t));
    self->type = JS_NODE_BITWISE_AND;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_bitwise_and_free(struct js_node_bitwise_and_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_bitwise_and_head(struct js_node_bitwise_and_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_bitwise_and_body(struct js_node_bitwise_and_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_bitwise_and_exec(struct js_node_bitwise_and_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    if (js_value_is_int(left)) {
        js_value_int_def(intLeft, left);
        js_value_exec_def(right, self->value);
        if (js_value_is_int(right)) {
            js_value_int_def(intRight, left);
            js_value_release(left);
            js_value_release(right);
            js_context_push_typed(context, js_value_int_new(context->memory, intLeft & intRight));
        } else {
            js_value_release(right);
            js_context_push_typed(context, left);
        }
    } else {
        js_context_push_typed(context, left);
    }
}

struct js_node_equal_t* js_node_equal_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_equal_t* self = (struct js_node_equal_t*) malloc(sizeof(struct js_node_equal_t));
    self->type = JS_NODE_EQUAL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_equal_free(struct js_node_equal_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_equal_head(struct js_node_equal_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_equal_body(struct js_node_equal_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_equal_exec(struct js_node_equal_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_value_def(value, js_value_bool(js_value_is_equal(context, left, right)));
    js_value_release(left);
    js_value_release(right);
    js_context_push_typed(context, value);
}

struct js_node_not_equal_t* js_node_not_equal_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_not_equal_t* self = (struct js_node_not_equal_t*) malloc(sizeof(struct js_node_not_equal_t));
    self->type = JS_NODE_NOT_EQUAL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_not_equal_free(struct js_node_not_equal_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_not_equal_head(struct js_node_not_equal_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_not_equal_body(struct js_node_not_equal_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_not_equal_exec(struct js_node_not_equal_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_value_def(value, js_value_bool(!js_value_is_equal(context, left, right)));
    js_value_release(left);
    js_value_release(right);
    js_context_push_typed(context, value);
}

struct js_node_lower_equal_t* js_node_lower_equal_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_lower_equal_t* self = (struct js_node_lower_equal_t*) malloc(sizeof(struct js_node_lower_equal_t));
    self->type = JS_NODE_LOWER_EQUAL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_lower_equal_free(struct js_node_lower_equal_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_lower_equal_head(struct js_node_lower_equal_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_lower_equal_body(struct js_node_lower_equal_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_lower_equal_exec(struct js_node_lower_equal_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_value_def(value, js_value_bool(js_value_is_compare(context, left, right) <= 0));
    js_value_release(left);
    js_value_release(right);
    js_context_push_typed(context, value);
}

struct js_node_greater_equal_t* js_node_greater_equal_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_greater_equal_t* self = (struct js_node_greater_equal_t*) malloc(sizeof(struct js_node_greater_equal_t));
    self->type = JS_NODE_GREATER_EQUAL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_greater_equal_free(struct js_node_greater_equal_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_greater_equal_head(struct js_node_greater_equal_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_greater_equal_body(struct js_node_greater_equal_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_greater_equal_exec(struct js_node_greater_equal_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_value_def(value, js_value_bool(js_value_is_compare(context, left, right) >= 0));
    js_value_release(left);
    js_value_release(right);
    js_context_push_typed(context, value);
}

struct js_node_lower_than_t* js_node_lower_than_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_lower_than_t* self = (struct js_node_lower_than_t*) malloc(sizeof(struct js_node_lower_than_t));
    self->type = JS_NODE_LOWER_THAN;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_lower_than_free(struct js_node_lower_than_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_lower_than_head(struct js_node_lower_than_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_lower_than_body(struct js_node_lower_than_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_lower_than_exec(struct js_node_lower_than_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_value_def(value, js_value_bool(js_value_is_compare(context, left, right) < 0));
    js_value_release(left);
    js_value_release(right);
    js_context_push_typed(context, value);
}

struct js_node_greater_than_t* js_node_greater_than_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_greater_than_t* self = (struct js_node_greater_than_t*) malloc(sizeof(struct js_node_greater_than_t));
    self->type = JS_NODE_GREATER_THAN;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_greater_than_free(struct js_node_greater_than_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_greater_than_head(struct js_node_greater_than_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_greater_than_body(struct js_node_greater_than_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_greater_than_exec(struct js_node_greater_than_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_value_def(value, js_value_bool(js_value_is_compare(context, left, right) > 0));
    js_value_release(left);
    js_value_release(right);
    js_context_push_typed(context, value);
}

struct js_node_shift_left_t* js_node_shift_left_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_shift_left_t* self = (struct js_node_shift_left_t*) malloc(sizeof(struct js_node_shift_left_t));
    self->type = JS_NODE_SHIFT_LEFT;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_shift_left_free(struct js_node_shift_left_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_shift_left_head(struct js_node_shift_left_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_shift_left_body(struct js_node_shift_left_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_shift_left_exec(struct js_node_shift_left_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_value_int_new_def(context, value, int_left << int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else {
        js_value_release(right);
        js_context_push_typed(context, left);
    }
}

struct js_node_shift_right_t* js_node_shift_right_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_shift_right_t* self = (struct js_node_shift_right_t*) malloc(sizeof(struct js_node_shift_right_t));
    self->type = JS_NODE_SHIFT_RIGHT;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_shift_right_free(struct js_node_shift_right_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_shift_right_head(struct js_node_shift_right_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_shift_right_body(struct js_node_shift_right_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_shift_right_exec(struct js_node_shift_right_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_value_int_new_def(context, value, int_left >> int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else {
        js_value_release(right);
        js_context_push_typed(context, left);
    }
}

struct js_node_sum_t* js_node_sum_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_sum_t* self = (struct js_node_sum_t*) malloc(sizeof(struct js_node_sum_t));
    self->type = JS_NODE_SUM;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_sum_free(struct js_node_sum_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_sum_head(struct js_node_sum_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_sum_body(struct js_node_sum_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_sum_exec(struct js_node_sum_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_value_int_new_def(context, value, int_left + int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else if (js_value_is_numint(left) && js_value_is_numint(left)) {
        js_value_numint_def(int_left, left);
        js_value_numint_def(int_right, right);
        js_value_num_new_def(context, value, int_left + int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else if (js_value_is_str(left)) {
        js_value_str_def(str_left, left);
        js_value_obj_to_str_def(str_right, right);
        js_value_str_len_def(str_len_left, left);
        js_value_str_len_def(str_len_right, right);
        size_t length = str_len_left + str_len_right;
        js_str buffer = (js_str) alloca(length + 1);
        strncpy(buffer, str_left, str_len_left);
        strncpy(buffer + str_len_left, str_right, str_len_right);
        buffer[length] = 0;
        js_hash hash = 1;
        int n; for (n = 0 ; n < length ; n++) { hash += js_str_hash_prime * buffer[n]; }
        js_value_str_new_def(context, value, buffer, length, hash)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else {
        js_value_release(right);
        js_context_push_typed(context, left);
    }
}

struct js_node_sub_t* js_node_sub_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_sub_t* self = (struct js_node_sub_t*) malloc(sizeof(struct js_node_sub_t));
    self->type = JS_NODE_SUB;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_sub_free(struct js_node_sub_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_sub_head(struct js_node_sub_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_sub_body(struct js_node_sub_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_sub_exec(struct js_node_sub_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_value_int_new_def(context, value, int_left - int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else if (js_value_is_numint(left) && js_value_is_numint(left)) {
        js_value_numint_def(int_left, left);
        js_value_numint_def(int_right, right);
        js_value_num_new_def(context, value, int_left - int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else {
        js_value_release(right);
        js_context_push_typed(context, left);
    }
}

struct js_node_mul_t* js_node_mul_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_mul_t* self = (struct js_node_mul_t*) malloc(sizeof(struct js_node_mul_t));
    self->type = JS_NODE_MUL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_mul_free(struct js_node_mul_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_mul_head(struct js_node_mul_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_mul_body(struct js_node_mul_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_mul_exec(struct js_node_mul_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_value_int_new_def(context, value, int_left * int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else if (js_value_is_numint(left) && js_value_is_numint(left)) {
        js_value_numint_def(int_left, left);
        js_value_numint_def(int_right, right);
        js_value_num_new_def(context, value, int_left * int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else {
        js_value_release(right);
        js_context_push_typed(context, left);
    }
}

struct js_node_div_t* js_node_div_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_div_t* self = (struct js_node_div_t*) malloc(sizeof(struct js_node_div_t));
    self->type = JS_NODE_DIV;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_div_free(struct js_node_div_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_div_head(struct js_node_div_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_div_body(struct js_node_div_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_div_exec(struct js_node_div_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_numint(left) && js_value_is_numint(left)) {
        js_value_numint_def(int_left, left);
        js_value_numint_def(int_right, right);
        js_value_num_new_def(context, value, int_left / int_right)
        js_value_release(left);
        js_value_release(right);
        js_context_push_typed(context, value);
    } else {
        js_value_release(right);
        js_context_push_typed(context, left);
    }
}

struct js_node_not_t* js_node_not_new(struct js_node_t* node) {
    struct js_node_not_t* self = (struct js_node_not_t*) malloc(sizeof(struct js_node_not_t));
    self->type = JS_NODE_NOT;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_not_free(struct js_node_not_t* self) {
    js_node_free(self->node);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_not_head(struct js_node_not_t* self) {
    js_node_head(self->node);
}

void js_node_not_body(struct js_node_not_t* self) {
    js_node_body(self->node);
}

void js_node_not_exec(struct js_node_not_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
    if (js_value_is_bool(value)) {
        js_value_bool_def(bool_value, value);
        js_value_release(value);
        js_context_push_typed(context, js_value_bool(!bool_value));
    }
}

struct js_node_neg_t* js_node_neg_new(struct js_node_t* node) {
    struct js_node_neg_t* self = (struct js_node_neg_t*) malloc(sizeof(struct js_node_neg_t));
    self->type = JS_NODE_NEG;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_neg_free(struct js_node_neg_t* self) {
    js_node_free(self->node);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_neg_head(struct js_node_neg_t* self) {
    js_node_head(self->node);
}

void js_node_neg_body(struct js_node_neg_t* self) {
    js_node_body(self->node);
}

void js_node_neg_exec(struct js_node_neg_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
    if (js_value_is_numint(value)) {
        js_value_numint_def(num_value, value);
        js_value_release(value);
        js_value_num_new_def(context, result, -num_value);
        js_context_push_typed(context, result);
    }
}

struct js_node_pre_inc_t* js_node_pre_inc_new(struct js_node_t* node) {
    struct js_node_pre_inc_t* self = (struct js_node_pre_inc_t*) malloc(sizeof(struct js_node_pre_inc_t));
    self->type = JS_NODE_PRE_INC;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_pre_inc_free(struct js_node_pre_inc_t* self) {
    js_node_free(self->node);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_pre_inc_head(struct js_node_pre_inc_t* self) {
    js_node_head(self->node);
}

void js_node_pre_inc_body(struct js_node_pre_inc_t* self) {
    js_node_body(self->node);
}

void js_node_pre_inc_exec(struct js_node_pre_inc_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
}

struct js_node_pre_dec_t* js_node_pre_dec_new(struct js_node_t* node) {
    struct js_node_pre_dec_t* self = (struct js_node_pre_dec_t*) malloc(sizeof(struct js_node_pre_dec_t));
    self->type = JS_NODE_PRE_DEC;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_pre_dec_free(struct js_node_pre_dec_t* self) {
    js_node_free(self->node);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_pre_dec_head(struct js_node_pre_dec_t* self) {
    js_node_head(self->node);
}

void js_node_pre_dec_body(struct js_node_pre_dec_t* self) {
    js_node_body(self->node);
}

void js_node_pre_dec_exec(struct js_node_pre_dec_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
}

struct js_node_pos_inc_t* js_node_pos_inc_new(struct js_node_t* node) {
    struct js_node_pos_inc_t* self = (struct js_node_pos_inc_t*) malloc(sizeof(struct js_node_pos_inc_t));
    self->type = JS_NODE_POS_INC;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_pos_inc_free(struct js_node_pos_inc_t* self) {
    js_node_free(self->node);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_pos_inc_head(struct js_node_pos_inc_t* self) {
    js_node_head(self->node);
}

void js_node_pos_inc_body(struct js_node_pos_inc_t* self) {
    js_node_body(self->node);
}

void js_node_pos_inc_exec(struct js_node_pos_inc_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
}

struct js_node_pos_dec_t* js_node_pos_dec_new(struct js_node_t* node) {
    struct js_node_pos_dec_t* self = (struct js_node_pos_dec_t*) malloc(sizeof(struct js_node_pos_dec_t));
    self->type = JS_NODE_POS_DEC;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_pos_dec_free(struct js_node_pos_dec_t* self) {
    js_node_free(self->node);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_pos_dec_head(struct js_node_pos_dec_t* self) {
    js_node_head(self->node);
}

void js_node_pos_dec_body(struct js_node_pos_dec_t* self) {
    js_node_body(self->node);
}

void js_node_pos_dec_exec(struct js_node_pos_dec_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
}

struct js_node_field_t* js_node_field_new(struct js_node_t* node, struct js_node_id_t* value) {
    struct js_node_field_t* self = (struct js_node_field_t*) malloc(sizeof(struct js_node_field_t));
    self->type = JS_NODE_FIELD;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_field_free(struct js_node_field_t* self) {
    js_node_free(self->node);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_field_head(struct js_node_field_t* self) {
    js_node_head(self->node);
}

void js_node_field_body(struct js_node_field_t* self) {
    js_node_body(self->node);
}

void js_node_field_exec(struct js_node_field_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
}

struct js_node_array_t* js_node_array_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_array_t* self = (struct js_node_array_t*) malloc(sizeof(struct js_node_array_t));
    self->type = JS_NODE_ARRAY;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_array_free(struct js_node_array_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_array_head(struct js_node_array_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_array_body(struct js_node_array_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_array_exec(struct js_node_array_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
}

struct js_node_call_t* js_node_call_new(struct js_node_t* node, struct js_node_t* value) {
    struct js_node_call_t* self = (struct js_node_call_t*) malloc(sizeof(struct js_node_call_t));
    self->type = JS_NODE_CALL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_call_free(struct js_node_call_t* self) {
    js_node_free(self->node);
    js_node_free(self->value);
    if (self->next) js_node_free(self->next);
    free(self);
}

void js_node_call_head(struct js_node_call_t* self) {
    js_node_head(self->node);
    js_node_head(self->value);
}

void js_node_call_body(struct js_node_call_t* self) {
    js_node_body(self->node);
    js_node_body(self->value);
}

void js_node_call_exec(struct js_node_call_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
}

