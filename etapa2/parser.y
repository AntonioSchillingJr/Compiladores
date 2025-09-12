
%{
#include<stdio.h>
int yylex(void);
void yyerror (char const *mensagem);
int get_line_number(void);
%}

%define parse.error verbose

%token TK_TIPO
%token TK_VAR
%token TK_SENAO
%token TK_DECIMAL
%token TK_SE
%token TK_INTEIRO
%token TK_ATRIB
%token TK_RETORNA
%token TK_SETA
%token TK_ENQUANTO
%token TK_COM
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_ID
%token TK_LI_INTEIRO
%token TK_LI_DECIMAL
%token TK_ER

%%

programa: %empty | lista ';' ;
lista: elemento | lista ',' elemento ;
elemento: declaracao_variavel_sem_init | definicao_funcao;
declaracao_variavel_sem_init: TK_VAR TK_ID TK_ATRIB tipo ;
declaracao_variavel: declaracao_variavel_sem_init | declaracao_variavel_sem_init TK_COM literal_tipo ;
cabecalho_funcao: TK_ID TK_SETA tipo param_opt TK_ATRIB ;
param_opt: %empty | TK_COM lista_param ;
lista_param: TK_ID TK_ATRIB tipo | lista_param ',' TK_ID TK_ATRIB tipo ;
bloco_de_comandos: '[' lista_comandos_opt ']' ;
lista_comandos_opt: %empty | lista_comandos ;
lista_comandos: comando_simples | lista_comandos comando_simples ;
comando_simples: bloco_de_comandos
               | declaracao_variavel
               | atribuicao
               | chamada_funcao
               | retorno
               | comando_se
               | comando_enquanto ;
definicao_funcao: cabecalho_funcao bloco_de_comandos ;
atribuicao: TK_ID TK_ATRIB expr ;
chamada_funcao: TK_ID '(' args ')' ;
args: %empty | expr | args ',' expr ;
retorno: TK_RETORNA expr TK_ATRIB tipo ;
comando_se: TK_SE '(' expr ')' bloco_de_comandos | TK_SE '(' expr ')' bloco_de_comandos TK_SENAO bloco_de_comandos ;
comando_enquanto: TK_ENQUANTO '(' expr ')' bloco_de_comandos ;
tipo: TK_DECIMAL | TK_INTEIRO ;
literal_tipo: TK_LI_DECIMAL | TK_LI_INTEIRO ;
expr: expr_or ;
expr_or: expr_or '|' expr_and | expr_and ;
expr_and: expr_and '&' expr_eq | expr_eq ;
expr_eq: expr_eq TK_OC_EQ expr_rel | expr_eq TK_OC_NE expr_rel | expr_rel ;
expr_rel: expr_rel '<' expr_add | expr_rel '>' expr_add
        | expr_rel TK_OC_LE expr_add | expr_rel TK_OC_GE expr_add
        | expr_add ;
expr_add: expr_add '+' expr_mul | expr_add '-' expr_mul | expr_mul ;
expr_mul: expr_mul '*' expr_un | expr_mul '/' expr_un | expr_mul '%' expr_un | expr_un ;
expr_un: '+' expr_un | '-' expr_un | '!' expr_un | expr_zero ;
expr_zero: chamada_funcao | TK_ID | TK_LI_INTEIRO | TK_LI_DECIMAL | '(' expr ')' ;

%%
void yyerror (char const *mensagem) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", get_line_number(), mensagem);
}