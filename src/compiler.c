#include "app/compiler.h"
#include "app/lexer.h"
#include <stdio.h>
#include <stdlib.h>

static Lexer g_lexer;
static Token g_token; // the token we are currently looking at

static void advance() { g_token = lexer_next(&g_lexer); }

static void expect(TokenType type, const char *message) {
	if (g_token.type != type) {
		fprintf(stderr, "error: %s\n", message);
		exit(65);
	}
	advance();
}

void compile(FILE *out, const char *source) {
	init_lexer(&g_lexer, source);
	advance();
	expect(TOKEN_RET, "expected 'ret'");
	Token value = g_token;
	expect(TOKEN_NUMBER, "expected a number after 'ret'");
	expect(TOKEN_EOF, "expected end of file");

	fprintf(out, ".global _start\n");
	fprintf(out, ".text\n");
	fprintf(out, "_start:\n");
	fprintf(out, "    mov $%.*s, %%edi\n", value.length, value.start);
	fprintf(out, "    mov $60, %%eax\n");
	fprintf(out, "    syscall\n");
}
