#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "xstring.h"
#include "js.h"

void js_node_free(struct js_node_t* self) {
    if (!self) return;
    switch(self->type) {
        case JS_NODE_ERROR: return js_node_error_free((struct js_node_error_t*) self);
        case JS_NODE_FUNCTION: return js_node_function_free((struct js_node_function_t*) self);
        case JS_NODE_CLASS: return js_node_class_free((struct js_node_class_t*) self);
        case JS_NODE_CONSTRUCTOR: return js_node_constructor_free((struct js_node_constructor_t*) self);
        case JS_NODE_FIELD: return js_node_field_free((struct js_node_field_t*) self);
        case JS_NODE_IF: return js_node_if_free((struct js_node_if_t*) self);
        case JS_NODE_WHILE: return js_node_while_free((struct js_node_while_t*) self);
        case JS_NODE_RETURN: return js_node_return_free((struct js_node_return_t*) self);
        case JS_NODE_STMT_EXP: return js_node_stmtexp_free((struct js_node_stmtexp_t*) self);
        case JS_NODE_VAR_ITEM: return js_node_var_item_free((struct js_node_var_item_t*) self);
        case JS_NODE_ID: return js_node_id_free((struct js_node_id_t*) self);
        case JS_NODE_STRING: return js_node_string_free((struct js_node_string_t*) self);
        case JS_NODE_NUM: return js_node_num_free((struct js_node_num_t*) self);
        case JS_NODE_INT: return js_node_int_free((struct js_node_int_t*) self);
        case JS_NODE_TRUE: return js_node_true_free((struct js_node_true_t*) self);
        case JS_NODE_FALSE: return js_node_false_free((struct js_node_false_t*) self);
        case JS_NODE_THIS: return js_node_this_free((struct js_node_this_t*) self);
        case JS_NODE_SUPER: return js_node_super_free((struct js_node_super_t*) self);
        case JS_NODE_BREAK: return js_node_break_free((struct js_node_break_t*) self);
        case JS_NODE_CONTINUE: return js_node_continue_free((struct js_node_continue_t*) self);
        case JS_NODE_EMPTY: return js_node_empty_free((struct js_node_empty_t*) self);
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
        case JS_NODE_GET: return js_node_get_free((struct js_node_get_t*) self);
        case JS_NODE_ARRAY: return js_node_array_free((struct js_node_array_t*) self);
        case JS_NODE_GET_ARRAY: return js_node_get_array_free((struct js_node_get_array_t*) self);
        case JS_NODE_CALL: return js_node_call_free((struct js_node_call_t*) self);
        case JS_NODE_OBJ: return js_node_obj_free((struct js_node_obj_t*) self);
        case JS_NODE_OBJ_ENTRY: return js_node_obj_entry_free((struct js_node_obj_entry_t*) self);
        case JS_NODE_NEW: return js_node_new_free((struct js_node_new_t*) self);
    }
    js_node_free_self(self);
}

void js_node_head(struct js_node_t* self, struct js_compiler_t* compiler) {
    struct js_node_t* node = self;
    while (node) {
        switch(node->type) {
            case JS_NODE_FUNCTION: js_node_function_head((struct js_node_function_t*) node, compiler); break;
            case JS_NODE_CLASS: js_node_class_head((struct js_node_class_t*) node, compiler); break;
            case JS_NODE_CONSTRUCTOR: js_node_constructor_head((struct js_node_constructor_t*) node, compiler); break;
            case JS_NODE_FIELD: js_node_field_head((struct js_node_field_t*) node, compiler); break;
            case JS_NODE_IF: js_node_if_head((struct js_node_if_t*) node, compiler); break;
            case JS_NODE_WHILE: js_node_while_head((struct js_node_while_t*) node, compiler); break;
            case JS_NODE_RETURN: js_node_return_head((struct js_node_return_t*) node, compiler); break;
            case JS_NODE_STMT_EXP: js_node_stmtexp_head((struct js_node_stmtexp_t*) node, compiler); break;
            case JS_NODE_VAR_ITEM: js_node_var_item_head((struct js_node_var_item_t*) node, compiler); break;
            case JS_NODE_EMPTY: js_node_empty_head((struct js_node_empty_t*) node, compiler); break;
            case JS_NODE_BREAK: js_node_break_head((struct js_node_break_t*) node, compiler); break;
            case JS_NODE_CONTINUE: js_node_continue_head((struct js_node_continue_t*) node, compiler); break;
            case JS_NODE_NULL: js_node_null_head((struct js_node_null_t*) node, compiler); break;
            case JS_NODE_ID: js_node_id_head((struct js_node_id_t*) node, compiler); break;
            case JS_NODE_STRING: js_node_string_head((struct js_node_string_t*) node, compiler); break;
            case JS_NODE_NUM: js_node_num_head((struct js_node_num_t*) node, compiler); break;
            case JS_NODE_INT: js_node_int_head((struct js_node_int_t*) node, compiler); break;
            case JS_NODE_TRUE: js_node_true_head((struct js_node_true_t*) node, compiler); break;
            case JS_NODE_FALSE: js_node_false_head((struct js_node_false_t*) node, compiler); break;
            case JS_NODE_THIS: js_node_this_head((struct js_node_this_t*) node, compiler); break;
            case JS_NODE_SUPER: js_node_super_head((struct js_node_super_t*) node, compiler); break;
            case JS_NODE_ASSIGNMENT: js_node_assignment_head((struct js_node_assignment_t*) node, compiler); break;
            case JS_NODE_TERNARY: js_node_ternary_head((struct js_node_ternary_t*) node, compiler); break;
            case JS_NODE_OR: js_node_or_head((struct js_node_or_t*) node, compiler); break;
            case JS_NODE_AND: js_node_and_head((struct js_node_and_t*) node, compiler); break;
            case JS_NODE_BITWISE_OR: js_node_bitwise_or_head((struct js_node_bitwise_or_t*) node, compiler); break;
            case JS_NODE_BITWISE_XOR: js_node_bitwise_xor_head((struct js_node_bitwise_xor_t*) node, compiler); break;
            case JS_NODE_BITWISE_AND: js_node_bitwise_and_head((struct js_node_bitwise_and_t*) node, compiler); break;
            case JS_NODE_EQUAL: js_node_equal_head((struct js_node_equal_t*) node, compiler); break;
            case JS_NODE_NOT_EQUAL: js_node_not_equal_head((struct js_node_not_equal_t*) node, compiler); break;
            case JS_NODE_LOWER_EQUAL: js_node_lower_equal_head((struct js_node_lower_equal_t*) node, compiler); break;
            case JS_NODE_GREATER_EQUAL: js_node_greater_equal_head((struct js_node_greater_equal_t*) node, compiler); break;
            case JS_NODE_LOWER_THAN: js_node_lower_than_head((struct js_node_lower_than_t*) node, compiler); break;
            case JS_NODE_GREATER_THAN: js_node_greater_than_head((struct js_node_greater_than_t*) node, compiler); break;
            case JS_NODE_SHIFT_LEFT: js_node_shift_left_head((struct js_node_shift_left_t*) node, compiler); break;
            case JS_NODE_SHIFT_RIGHT: js_node_shift_right_head((struct js_node_shift_right_t*) node, compiler); break;
            case JS_NODE_SUM: js_node_sum_head((struct js_node_sum_t*) node, compiler); break;
            case JS_NODE_SUB: js_node_sub_head((struct js_node_sub_t*) node, compiler); break;
            case JS_NODE_MUL: js_node_mul_head((struct js_node_mul_t*) node, compiler); break;
            case JS_NODE_DIV: js_node_div_head((struct js_node_div_t*) node, compiler); break;
            case JS_NODE_NOT: js_node_not_head((struct js_node_not_t*) node, compiler); break;
            case JS_NODE_NEG: js_node_neg_head((struct js_node_neg_t*) node, compiler); break;
            case JS_NODE_PRE_INC: js_node_pre_inc_head((struct js_node_pre_inc_t*) node, compiler); break;
            case JS_NODE_PRE_DEC: js_node_pre_dec_head((struct js_node_pre_dec_t*) node, compiler); break;
            case JS_NODE_POS_INC: js_node_pos_inc_head((struct js_node_pos_inc_t*) node, compiler); break;
            case JS_NODE_POS_DEC: js_node_pos_dec_head((struct js_node_pos_dec_t*) node, compiler); break;
            case JS_NODE_GET: js_node_get_head((struct js_node_get_t*) node, compiler); break;
            case JS_NODE_GET_ARRAY: js_node_get_array_head((struct js_node_get_array_t*) node, compiler); break;
            case JS_NODE_ARRAY: js_node_array_head((struct js_node_array_t*) node, compiler); break;
            case JS_NODE_CALL: js_node_call_head((struct js_node_call_t*) node, compiler); break;
            case JS_NODE_OBJ: return js_node_obj_head((struct js_node_obj_t*) self, compiler); break;
            case JS_NODE_OBJ_ENTRY: return js_node_obj_entry_head((struct js_node_obj_entry_t*) self, compiler); break;
            case JS_NODE_NEW: return js_node_new_head((struct js_node_new_t*) self, compiler); break;
        }
        node = node->next;
    }
}

void js_node_body(struct js_node_t* self, struct js_compiler_t* compiler) {
    struct js_node_t* node = self;
    while (node) {
        switch(node->type) {
            case JS_NODE_FUNCTION: js_node_function_body((struct js_node_function_t*) node, compiler); break;
            case JS_NODE_CLASS: js_node_class_body((struct js_node_class_t*) node, compiler); break;
            case JS_NODE_CONSTRUCTOR: js_node_constructor_body((struct js_node_constructor_t*) node, compiler); break;
            case JS_NODE_FIELD: js_node_field_body((struct js_node_field_t*) node, compiler); break;
            case JS_NODE_IF: js_node_if_body((struct js_node_if_t*) node, compiler); break;
            case JS_NODE_WHILE: js_node_while_body((struct js_node_while_t*) node, compiler); break;
            case JS_NODE_RETURN: js_node_return_body((struct js_node_return_t*) node, compiler); break;
            case JS_NODE_STMT_EXP: js_node_stmtexp_body((struct js_node_stmtexp_t*) node, compiler); break;
            case JS_NODE_VAR_ITEM: js_node_var_item_body((struct js_node_var_item_t*) node, compiler); break;
            case JS_NODE_EMPTY: js_node_empty_body((struct js_node_empty_t*) node, compiler); break;
            case JS_NODE_BREAK: js_node_break_body((struct js_node_break_t*) node, compiler); break;
            case JS_NODE_CONTINUE: js_node_continue_body((struct js_node_continue_t*) node, compiler); break;
            case JS_NODE_NULL: js_node_null_body((struct js_node_null_t*) node, compiler); break;
            case JS_NODE_ID: js_node_id_body((struct js_node_id_t*) node, compiler); break;
            case JS_NODE_STRING: js_node_string_body((struct js_node_string_t*) node, compiler); break;
            case JS_NODE_NUM: js_node_num_body((struct js_node_num_t*) node, compiler); break;
            case JS_NODE_INT: js_node_int_body((struct js_node_int_t*) node, compiler); break;
            case JS_NODE_TRUE: js_node_true_body((struct js_node_true_t*) node, compiler); break;
            case JS_NODE_FALSE: js_node_false_body((struct js_node_false_t*) node, compiler); break;
            case JS_NODE_THIS: js_node_this_body((struct js_node_this_t*) node, compiler); break;
            case JS_NODE_SUPER: js_node_super_body((struct js_node_super_t*) node, compiler); break;
            case JS_NODE_ASSIGNMENT: js_node_assignment_body((struct js_node_assignment_t*) node, compiler); break;
            case JS_NODE_TERNARY: js_node_ternary_body((struct js_node_ternary_t*) node, compiler); break;
            case JS_NODE_OR: js_node_or_body((struct js_node_or_t*) node, compiler); break;
            case JS_NODE_AND: js_node_and_body((struct js_node_and_t*) node, compiler); break;
            case JS_NODE_BITWISE_OR: js_node_bitwise_or_body((struct js_node_bitwise_or_t*) node, compiler); break;
            case JS_NODE_BITWISE_XOR: js_node_bitwise_xor_body((struct js_node_bitwise_xor_t*) node, compiler); break;
            case JS_NODE_BITWISE_AND: js_node_bitwise_and_body((struct js_node_bitwise_and_t*) node, compiler); break;
            case JS_NODE_EQUAL: js_node_equal_body((struct js_node_equal_t*) node, compiler); break;
            case JS_NODE_NOT_EQUAL: js_node_not_equal_body((struct js_node_not_equal_t*) node, compiler); break;
            case JS_NODE_LOWER_EQUAL: js_node_lower_equal_body((struct js_node_lower_equal_t*) node, compiler); break;
            case JS_NODE_GREATER_EQUAL: js_node_greater_equal_body((struct js_node_greater_equal_t*) node, compiler); break;
            case JS_NODE_LOWER_THAN: js_node_lower_than_body((struct js_node_lower_than_t*) node, compiler); break;
            case JS_NODE_GREATER_THAN: js_node_greater_than_body((struct js_node_greater_than_t*) node, compiler); break;
            case JS_NODE_SHIFT_LEFT: js_node_shift_left_body((struct js_node_shift_left_t*) node, compiler); break;
            case JS_NODE_SHIFT_RIGHT: js_node_shift_right_body((struct js_node_shift_right_t*) node, compiler); break;
            case JS_NODE_SUM: js_node_sum_body((struct js_node_sum_t*) node, compiler); break;
            case JS_NODE_SUB: js_node_sub_body((struct js_node_sub_t*) node, compiler); break;
            case JS_NODE_MUL: js_node_mul_body((struct js_node_mul_t*) node, compiler); break;
            case JS_NODE_DIV: js_node_div_body((struct js_node_div_t*) node, compiler); break;
            case JS_NODE_NOT: js_node_not_body((struct js_node_not_t*) node, compiler); break;
            case JS_NODE_NEG: js_node_neg_body((struct js_node_neg_t*) node, compiler); break;
            case JS_NODE_PRE_INC: js_node_pre_inc_body((struct js_node_pre_inc_t*) node, compiler); break;
            case JS_NODE_PRE_DEC: js_node_pre_dec_body((struct js_node_pre_dec_t*) node, compiler); break;
            case JS_NODE_POS_INC: js_node_pos_inc_body((struct js_node_pos_inc_t*) node, compiler); break;
            case JS_NODE_POS_DEC: js_node_pos_dec_body((struct js_node_pos_dec_t*) node, compiler); break;
            case JS_NODE_GET: js_node_get_body((struct js_node_get_t*) node, compiler); break;
            case JS_NODE_GET_ARRAY: js_node_get_array_body((struct js_node_get_array_t*) node, compiler); break;
            case JS_NODE_ARRAY: js_node_array_body((struct js_node_array_t*) node, compiler); break;
            case JS_NODE_CALL: js_node_call_body((struct js_node_call_t*) node, compiler); break;
            case JS_NODE_OBJ: return js_node_obj_body((struct js_node_obj_t*) self, compiler); break;
            case JS_NODE_OBJ_ENTRY: return js_node_obj_entry_body((struct js_node_obj_entry_t*) self, compiler); break;
            case JS_NODE_NEW: return js_node_new_body((struct js_node_new_t*) self, compiler); break;
        }
        node = node->next;
    }
}

void js_node_exec_single(struct js_node_t* self, struct js_context_t* context) {
    switch(self->type) {
        case JS_NODE_FUNCTION: js_node_function_exec((struct js_node_function_t*) self, context); break;
        case JS_NODE_CLASS: js_node_class_exec((struct js_node_class_t*) self, context); break;
        case JS_NODE_CONSTRUCTOR: js_node_constructor_exec((struct js_node_constructor_t*) self, context); break;
        case JS_NODE_FIELD: js_node_field_exec((struct js_node_field_t*) self, context); break;
        case JS_NODE_IF: js_node_if_exec((struct js_node_if_t*) self, context); break;
        case JS_NODE_WHILE: js_node_while_exec((struct js_node_while_t*) self, context); break;
        case JS_NODE_STMT_EXP: js_node_stmtexp_exec((struct js_node_stmtexp_t*) self, context); break;
        case JS_NODE_VAR_ITEM: js_node_var_item_exec((struct js_node_var_item_t*) self, context); break;
        case JS_NODE_RETURN: js_node_return_exec((struct js_node_return_t*) self, context); break;
        case JS_NODE_EMPTY: js_node_empty_exec((struct js_node_empty_t*) self, context); break;
        case JS_NODE_BREAK: js_node_break_exec((struct js_node_break_t*) self, context); break;
        case JS_NODE_CONTINUE: js_node_continue_exec((struct js_node_continue_t*) self, context); break;
        case JS_NODE_NULL: js_node_null_exec((struct js_node_null_t*) self, context); break;
        case JS_NODE_ID: js_node_id_exec((struct js_node_id_t*) self, context); break;
        case JS_NODE_STRING: js_node_string_exec((struct js_node_string_t*) self, context); break;
        case JS_NODE_NUM: js_node_num_exec((struct js_node_num_t*) self, context); break;
        case JS_NODE_INT: js_node_int_exec((struct js_node_int_t*) self, context); break;
        case JS_NODE_TRUE: js_node_true_exec((struct js_node_true_t*) self, context); break;
        case JS_NODE_FALSE: js_node_false_exec((struct js_node_false_t*) self, context); break;
        case JS_NODE_THIS: js_node_this_exec((struct js_node_this_t*) self, context); break;
        case JS_NODE_SUPER: js_node_super_exec((struct js_node_super_t*) self, context); break;
        case JS_NODE_ASSIGNMENT: js_node_assignment_exec((struct js_node_assignment_t*) self, context); break;
        case JS_NODE_TERNARY: js_node_ternary_exec((struct js_node_ternary_t*) self, context); break;
        case JS_NODE_OR: js_node_or_exec((struct js_node_or_t*) self, context); break;
        case JS_NODE_AND: js_node_and_exec((struct js_node_and_t*) self, context); break;
        case JS_NODE_BITWISE_OR: js_node_bitwise_or_exec((struct js_node_bitwise_or_t*) self, context); break;
        case JS_NODE_BITWISE_XOR: js_node_bitwise_xor_exec((struct js_node_bitwise_xor_t*) self, context); break;
        case JS_NODE_BITWISE_AND: js_node_bitwise_and_exec((struct js_node_bitwise_and_t*) self, context); break;
        case JS_NODE_EQUAL: js_node_equal_exec((struct js_node_equal_t*) self, context); break;
        case JS_NODE_NOT_EQUAL: js_node_not_equal_exec((struct js_node_not_equal_t*) self, context); break;
        case JS_NODE_LOWER_EQUAL: js_node_lower_equal_exec((struct js_node_lower_equal_t*) self, context); break;
        case JS_NODE_GREATER_EQUAL: js_node_greater_equal_exec((struct js_node_greater_equal_t*) self, context); break;
        case JS_NODE_LOWER_THAN: js_node_lower_than_exec((struct js_node_lower_than_t*) self, context); break;
        case JS_NODE_GREATER_THAN: js_node_greater_than_exec((struct js_node_greater_than_t*) self, context); break;
        case JS_NODE_SHIFT_LEFT: js_node_shift_left_exec((struct js_node_shift_left_t*) self, context); break;
        case JS_NODE_SHIFT_RIGHT: js_node_shift_right_exec((struct js_node_shift_right_t*) self, context); break;
        case JS_NODE_SUM: js_node_sum_exec((struct js_node_sum_t*) self, context); break;
        case JS_NODE_SUB: js_node_sub_exec((struct js_node_sub_t*) self, context); break;
        case JS_NODE_MUL: js_node_mul_exec((struct js_node_mul_t*) self, context); break;
        case JS_NODE_DIV: js_node_div_exec((struct js_node_div_t*) self, context); break;
        case JS_NODE_NOT: js_node_not_exec((struct js_node_not_t*) self, context); break;
        case JS_NODE_NEG: js_node_neg_exec((struct js_node_neg_t*) self, context); break;
        case JS_NODE_PRE_INC: js_node_pre_inc_exec((struct js_node_pre_inc_t*) self, context); break;
        case JS_NODE_PRE_DEC: js_node_pre_dec_exec((struct js_node_pre_dec_t*) self, context); break;
        case JS_NODE_POS_INC: js_node_pos_inc_exec((struct js_node_pos_inc_t*) self, context); break;
        case JS_NODE_POS_DEC: js_node_pos_dec_exec((struct js_node_pos_dec_t*) self, context); break;
        case JS_NODE_GET: js_node_get_exec((struct js_node_get_t*) self, context); break;
        case JS_NODE_GET_ARRAY: js_node_get_array_exec((struct js_node_get_array_t*) self, context); break;
        case JS_NODE_ARRAY: js_node_array_exec((struct js_node_array_t*) self, context); break;
        case JS_NODE_CALL: js_node_call_exec((struct js_node_call_t*) self, context); break;
        case JS_NODE_OBJ: return js_node_obj_exec((struct js_node_obj_t*) self, context); break;
        case JS_NODE_OBJ_ENTRY: return js_node_obj_entry_exec((struct js_node_obj_entry_t*) self, context); break;
        case JS_NODE_NEW: return js_node_new_exec((struct js_node_new_t*) self, context); break;
    }
}

void js_node_exec(struct js_node_t* self, struct js_context_t* context) {
    struct js_node_t* node = self;
    while (node) {
        js_node_exec_single(node, context);
        node = node->next;
    }
}

struct js_node_error_t* js_node_error_new(struct flow_memory_t* memory, char* message, char* word, size_t line, size_t column, struct js_node_error_t* next) {
    struct js_node_error_t* self = flow_memory_alloc_typed(memory, struct js_node_error_t);
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
    flow_memory_item_free(self);
}

struct js_node_error_t* js_nodes_error_is(struct js_node_t* self) {
    struct js_node_t* node = self;
    while (node) {
        if (js_node_error_is(node)) return js_node_error_type(node);
        node = node->next;
    }
    return 0;
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

struct js_node_class_t* js_node_class_new(struct flow_memory_t* memory, struct js_node_id_t* name, struct js_node_id_t* extends, struct js_node_t* constructor, struct js_node_t* field, struct js_node_t* method) {
    struct js_node_class_t* self = flow_memory_alloc_typed(memory, struct js_node_class_t);
    self->type = JS_NODE_CLASS;
    self->next = 0;
    self->name = name;
    self->extends = extends;
    self->constructor = constructor;
    self->field = field;
    self->method = method;
    return self;
}

void js_node_class_free(struct js_node_class_t* self) {
    js_node_free_typed(self->name);
    js_node_free_typed(self->extends);
    js_node_free_typed(self->constructor);
    js_node_free_typed(self->field);
    js_node_free_typed(self->method);
    js_node_free_self(self);
}

void js_node_class_head(struct js_node_class_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->name, compiler);
}

void js_node_class_body(struct js_node_class_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->name, compiler);
}

void js_node_class_exec(struct js_node_class_t* self, struct js_context_t* context) {
}

struct js_node_function_t* js_node_function_new(struct flow_memory_t* memory, struct js_node_id_t* name, struct js_node_param_t* param, struct js_node_t* statement) {
    struct js_node_function_t* self = flow_memory_alloc_typed(memory, struct js_node_function_t);
    self->type = JS_NODE_FUNCTION;
    self->next = 0;
    self->name = name;
    self->param = param;
    self->statement = statement;
    return self;
}

void js_node_function_free(struct js_node_function_t* self) {
    js_node_free_typed(self->name);
    js_node_free_typed(self->param);
    js_node_free_typed(self->statement);
    js_node_free_self(self);
}

void js_node_function_head(struct js_node_function_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->name, compiler);
    js_node_head_typed(self->param, compiler);
    js_node_head_typed(self->statement, compiler);
}

void js_node_function_body(struct js_node_function_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->name, compiler);
    js_node_body_typed(self->param, compiler);
    js_node_body_typed(self->statement, compiler);
}

void js_node_function_exec(struct js_node_function_t* self, struct js_context_t* context) {
    js_value_func_new(context, self);
    js_context_peek_func_def(context, func);
    js_value_retain(func);
    js_context_pop(context);
    
    if (js_context_peek_is_obj(context)) {
        js_context_peek_def(context, this_value);
        js_value_obj_def(this_obj, this_value);
        js_value_obj_set(context, this_obj, self->name, js_value_cast(func));
    }
    
    js_value_release(func);
}

struct js_node_param_t* js_node_param_new(struct flow_memory_t* memory, struct js_node_id_t* name) {
    struct js_node_param_t* self = flow_memory_alloc_typed(memory, struct js_node_param_t);
    self->type = JS_NODE_PARAM;
    self->next = 0;
    self->name = name;
    return self;
}

void js_node_param_free(struct js_node_param_t* self) {
    js_node_free_typed(self->name);
    js_node_free_self(self);
}

void js_node_param_head(struct js_node_param_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->name, compiler);
}

void js_node_param_body(struct js_node_param_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->name, compiler);
}

void js_node_param_exec(struct js_node_param_t* self, struct js_context_t* context) {
}

struct js_node_constructor_t* js_node_constructor_new(struct flow_memory_t* memory, struct js_node_t* params, struct js_node_t* statement) {
    struct js_node_constructor_t* self = flow_memory_alloc_typed(memory, struct js_node_constructor_t);
    self->type = JS_NODE_CONSTRUCTOR;
    self->next = 0;
    self->params = params;
    self->statement = statement;
    return self;
}

void js_node_constructor_free(struct js_node_constructor_t* self) {
    js_node_free_typed(self->params);
    js_node_free_typed(self->statement);
    js_node_free_self(self);
}

void js_node_constructor_head(struct js_node_constructor_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->params, compiler);
    js_node_head_typed(self->statement, compiler);
}

void js_node_constructor_body(struct js_node_constructor_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->params, compiler);
    js_node_body_typed(self->statement, compiler);
}

void js_node_constructor_exec(struct js_node_constructor_t* self, struct js_context_t* context) {
    
}

struct js_node_field_t* js_node_field_new(struct flow_memory_t* memory, struct js_node_id_t* name) {
    struct js_node_field_t* self = flow_memory_alloc_typed(memory, struct js_node_field_t);
    self->type = JS_NODE_FIELD;
    self->next = 0;
    self->name = name;
    return self;
}

void js_node_field_free(struct js_node_field_t* self) {
    js_node_free_typed(self->name);
    js_node_free_self(self);
}

void js_node_field_head(struct js_node_field_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->name, compiler);
}

void js_node_field_body(struct js_node_field_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->name, compiler);
}

void js_node_field_exec(struct js_node_field_t* self, struct js_context_t* context) {
}

struct js_node_var_item_t* js_node_var_item_new(struct flow_memory_t* memory, struct js_node_id_t* name, struct js_node_t* value) {
    struct js_node_var_item_t* self = flow_memory_alloc_typed(memory, struct js_node_var_item_t);
    self->type = JS_NODE_VAR_ITEM;
    self->next = 0;
    self->name = name;
    self->value = value;
    return self;
}

void js_node_var_item_free(struct js_node_var_item_t* self) {
    js_node_id_free(self->name);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_var_item_head(struct js_node_var_item_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->name, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_var_item_body(struct js_node_var_item_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->name, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_var_item_exec(struct js_node_var_item_t* self, struct js_context_t* context) {
    if (js_context_peek_is_obj(context)) {
        js_context_peek_obj_def(context, obj);
        js_value_exec_def(value, self->value);
        js_value_obj_set(context, obj, self->name, value);
        js_context_pop(context);
    }
}

struct js_node_empty_t* js_node_empty_new(struct flow_memory_t* memory) {
    struct js_node_empty_t* self = flow_memory_alloc_typed(memory, struct js_node_empty_t);
    self->type = JS_NODE_EMPTY;
    self->next = 0;
    return self;
}

void js_node_empty_free(struct js_node_empty_t* self) {
    js_node_free_self(self);
}

void js_node_empty_head(struct js_node_empty_t* self, struct js_compiler_t* compiler) {
}

void js_node_empty_body(struct js_node_empty_t* self, struct js_compiler_t* compiler) {
}

void js_node_empty_exec(struct js_node_empty_t* self, struct js_context_t* context) {
}

struct js_node_break_t* js_node_break_new(struct flow_memory_t* memory) {
    struct js_node_break_t* self = flow_memory_alloc_typed(memory, struct js_node_break_t);
    self->type = JS_NODE_BREAK;
    self->next = 0;
    return self;
}

void js_node_break_free(struct js_node_break_t* self) {
    js_node_free_self(self);
}

void js_node_break_head(struct js_node_break_t* self, struct js_compiler_t* compiler) {
}

void js_node_break_body(struct js_node_break_t* self, struct js_compiler_t* compiler) {
}

void js_node_break_exec(struct js_node_break_t* self, struct js_context_t* context) {
}

struct js_node_continue_t* js_node_continue_new(struct flow_memory_t* memory) {
    struct js_node_continue_t* self = flow_memory_alloc_typed(memory, struct js_node_continue_t);
    self->type = JS_NODE_CONTINUE;
    self->next = 0;
    return self;
}

void js_node_continue_free(struct js_node_continue_t* self) {
    js_node_free_self(self);
}

void js_node_continue_head(struct js_node_continue_t* self, struct js_compiler_t* compiler) {
}

void js_node_continue_body(struct js_node_continue_t* self, struct js_compiler_t* compiler) {
}

void js_node_continue_exec(struct js_node_continue_t* self, struct js_context_t* context) {
}

struct js_node_stmtexp_t* js_node_stmtexp_new(struct flow_memory_t* memory, struct js_node_t* expression) {
    struct js_node_stmtexp_t* self = flow_memory_alloc_typed(memory, struct js_node_stmtexp_t);
    self->type = JS_NODE_STMT_EXP;
    self->next = 0;
    self->expression = expression;
    return self;
}

void js_node_stmtexp_free(struct js_node_stmtexp_t* self) {
    js_node_free_typed(self->expression);
    js_node_free_self(self);
}

void js_node_stmtexp_head(struct js_node_stmtexp_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->expression, compiler);
}

void js_node_stmtexp_body(struct js_node_stmtexp_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->expression, compiler);
}

void js_node_stmtexp_exec(struct js_node_stmtexp_t* self, struct js_context_t* context) {
    js_node_exec_typed(self->expression, context);
    js_context_pop(context);
}

struct js_node_if_t* js_node_if_new(struct flow_memory_t* memory, struct js_node_t* expression, struct js_node_t* statement, struct js_node_t* else_statement) {
    struct js_node_if_t* self = flow_memory_alloc_typed(memory, struct js_node_if_t);
    self->type = JS_NODE_IF;
    self->next = 0;
    self->expression = expression;
    self->statement = statement;
    self->else_statement = else_statement;
    return self;
}

void js_node_if_free(struct js_node_if_t* self) {
    js_node_free_typed(self->expression);
    js_node_free_typed(self->statement);
    js_node_free_typed(self->else_statement);
    js_node_free_self(self);
}

void js_node_if_head(struct js_node_if_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->expression, compiler);
    js_node_head_typed(self->statement, compiler);
    js_node_head_typed(self->else_statement, compiler);
}

void js_node_if_body(struct js_node_if_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->expression, compiler);
    js_node_body_typed(self->statement, compiler);
    js_node_body_typed(self->else_statement, compiler);
}

void js_node_if_exec(struct js_node_if_t* self, struct js_context_t* context) {
    js_node_exec_typed(self->expression, context);
    js_context_peek_def(context, expression);
    js_value_bool_def(is_true, expression);
    js_context_pop(context);
    if (is_true) {
        js_node_exec_typed(self->statement, context);
    } else {
        js_node_exec_typed(self->else_statement, context);
    }
}

struct js_node_while_t* js_node_while_new(struct flow_memory_t* memory, struct js_node_t* expression, struct js_node_t* statement) {
    struct js_node_while_t* self = flow_memory_alloc_typed(memory, struct js_node_while_t);
    self->type = JS_NODE_WHILE;
    self->next = 0;
    self->expression = expression;
    self->statement = statement;
    return self;
}

void js_node_while_free(struct js_node_while_t* self) {
    js_node_free_typed(self->expression);
    js_node_free_typed(self->statement);
    js_node_free_self(self);
}

void js_node_while_head(struct js_node_while_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->expression, compiler);
    js_node_head_typed(self->statement, compiler);
}

void js_node_while_body(struct js_node_while_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->expression, compiler);
    js_node_body_typed(self->statement, compiler);
}

void js_node_while_exec(struct js_node_while_t* self, struct js_context_t* context) {
    js_node_exec_typed(self->expression, context);
    js_context_peek_def(context, expression);
    js_value_bool_def(is_true, expression);
    js_context_pop(context);
    while (is_true) {
        js_node_exec_typed(self->statement, context);
    }
}

struct js_node_return_t* js_node_return_new(struct flow_memory_t* memory, struct js_node_t* expression) {
    struct js_node_return_t* self = flow_memory_alloc_typed(memory, struct js_node_return_t);
    self->type = JS_NODE_RETURN;
    self->next = 0;
    self->expression = expression;
    return self;
}

void js_node_return_free(struct js_node_return_t* self) {
    js_node_free_typed(self->expression);
    js_node_free_self(self);
}

void js_node_return_head(struct js_node_return_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->expression, compiler);
}

void js_node_return_body(struct js_node_return_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->expression, compiler);
}

void js_node_return_exec(struct js_node_return_t* self, struct js_context_t* context) {
    js_node_exec_typed(self->expression, context);
}

struct js_node_id_t* js_node_id_new(struct flow_memory_t* memory, char* word, size_t length, js_hash hash) {
    struct js_node_id_t* self = flow_memory_alloc_typed(memory, struct js_node_id_t);
    self->type = JS_NODE_ID;
    self->next = 0;
    self->word = word;
    self->length = length;
    self->hash = hash;
    return self;
}

void js_node_id_free(struct js_node_id_t* self) {
    js_free(self->word);
    js_node_free_self(self);
}

void js_node_id_head(struct js_node_id_t* self, struct js_compiler_t* compiler) {
}

void js_node_id_body(struct js_node_id_t* self, struct js_compiler_t* compiler) {
}

void js_node_id_exec(struct js_node_id_t* self, struct js_context_t* context) {
    if (js_context_peek_is_obj(context)) {
        js_context_peek_obj_def(context, obj);
        js_value_obj_get(context, obj, self);
    } else {
        js_value_null_new(context);
    }
}

struct js_node_string_t* js_node_string_new(struct flow_memory_t* memory, char* value, size_t length) {
    struct js_node_string_t* self = flow_memory_alloc_typed(memory, struct js_node_string_t);
    self->type = JS_NODE_STRING;
    self->next = 0;
    self->value = value;
    self->length = strlen(value);
    self->hash = js_hash_perform(value, length);
    return self;
}

void js_node_string_free(struct js_node_string_t* self) {
    free(self->value);
    js_node_free_self(self);
}

void js_node_string_head(struct js_node_string_t* self, struct js_compiler_t* compiler) {
}

void js_node_string_body(struct js_node_string_t* self, struct js_compiler_t* compiler) {
}

void js_node_string_exec(struct js_node_string_t* self, struct js_context_t* context) {
    js_value_str_new(context, self->value, self->length, self->hash);
}

struct js_node_num_t* js_node_num_new(struct flow_memory_t* memory, js_num value) {
    struct js_node_num_t* self = flow_memory_alloc_typed(memory, struct js_node_num_t);
    self->type = JS_NODE_NUM;
    self->next = 0;
    self->value = value;
    return self;
}

void js_node_num_free(struct js_node_num_t* self) {
    js_node_free_self(self);
}

void js_node_num_head(struct js_node_num_t* self, struct js_compiler_t* compiler) {
}

void js_node_num_body(struct js_node_num_t* self, struct js_compiler_t* compiler) {
}

void js_node_num_exec(struct js_node_num_t* self, struct js_context_t* context) {
    js_value_num_new(context, self->value);
}

struct js_node_int_t* js_node_int_new(struct flow_memory_t* memory, js_int value) {
    struct js_node_int_t* self = flow_memory_alloc_typed(memory, struct js_node_int_t);
    self->type = JS_NODE_INT;
    self->next = 0;
    self->value = value;
    return self;
}

void js_node_int_free(struct js_node_int_t* self) {
    js_node_free_self(self);
}

void js_node_int_head(struct js_node_int_t* self, struct js_compiler_t* compiler) {
}

void js_node_int_body(struct js_node_int_t* self, struct js_compiler_t* compiler) {
}

void js_node_int_exec(struct js_node_int_t* self, struct js_context_t* context) {
    js_value_int_new(context, self->value);
}

struct js_node_true_t* js_node_true_new(struct flow_memory_t* memory) {
    struct js_node_true_t* self = flow_memory_alloc_typed(memory, struct js_node_true_t);
    self->type = JS_NODE_TRUE;
    self->next = 0;
    return self;
}

void js_node_true_free(struct js_node_true_t* self) {
    js_node_free_self(self);
}

void js_node_true_head(struct js_node_true_t* self, struct js_compiler_t* compiler) {
}

void js_node_true_body(struct js_node_true_t* self, struct js_compiler_t* compiler) {
}

void js_node_true_exec(struct js_node_true_t* self, struct js_context_t* context) {
    js_value_true_new(context);
}

struct js_node_false_t* js_node_false_new(struct flow_memory_t* memory) {
    struct js_node_false_t* self = flow_memory_alloc_typed(memory, struct js_node_false_t);
    self->type = JS_NODE_FALSE;
    self->next = 0;
    return self;
}

void js_node_false_free(struct js_node_false_t* self) {
    js_node_free_self(self);
}

void js_node_false_head(struct js_node_false_t* self, struct js_compiler_t* compiler) {
}

void js_node_false_body(struct js_node_false_t* self, struct js_compiler_t* compiler) {
}

void js_node_false_exec(struct js_node_false_t* self, struct js_context_t* context) {
    js_value_false_new(context);
}

struct js_node_null_t* js_node_null_new(struct flow_memory_t* memory) {
    struct js_node_null_t* self = flow_memory_alloc_typed(memory, struct js_node_null_t);
    self->type = JS_NODE_NULL;
    self->next = 0;
    return self;
}

void js_node_null_free(struct js_node_null_t* self) {
    js_node_free_self(self);
}

void js_node_null_head(struct js_node_null_t* self, struct js_compiler_t* compiler) {
}

void js_node_null_body(struct js_node_null_t* self, struct js_compiler_t* compiler) {
}

void js_node_null_exec(struct js_node_null_t* self, struct js_context_t* context) {
    js_value_null_new(context);
}

struct js_node_this_t* js_node_this_new(struct flow_memory_t* memory) {
    struct js_node_this_t* self = flow_memory_alloc_typed(memory, struct js_node_this_t);
    self->type = JS_NODE_THIS;
    self->next = 0;
    return self;
}

void js_node_this_free(struct js_node_this_t* self) {
    js_node_free_self(self);
}

void js_node_this_head(struct js_node_this_t* self, struct js_compiler_t* compiler) {
}

void js_node_this_body(struct js_node_this_t* self, struct js_compiler_t* compiler) {
}

void js_node_this_exec(struct js_node_this_t* self, struct js_context_t* context) {
    js_context_peek_def(context, this);
    js_context_push_typed(context, this);
}

struct js_node_obj_t* js_node_obj_new(struct flow_memory_t* memory, struct js_node_obj_entry_t* entry) {
    struct js_node_obj_t* self = flow_memory_alloc_typed(memory, struct js_node_obj_t);
    self->type = JS_NODE_OBJ;
    self->next = 0;
    self->entry = entry;
    return self;
}

void js_node_obj_free(struct js_node_obj_t* self) {
    if (self->entry) js_node_free(js_node_cast(self->entry));
    js_node_free_self(self);
}

void js_node_obj_head(struct js_node_obj_t* self, struct js_compiler_t* compiler) {
    if (self->entry) js_node_head(js_node_cast(self->entry), compiler);
}

void js_node_obj_body(struct js_node_obj_t* self, struct js_compiler_t* compiler) {
    if (self->entry) js_node_body(js_node_cast(self->entry), compiler);
}

void js_node_obj_exec(struct js_node_obj_t* self, struct js_context_t* context) {
    js_value_obj_new(context);
    if (self->entry) js_node_exec(js_node_cast(self->entry), context);
}

struct js_node_obj_entry_t* js_node_obj_entry_new(struct flow_memory_t* memory, struct js_node_id_t* name, struct js_node_t* value) {
    struct js_node_obj_entry_t* self = flow_memory_alloc_typed(memory, struct js_node_obj_entry_t);
    self->type = JS_NODE_OBJ_ENTRY;
    self->next = 0;
    self->name = name;
    self->value = value;
    return self;
}

void js_node_obj_entry_free(struct js_node_obj_entry_t* self) {
    js_node_free_typed(self->name);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_obj_entry_head(struct js_node_obj_entry_t* self, struct js_compiler_t* compiler) {
    js_node_head(js_node_cast(self->name), compiler);
    js_node_head(js_node_cast(self->value), compiler);
}

void js_node_obj_entry_body(struct js_node_obj_entry_t* self, struct js_compiler_t* compiler) {
    js_node_body(js_node_cast(self->name), compiler);
    js_node_body(js_node_cast(self->value), compiler);
}

void js_node_obj_entry_exec(struct js_node_obj_entry_t* self, struct js_context_t* context) {
    js_node_exec(self->value, context);
    js_context_peek_def(context, value);
    js_value_retain(value);
    js_context_pop(context);
    
    js_context_peek_obj_def(context, obj);
    js_value_obj_set(context, obj, self->name, value);
    
    js_value_release(value);
    
    if (self->next) js_node_exec(js_node_cast(self->next), context);
}

struct js_node_array_t* js_node_array_new(struct flow_memory_t* memory, struct js_node_t* node) {
    struct js_node_array_t* self = flow_memory_alloc_typed(memory, struct js_node_array_t);
    self->type = JS_NODE_ARRAY;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_array_free(struct js_node_array_t* self) {
    js_node_free_typed(self->node);
    js_node_free_self(self);
}

void js_node_array_head(struct js_node_array_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
}

void js_node_array_body(struct js_node_array_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
}

void js_node_array_exec(struct js_node_array_t* self, struct js_context_t* context) {
    js_value_array_new(context);
    js_context_peek_def(context, array);
    js_value_retain(array);
    js_context_pop(context);
    
    struct js_node_t* node = self->node;
    while (node) {
        js_node_exec_single(node, context);
        js_context_peek_def(context, value);
        js_value_retain(value);
        js_context_pop(context);
        
        js_context_push_typed(context, array);
        js_context_push_typed(context, value);
        
        js_value_array_add(context);
        
        js_value_release(array);
        js_value_release(value);
        js_value_release(value);
        
        node = node->next;
    }
    
    js_context_push_typed(context, array);
    js_value_release(array);
}

struct js_node_new_t* js_node_new_new(struct flow_memory_t* memory, struct js_node_id_t* id, struct js_node_t* param) {
    struct js_node_new_t* self = flow_memory_alloc_typed(memory, struct js_node_new_t);
    self->type = JS_NODE_NEW;
    self->next = 0;
    self->id = id;
    self->param = param;
    return self;
}

void js_node_new_free(struct js_node_new_t* self) {
    js_node_free_typed(self->id);
    js_node_free_typed(self->param);
    js_node_free_self(self);
}

void js_node_new_head(struct js_node_new_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->id, compiler);
    js_node_head_typed(self->param, compiler);
}

void js_node_new_body(struct js_node_new_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->id, compiler);
    js_node_body_typed(self->param, compiler);
}

void js_node_new_exec(struct js_node_new_t* self, struct js_context_t* context) {
    js_value_obj_new(context);
}

struct js_node_super_t* js_node_super_new(struct flow_memory_t* memory) {
    struct js_node_super_t* self = flow_memory_alloc_typed(memory, struct js_node_super_t);
    self->type = JS_NODE_SUPER;
    self->next = 0;
    return self;
}

void js_node_super_free(struct js_node_super_t* self) {
    js_node_free_self(self);
}

void js_node_super_head(struct js_node_super_t* self, struct js_compiler_t* compiler) {
}

void js_node_super_body(struct js_node_super_t* self, struct js_compiler_t* compiler) {
}

void js_node_super_exec(struct js_node_super_t* self, struct js_context_t* context) {
}

struct js_node_assignment_t* js_node_assignment_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_id_t* name, struct js_node_t* value) {
    struct js_node_assignment_t* self = flow_memory_alloc_typed(memory, struct js_node_assignment_t);
    self->type = JS_NODE_ASSIGNMENT;
    self->next = 0;
    self->node = node;
    self->name = name;
    self->value = value;
    return self;
}

void js_node_assignment_free(struct js_node_assignment_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->name);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_assignment_head(struct js_node_assignment_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_assignment_body(struct js_node_assignment_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_assignment_exec(struct js_node_assignment_t* self, struct js_context_t* context) {
    if (js_context_peek_is_obj(context)) {
        js_context_peek_obj_def(context, obj);
        js_value_exec_def(value, self->value);
        js_value_obj_set(context, obj, self->name, value);
    } else {
        js_node_exec(self->value, context);
    }
}

struct js_node_ternary_t* js_node_ternary_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* true_value, struct js_node_t* false_value) {
    struct js_node_ternary_t* self = flow_memory_alloc_typed(memory, struct js_node_ternary_t);
    self->type = JS_NODE_TERNARY;
    self->next = 0;
    self->node = node;
    self->true_value = true_value;
    self->false_value = false_value;
    return self;
}

void js_node_ternary_free(struct js_node_ternary_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->true_value);
    js_node_free_typed(self->false_value);
    js_node_free_self(self);
}

void js_node_ternary_head(struct js_node_ternary_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->true_value, compiler);
    js_node_head_typed(self->false_value, compiler);
}

void js_node_ternary_body(struct js_node_ternary_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->true_value, compiler);
    js_node_body_typed(self->false_value, compiler);
}

void js_node_ternary_exec(struct js_node_ternary_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
    js_value_bool_def(is_true, value);
    js_context_pop(context);
    if (is_true) {
        js_node_exec(self->true_value, context);
    } else {
        js_node_exec(self->false_value, context);
    }
}

struct js_node_or_t* js_node_or_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_or_t* self = flow_memory_alloc_typed(memory, struct js_node_or_t);
    self->type = JS_NODE_OR;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_or_free(struct js_node_or_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_or_head(struct js_node_or_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_or_body(struct js_node_or_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_or_exec(struct js_node_or_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_bool_def(is_left_true, left);
    js_context_pop(context);
    if (is_left_true) {
        js_value_true_new(context);
    } else {
        js_value_exec_def(right, self->value);
        js_value_bool_def(is_right_true, right);
        js_context_pop(context);
        if (is_right_true) {
            js_value_true_new(context);
        } else {
            js_value_false_new(context);
        }
    }
}

struct js_node_and_t* js_node_and_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_and_t* self = flow_memory_alloc_typed(memory, struct js_node_and_t);
    self->type = JS_NODE_AND;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_and_free(struct js_node_and_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_and_head(struct js_node_and_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_and_body(struct js_node_and_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_and_exec(struct js_node_and_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_bool_def(is_left_true, left);
    js_context_pop(context);
    if (!is_left_true) {
        js_value_false_new(context);
    } else {
        js_value_exec_def(right, self->value);
        js_value_bool_def(is_right_true, right);
        js_context_pop(context);
        if (!is_right_true) {
            js_value_false_new(context);
        } else {
            js_value_true_new(context);
        }
    }
}

struct js_node_bitwise_or_t* js_node_bitwise_or_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_bitwise_or_t* self = flow_memory_alloc_typed(memory, struct js_node_bitwise_or_t);
    self->type = JS_NODE_BITWISE_OR;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_bitwise_or_free(struct js_node_bitwise_or_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_bitwise_or_head(struct js_node_bitwise_or_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_bitwise_or_body(struct js_node_bitwise_or_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_bitwise_or_exec(struct js_node_bitwise_or_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    if (js_value_is_int(left)) {
        js_value_int_def(intLeft, left);
        js_context_pop(context);
        js_value_exec_def(right, self->value);
        if (js_value_is_int(right)) {
            js_value_int_def(intRight, left);
            js_context_pop(context);
            js_value_int_new(context, intLeft | intRight);
        }
    } else {
        js_context_pop(context);
        js_context_pop(context);
    }
}

struct js_node_bitwise_xor_t* js_node_bitwise_xor_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_bitwise_xor_t* self = flow_memory_alloc_typed(memory, struct js_node_bitwise_xor_t);
    self->type = JS_NODE_BITWISE_XOR;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_bitwise_xor_free(struct js_node_bitwise_xor_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_bitwise_xor_head(struct js_node_bitwise_xor_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_bitwise_xor_body(struct js_node_bitwise_xor_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_bitwise_xor_exec(struct js_node_bitwise_xor_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    if (js_value_is_int(left)) {
        js_value_int_def(intLeft, left);
        js_context_pop(context);
        js_value_exec_def(right, self->value);
        if (js_value_is_int(right)) {
            js_value_int_def(intRight, left);
            js_context_pop(context);
            js_value_int_new(context, intLeft ^ intRight);
        }
    } else {
        js_context_pop(context);
        js_context_pop(context);
        js_context_push_typed(context, left);
    }
}

struct js_node_bitwise_and_t* js_node_bitwise_and_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_bitwise_and_t* self = flow_memory_alloc_typed(memory, struct js_node_bitwise_and_t);
    self->type = JS_NODE_BITWISE_AND;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_bitwise_and_free(struct js_node_bitwise_and_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_bitwise_and_head(struct js_node_bitwise_and_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_bitwise_and_body(struct js_node_bitwise_and_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_bitwise_and_exec(struct js_node_bitwise_and_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    if (js_value_is_int(left)) {
        js_value_int_def(intLeft, left);
        js_context_pop(context);
        js_value_exec_def(right, self->value);
        if (js_value_is_int(right)) {
            js_value_int_def(intRight, left);
            js_context_pop(context);
            js_value_int_new(context, intLeft & intRight);
        }
    } else {
        js_context_pop(context);
        js_context_pop(context);
        js_context_push_typed(context, left);
    }
}

struct js_node_equal_t* js_node_equal_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_equal_t* self = flow_memory_alloc_typed(memory, struct js_node_equal_t);
    self->type = JS_NODE_EQUAL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_equal_free(struct js_node_equal_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_equal_head(struct js_node_equal_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_equal_body(struct js_node_equal_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_equal_exec(struct js_node_equal_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_bool is_true = js_value_is_equal(context, left, right);
    js_context_pop(context);
    js_context_pop(context);
    if (is_true) js_value_true_new(context);
    else js_value_false_new(context);
}

struct js_node_not_equal_t* js_node_not_equal_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_not_equal_t* self = flow_memory_alloc_typed(memory, struct js_node_not_equal_t);
    self->type = JS_NODE_NOT_EQUAL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_not_equal_free(struct js_node_not_equal_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_not_equal_head(struct js_node_not_equal_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_not_equal_body(struct js_node_not_equal_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_not_equal_exec(struct js_node_not_equal_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_bool is_true = js_value_is_equal(context, left, right);
    js_context_pop(context);
    js_context_pop(context);
    if (!is_true) js_value_true_new(context);
    else js_value_false_new(context);
}

struct js_node_lower_equal_t* js_node_lower_equal_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_lower_equal_t* self = flow_memory_alloc_typed(memory, struct js_node_lower_equal_t);
    self->type = JS_NODE_LOWER_EQUAL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_lower_equal_free(struct js_node_lower_equal_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_lower_equal_head(struct js_node_lower_equal_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_lower_equal_body(struct js_node_lower_equal_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_lower_equal_exec(struct js_node_lower_equal_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_bool is_true = js_value_is_compare(context, left, right) <= 0;
    js_context_pop(context);
    js_context_pop(context);
    if (is_true) js_value_true_new(context);
    else js_value_false_new(context);
}

struct js_node_greater_equal_t* js_node_greater_equal_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_greater_equal_t* self = flow_memory_alloc_typed(memory, struct js_node_greater_equal_t);
    self->type = JS_NODE_GREATER_EQUAL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_greater_equal_free(struct js_node_greater_equal_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_greater_equal_head(struct js_node_greater_equal_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_greater_equal_body(struct js_node_greater_equal_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_greater_equal_exec(struct js_node_greater_equal_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_bool is_true = js_value_is_compare(context, left, right) >= 0;
    js_context_pop(context);
    js_context_pop(context);
    if (is_true) js_value_true_new(context);
    else js_value_false_new(context);
}

struct js_node_lower_than_t* js_node_lower_than_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_lower_than_t* self = flow_memory_alloc_typed(memory, struct js_node_lower_than_t);
    self->type = JS_NODE_LOWER_THAN;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_lower_than_free(struct js_node_lower_than_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_lower_than_head(struct js_node_lower_than_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_lower_than_body(struct js_node_lower_than_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_lower_than_exec(struct js_node_lower_than_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_bool is_true = js_value_is_compare(context, left, right) < 0;
    js_context_pop(context);
    js_context_pop(context);
    if (is_true) js_value_true_new(context);
    else js_value_false_new(context);
}

struct js_node_greater_than_t* js_node_greater_than_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_greater_than_t* self = flow_memory_alloc_typed(memory, struct js_node_greater_than_t);
    self->type = JS_NODE_GREATER_THAN;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_greater_than_free(struct js_node_greater_than_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_greater_than_head(struct js_node_greater_than_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_greater_than_body(struct js_node_greater_than_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_greater_than_exec(struct js_node_greater_than_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    js_bool is_true = js_value_is_compare(context, left, right) > 0;
    js_context_pop(context);
    js_context_pop(context);
    if (is_true) js_value_true_new(context);
    else js_value_false_new(context);
}

struct js_node_shift_left_t* js_node_shift_left_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_shift_left_t* self = flow_memory_alloc_typed(memory, struct js_node_shift_left_t);
    self->type = JS_NODE_SHIFT_LEFT;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_shift_left_free(struct js_node_shift_left_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_shift_left_head(struct js_node_shift_left_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_shift_left_body(struct js_node_shift_left_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_shift_left_exec(struct js_node_shift_left_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_int_new(context, int_left << int_right);
    } else {
        js_context_pop(context);
    }
}

struct js_node_shift_right_t* js_node_shift_right_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_shift_right_t* self = flow_memory_alloc_typed(memory, struct js_node_shift_right_t);
    self->type = JS_NODE_SHIFT_RIGHT;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_shift_right_free(struct js_node_shift_right_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_shift_right_head(struct js_node_shift_right_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_shift_right_body(struct js_node_shift_right_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_shift_right_exec(struct js_node_shift_right_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_int_new(context, int_left >> int_right);
    } else {
        js_context_pop(context);
    }
}

struct js_node_sum_t* js_node_sum_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_sum_t* self = flow_memory_alloc_typed(memory, struct js_node_sum_t);
    self->type = JS_NODE_SUM;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_sum_free(struct js_node_sum_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_sum_head(struct js_node_sum_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_sum_body(struct js_node_sum_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_sum_exec(struct js_node_sum_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_retain(left);
    js_context_pop(context);
    js_value_exec_def(right, self->value);
    js_value_retain(right);
    js_context_pop(context);
    js_context_push(context, right);
    js_value_release(right);
    js_context_push(context, left);
    js_value_release(left);
    if (js_value_is_int(left)) {
        js_value_int_sum(context);
    } else if (js_value_is_num(left)) {
        js_value_num_sum(context);
    } else if (js_value_is_str(left)) {
        js_value_str_concat(context);
    } else {
        js_context_pop(context);
        js_context_push_typed(context, left);
    }
}

struct js_node_sub_t* js_node_sub_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_sub_t* self = flow_memory_alloc_typed(memory, struct js_node_sub_t);
    self->type = JS_NODE_SUB;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_sub_free(struct js_node_sub_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_sub_head(struct js_node_sub_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_sub_body(struct js_node_sub_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_sub_exec(struct js_node_sub_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_int_new(context, int_left - int_right);
    } else if (js_value_is_numint(left) && js_value_is_numint(left)) {
        js_value_numint_def(int_left, left);
        js_value_numint_def(int_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_num_new(context, int_left - int_right);
    } else {
        js_context_pop(context);
    }
}

struct js_node_mul_t* js_node_mul_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_mul_t* self = flow_memory_alloc_typed(memory, struct js_node_mul_t);
    self->type = JS_NODE_MUL;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_mul_free(struct js_node_mul_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_mul_head(struct js_node_mul_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_mul_body(struct js_node_mul_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_mul_exec(struct js_node_mul_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_int(left) && js_value_is_int(left)) {
        js_value_int_def(int_left, left);
        js_value_int_def(int_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_int_new(context, int_left * int_right);
    } else if (js_value_is_numint(left) && js_value_is_numint(left)) {
        js_value_numint_def(int_left, left);
        js_value_numint_def(int_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_num_new(context, int_left * int_right);
    } else {
        js_context_pop(context);
    }
}

struct js_node_div_t* js_node_div_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_div_t* self = flow_memory_alloc_typed(memory, struct js_node_div_t);
    self->type = JS_NODE_DIV;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_div_free(struct js_node_div_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_div_head(struct js_node_div_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_div_body(struct js_node_div_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_div_exec(struct js_node_div_t* self, struct js_context_t* context) {
    js_value_exec_def(left, self->node);
    js_value_exec_def(right, self->value);
    if (js_value_is_numint(left) && js_value_is_numint(left)) {
        js_value_numint_def(int_left, left);
        js_value_numint_def(int_right, right);
        js_context_pop(context);
        js_context_pop(context);
        js_value_num_new(context, int_left / int_right);
    } else {
        js_context_pop(context);
    }
}

struct js_node_not_t* js_node_not_new(struct flow_memory_t* memory, struct js_node_t* node) {
    struct js_node_not_t* self = flow_memory_alloc_typed(memory, struct js_node_not_t);
    self->type = JS_NODE_NOT;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_not_free(struct js_node_not_t* self) {
    js_node_free_typed(self->node);
    js_node_free_self(self);
}

void js_node_not_head(struct js_node_not_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
}

void js_node_not_body(struct js_node_not_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
}

void js_node_not_exec(struct js_node_not_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
    js_bool is_true = js_value_is_bool_true(value);
    js_context_pop(context);
    if (!is_true) js_value_true_new(context);
    else js_value_false_new(context);
}

struct js_node_neg_t* js_node_neg_new(struct flow_memory_t* memory, struct js_node_t* node) {
    struct js_node_neg_t* self = flow_memory_alloc_typed(memory, struct js_node_neg_t);
    self->type = JS_NODE_NEG;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_neg_free(struct js_node_neg_t* self) {
    js_node_free_typed(self->node);
    js_node_free_self(self);
}

void js_node_neg_head(struct js_node_neg_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
}

void js_node_neg_body(struct js_node_neg_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
}

void js_node_neg_exec(struct js_node_neg_t* self, struct js_context_t* context) {
    js_value_exec_def(value, self->node);
    if (js_value_is_int(value)) {
        js_value_int_def(int_value, value);
        js_context_pop(context);
        js_value_int_new(context, -int_value);
    } else if (js_value_is_num(value)) {
        js_value_num_def(num_value, value);
        js_context_pop(context);
        js_value_num_new(context, -num_value);
    }
}

struct js_node_pre_inc_t* js_node_pre_inc_new(struct flow_memory_t* memory, struct js_node_t* node) {
    struct js_node_pre_inc_t* self = flow_memory_alloc_typed(memory, struct js_node_pre_inc_t);
    self->type = JS_NODE_PRE_INC;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_pre_inc_free(struct js_node_pre_inc_t* self) {
    js_node_free_typed(self->node);
    js_node_free_self(self);
}

void js_node_pre_inc_head(struct js_node_pre_inc_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
}

void js_node_pre_inc_body(struct js_node_pre_inc_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
}

void js_node_pre_inc_exec(struct js_node_pre_inc_t* self, struct js_context_t* context) {
    // TODO
}

struct js_node_pre_dec_t* js_node_pre_dec_new(struct flow_memory_t* memory, struct js_node_t* node) {
    struct js_node_pre_dec_t* self = flow_memory_alloc_typed(memory, struct js_node_pre_dec_t);
    self->type = JS_NODE_PRE_DEC;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_pre_dec_free(struct js_node_pre_dec_t* self) {
    js_node_free_typed(self->node);
    js_node_free_self(self);
}

void js_node_pre_dec_head(struct js_node_pre_dec_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
}

void js_node_pre_dec_body(struct js_node_pre_dec_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
}

void js_node_pre_dec_exec(struct js_node_pre_dec_t* self, struct js_context_t* context) {
    // TODO
}

struct js_node_pos_inc_t* js_node_pos_inc_new(struct flow_memory_t* memory, struct js_node_t* node) {
    struct js_node_pos_inc_t* self = flow_memory_alloc_typed(memory, struct js_node_pos_inc_t);
    self->type = JS_NODE_POS_INC;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_pos_inc_free(struct js_node_pos_inc_t* self) {
    js_node_free_typed(self->node);
    js_node_free_self(self);
}

void js_node_pos_inc_head(struct js_node_pos_inc_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
}

void js_node_pos_inc_body(struct js_node_pos_inc_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
}

void js_node_pos_inc_exec(struct js_node_pos_inc_t* self, struct js_context_t* context) {
    // TODO
}

struct js_node_pos_dec_t* js_node_pos_dec_new(struct flow_memory_t* memory, struct js_node_t* node) {
    struct js_node_pos_dec_t* self = flow_memory_alloc_typed(memory, struct js_node_pos_dec_t);
    self->type = JS_NODE_POS_DEC;
    self->next = 0;
    self->node = node;
    return self;
}

void js_node_pos_dec_free(struct js_node_pos_dec_t* self) {
    js_node_free_typed(self->node);
    js_node_free_self(self);
}

void js_node_pos_dec_head(struct js_node_pos_dec_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
}

void js_node_pos_dec_body(struct js_node_pos_dec_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
}

void js_node_pos_dec_exec(struct js_node_pos_dec_t* self, struct js_context_t* context) {
    // TODO
}

struct js_node_get_t* js_node_get_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_id_t* name) {
    struct js_node_get_t* self = flow_memory_alloc_typed(memory, struct js_node_get_t);
    self->type = JS_NODE_GET;
    self->next = 0;
    self->node = node;
    self->name = name;
    return self;
}

void js_node_get_free(struct js_node_get_t* self) {
    js_node_free_typed(self->node);
    js_node_id_free(self->name);
    js_node_free_self(self);
}

void js_node_get_head(struct js_node_get_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->name, compiler);
}

void js_node_get_body(struct js_node_get_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->name, compiler);
}

void js_node_get_exec(struct js_node_get_t* self, struct js_context_t* context) {
    js_node_exec(self->node, context);
    js_context_peek_def(context, value);
    js_value_retain(value);
    js_context_pop(context);
    
    if (js_value_is_obj(value)) {
        js_value_obj_def(obj, value);
        js_value_obj_get(context, obj, self->name);
    } else {
        js_value_null_new(context);
    }
    
    js_value_release(value);
}

struct js_node_get_array_t* js_node_get_array_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* value) {
    struct js_node_get_array_t* self = flow_memory_alloc_typed(memory, struct js_node_get_array_t);
    self->type = JS_NODE_GET_ARRAY;
    self->next = 0;
    self->node = node;
    self->value = value;
    return self;
}

void js_node_get_array_free(struct js_node_get_array_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->value);
    js_node_free_self(self);
}

void js_node_get_array_head(struct js_node_get_array_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->value, compiler);
}

void js_node_get_array_body(struct js_node_get_array_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->value, compiler);
}

void js_node_get_array_exec(struct js_node_get_array_t* self, struct js_context_t* context) {
    // TODO
}

struct js_node_call_t* js_node_call_new(struct flow_memory_t* memory, struct js_node_t* node, struct js_node_t* param) {
    struct js_node_call_t* self = flow_memory_alloc_typed(memory, struct js_node_call_t);
    self->type = JS_NODE_CALL;
    self->next = 0;
    self->node = node;
    self->param = param;
    return self;
}

void js_node_call_free(struct js_node_call_t* self) {
    js_node_free_typed(self->node);
    js_node_free_typed(self->param);
    js_node_free_self(self);
}

void js_node_call_head(struct js_node_call_t* self, struct js_compiler_t* compiler) {
    js_node_head_typed(self->node, compiler);
    js_node_head_typed(self->param, compiler);
}

void js_node_call_body(struct js_node_call_t* self, struct js_compiler_t* compiler) {
    js_node_body_typed(self->node, compiler);
    js_node_body_typed(self->param, compiler);
}

void js_node_call_exec(struct js_node_call_t* self, struct js_context_t* context) {
    js_context_peek_obj_def(context, obj);
    js_value_exec_def(func_obj, self->node);
    if (js_value_is_func(func_obj)) {
        js_value_retain(func_obj);
        js_context_pop(context);
        
        js_value_obj_new(context);
        js_context_peek_obj_def(context, param_obj);
        js_value_retain(param_obj);
        js_context_pop(context);
        
        js_value_obj_new(context);
        js_context_peek_obj_def(context, var_obj);
        js_value_retain(var_obj);
        js_context_pop(context);
        
        var_obj->parent = param_obj;
        js_value_retain(param_obj);
        param_obj->parent = obj;
        js_value_retain(obj);
        js_context_push_typed(context, var_obj);
        
        struct js_node_t* call_param_node = self->param;
        struct js_value_func_t* func = js_value_func_cast(func_obj);
        struct js_node_param_t* func_param_node = func->function->param;
        while (func_param_node) {
            js_value_exec_def(param_value, call_param_node);
            js_value_obj_set(context, param_obj, func_param_node->name, param_value);
            js_context_pop(context);
            func_param_node = (struct js_node_param_t*) func_param_node->next;
            call_param_node = call_param_node->next;
        }
        
        js_node_exec_typed(func->function->statement, context);
        
        js_context_peek_def(context, result_value);
        js_value_retain(result_value);
        js_context_pop(context);
        
        js_context_pop(context);
        js_context_push(context, result_value);
        
        js_value_release(param_obj);
        js_value_release(var_obj);
        js_value_release(func_obj);
        js_value_release(result_value);
    } else {
        js_context_pop(context);
        js_value_null_new(context);
    }
}
