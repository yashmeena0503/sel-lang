#include "app/lexer.h"
#include <string.h>

static int is_digit(char c) { return c >= '0' && c <= '9'; }
static int is_alpha(char c) { return (c >= 'a' && c <= 'z') || c == '_'; }

// calculates the length of keyword using `cur - start` and returns the token
static Token make_token(Lexer *lexer, TokenType type, const char *start) {
	return (Token){type, start, (int)(lexer->cur - start)};
}

void init_lexer(Lexer *lexer, const char *source) { lexer->cur = source; }

Token lexer_next(Lexer *lexer) {
	// detect and skip whitespace and `//` comments until we hit a real token
	for (;;) {
		char c = *lexer->cur;
		if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
			lexer->cur++;
		} else if (c == '/' && lexer->cur[1] == '/') {
			// stop at '\n' (the whitespace branch eats it) or '\0' (comment at
			// EOF)
			while (*lexer->cur != '\0' && *lexer->cur != '\n')
				lexer->cur++;
		} else {
			break;
		}
	}

	const char *start = lexer->cur;

	// detect end of file
	if (*lexer->cur == '\0')
		return make_token(lexer, TOKEN_EOF, start);

	// detect and make_token number
	if (is_digit(*lexer->cur)) {
		while (is_digit(*lexer->cur))
			lexer->cur++;
		return make_token(lexer, TOKEN_NUMBER, start);
	}

	// detect and make_token words
	if (is_alpha(*lexer->cur)) {
		while (is_alpha(*lexer->cur))
			lexer->cur++;
		if (lexer->cur - start == 3 && memcmp(start, "ret", 3) == 0) {
			return make_token(lexer, TOKEN_RET, start);
		}
		return make_token(lexer, TOKEN_ERROR, start);
	}

	// detect and make_token for everything else as anything left here onwards
	// is a single char
	char c = *lexer->cur++;
	switch (c) {
	case '+':
		return make_token(lexer, TOKEN_PLUS, start);
	case '-':
		return make_token(lexer, TOKEN_MINUS, start);
	case '*':
		return make_token(lexer, TOKEN_STAR, start);
	case '/': // a lone '/'; "//" was already consumed as a comment above
		return make_token(lexer, TOKEN_SLASH, start);
	case '(':
		return make_token(lexer, TOKEN_LPAREN, start);
	case ')':
		return make_token(lexer, TOKEN_RPAREN, start);
	}
	return make_token(lexer, TOKEN_ERROR, start);
}
