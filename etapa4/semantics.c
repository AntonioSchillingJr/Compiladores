#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "semantics.h"

int SEM_current_function_type = TYPE_UNTYPED;
Symbol *SEM_current_function = NULL;

CallArgsFrame SEM_call_stack[MAX_CALL_DEPTH];
int SEM_call_sp = -1;

extern int get_line_number(void);

void sem_error(int code, const char *fmt, ...)
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
