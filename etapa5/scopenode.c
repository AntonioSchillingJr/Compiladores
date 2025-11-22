#include "scopenode.h"


ScopeNode *ScopeNode_get_bottom_reference(ScopeNode *_scope_stack) {
    if (_scope_stack == NULL) {
        return NULL;
    }

    while (_scope_stack->next != NULL) {
        _scope_stack = _scope_stack->next;
    }

    return _scope_stack;
}

ScopeNode *ScopeNode_create_empty() {
    ScopeNode *new_scope_list = malloc(sizeof(ScopeNode));    
    new_scope_list->scope = NULL;
    new_scope_list->next = NULL;
    return new_scope_list;
}

ScopeNode *ScopeNode_create(Scope *_scope) {
    ScopeNode *new_scope_list = malloc(sizeof(ScopeNode));    
    new_scope_list->scope = _scope;
    new_scope_list->next = NULL;
    return new_scope_list;
}

bool ScopeNode_is_empty(ScopeNode *_scope_stack) {
    return is_nullified(_scope_stack);
}

ScopeNode *ScopeNode_add_empty_node(ScopeNode *_scope_stack) {

    if (_scope_stack == NULL) {
        return ScopeNode_create_empty();
    }

    // empilha sobre a lista ja existente
    ScopeNode *new_node = malloc(sizeof(ScopeNode));
    new_node->scope = NULL;
    new_node->next = _scope_stack;

    return new_node;
}

ScopeNode *ScopeNode_create_with_symbol(Symbol *_symbol) {
    ScopeNode *new_scope_list = malloc(sizeof(ScopeNode));    
    new_scope_list->scope = Scope_create(_symbol);
    new_scope_list->next = NULL;
    return new_scope_list;
}

ScopeNode *ScopeNode_create_with_new_symbol(const char *_key, int _line, int _nature, int _type, const char *_value) {
    ScopeNode *new_scope_list = malloc(sizeof(ScopeNode));    
    new_scope_list->scope = Scope_create_with_new_symbol(_key, _line, _nature, _type, _value);
    new_scope_list->next = NULL;
    return new_scope_list;
}

ScopeNode *ScopeNode_add_scope(ScopeNode *_scope_stack, Scope *_scope) {
    
    if (_scope_stack == NULL) {
        return ScopeNode_create(_scope);
    }

    // empilha sobre a lista ja existente
    ScopeNode *new_node = malloc(sizeof(ScopeNode));
    new_node->scope = _scope;
    new_node->next = _scope_stack;

    return new_node;
}

ScopeNode *ScopeNode_add_symbol(ScopeNode *_scope_stack, Symbol *_symbol) {

    if (_scope_stack == NULL) {
        return ScopeNode_create_with_symbol(_symbol);
    }

    _scope_stack->scope = Scope_add_symbol(_scope_stack->scope, _symbol);
    return _scope_stack;
}

Symbol *ScopeNode_get_symbol_by_key(ScopeNode *_scope_stack, const char *_key_to_be_searched) {
    
    while (_scope_stack != NULL) {
        
        Symbol *possible_result = Scope_get_symbol_by_key(_scope_stack->scope, _key_to_be_searched);
        if (possible_result != NULL) {
            return possible_result;
        }
        
        _scope_stack = _scope_stack->next;
    }

    return NULL;
}

ScopeNode *ScopeNode_destroy_list(ScopeNode *_scope_stack) {

    while (_scope_stack != NULL) {

        // salva valor antes da mudanca
        ScopeNode *aux = _scope_stack;

        // update para proximo valor antes de destruir memoria
        _scope_stack = _scope_stack->next;

        // limpa recursos do node atual
        aux->scope = Scope_destroy(aux->scope);
        free(aux);
    }

    return NULL;
}

ScopeNode *ScopeNode_pop_scope(ScopeNode *_scope_stack) {
    if ((_scope_stack == NULL) || (_scope_stack->next == NULL)) {
        return ScopeNode_destroy_list(_scope_stack);
    }

    // salva valor para poder liberar memoria sem erro
    ScopeNode *new_top = _scope_stack->next;

    // libera memoria utilzada pelo nodo superior
    _scope_stack->scope = Scope_destroy(_scope_stack->scope);
    free(_scope_stack);

    return new_top;
}

void ScopeNode_print(ScopeNode *_scope_stack) {
    printf("Scope Stack: \n");
    if (_scope_stack == NULL) {
        printf("\tNULL\n");
    } 
    else {
        while (_scope_stack != NULL) {
            printf("\t");
            Scope_print(_scope_stack->scope);
            _scope_stack = _scope_stack->next;
        }
    }
}

void Scopenode_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);
        char key_test[] = "KeyTest";
        char key_test_2[] = "KeyTest2";
        char key_test_3[] = "KeyTest3";
        char key_test_4[] = "KeyTest4";
        char non_existent_key_value[] = "NonExistentKeyValue";
        char value_test[] = "ValueTest";
        char value_test_2[] = "ValueTest2";
        char value_test_3[] = "ValueTest3";
        char value_test_4[] = "ValueTest4";
   
        printf("Creating empty ScopeNode:\n");
        ScopeNode *scopenode_test = ScopeNode_create_empty();
        ScopeNode_print(scopenode_test);

        printf("Adding a Symbol to current Scope:\n");
        Symbol *symbol_test = Symbol_create(key_test_2, 2, 20, 200, value_test_2);
        scopenode_test = ScopeNode_add_symbol(scopenode_test, symbol_test);
        ScopeNode_print(scopenode_test);

        Scope *scope_test = Scope_create_with_new_symbol(key_test, 1, 10, 100, value_test);
        scope_test = Scope_add_new_symbol(scope_test, key_test_3, 3, 30, 300, value_test_3);

        printf("Adding an existing Scope:\n");
        scopenode_test = ScopeNode_add_scope(scopenode_test, scope_test);
        ScopeNode_print(scopenode_test);

        symbol_test = Symbol_create(key_test_4, 4, 40, 400, value_test_4);

        printf("Adding an existing Symbol:\n");
        scopenode_test = ScopeNode_add_symbol(scopenode_test, symbol_test);
        ScopeNode_print(scopenode_test);

        printf("Retrieving symbol with \"%s\" as key:\n\t", 
            key_test);
        Symbol_println(ScopeNode_get_symbol_by_key(scopenode_test, key_test));

        printf("Retrieving symbol with \"%s\" as key:\n\t", 
            key_test_2);
        Symbol_println(ScopeNode_get_symbol_by_key(scopenode_test, key_test_2));

        printf("Retrieving symbol with \"%s\" as key (should be Null):\n\t", 
            non_existent_key_value);
        Symbol_println(ScopeNode_get_symbol_by_key(scopenode_test, non_existent_key_value));

        printf("Pop operation on ScopeNode:\n\t");
        scopenode_test = ScopeNode_pop_scope(scopenode_test);
        ScopeNode_print(scopenode_test);

        printf("Pop operation on ScopeNode (should be empty):\n\t");
        scopenode_test = ScopeNode_pop_scope(scopenode_test);
        ScopeNode_print(scopenode_test);

        scopenode_test = ScopeNode_destroy_list(scopenode_test);
        printf("Pointer state after destroy operation: %s.\n", is_nullified_string(scopenode_test));
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}
