#include "symbolnode.h"


SymbolNode *SymbolNode_create(Symbol _symbol) {
    SymbolNode *new_node = malloc(sizeof(SymbolNode));
    new_node->symbol = _symbol;
    new_node->next = NULL;
    return new_node;
}


SymbolNode *SymbolNode_add_node(SymbolNode *_list, Symbol _symbol) {
    
    if (_list == NULL) {
        return SymbolNode_create(_symbol);
    }

    // empilha sobre a lista ja existente
    SymbolNode *new_node = (SymbolNode*)malloc(sizeof(SymbolNode));
    new_node->symbol = _symbol;
    new_node->next = _list;

    return new_node;
}