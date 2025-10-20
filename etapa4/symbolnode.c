#include "symbolnode.h"

SymbolNode *SymbolNode_create(Symbol *_symbol) {
    SymbolNode *new_node = malloc(sizeof(SymbolNode));
    new_node->symbol = _symbol;
    new_node->next = NULL;
    return new_node;
}

SymbolNode *SymbolNode_add_node(SymbolNode *_list, Symbol *_symbol) {
    
    if (_list == NULL) {
        return SymbolNode_create(_symbol);
    }

    // empilha sobre a lista ja existente
    SymbolNode *new_node = malloc(sizeof(SymbolNode));
    new_node->symbol = _symbol;
    new_node->next = _list;

    return new_node;
}

Symbol *SymbolNode_get_Symbol_by_key(SymbolNode *_list, const char *_key_to_be_searched) {
    
    while (_list != NULL) {
        
        if (Symbol_is_key_equal_to_string(_list->symbol, _key_to_be_searched)) {
            return _list->symbol;
        }
        
        _list = _list->next;
    }

    return NULL;
}

SymbolNode *SymbolNode_destroy_list(SymbolNode *_list) {

    while (_list != NULL) {

        // salva valor antes da mudanca
        SymbolNode *aux = _list;

        // update para proximo valor antes de destruir memoria
        _list = _list->next;

        // limpa recursos do node atual
        aux->symbol = Symbol_destroy(aux->symbol);
        free(aux);
    }

    return NULL;
}

void SymbolNode_print(SymbolNode *_list) {
    printf("[ ");
    while (_list != NULL) {
        Symbol_print(_list->symbol);
        if (_list->next != NULL) {
            printf(", ");
        } 
        _list = _list->next;
    }
    printf(" ]\n");
}

void SymbolNode_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);
        char key_test[] = "KeyTest";
        char key_test_2[] = "KeyTest2";
        char non_existent_key_value[] = "NonExistentKeyValue";
        char value_test[] = "ValueTest";
        char value_test_2[] = "ValueTest2";

        Symbol *symbol_test = Symbol_create(key_test, 1, 10, 100, value_test);
        Symbol *symbol_test_2 = Symbol_create(key_test_2, 2, 20, 200, value_test_2);
    
        printf("Creating list with one Symbol:\n");
        SymbolNode *symbolnode_test = SymbolNode_create(symbol_test);
        SymbolNode_print(symbolnode_test);

        printf("Adding another Symbol:\n");
        symbolnode_test = SymbolNode_add_node(symbolnode_test, symbol_test_2);
        SymbolNode_print(symbolnode_test);

        printf("Retrieving node with \"%s\" as key:\n", 
            key_test);
        Symbol_println(SymbolNode_get_Symbol_by_key(symbolnode_test, key_test));

        printf("Retrieving node with \"%s\" as key:\n", 
            key_test_2);
        Symbol_println(SymbolNode_get_Symbol_by_key(symbolnode_test, key_test_2));

        printf("Retrieving node with \"%s\" as key (should be Null):\n", 
            non_existent_key_value);
        Symbol_println(SymbolNode_get_Symbol_by_key(symbolnode_test, non_existent_key_value));

        symbolnode_test = SymbolNode_destroy_list(symbolnode_test);
        printf("Pointer state after destroy operation: %s.\n", is_nullified_string(symbolnode_test));
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}