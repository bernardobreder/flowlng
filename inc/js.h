
#define JS_TOKEN_TYPE_EOF 0
#define JS_TOKEN_TYPE_ID 1
#define JS_TOKEN_TYPE_STRING 2
#define JS_TOKEN_TYPE_NUMBER 3

#define JS_TOKEN_TYPE_FOR_WORD 271
#define JS_TOKEN_TYPE_FALSE_WORD 272
#define JS_TOKEN_TYPE_REQUIRE_WORD 273
#define JS_TOKEN_TYPE_IF_WORD 274
#define JS_TOKEN_TYPE_OR_WORD 275
#define JS_TOKEN_TYPE_TRUE_WORD 276
#define JS_TOKEN_TYPE_WHILE_WORD 277
#define JS_TOKEN_TYPE_RETURN_WORD 278
#define JS_TOKEN_TYPE_DO_WORD 279
#define JS_TOKEN_TYPE_ELSE_WORD 280
#define JS_TOKEN_TYPE_FUNCTION_WORD 281

#define JS_NODE_ERROR 1
#define JS_NODE_ID 2

struct js_token_t {
    unsigned short type;
    char* word;
    int line;
    int column;
    struct js_token_t* next;
};

struct js_node_t {
    unsigned char type;
    struct js_next_t* next;
};

struct js_node_error_t {
    unsigned char type;
    struct js_next_t* next;
    char* message;
    char* word;
    int line_begin;
    int line_end;
    int column_begin;
    int column_end;
};

struct js_node_id_t {
    unsigned char type;
    struct js_next_t* next;
    char* word;
};

void js_token_free(struct js_token_t* self);

void js_tokens_free(struct js_token_t* self);

struct js_token_t* js_lexer(const char* text);

struct js_node_t* js_parser(const struct js_token_t* tokens);

void js_node_free(struct js_node_t* self);

void js_nodes_free(struct js_node_t* self);

unsigned char js_node_has_error(struct js_node_t* self);

struct js_node_error_t* js_node_error_new(char* message, char* word, int line_begin, int column_begin, int line_end, int column_end);
