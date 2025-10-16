#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SYMBOL_MAX_SIZE 50  // padronizando tamanho maximo dos buffers utilizados

/**
 * @brief uma entrada da "tabela" de simbolos
 */
typedef struct Symbol {
    char key[SYMBOL_MAX_SIZE];
    int line;
    int nature;
    int type;
    char value[SYMBOL_MAX_SIZE]; 
} Symbol;

size_t Symbol_string_length(const char *_source);
void Symbol_set_key(Symbol *_symbol, const char *_source);
void Symbol_set_value(Symbol *_symbol, const char *_source);
Symbol *Symbol_create(const char *_key, int _line, int _nature, int _type, const char *_value);
Symbol *Symbol_destroy(Symbol *_symbol);
bool Symbol_is_null(Symbol *_symbol);
void Symbol_print(Symbol *_symbol);
void Symbol_test_implementation();

#endif  // #ifndef SYMBOL_H