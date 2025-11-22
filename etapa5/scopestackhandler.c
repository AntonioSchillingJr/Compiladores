#include <stdlib.h>
#include <stdio.h> 
#include "scopestackhandler.h"

// global auxiliar para evitar ter que controlar passagem de parametros desnecssarios
// entre funcoes no arquivo do parser 
ScopeStackHandler *scope_stack_handler = NULL;

// start handler
void ScopeStackHandler_start() {
    
    // libera qualquer operacao anterior como garantia
    ScopeStackHandler_end();

    // prepara handler para novas operacoes
    scope_stack_handler = malloc(sizeof(ScopeStackHandler));
    scope_stack_handler->scope_stack = ScopeNode_create_empty();
}

// push scope
void ScopeStackHandler_push_empty_scope() {
    scope_stack_handler->scope_stack = ScopeNode_add_empty_node(scope_stack_handler->scope_stack);
}

// add symbol to current scope
void ScopeStackHandler_add_symbol_to_current_scope(Symbol *_symbol) {
    scope_stack_handler->scope_stack = ScopeNode_add_symbol(scope_stack_handler->scope_stack, _symbol);
}

void ScopeStackHandler_add_new_symbol_to_current_scope(const char *_key, int _line, int _nature, int _type, const char *_value) {
    Symbol *new_symbol = Symbol_create(_key, _line, _nature, _type, _value);
    scope_stack_handler->scope_stack = ScopeNode_add_symbol(scope_stack_handler->scope_stack, new_symbol);
}

// add symbol to global (first/bottom) scope 
void ScopeStackHandler_add_to_global_scope(Symbol *_symbol) {
    // OBS: ja que ficar modificando referencias adiciona complexidade desnecessaria e a reordenacao 
    // nao precisa ser realizada ja que a ordem dos simbolos na lista nao importa para o look up, 
    // basta localizar o escopo global e adicionar ao final da lista dele uma nova referencia 
    
    // assumindo que ScopeStackHandler_start() foi chamada, 
    // sempre scope_stack_handler != NULL
    // if (scope_stack_handler == NULL) { nao existe necessidade de cuidar desse caso }

    // localiza fundo da pilha de escopos
    ScopeNode *global_scope = ScopeNode_get_bottom_reference(scope_stack_handler->scope_stack);
    
    // assumindo que ScopeStackHandler_start() foi chamada, 
    // sempre global_scope != NULL
    // if (global_scope == NULL) { nao existe necessidade de cuidar desse caso }

    // adiciona novo simbolo no final da lista global
    // OBS: salvar o novo valor de scope cobre o caso scope == NULL
    global_scope->scope = Scope_add_symbol_at_the_end(global_scope->scope, _symbol);
}

// pop scope
void ScopeStackHandler_pop_scope() { 
    scope_stack_handler->scope_stack = ScopeNode_pop_scope(scope_stack_handler->scope_stack);
}

// check if symbol is in current scope
bool ScopeStackHandler_is_key_defined_in_current_scope(const char *_key) {
    // se o retorno do look up for diferente de NULL, achou algum simbolo no escopo atual com a mesma chave
    // se tem a mesma chave, entao ja foi definido
    return (Scope_get_symbol_by_key(scope_stack_handler->scope_stack->scope, _key) != NULL);
}


// get first symbol that matches key, from any scope of the stack
Symbol *ScopeStackHandler_get_symbol_by_key(const char *_key) {
    return ScopeNode_get_symbol_by_key(scope_stack_handler->scope_stack, _key);
}

// destroy handler
void ScopeStackHandler_end() {
    if (scope_stack_handler != NULL) {
        scope_stack_handler->scope_stack = ScopeNode_destroy_list(scope_stack_handler->scope_stack);
        free(scope_stack_handler);
    }

    scope_stack_handler = NULL;
}

void ScopeStackHandler_print() {
    ScopeNode_print(scope_stack_handler->scope_stack);
}

void ScopeStackHandler_test_implementation() {
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
        
        printf("Before start function:\t");
        if (scope_stack_handler == NULL) {
            printf("Handler not initialized.\n");
        } 
        else {
            printf("Handler was initialized with trash value.\n");
        }

        printf("Handler start function called:\t");
        ScopeStackHandler_start();
        if (scope_stack_handler != NULL) {
            printf("Handler operating as expected.\n");
        } 
        else {
            printf("Hadler some error ocurred.\n");
        }

        printf("Handler value after initialization:\n");
        ScopeStackHandler_print();

        printf("Push operation of a new empty Scope:\n");
        ScopeStackHandler_push_empty_scope();
        ScopeStackHandler_print();

        printf("Adding a Symbol to current Scope:\n");
        ScopeStackHandler_add_new_symbol_to_current_scope(key_test_2, 2, 20, 200, value_test_2);
        ScopeStackHandler_print();

        printf("Adding a Symbol to current Scope:\n");
        ScopeStackHandler_add_new_symbol_to_current_scope(key_test_3, 3, 30, 300, value_test_3);
        ScopeStackHandler_print();

        printf("Adding a Symbol to global Scope:\n");
        ScopeStackHandler_add_to_global_scope(Symbol_create(key_test_4, 4, 40, 400, value_test_4));
        ScopeStackHandler_print();

        printf("Pop operation on current top Scope:\n");
        ScopeStackHandler_pop_scope();
        ScopeStackHandler_print();

        printf("Adding a Symbol to current Scope:\n");
        ScopeStackHandler_add_new_symbol_to_current_scope(key_test, 1, 10, 100, value_test);
        ScopeStackHandler_print();

        printf("Adding a Symbol to global (current) Scope:\n");
        ScopeStackHandler_add_to_global_scope(Symbol_create(key_test_2, 2, 20, 200, value_test_2));
        ScopeStackHandler_print();

        printf("Retrieving symbol with \"%s\" as key:\n\t", 
            key_test);
        Symbol_println(ScopeStackHandler_get_symbol_by_key(key_test));

        printf("Retrieving symbol with \"%s\" as key:\n\t", 
            key_test_2);
        Symbol_println(ScopeStackHandler_get_symbol_by_key(key_test_2));

        printf("Retrieving symbol with \"%s\" as key (should be Null):\n\t", 
            non_existent_key_value);
        Symbol_println(ScopeStackHandler_get_symbol_by_key(non_existent_key_value));


        printf("Destroy operation:\n\t");
        ScopeStackHandler_end();
        printf("Pointer state after destroy operation: %s.\n", is_nullified_string(scope_stack_handler));
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}
