#ifndef APP_PARSER_H
#define APP_PARSER_H

#include "app/ast.h"

// parses `source` into an AST; prints an error and exits on bad syntax
Node *parse(const char *source);

#endif
