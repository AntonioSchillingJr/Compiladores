#ifndef SCOPE_H
#define SCOPE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "symbolnode.h"

typedef struct {
    SymbolNode *symbolList; 
} Scope;

Scope *Scope_create_with_new_symbol(const char *_key, int _line, int _nature, int _type, const char *_value);
Scope *Scope_create(Symbol *_symbol);
Scope *Scope_add_new_symbol(Scope *_scope, const char *_key, int _line, int _nature, int _type, const char *_value);
Scope *Scope_add_symbol(Scope *_scope, Symbol *_symbol);
Scope *Scope_add_symbol_at_the_end(Scope *_scope, Symbol *_symbol);
Symbol *Scope_get_symbol_by_key(Scope *_scope, const char *_key_to_be_searched);
Scope *Scope_destroy(Scope *_scope);
void Scope_print(Scope *_scope);
void Scope_test_implementation();

#endif  // #ifndef SCOPE_H