/*
Função principal para realização da análise sintática.
*/
#include <stdio.h>
#include "parser.tab.h" //arquivo gerado com bison -d parser.y
                        //inclua tal comando no teu workflow (Makefile)
extern int yylex_destroy(void);

int main (int argc, char **argv)
{
  int ret = yyparse();
  yylex_destroy();
  return ret;
}

// compilação:
// bison -d parser.y; flex scanner.l; gcc -c main.c; gcc -c parser.tab.c; gcc -c lex.yy.c; gcc -o e2 main.o parser.tab.o lex.yy.o;
// executar ($? == valor de retorno do programa):
// ./e2 < input.txt; echo $?;
