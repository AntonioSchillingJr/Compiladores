#ifndef SCOPE_STACK_HANDLER_H
#define SCOPE_STACK_HANDLER_H

#include <stdbool.h>

#include "scopenode.h"

typedef struct ScopeStackHandler {
    ScopeNode *scope_stack;
} ScopeStackHandler;

void ScopeStackHandler_start();
void ScopeStackHandler_push_empty_scope();
void ScopeStackHandler_add_symbol_to_current_scope(Symbol *_symbol);
void ScopeStackHandler_add_new_symbol_to_current_scope(const char *_key, int _line, int _nature, int _type, const char *_value);
void ScopeStackHandler_add_to_global_scope(Symbol *_symbol);
void ScopeStackHandler_pop_scope();
bool ScopeStackHandler_is_key_defined_in_current_scope(const char *_key);
Symbol *ScopeStackHandler_get_symbol_by_key(const char *_key);
void ScopeStackHandler_end();
void ScopeStackHandler_print();
void ScopeStackHandler_test_implementation();

#endif  // #ifndef SCOPE_STACK_HANDLER_H