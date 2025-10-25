#include "scope.h"

Scope *Scope_create_with_new_symbol(const char *_key, int _line, int _nature, int _type, const char *_value) {
    Scope *new_scope = malloc(sizeof(Scope));
    new_scope->symbolList = SymbolNode_create_with_new_symbol(_key, _line, _nature, _type, _value);
    return new_scope;
}

Scope *Scope_create(Symbol *_symbol) {
    Scope *new_scope = malloc(sizeof(Scope));
    new_scope->symbolList = SymbolNode_create(_symbol);
    return new_scope;
}

Scope *Scope_add_new_symbol(Scope *_scope, const char *_key, int _line, int _nature, int _type, const char *_value) {
    
    if (_scope == NULL) {
        return Scope_create_with_new_symbol(_key, _line, _nature, _type, _value);
    }
    Symbol *new_symbol = Symbol_create(_key, _line, _nature, _type, _value);
    _scope->symbolList = SymbolNode_add_node(_scope->symbolList, new_symbol);

    return _scope;
}

Scope *Scope_add_symbol(Scope *_scope, Symbol *_symbol) {
    
    if (_scope == NULL) {
        return Scope_create(_symbol);
    }

    _scope->symbolList = SymbolNode_add_node(_scope->symbolList, _symbol);

    return _scope;
}

Scope *Scope_add_symbol_at_the_end(Scope *_scope, Symbol *_symbol) {
    
    if (_scope == NULL) {
        return Scope_create(_symbol);
    }

    _scope->symbolList = SymbolNode_add_node_at_the_end(_scope->symbolList, _symbol);

    return _scope;
}

Symbol *Scope_get_symbol_by_key(Scope *_scope, const char *_key_to_be_searched) {
    if (_scope == NULL) {
        return NULL;
    }
    
    return SymbolNode_get_symbol_by_key(_scope->symbolList, _key_to_be_searched);
}

Scope *Scope_destroy(Scope *_scope) {
    if (_scope != NULL) {
        _scope->symbolList = SymbolNode_destroy_list(_scope->symbolList);
        free(_scope);
    }

    return NULL;
}

void Scope_print(Scope *_scope) {
    if (_scope == NULL) {
        printf("Scope: Null\n");
    } 
    else {
        printf("Scope: ");
        SymbolNode_print(_scope->symbolList);
    }
}

void Scope_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);
        char key_test[] = "KeyTest";
        char key_test_2[] = "KeyTest2";
        char key_test_3[] = "KeyTest3";
        char non_existent_key_value[] = "NonExistentKeyValue";
        char value_test[] = "ValueTest";
        char value_test_2[] = "ValueTest2";
        char value_test_3[] = "ValueTest3";
   
        printf("Creating Scope with one new Symbol:\n");
        Scope *scope_test = Scope_create_with_new_symbol(key_test_2, 2, 20, 200, value_test_2);
        Scope_print(scope_test);

        printf("Adding a new Symbol:\n");
        scope_test = Scope_add_new_symbol(scope_test, key_test, 1, 10, 100, value_test);
        Scope_print(scope_test);

        printf("Adding a Symbol at the end:\n");
        scope_test = Scope_add_symbol_at_the_end(scope_test, Symbol_create(key_test_3, 3, 30, 300, value_test_3));
        Scope_print(scope_test);

        printf("Retrieving symbol with \"%s\" as key:\n", 
            key_test);
        Symbol_println(Scope_get_symbol_by_key(scope_test, key_test));

        printf("Retrieving symbol with \"%s\" as key:\n", 
            key_test_2);
        Symbol_println(Scope_get_symbol_by_key(scope_test, key_test_2));

        printf("Retrieving symbol with \"%s\" as key (should be Null):\n", 
            non_existent_key_value);
        Symbol_println(Scope_get_symbol_by_key(scope_test, non_existent_key_value));

        scope_test = Scope_destroy(scope_test);
        printf("Pointer state after destroy operation: %s.\n", is_nullified_string(scope_test));
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}
