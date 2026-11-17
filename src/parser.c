#include "app/parser.h"
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

// number = NUMBER
static Node *number(void) {
	Token t = g_token; // copy first, because expect() moves g_token to the next token
	expect(TOKEN_NUMBER, "expected a number");
	// strtol stops at the first non-digit, so the token needn't be NUL-terminated
	return new_number(strtol(t.start, NULL, 10));
}

// expr = number (("+" | "-") number)*
static Node *expr(void) {
	Node *node = number();
	while (g_token.type == TOKEN_PLUS || g_token.type == TOKEN_MINUS) {
		NodeKind kind = g_token.type == TOKEN_PLUS ? NODE_ADD : NODE_SUB;
		advance();
		// the tree so far becomes the left child of the new node
		node = new_binary(kind, node, number());
	}
	return node;
}

// program = "ret" expr EOF
Node *parse(const char *source) {
	init_lexer(&g_lexer, source);
	advance();
	expect(TOKEN_RET, "expected 'ret'");
	Node *root = expr();
	expect(TOKEN_EOF, "expected end of file");
	return root;
}
