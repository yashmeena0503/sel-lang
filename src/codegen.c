#include "app/codegen.h"

void gen_expr(FILE *out, Node *node) {
	if (node->kind == NODE_NUMBER) {
		fprintf(out, "    mov $%ld, %%rax\n", node->value);
		return;
	}
	// one child, so no push/pop: compute it into rax, then flip the sign in place
	if (node->kind == NODE_NEG) {
		gen_expr(out, node->lhs);
		fprintf(out, "    neg %%rax\n");
		return;
	}
	gen_expr(out, node->lhs);
	// the rhs code overwrites rax, so the stack holds lhs meanwhile.
    // The stack (not a register) because nesting depth is unbounded
	fprintf(out, "    push %%rax\n");
	gen_expr(out, node->rhs);
    fprintf(out, "    mov %%rax, %%rcx\n");
    fprintf(out, "    pop %%rax\n");
    // now rax = lhs, rcx = rhs
    switch (node->kind) {
        case NODE_ADD:
            fprintf(out, "    add %%rcx, %%rax\n");
            break;
        case NODE_SUB:
            fprintf(out, "    sub %%rcx, %%rax\n");
            break;
        case NODE_MUL:
            fprintf(out, "    imul %%rcx, %%rax\n");
            break;
        case NODE_DIV:
            // idiv has implicit operands: it divides the 128-bit value rdx:rax by rcx.
            // cqo fills rdx with rax's sign bit (sign-extends rax to 128 bits).
            // after: rax = quotient, rdx = remainder
            fprintf(out, "    cqo\n");
            fprintf(out, "    idiv %%rcx\n");
            break;
        case NODE_NUMBER: // NUMBER and NEG are handled above;
        case NODE_NEG:    // listed so -Wswitch shuts up
            break;
    }
}
