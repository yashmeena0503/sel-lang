#ifndef APP_LEXER_H
#define APP_LEXER_H

typedef enum {
	TOKEN_RET,
	TOKEN_NUMBER,
	TOKEN_EOF,
	TOKEN_ERROR,
} TokenType;

typedef struct {
	TokenType type;
	const char *start;
	int length;
} Token;

typedef struct {
	const char *cur;
} Lexer;

void init_lexer(Lexer *lexer, const char *source);
Token lexer_next(Lexer *lexer);

#endif
