#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "js.h"

struct js_token_t* js_lexer(const char* text) {
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
			unsigned short type = JS_TOKEN_TYPE_ID;
			long begin = pc - text;
			do { c = *(++pc); } while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$');
			size_t size = (size_t) ((pc - text) - begin);
			char* word = (char*) strndup(text + begin, size);

            if (size == 2) {
                if (!strcmp(word, "do")) type = JS_TOKEN_TYPE_DO_WORD;
                else if (!strcmp(word, "if")) type = JS_TOKEN_TYPE_IF_WORD;
            } else if (size == 3) {
                if (!strcmp(word, "for")) type = JS_TOKEN_TYPE_FOR_WORD;
            } else if (size == 4) {
                if (!strcmp(word, "true")) type = JS_TOKEN_TYPE_TRUE_WORD;
                else if (!strcmp(word, "else")) type = JS_TOKEN_TYPE_ELSE_WORD;
            } else if (size == 5) {
                if (!strcmp(word, "false")) type = JS_TOKEN_TYPE_FALSE_WORD;
                else if (!strcmp(word, "while")) type = JS_TOKEN_TYPE_WHILE_WORD;
            } else if (size == 6) {
                if (!strcmp(word, "return")) type = JS_TOKEN_TYPE_RETURN_WORD;
            } else if (size == 7) {
                if (!strcmp(word, "require")) type = JS_TOKEN_TYPE_REQUIRE_WORD;
            } else if (size == 8) {
                if (!strcmp(word, "function")) type = JS_TOKEN_TYPE_FUNCTION_WORD;
            }
            
            struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
			token->type = type;
			token->line = lin;
			token->column = col;
			token->word = type == JS_TOKEN_TYPE_ID ? word : 0;
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
			
			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
			token->type = JS_TOKEN_TYPE_NUMBER;
			token->line = lin;
			token->column = col;
			token->word = word;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;
			
			col += size;
			pc--;
		} else if (c == '\"' || c == '\'') {
			unsigned short type = JS_TOKEN_TYPE_STRING;
			long begin = pc - text;
			c = *(++pc);
			col++;
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

			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
			token->type = type;
			token->line = lin;
			token->column = col;
			token->word = word;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;
		} else if (c == '/' && pc[1] == '*') {
			unsigned short type = JS_TOKEN_TYPE_STRING;
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

			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
			token->type = type;
			token->line = lin;
			token->column = col;
			token->word = word;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;
		} else if (c == '/' && pc[1] == '/') {
			unsigned short type = JS_TOKEN_TYPE_STRING;
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

			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
			token->type = type;
			token->line = lin;
			token->column = col;
			token->word = word;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;

			lin++;
			col = 1;
		} else {
			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
			token->type = c;
			token->line = lin;
			token->column = col;
			token->word = 0;
			token->next = 0;

            if (tail) tail->next = token;
            else head = token;
            tail = token;
		}
		c = *(++pc);
	}
	return head;
}
