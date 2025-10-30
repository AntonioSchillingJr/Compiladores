#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "semantics.h"

int     SEM_current_function_type = TYPE_UNTYPED;
Symbol *SEM_current_function      = NULL;

typedef struct {
  int count;
  int types[MAX_CALL_ARGS];
} CallArgsFrame;

static CallArgsFrame SEM_call_stack[MAX_CALL_DEPTH];
static int           SEM_call_sp = -1;

extern int get_line_number(void);

void SEM_error(int code, const char *fmt, ...)
{
  int line = get_line_number();
  fprintf(stderr, "Erro (linha %d): ", line);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  fputc('\n', stderr);
  exit(code);
}

int promote_bin(int a, int b) {
  if (a == TYPE_UNTYPED) return b;
  if (b == TYPE_UNTYPED) return a;
  if (a == TYPE_INT   && b == TYPE_INT)   return TYPE_INT;
  if (a == TYPE_FLOAT && b == TYPE_FLOAT) return TYPE_FLOAT;
  return -1; /* incompatível */
}

void SEM_args_begin(void) {
  if (SEM_call_sp + 1 < MAX_CALL_DEPTH) {
    SEM_call_sp++;
    SEM_call_stack[SEM_call_sp].count = 0;
  }
}

void SEM_args_push(int t) {
  if (SEM_call_sp < 0) return;
  CallArgsFrame *f = &SEM_call_stack[SEM_call_sp];
  if (f->count < MAX_CALL_ARGS) {
    f->types[f->count++] = t;
  }
}

int SEM_args_count(void) {
  return (SEM_call_sp >= 0) ? SEM_call_stack[SEM_call_sp].count : 0;
}

int SEM_args_type_at(int i) {
  if (SEM_call_sp < 0) return TYPE_UNTYPED;
  CallArgsFrame *f = &SEM_call_stack[SEM_call_sp];
  if (i < 0 || i >= f->count) return TYPE_UNTYPED;
  return f->types[i];
}

void SEM_args_end(void) {
  if (SEM_call_sp >= 0) SEM_call_sp--;
}

asd_tree_t* set_dtype(asd_tree_t* n, int t){
  if (n) n->dtype = t;
  return n;
}
