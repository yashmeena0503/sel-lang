# sel

A compiler for **sel**, a small programming language that is still in development.
- Compiles to x86-64 assembly (for now) using raw syscalls and no libc
- Written in C11

Right now the language only has `ret` and arithmetic expressions.\
Variables, control flow and functions are planned.

## Example

```
// examples/prec.sel
ret 1 + 2 * 3
```

The program's result becomes its exit code:

```sh
make
./build/sel examples/prec.sel out.s    # sel -> assembly
as out.s -o out.o                      # assembly -> object file
ld out.o -o out                        # object file -> executable
./out; echo $?                         # prints 7
```

Generated `out.s`. Each subtree leaves its result in `%rax`, and the stack holds the pending left side:

```asm
.global _start
.text
_start:
    mov $1, %rax        # lhs of +
    push %rax           # park it, the rhs code will overwrite rax
    mov $2, %rax        # lhs of *
    push %rax
    mov $3, %rax
    mov %rax, %rcx      # rcx = rhs
    pop %rax            # rax = lhs
    imul %rcx, %rax     # rax = 6
    mov %rax, %rcx
    pop %rax
    add %rcx, %rax      # rax = 7
    mov %rax, %rdi      # exit status = result
    mov $60, %eax       # exit syscall
    syscall
```

## Requirements

Linux x86-64, `gcc`, `make`, and GNU `as` / `ld` (binutils). `clang-format` is optional (`make fmt`).

## Language so far

```
program = "ret" expr EOF
expr    = term (("+" | "-") term)*
term    = primary (("*" | "/") primary)*
primary = NUMBER | "(" expr ")"
```

- `+ - * /` on integers, with `*` and `/` binding tighter than `+` and `-`
- All operators are left-associative (`10 - 3 - 2` is `5`)
- `( )` group sub-expressions and override precedence
- `//` line comments

## How it works

| Stage | File | What it does |
|---|---|---|
| Lexer | `src/lexer.c` | Hand-written, single pass, no regex. Tokens are a pointer and length into the source buffer, so nothing is copied. |
| Parser | `src/parser.c` | Recursive descent, one function per grammar rule. Precedence comes from the layering of the rules, associativity from their loops. Builds an AST. |
| Codegen | `src/codegen.c` | Post-order walk of the AST. Results go in `%rax`, pending values on the stack. Division uses `cqo` + `idiv`. |
| Glue | `src/compiler.c` | Runs parse and codegen, then emits the `_start` and exit-syscall wrapper. |

## Tests

```sh
make test
```

Each `examples/X.sel` has an `examples/X.exp` holding the expected exit code, or `compile-error`. `tests/run.sh` compiles, assembles, links and runs every example and compares the result.
