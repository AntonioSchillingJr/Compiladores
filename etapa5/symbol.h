#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"

#define SYMBOL_MAX_SIZE 50  // padronizando tamanho maximo dos buffers utilizados
#define SYMBOL_MAX_PARAMS 32

/**
 * @brief uma entrada da "tabela" de simbolos
 */
typedef struct Symbol {
    char key[SYMBOL_MAX_SIZE];
    int line;
    int nature;
    int type;
    char value[SYMBOL_MAX_SIZE];
    int param_count;
    int param_types[SYMBOL_MAX_PARAMS];
    long offset;
    const char *base;
} Symbol;

void Symbol_init_params(Symbol *s);
void Symbol_add_param(Symbol *s, int param_type);
size_t Symbol_string_length(const char *_source);
bool Symbol_is_string_equal_to(const char *_string1, const char *_string2);
bool Symbol_compare_key(Symbol *_symbol1, Symbol *_symbol2);
bool Symbol_is_key_equal_to_string(Symbol *_symbol, const char *_key);
void Symbol_set_key(Symbol *_symbol, const char *_source);
void Symbol_set_value(Symbol *_symbol, const char *_source);
Symbol *Symbol_create(const char *_key, int _line, int _nature, int _type, const char *_value);
Symbol *Symbol_destroy(Symbol *_symbol);
bool Symbol_is_null(Symbol *_symbol);
void Symbol_print(Symbol *_symbol);
void Symbol_println(Symbol *_symbol);
void Symbol_test_implementation();

#endif  // #ifndef SYMBOL_H