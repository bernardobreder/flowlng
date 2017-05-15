#include <stdlib.h>
#include <string.h>
#include "js.h"

// http://hepunx.rl.ac.uk/~adye/jsspec11/llr.htm

#define js_node_def() struct js_node_t *head = 0, *tail = 0
#define js_node_add(NODE) { if (tail) { tail->next = NODE; } else { head = NODE; } } tail = NODE;
#define js_parser_token()
#define js_parser_step() self->token = self->token->next;
#define js_parser_skip_comment() while (js_parser_type(JS_TOKEN_COMMENT)) { js_parser_step(); }
#define js_parser_next() js_parser_step(); js_parser_skip_comment();
#define js_parser_token_get()
#define js_parser_type(TYPE) (self->token->type == TYPE)
#define js_parser_type_not(TYPE) (self->token->type != TYPE)
#define js_parser_error(MSG) (struct js_node_t*) js_node_error_new(strdup(MSG), (self->token->word ? strdup(self->token->word) : strdup("<EOF>")), self->token->line, self->token->column, self->token->line, self->token->column + (self->token->word ? strlen(self->token->word) : 0))

struct js_parser_t* js_parser_new(struct js_token_t* tokens) {
    struct js_parser_t* self = (struct js_parser_t*) malloc(sizeof(struct js_parser_t));
    self->tokens = tokens;
    self->token = tokens;
    return self;
}

void js_parser_free(struct js_parser_t* self) {
    free(self);
}

struct js_node_t* js_parser_statement_nullable(struct js_parser_t* self);

struct js_node_t* js_parser_expression_nullable(struct js_parser_t* self);

struct js_node_t* js_parser_compound_statement(struct js_parser_t* self);

struct js_node_t* js_parser_expression_primary(struct js_parser_t* self) {
    if (js_parser_type('(')) {
        js_parser_next();
        
        struct js_node_t* expression = js_parser_expression_nullable(self);
        if (!expression) return 0;
        js_parser_token_get();
        
        if (js_parser_type_not(')')) {
            js_node_free(expression);
            return 0;
        }
        js_parser_next();
    } else if (js_parser_type_not(JS_TOKEN_ID)) {
        
    }
    return 0;
}

struct js_node_t* js_parser_expression_member(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_constructor_call(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_constructor(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_unary(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_multiplicative(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_additive(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_shift(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_relational(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_equality(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_bitwise_and(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_bitwise_xor(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_bitwise_or(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_and(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_or(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_conditional(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_assignment(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_expression_nullable(struct js_parser_t* self) {
    return 0;
}

struct js_node_t* js_parser_statement_condition_nullable(struct js_parser_t* self) {
    js_parser_token();
    
    if (js_parser_type_not('(')) return 0;
    js_parser_next();
    
    struct js_node_t* expression = js_parser_expression_nullable(self);
    if (!expression) return 0;
    js_parser_token_get();
    
    if (js_parser_type_not(')')) return 0;
    js_parser_next();
    
    return expression;
}

struct js_node_t* js_parser_statement_if(struct js_parser_t* self) {
    js_parser_token();
    if (js_parser_type_not(JS_TOKEN_IF)) {
        return js_parser_error("statement if: 'if' expected");
    }
    js_parser_next();
    
    struct js_node_t* expression = js_parser_statement_condition_nullable(self);
    if (!expression) {
        return js_parser_error("statement if: if '( <expression> )' expected");
    }
    js_parser_token_get();
    
    struct js_node_t* statement = js_parser_statement_nullable(self);
    if (!statement) {
        js_node_free(expression);
        return js_parser_error("statement if: if ( <expression> ) '<statement>' expected");
    }
    
    struct js_node_t* else_expression = 0;
    if (js_parser_type(JS_TOKEN_ELSE)) {
        js_parser_next();
        
        else_expression = js_parser_expression_nullable(self);
        if (!else_expression) {
            js_node_free(expression);
            js_node_free(statement);
            return js_parser_error("statement if: if ( <expression> ) <statement> else '<expression>' expected");
        }
        js_parser_token_get();
    }
    
    return (struct js_node_t*) js_node_if_new(expression, statement);
}

struct js_node_t* js_parser_statement_while(struct js_parser_t* self) {
    js_parser_token();
    if (js_parser_type_not(JS_TOKEN_WHILE)) {
        return js_parser_error("statement while: 'while' expected");
    }
    js_parser_next();
    
    struct js_node_t* expression = js_parser_statement_condition_nullable(self);
    if (!expression) {
        return js_parser_error("statement while: while '( <expression> )' expected");
    }
    js_parser_token_get();
    
    struct js_node_t* statement = js_parser_statement_nullable(self);
    if (!statement) {
        js_node_free(expression);
        return js_parser_error("statement while: while ( <expression> ) '<statement>' expected");
    }
    
    return (struct js_node_t*) js_node_while_new(expression, statement);
}

struct js_node_t* js_parser_statement_nullable(struct js_parser_t* self) {
    js_parser_token();
    if (js_parser_type(';')) {
        js_parser_next();
        return (struct js_node_t*) js_node_empty_new();
    } else if (js_parser_type(JS_TOKEN_BREAK)) {
        js_parser_next();
        if (js_parser_type(';')) {
            js_parser_next();
        }
        return (struct js_node_t*) js_node_break_new();
    } else if (js_parser_type(JS_TOKEN_CONTINUE)) {
        js_parser_next();
        if (js_parser_type(';')) {
            js_parser_next();
        }
        return (struct js_node_t*) js_node_continue_new();
    } else if (js_parser_type('{')) {
        return js_parser_compound_statement(self);
    } else if (js_parser_type(JS_TOKEN_IF)) {
        return js_parser_statement_if(self);
    } else if (js_parser_type(JS_TOKEN_WHILE)) {
        return js_parser_statement_while(self);
    }
    return 0;
}

struct js_node_t* js_parser_compound_statement(struct js_parser_t* self) {
    js_node_def();
    js_parser_token();
    
    if (js_parser_type_not('{')) {
        return js_parser_error("compound statement: '{' expected");
    }
    js_parser_next();
    
    while (js_parser_type_not('}')) {
        struct js_node_t* node = js_parser_statement_nullable(self);
        if (!node) {
            js_nodes_free(head);
            return js_parser_error("compound statement: { '<statement>' expected");
        }
        js_parser_token_get();
        js_node_add(node);
    }
    
    if (js_parser_type_not('}')) {
        return js_parser_error("compound statement: '}' expected");
    }
    js_parser_next();
    return head;
}

struct js_node_t* js_parser_parameter_list(struct js_parser_t* self) {
    js_node_def();
    js_parser_token();
    if (js_parser_type_not(JS_TOKEN_ID)) {
        return js_parser_error("parameter list: '<id>' expected");
    }
    struct js_node_t* node = (struct js_node_t*) js_node_id_new(strdup(self->token->word));
    js_node_add(node);
    js_parser_next();
    while (js_parser_type(',')) {
        js_parser_next();
        if (js_parser_type_not(JS_TOKEN_ID)) {
            js_nodes_free(head);
            return js_parser_error("parameter list: '<id>' expected");
        }
        node = (struct js_node_t*) js_node_id_new(strdup(self->token->word));
        js_node_add(node);
        js_parser_next();
    }
    return head;
}

struct js_node_t* js_parser_parameter_list_opt(struct js_parser_t* self) {
    js_parser_token();
    if (js_parser_type_not(JS_TOKEN_ID)) return 0;
    return js_parser_parameter_list(self);
}

struct js_node_t* js_parser_element_nullable(struct js_parser_t* self) {
    js_parser_token();
    if (js_parser_type(JS_TOKEN_FUNCTION)) {
        js_parser_next();

        if (js_parser_type_not(JS_TOKEN_ID)) {
            return js_parser_error("element: function '<id>' expected");
        }
        struct js_node_t* node = (struct js_node_t*) js_node_id_new(strdup(self->token->word));
        js_parser_next();
        
        if (js_parser_type_not('(')) {
            js_node_free(node);
            return js_parser_error("element: function <id> '(' expected");
        }
        js_parser_next();
        
        struct js_node_t* parameters = js_parser_parameter_list_opt(self);
        if (parameters && js_node_has_error(parameters)) {
            js_node_free(node);
            return parameters;
        }
        js_parser_token_get();

        if (js_parser_type_not(')')) {
            js_node_free(node);
            js_node_free(parameters);
            return js_parser_error("element: function <id> ( <parameter_list> ')' expected");
        }
        js_parser_next();

        struct js_node_t* statements = js_parser_compound_statement(self);
        if (statements && js_node_has_error(statements)) {
            js_node_free(node);
            js_node_free(parameters);
            return statements;
        }

        return (struct js_node_t*) js_node_function_new(node, parameters, statements);
    } else {
        return js_parser_statement_nullable(self);
    }
}

struct js_node_t* js_parser_program(struct js_parser_t* self) {
    struct js_node_t* element = js_parser_element_nullable(self);
    if (!element) return 0;
    element->next = js_parser_program(self);
    return element;
}

struct js_node_t* js_parser(struct js_parser_t* self, struct js_token_t* tokens) {
    js_parser_skip_comment();
    return js_parser_program(self);
}

//  Statement:
//           ForParen ; ExpressionOpt ; ExpressionOpt ) Statement
//           ForBegin ; ExpressionOpt ; ExpressionOpt ) Statement
//           ForBegin in Expression ) Statement
//           with ( Expression ) Statement
//           return ExpressionOpt ;
//           CompoundStatement
//           VariablesOrExpression ;
//  ForParen:
//           for (
//  ForBegin:
//           ForParen VariablesOrExpression
//  VariablesOrExpression:
//           var Variables
//           Expression
//  Variables:
//           Variable
//           Variable , Variables
//  Variable:
//           Identifier
//           Identifier = AssignmentExpression
//  ExpressionOpt:
//           empty
//           Expression
//  Expression:
//           AssignmentExpression
//           AssignmentExpression , Expression
//  AssignmentExpression:
//           ConditionalExpression
//           ConditionalExpression AssignmentOperator AssignmentExpression
//  ConditionalExpression:
//           OrExpression
//           OrExpression ? AssignmentExpression : AssignmentExpression
//  OrExpression:
//           AndExpression
//           AndExpression || OrExpression
//  AndExpression:
//           BitwiseOrExpression
//           BitwiseOrExpression && AndExpression
//  BitwiseOrExpression:
//           BitwiseXorExpression
//           BitwiseXorExpression | BitwiseOrExpression
//  BitwiseXorExpression:
//           BitwiseAndExpression
//           BitwiseAndExpression ^ BitwiseXorExpression
//  BitwiseAndExpression:
//           EqualityExpression
//           EqualityExpression & BitwiseAndExpression
//  EqualityExpression:
//           RelationalExpression
//           RelationalExpression EqualityualityOperator EqualityExpression
//  RelationalExpression:
//           ShiftExpression
//           RelationalExpression RelationalationalOperator ShiftExpression
//  ShiftExpression:
//           AdditiveExpression
//           AdditiveExpression ShiftOperator ShiftExpression
//  AdditiveExpression:
//           MultiplicativeExpression
//           MultiplicativeExpression + AdditiveExpression
//           MultiplicativeExpression - AdditiveExpression
//  MultiplicativeExpression:
//           UnaryExpression
//           UnaryExpression MultiplicativeOperator MultiplicativeExpression
//  UnaryExpression:
//           MemberExpression
//           UnaryOperator UnaryExpression
//           - UnaryExpression
//           IncrementOperator MemberExpression
//           MemberExpression IncrementOperator
//           new Constructor
//           delete MemberExpression
//  Constructor:
//           this . ConstructorCall
//           ConstructorCall
//  ConstructorCall:
//           Identifier
//           Identifier ( ArgumentListOpt )
//           Identifier . ConstructorCall
//  MemberExpression:
//           PrimaryExpression
//           PrimaryExpression . MemberExpression
//           PrimaryExpression [ Expression ]
//           PrimaryExpression ( ArgumentListOpt )
//  ArgumentListOpt:
//           empty
//           ArgumentList
//  ArgumentList:
//           AssignmentExpression
//           AssignmentExpression , ArgumentList
//  PrimaryExpression:
//           ( Expression )
//           Identifier
//           IntegerLiteral
//           FloatingPointLiteral
//           StringLiteral
//           false
//           true
//           null
//           this
