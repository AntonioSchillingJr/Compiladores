#include "symbol.h"
#include "symbolnode.h"

// compilation:
// gcc -Wall -g -fsanitize=address main.c utils.c symbol.c symbolnode.c -o etapa4; ./etapa4

int main() {
    
    Symbol_test_implementation();
    SymbolNode_test_implementation();

    return 0;
}