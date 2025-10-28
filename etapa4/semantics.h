#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <stdarg.h>
#include <stdbool.h>
#include "errors.h"
#include "asd.h"
#include "symbol.h"
#include "scopestackhandler.h"

#define MAX_CALL_ARGS 128
#define MAX_CALL_DEPTH 128

/* Tipos da linguagem */
enum {
  TYPE_UNTYPED = 0,
  TYPE_INT     = 1,
  TYPE_FLOAT   = 2
};

/* Natures */
enum {
  NATURE_UNDEF = 0,
  NATURE_VAR   = 1,
  NATURE_FUNC  = 2,
  NATURE_LIT   = 3
};

typedef struct {
  int count;
  int types[MAX_CALL_ARGS];
} CallArgsFrame;

/* Estado da função corrente (p/ checar return e parâmetros) */
extern int SEM_current_function_type;
/* Ponteiro para o símbolo da função corrente (para guardar tipos de parâmetros) */
extern Symbol *SEM_current_function;

extern CallArgsFrame SEM_call_stack[MAX_CALL_DEPTH];
extern int SEM_call_sp;

void sem_error(int code, const char *fmt, ...);

/* int+int -> int; float+float -> float; misto -> erro */
static inline int promote_bin(int a, int b) {
  if (a == TYPE_UNTYPED) return b;
  if (b == TYPE_UNTYPED) return a;
  if (a == TYPE_INT   && b == TYPE_INT)   return TYPE_INT;
  if (a == TYPE_FLOAT && b == TYPE_FLOAT) return TYPE_FLOAT;
  return -1; /* incompatível */
}

static inline void SEM_args_begin(void) {
  if (SEM_call_sp + 1 < MAX_CALL_DEPTH) {
    SEM_call_sp++;
    SEM_call_stack[SEM_call_sp].count = 0;
  }
}
static inline void SEM_args_push(int t) {
  CallArgsFrame *f = &SEM_call_stack[SEM_call_sp];
  if (f->count < MAX_CALL_ARGS) f->types[f->count++] = t;
}
static inline int SEM_args_count(void) {
  return SEM_call_stack[SEM_call_sp].count;
}
static inline int SEM_args_type_at(int i) {
  return SEM_call_stack[SEM_call_sp].types[i];
}
static inline void SEM_args_end(void) {
  if (SEM_call_sp >= 0) SEM_call_sp--;
}

/* helper para anotar tipo em nó */
static inline asd_tree_t* set_dtype(asd_tree_t* n, int t){ if(n) n->dtype=t; return n; }

#endif
