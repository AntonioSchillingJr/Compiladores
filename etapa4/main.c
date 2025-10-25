#include "symbol.h"
#include "symbolnode.h"
#include "scope.h"
#include "scopenode.h"
#include "scopestackhandler.h"

// compilation and execution:
// gcc -Wall -g -fsanitize=address main.c utils.c symbol.c symbolnode.c scope.c scopenode.c scopestackhandler.c -o etapa4; ./etapa4

extern ScopeStackHandler *scope_stack_handler;

int main() {
    
    Symbol_test_implementation();
    SymbolNode_test_implementation();
    Scope_test_implementation();
    Scopenode_test_implementation();
    ScopeStackHandler_test_implementation();

    return 0;
}