#include "app/ast.h"
#include <stdlib.h>

// calloc zeroes the node, so fields a kind doesn't use lhs/rhs on a number
// (e.g. lhs/rhs on a number) are NULL instead of garbage
Node *new_number(long value) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = NODE_NUMBER;
	node->value = value;
	return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

// one child, stored in lhs; rhs stays NULL thanks to calloc
Node *new_unary(NodeKind kind, Node *operand) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = operand;
	return node;
}
