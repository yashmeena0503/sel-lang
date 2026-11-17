#include "app/codegen.h"

void gen_expr(FILE *out, Node *node) {
	if (node->kind == NODE_NUMBER) {
		fprintf(out, "    mov $%ld, %%rax\n", node->value);
		return;
	}
	gen_expr(out, node->lhs);
	// the rhs code overwrites rax, so the stack holds lhs meanwhile. The
	// stack (not a register) because nesting depth is unbounded
	fprintf(out, "    push %%rax\n");
	gen_expr(out, node->rhs);
	fprintf(out, "    mov %%rax, %%rcx\n");
	fprintf(out, "    pop %%rax\n");
	// now rax = lhs, rcx = rhs
	if (node->kind == NODE_ADD)
		fprintf(out, "    add %%rcx, %%rax\n");
	else
		fprintf(out, "    sub %%rcx, %%rax\n");
}
