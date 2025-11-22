#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <stdbool.h>
#include "errors.h"
#include "asd.h"
#include "symbol.h"

#define MAX_CALL_ARGS   128
#define MAX_CALL_DEPTH  128

/* Tipos da linguagem */
enum {
  TYPE_UNTYPED = 0,
  TYPE_INT     = 1,
  TYPE_DEC     = 2
};

/* Natures */
enum {
  NATURE_UNDEF = 0,
  NATURE_VAR   = 1,
  NATURE_FUNC  = 2,
  NATURE_LIT   = 3
};

/* Estado global da função corrente (usado no parser) */
extern int     SEM_current_function_type;
extern Symbol *SEM_current_function;

/* int+int -> int; decimal+decimal -> decimal; misto -> erro */
int promote_bin(int a, int b);

/* Infra de erro */
void SEM_error(int code, const char *fmt, ...);

/* checar/empilhar tipos de argumentos de chamada de função */
void SEM_args_begin(void);
void SEM_args_push(int t);
int  SEM_args_count(void);
int  SEM_args_type_at(int i);
void SEM_args_end(void);

/* Helper para anotar dtype em nó da AST */
asd_tree_t* set_dtype(asd_tree_t* n, int t);

#endif /* SEMANTICS_H */
