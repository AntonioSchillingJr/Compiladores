/* Grupo S */
/* Antônio Schilling e Arthur Amaral */

%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>

static valor_lexico_t* mk_val(int line, int kind, const char* yytext) {
    valor_lexico_t* v = (valor_lexico_t*)calloc(1, sizeof(valor_lexico_t));
    v->line = line;
    v->token_kind = kind;
    v->value = strdup(yytext); 
    return v;
}
%}

%option noyywrap
%option yylineno
%option noinput nounput

%%
 /* Comentários Ignorados */

\/\/.* { }

 /* Espaços, Tabs e Quebras de Linha Ignorados */

[ \n\r\t] { }

 /* Palavras Reservadas da Linguagem */

tipo { return TK_TIPO; }
var { return TK_VAR; }
senao { return TK_SENAO; }
decimal { return TK_DECIMAL; }
se { return TK_SE; }
inteiro { return TK_INTEIRO; }
retorna { return TK_RETORNA; }
enquanto { return TK_ENQUANTO; }
com { return TK_COM; }

 /* Caracteres Especiais */

[!*/%+\-<>\[\]\(\)=,;&|] { return yytext[0]; }

 /* Operadores Compostos */

\<= { return TK_OC_LE; }
\>= { return TK_OC_GE; }
== { return TK_OC_EQ; }
!= { return TK_OC_NE; }
:= { return TK_ATRIB; }
-> { return TK_SETA; }

 /* Identificadores = 1 */

[a-z_][a-z0-9_]* { yylval.valor_lexico = mk_val(yylineno, 1, yytext); return TK_ID; }

 /* Literais = 2 */

[0-9]+ { yylval.valor_lexico = mk_val(yylineno, 2, yytext); return TK_LI_INTEIRO; }
[0-9]*\.[0-9]+ { yylval.valor_lexico = mk_val(yylineno, 2, yytext); return TK_LI_DECIMAL; }

 /* Token de Erro */

. { return TK_ER; }

%%

int get_line_number(void) {
    return yylineno;
}