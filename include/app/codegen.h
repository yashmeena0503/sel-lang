#ifndef APP_CODEGEN_H
#define APP_CODEGEN_H

#include "app/ast.h"
#include <stdio.h>

// emits code that leaves the value of `node` in %rax
void gen_expr(FILE *out, Node *node);

#endif
