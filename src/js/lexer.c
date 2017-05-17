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
			unsigned short type = JS_TOKEN_ID;
			long begin = pc - text;
			do { c = *(++pc); } while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$');
			size_t size = (size_t) ((pc - text) - begin);
			char* word = (char*) text + begin;

            if (size == 2) {
                if (!strncmp(word, "do", 2)) type = JS_TOKEN_DO;
                else if (!strncmp(word, "if", 2)) type = JS_TOKEN_IF;
                else if (!strncmp(word, "or", 2)) type = JS_TOKEN_OR;
            } else if (size == 3) {
                if (!strncmp(word, "for", 3)) type = JS_TOKEN_FOR;
                else if (!strncmp(word, "var", 3)) type = JS_TOKEN_VAR;
                else if (!strncmp(word, "end", 3)) type = JS_TOKEN_END;
                else if (!strncmp(word, "and", 3)) type = JS_TOKEN_AND;
                else if (!strncmp(word, "xor", 3)) type = JS_TOKEN_XOR;
                else if (!strncmp(word, "pow", 3)) type = JS_TOKEN_POW;
            } else if (size == 4) {
                if (!strncmp(word, "true", 4)) type = JS_TOKEN_TRUE;
                else if (!strncmp(word, "else", 4)) type = JS_TOKEN_ELSE;
                else if (!strncmp(word, "null", 4)) type = JS_TOKEN_NULL;
                else if (!strncmp(word, "super", 4)) type = JS_TOKEN_SUPER;
            } else if (size == 5) {
                if (!strncmp(word, "false", 5)) type = JS_TOKEN_FALSE;
                else if (!strncmp(word, "while", 5)) type = JS_TOKEN_WHILE;
                else if (!strncmp(word, "break", 5)) type = JS_TOKEN_BREAK;
                else if (!strncmp(word, "bitor", 5)) type = JS_TOKEN_BITOR;
            } else if (size == 6) {
                if (!strncmp(word, "return", 6)) type = JS_TOKEN_RETURN;
                else if (!strncmp(word, "bitand", 6)) type = JS_TOKEN_BITAND;
            } else if (size == 7) {
                if (!strncmp(word, "require", 7)) type = JS_TOKEN_REQUIRE;
            } else if (size == 8) {
                if (!strncmp(word, "function", 8)) type = JS_TOKEN_FUNCTION;
                else if (!strncmp(word, "continue", 8)) type = JS_TOKEN_CONTINUE;
            }
            
            struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
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
			
			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
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

			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
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

			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
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

			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
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
			struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
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
        struct js_token_t* token = (struct js_token_t*) malloc(sizeof(struct js_token_t));
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
