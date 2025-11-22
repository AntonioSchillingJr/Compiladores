#include "symbol.h"
#include "symbolnode.h"
#include "scope.h"
#include "scopenode.h"
#include "scopestackhandler.h"
#include "list.h"
#include "label.h"
#include "ilocinstruction.h"

/*
compilation:
    gcc 
    -Wall -g -fsanitize=address 
    test_main.c 
    utils.c symbol.c symbolnode.c scope.c scopenode.c scopestackhandler.c list.c label.c ilocinstruction.c -o etapa5;

execution:
    ./etapa5;
*/

int main() {

    Symbol_test_implementation();
    SymbolNode_test_implementation();
    Scope_test_implementation();
    Scopenode_test_implementation();
    ScopeStackHandler_test_implementation();
    List_test_implementation();
    Label_test_implementation();
    ILOCInstruction_test_implementation();

    return 0;
}