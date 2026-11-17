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
	// skip whitespace and `//` comments until we hit a real token
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

	if (is_digit(*lexer->cur)) {
		while (is_digit(*lexer->cur))
			lexer->cur++;
		return make_token(lexer, TOKEN_NUMBER, start);
	}
	if (is_alpha(*lexer->cur)) {
		while (is_alpha(*lexer->cur))
			lexer->cur++;
		if (lexer->cur - start == 3 && memcmp(start, "ret", 3) == 0) {
			return make_token(lexer, TOKEN_RET, start);
		}
		return make_token(lexer, TOKEN_ERROR, start);
	}
	lexer->cur++;
	return make_token(lexer, TOKEN_ERROR, start);
}
