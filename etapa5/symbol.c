#include "symbol.h"
#include "semantics.h"


size_t Symbol_string_length(const char *_source) {

    if (_source == NULL) {
        return 0;
    }

    size_t length = 0;
    while ((_source[length] != '\0') && (length < (SYMBOL_MAX_SIZE - 1))) {
        length++;
    }
    return length + 1;
}

bool Symbol_is_string_equal_to(const char *_string1, const char *_string2) {
    
    size_t length1 = Symbol_string_length(_string1);
    size_t length2 = Symbol_string_length(_string2);

    if (length1 != length2) {
        return false;
    }

    for (size_t i = 0; i < length1; i++) {
        if (_string1[i] != _string2[i]) {
            return false;
        }
    }

    return true;
}

bool Symbol_compare_key(Symbol *_symbol1, Symbol *_symbol2) {
    if ((_symbol1 == NULL) || (_symbol2 == NULL)) {
        return (_symbol1 == _symbol2);
    }

    return Symbol_is_string_equal_to(_symbol1->key, _symbol2->key);
}

bool Symbol_is_key_equal_to_string(Symbol *_symbol, const char *_key) {
    return ((_symbol != NULL) && Symbol_is_string_equal_to(_symbol->key, _key));
}

void Symbol_set_key(Symbol *_symbol, const char *_source) {

    if (_symbol == NULL) {
        return;
    }

    if (_source == NULL) {
        _symbol->key[0] = '\0';
    }

    size_t source_length = Symbol_string_length(_source);
    for (size_t i = 0; i < source_length; i++) {
        _symbol->key[i] = _source[i];
    }

    // padroniza para seguir convencao de string em C, caso
    // source fosse maior que 50 chars (e para garantir uso como "%s")
    _symbol->key[source_length - 1] = '\0';
}

void Symbol_set_value(Symbol *_symbol, const char *_source) {

    if (_symbol == NULL) {
        return;
    }

    if (_source == NULL) {
        _symbol->value[0] = '\0';
    }


    size_t source_length = Symbol_string_length(_source);
    for (size_t i = 0; i < source_length; i++) {
        _symbol->value[i] = _source[i];
    }

    // padroniza para seguir convencao de string em C, caso
    // source fosse maior que 50 chars (e para garantir uso como "%s")
    _symbol->value[source_length - 1] = '\0';
}

Symbol *Symbol_create(const char *_key, int _line, int _nature, int _type, const char *_value) {
    
    Symbol *new_symbol = malloc(sizeof(Symbol));
    Symbol_set_key(new_symbol, _key);
    new_symbol->line = _line;
    new_symbol->nature = _nature;
    new_symbol->type = _type;
    Symbol_set_value(new_symbol, _value);
    Symbol_init_params(new_symbol);

    new_symbol->offset = 0;
    new_symbol->base   = NULL;

    return new_symbol;
}

Symbol *Symbol_destroy(Symbol *_symbol) {
    if (_symbol != NULL) {
        free(_symbol);
    }

    return NULL;
}

bool Symbol_is_null(Symbol *_symbol) {
    return (_symbol == NULL);
}

void Symbol_println(Symbol *_symbol) {
    if (_symbol == NULL) {
        printf("Null\n");
    } 
    else {
        printf("[");
        printf("Key: \"%s\", ", _symbol->key);
        printf("Line: %d, ", _symbol->line);
        printf("Nature: %d, ", _symbol->nature);
        printf("Type: %d, ", _symbol->type);
        printf("Value: \"%s\"", _symbol->value);
        printf("]\n");
    }
}

void Symbol_print(Symbol *_symbol) {
    if (_symbol == NULL) {
        printf("Null");
    } 
    else {
        printf("[");
        printf("Key: \"%s\", ", _symbol->key);
        printf("Line: %d, ", _symbol->line);
        printf("Nature: %d, ", _symbol->nature);
        printf("Type: %d, ", _symbol->type);
        printf("Value: \"%s\"", _symbol->value);
        printf("]");
    }
}

void Symbol_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);
        char key_test[] = "KeyTest";
        char key_test_2[] = "KeyTest2";  
        char value_test[] = "ValueTest"; 

        Symbol *symbol_test = Symbol_create(key_test, 1, 10, 100, value_test);
        Symbol_println(symbol_test);

        printf("Comparing \"%s\" to \"%s\": %s\n", 
            symbol_test->key,
            key_test,
            bool_to_string(Symbol_is_key_equal_to_string(symbol_test, key_test)));
        printf("Comparing \"%s\" to \"%s\": %s\n", 
            symbol_test->key,
            key_test_2,
            bool_to_string(Symbol_is_key_equal_to_string(symbol_test, key_test_2)));
        
        Symbol_set_key(symbol_test, "SymbolAnotherKeyTest");
        Symbol_println(symbol_test);
        printf("Comparing \"%s\" to \"%s\": %s\n", 
            symbol_test->key,
            key_test,
            bool_to_string(Symbol_is_key_equal_to_string(symbol_test, key_test)));
        
        symbol_test = Symbol_destroy(symbol_test);
        printf("Pointer state after destroy operation: %s.\n", is_nullified_string(symbol_test));
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}

void Symbol_init_params(Symbol *s){
    if (!s) return;
    s->param_count = 0;
    for (int i=0;i<SYMBOL_MAX_PARAMS;i++) s->param_types[i] = TYPE_UNTYPED;
}

void Symbol_add_param(Symbol *s, int param_type){
    if (!s) return;
    if (s->param_count < SYMBOL_MAX_PARAMS){
        s->param_types[s->param_count++] = param_type;
    }
}
