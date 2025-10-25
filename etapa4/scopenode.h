#ifndef SCOPENODE_H
#define SCOPENODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "scope.h"

typedef struct ScopeNode {
    Scope *scope;
    struct ScopeNode *next;
} ScopeNode;

ScopeNode *ScopeNode_get_bottom_reference(ScopeNode *_scope_stack);
ScopeNode *ScopeNode_create_empty();
ScopeNode *ScopeNode_create(Scope *_scope);
bool ScopeNode_is_empty(ScopeNode *_scope_stack);
ScopeNode *ScopeNode_add_empty_node(ScopeNode *_scope_stack);
ScopeNode *ScopeNode_create_with_symbol(Symbol *_symbol);
ScopeNode *ScopeNode_create_with_new_symbol(const char *_key, int _line, int _nature, int _type, const char *_value);
ScopeNode *ScopeNode_add_scope(ScopeNode *_scope_stack, Scope *_scope);
ScopeNode *ScopeNode_add_symbol(ScopeNode *_scope_stack, Symbol *_symbol);
Symbol *ScopeNode_get_symbol_by_key(ScopeNode *_scope_stack, const char *_key_to_be_searched);
ScopeNode *ScopeNode_destroy_list(ScopeNode *_scope_stack);
ScopeNode *ScopeNode_pop_scope(ScopeNode *_scope_stack);
void ScopeNode_print(ScopeNode *_scope_stack);
void Scopenode_test_implementation();

#endif  // #ifndef SCOPENODE_H