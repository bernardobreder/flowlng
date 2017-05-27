#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "xstring.h"
#include "js.h"

void js_token_free(struct js_token_t* self) {
    if (self->word) free(self->word);
    flow_memory_item_free(self);
}

void js_tokens_free(struct js_token_t* self) {
    struct js_token_t* aux = self;
    while (aux) {
        struct js_token_t* next = aux->next;
        js_token_free(aux);
        aux = next;
    }
}

struct js_token_t* js_lexer(struct flow_memory_t* memory, const char* text) {
    struct js_token_t *head = 0, *tail = 0;
    unsigned int lin = 1, col = 1;
	char* pc = (char*) text;
	char c = *pc;
	while (c) {
		if (c <= ' ') {
			if (c == '\n') {
				lin++;
				col = 1;
			} else {
				col++;
			}
		} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$') {
			unsigned short type = JS_TOKEN_ID;
			long begin = pc - text;
			do { c = *(++pc); } while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$');
			size_t size = (size_t) ((pc - text) - begin);
			char* word = (char*) text + begin;

            if (size == 2) {
                if (strcmp2(word, "do")) type = JS_TOKEN_DO;
                else if (strcmp2(word, "if")) type = JS_TOKEN_IF;
                else if (strcmp2(word, "or")) type = JS_TOKEN_OR;
            } else if (size == 3) {
                if (strcmp3(word, "for")) type = JS_TOKEN_FOR;
                else if (strcmp3(word, "var")) type = JS_TOKEN_VAR;
                else if (strcmp3(word, "end")) type = JS_TOKEN_END;
                else if (strcmp3(word, "and")) type = JS_TOKEN_AND;
                else if (strcmp3(word, "xor")) type = JS_TOKEN_XOR;
                else if (strcmp3(word, "pow")) type = JS_TOKEN_POW;
            } else if (size == 4) {
                if (strcmp4(word, "true")) type = JS_TOKEN_TRUE;
                else if (strcmp4(word, "else")) type = JS_TOKEN_ELSE;
                else if (strcmp4(word, "null")) type = JS_TOKEN_NULL;
            } else if (size == 5) {
                if (strcmp5(word, "false")) type = JS_TOKEN_FALSE;
                else if (strcmp5(word, "while")) type = JS_TOKEN_WHILE;
                else if (strcmp5(word, "break")) type = JS_TOKEN_BREAK;
                else if (strcmp5(word, "bitor")) type = JS_TOKEN_BITOR;
                else if (strcmp5(word, "super")) type = JS_TOKEN_SUPER;
                else if (strcmp5(word, "class")) type = JS_TOKEN_CLASS;
            } else if (size == 6) {
                if (strcmp6(word, "return")) type = JS_TOKEN_RETURN;
                else if (strcmp6(word, "bitand")) type = JS_TOKEN_BITAND;
            } else if (size == 7) {
                if (strcmp7(word, "require")) type = JS_TOKEN_REQUIRE;
                else if (strcmp7(word, "extends")) type = JS_TOKEN_EXTENDS;
            } else if (size == 8) {
                if (strcmp8(word, "function")) type = JS_TOKEN_FUNCTION;
                else if (strcmp8(word, "continue")) type = JS_TOKEN_CONTINUE;
            } else if (size == 11) {
                if (strcmp11(word, "constructor")) type = JS_TOKEN_CONSTRUCTOR;
            }
            
            struct js_token_t* token = flow_memory_alloc_typed(memory, struct js_token_t);
			token->type = type;
            token->word = strndup(word, size);
            token->line = lin;
			token->column = col;
			token->next = 0;

			if (tail) tail->next = token;
            else head = token;
            tail = token;
			
            col += size;
			pc--;
		} else if ((c >= '0' && c <= '9')) {
			unsigned char dot = 0;
			long begin = pc - text;
			c = *(++pc);
			while ((c >= '0' && c <= '9') || (!dot && c == '.')) {
                if (c == '.') dot = 1;
				c = *(++pc);
			}
            size_t size = (size_t) ((pc - text) - begin);
			char* word = (char*) strndup(text + begin, size);
			
			struct js_token_t* token = flow_memory_alloc_typed(memory, struct js_token_t);
			token->type = JS_TOKEN_NUMBER;
            token->word = word;
			token->line = lin;
			token->column = col;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;
			
			col += size;
			pc--;
		} else if (c == '\"' || c == '\'') {
			unsigned short type = JS_TOKEN_STRING;
			c = *(++pc);
            col++;
            long begin = pc - text;
            while ((c != '\"' && c != '\'') || (pc != text && pc[-1] == '\\')) {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
						col = 0;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
				col++;
			}
            size_t size = (size_t) ((pc - text) - begin);
			char* word = (char*) strndup(text + begin, size);

			struct js_token_t* token = flow_memory_alloc_typed(memory, struct js_token_t);
			token->type = type;
            token->word = word;
            token->line = lin;
			token->column = col;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;
		} else if (c == '/' && pc[1] == '*') {
			unsigned short type = JS_TOKEN_COMMENT;
			long begin = pc - text;
			pc += 3;
			col += 3;
			c = *pc;
			while (c != '/' || pc[-1] != '*') {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
						col = 0;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
				col++;
			}
            size_t size = (size_t) ((pc - text) - begin);
			char* word = (char*) strndup(text + begin, size);

			struct js_token_t* token = flow_memory_alloc_typed(memory, struct js_token_t);
			token->type = type;
            token->word = word;
            token->line = lin;
			token->column = col;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;
		} else if (c == '/' && pc[1] == '/') {
			unsigned short type = JS_TOKEN_COMMENT;
			long begin = pc - text;
			pc += 2;
			c = *pc;
			while (c != '\n') {
				if (c <= 0x7F) {
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
			}
            size_t size = (size_t) ((pc - text) - begin);
			char* word = (char*) strndup(text + begin, size);

			struct js_token_t* token = flow_memory_alloc_typed(memory, struct js_token_t);
			token->type = type;
            token->word = word;
            token->line = lin;
			token->column = col;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;

			lin++;
			col = 1;
		} else {
			struct js_token_t* token = flow_memory_alloc_typed(memory, struct js_token_t);
			token->type = c;
            token->word = strndup(pc, 1);
            token->line = lin;
			token->column = col;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;
		}
		c = *(++pc);
	}
    {
        struct js_token_t* token = flow_memory_alloc_typed(memory, struct js_token_t);
        token->type = JS_TOKEN_EOF;
        token->word = strdup("<EOF>");
        token->line = lin;
        token->column = col;
        token->next = 0;
        
        if (tail) tail->next = token;
        else head = token;
        tail = token;
    }
	return head;
}
