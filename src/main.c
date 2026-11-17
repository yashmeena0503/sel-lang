#include "app/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void compile(FILE *out) {
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

static char *read_file(const char *path) {
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		fprintf(stderr, "could not open file \"%s\".\n", path);
		exit(74);
	}
	fseek(file, 0L, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);
	char *buffer = (char *)malloc(file_size + 1);
	if (buffer == NULL) {
		fprintf(stderr, "not enough memory to read \"%s\".\n", path);
		exit(74);
	}
	size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
	if (bytes_read < file_size) {
		fprintf(stderr, "could not read file \"%s\".\n", path);
		exit(74);
	}
	buffer[bytes_read] = '\0';
	fclose(file);
	return buffer;
}

int main(int argc, const char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: sel <input> <output.s>\n");
		return 64;
	}
	char *source = read_file(argv[1]);
	init_lexer(&g_lexer, source);

	FILE *out = fopen(argv[2], "w");
	if (out == NULL) {
		fprintf(stderr, "could not open \"%s\"\n", argv[2]);
		return 74;
	}
	compile(out);
	fclose(out);
	free(source);
	return 0;
}
