#include "app/compiler.h"
#include "app/codegen.h"
#include "app/parser.h"
#include <stdio.h>

void compile(FILE *out, const char *source) {
	Node *root = parse(source);

	fprintf(out, ".global _start\n");
	fprintf(out, ".text\n");
	fprintf(out, "_start:\n");
	gen_expr(out, root);
	fprintf(out, "    mov %%rax, %%rdi\n"); // exit status = the result
	fprintf(out, "    mov $60, %%eax\n");
	fprintf(out, "    syscall\n");
}
