#ifndef APP_COMPILER_H
#define APP_COMPILER_H

#include <stdio.h>

// compiles `source` and writes x86-64 assembly to `out`; exits on error
void compile(FILE *out, const char *source);

#endif
