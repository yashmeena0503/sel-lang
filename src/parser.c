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

// primary -> expr -> term -> primary is a cycle, so expr needs a forward decl
static Node *expr(void);

// primary = NUMBER | "(" expr ")"
// the inner expr is parsed fully and returned as one resulting node
static Node *primary(void) {
	if (g_token.type == TOKEN_LPAREN) {
		advance();
		Node *node = expr();
		expect(TOKEN_RPAREN, "expected ')'");
		return node;
	}
	return number();
}

// unary = "-" unary | primary
// a prefix rule: it calls itself after eating the "-", so `- - 4` becomes neg(neg(4))
// it sits below term, so `-2 * 3` is `(-2) * 3`
static Node *unary(void) {
	if (g_token.type == TOKEN_MINUS) {
		advance();
		return new_unary(NODE_NEG, unary());
	}
	return primary();
}

// term = unary (("*" | "/") unary)*
static Node *term(void) {
	Node *node = unary();
	while (g_token.type == TOKEN_STAR || g_token.type == TOKEN_SLASH) {
		NodeKind kind = g_token.type == TOKEN_STAR ? NODE_MUL : NODE_DIV;
		advance();
		node = new_binary(kind, node, unary());
	}
	return node;
}

// expr = term (("+" | "-") term)*
// same loop as term, but its operands are terms, so `*` binds tighter
static Node *expr(void) {
	Node *node = term();
	while (g_token.type == TOKEN_PLUS || g_token.type == TOKEN_MINUS) {
		NodeKind kind = g_token.type == TOKEN_PLUS ? NODE_ADD : NODE_SUB;
		advance();
		// the tree so far becomes the left child of the new node
		node = new_binary(kind, node, term());
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
