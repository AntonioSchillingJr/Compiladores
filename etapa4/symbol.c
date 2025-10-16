#include "symbol.h"


size_t Symbol_string_length(const char *_source) {
    size_t length = 0;
    while ((_source[length] != '\0') && (length < (SYMBOL_MAX_SIZE - 1))) {
        length++;
    }
    return length + 1;
}

void Symbol_set_key(Symbol *_symbol, const char *_source) {
    size_t source_length = Symbol_string_length(_source);
    for (size_t i = 0; i < source_length; i++) {
        _symbol->key[i] = _source[i];
    }

    // padroniza para seguir convencao de string em C, caso
    // source fosse maior que 50 chars (e para garantir uso como "%s")
    _symbol->key[source_length - 1] = '\0';
}

void Symbol_set_value(Symbol *_symbol, const char *_source) {
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

    return new_symbol;
}

Symbol *Symbol_destroy(Symbol *_symbol) {
    if (_symbol != NULL) {
        free(_symbol);
    }

    return NULL;
}

bool Symbol_is_null(Symbol *_symbol) {
    return _symbol == NULL;
}

void Symbol_print(Symbol *_symbol) {
    printf("[");
    printf("Key: \"%s\", ", _symbol->key);
    printf("Line: %d, ", _symbol->line);
    printf("Nature: %d, ", _symbol->nature);
    printf("Type: %d, ", _symbol->type);
    printf("Value: \"%s\"", _symbol->value);
    printf("]\n");
}

void Symbol_test_implementation() {
    printf("--------------- Tests for %s implementation: Running now ---------------\n", __FILE__);
        Symbol *symbol_test = Symbol_create("KeyTest", 1, 10, 100, "ValueTest");
        Symbol_print(symbol_test);
        Symbol_set_key(symbol_test, "SymbolAnotherKeyTest");
        Symbol_print(symbol_test);
        symbol_test = Symbol_destroy(symbol_test);
        printf("%s\n", (Symbol_is_null(symbol_test) ? "Destroyed." : "Not destroyed."));
    printf("--------------- Tests for %s implementation: Ended ---------------\n", __FILE__);
}