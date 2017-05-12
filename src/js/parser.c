#include "js.h"

// http://hepunx.rl.ac.uk/~adye/jsspec11/llr.htm

#define js_node_def() struct js_node_t *head = 0, *tail = 0
#define js_node_add(NODE) { if (tail) { tail->next = NODE; } else { head = NODE; } } tail = NODE;
#define js_parser_token(TOKEN, TOKENS) struct js_token_t* TOKEN = *TOKENS;
#define js_parser_next(TOKEN, TOKENS) { (*tokens)++; TOKEN = *TOKENS; }
#define js_parser_type(TOKEN, TYPE) TOKEN->type == TYPE
#define js_parser_type_not(TOKEN, TYPE) TOKEN->type != TYPE
#define js_parser_error(TOKEN, MSG) js_node_error_new(strdup(MSG), strdup(TOKEN->word), TOKEN->line, TOKEN->column, TOKEN->line, TOKEN->column + strlen(TOKEN->word))

struct js_node_t* js_parser_compound_statement(const struct js_token_t** tokens) {
    return 0;
}

struct js_node_t* js_parser_statement(const struct js_token_t** tokens) {
    return 0;
}

struct js_node_t* js_parser_parameter_list(const struct js_token_t** tokens) {
    js_node_def();
    js_parser_token(token, tokens);
    if (js_parser_type_not(token, JS_TOKEN_TYPE_ID)) {
        return js_parser_error(token, "parameter list: '<ID>' expected");
    }
    struct js_node_t* node = js_node_id_new(token->word);
    js_node_add(node);
    js_parser_next(token, tokens);
    while (js_parser_type(token, ',')) {
        js_parser_next(token, tokens);
        if (js_parser_type_not(token, JS_TOKEN_TYPE_ID)) {
            js_nodes_free(head);
            return js_parser_error(token, "parameter list: '<ID>' expected");
        }
        js_parser_next(token, tokens);
    }
    return 0;
}

struct js_node_t* js_parser_parameter_list_opt(const struct js_token_t** tokens) {
    js_parser_token(token, tokens);
    if (!js_parser_type_not(token, JS_TOKEN_TYPE_ID)) return 0;
    return js_parser_parameter_list(tokens);
}

struct js_node_t* js_parser_element(const struct js_token_t** tokens) {
    js_parser_token(token, tokens);
    if (js_parser_type(token, JS_TOKEN_TYPE_FUNCTION_WORD)) {
        js_parser_next(token, tokens);

        if (js_parser_type_not(token, JS_TOKEN_TYPE_ID)) {
            return js_parser_error(token, "element: function '<ID>' expected");
        }
        struct js_node_t* node = js_node_id_new(token->word);
        js_parser_next(token, tokens);
        
        if (js_parser_type_not(token, '(')) {
            js_node_free(node);
            return js_parser_error(token, "element: function <ID> '(' expected");
        }
        js_parser_next(token, tokens);
        
        struct js_node_t* parameter_list = js_parser_parameter_list_opt(tokens);
        if (js_node_has_error(parameter_list)) {
            js_node_free(node);
            return parameter_list;
        }

        if (js_parser_type_not(token, ')')) {
            js_node_free(node);
            js_node_free(parameter_list);
            return js_parser_error(token, "element: function <ID> ( ParameterListOpt ')' expected");
        }
        js_parser_next(token, tokens);

        struct js_node_t* statement = js_parser_compound_statement(tokens);
        if (js_node_has_error(statement)) {
            js_node_free(node);
            js_node_free(parameter_list);
            return statement;
        }

        return statement;
    } else {
        return js_parser_statement(tokens);
    }
}

struct js_node_t* js_parser_program(const struct js_token_t** tokens) {
    struct js_node_t* element = js_parser_element(tokens);
    if (!element) return 0;
    element->next = js_parser_program(tokens);
    return element;
}

struct js_node_t* js_parser(const struct js_token_t* tokens) {
    return js_parser_program(&tokens);
}

//  ParameterList:
//           Identifier
//           Identifier , ParameterList
//  CompoundStatement:
//           { Statements }
//  Statements:
//           empty
//           Statement Statements
//  Statement:
//           ;
//           if Condition Statement
//           if Condition Statement else Statement
//           while Condition Statement
//           ForParen ; ExpressionOpt ; ExpressionOpt ) Statement
//           ForBegin ; ExpressionOpt ; ExpressionOpt ) Statement
//           ForBegin in Expression ) Statement
//           break ;
//           continue ;
//           with ( Expression ) Statement
//           return ExpressionOpt ;
//           CompoundStatement
//           VariablesOrExpression ;
//  Condition:
//           ( Expression )
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