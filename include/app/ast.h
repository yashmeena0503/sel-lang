#ifndef APP_AST_H
#define APP_AST_H

typedef enum {
	NODE_NUMBER,
	NODE_ADD,
	NODE_SUB,
} NodeKind;

// the struct contains pointers to itself, so it needs this forward typedef
// before the definition; inside it `Node *` isn't a known name yet otherwise
typedef struct Node Node;
struct Node {
	NodeKind kind;
	long value; // NODE_NUMBER only
	Node *lhs;  // NODE_ADD / NODE_SUB only
	Node *rhs;
};

Node *new_number(long value);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);

#endif
