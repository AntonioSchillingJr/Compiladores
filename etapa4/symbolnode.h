#ifndef SYMBOLNODE_H
#define SYMBOLNODE_H

#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"


/**
 * @brief lista de simbolos
 */
typedef struct SymbolNode {
    Symbol symbol;
    struct SymbolNode* next;
} SymbolNode;


#endif  // #ifndef SYMBOLNODE_H