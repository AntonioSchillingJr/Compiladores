#ifndef SYMBOLNODE_H
#define SYMBOLNODE_H

#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"


/**
 * @brief lista de simbolos
 */
typedef struct SymbolNode {
    Symbol *symbol;
    struct SymbolNode *next;
} SymbolNode;


SymbolNode *SymbolNode_create(Symbol *_symbol);
SymbolNode *SymbolNode_add_node(SymbolNode *_list, Symbol *_symbol);
Symbol *SymbolNode_get_Symbol_by_key(SymbolNode *_list, const char *_key_to_be_searched);
SymbolNode *SymbolNode_destroy_list(SymbolNode *_list);
void SymbolNode_print(SymbolNode *_list);
void SymbolNode_test_implementation();

#endif  // #ifndef SYMBOLNODE_H