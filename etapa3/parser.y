/* Grupo S */
/* Antônio Schilling e Arthur Amaral */

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "asd.h"
  
  extern asd_tree_t *arvore;

  int yylex(void);
  void yyerror (char const *mensagem);
  int get_line_number(void);
%}

%define parse.error verbose

%code requires {
  #include "asd.h"

  typedef struct valor_lexico {
    int   line;
    int   token_kind;
    char  *value;
  } valor_lexico;
}

%code {
  static inline void free_val(valor_lexico* v){
    if (v) { free(v->value); free(v); }
  }
}

%union {
  valor_lexico *valor_lexico; /* TK_ID e literais */
  asd_tree_t     *nodo;       /* nodo da AST para não-terminais */
}

/* Apenas IDs e literais tem valor */
%token <valor_lexico> TK_ID TK_LI_INTEIRO TK_LI_DECIMAL

/* Demais tokens sem valor */
%token TK_TIPO TK_VAR TK_SENAO TK_DECIMAL TK_SE TK_INTEIRO TK_ATRIB TK_RETORNA
%token TK_SETA TK_ENQUANTO TK_COM TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE TK_ER

/* Tipagem de não-terminais que retornam nós */
%type <nodo> programa lista elemento
%type <nodo> definicao_funcao cabecalho_funcao bloco_de_comandos lista_comandos lista_comandos_opt
%type <nodo> comando_simples declaracao_variavel declaracao_variavel_sem_init atribuicao
%type <nodo> chamada_funcao args args_opt
%type <nodo> retorno comando_se comando_enquanto
%type <nodo> expr expr_or expr_and expr_eq expr_rel expr_add expr_mul expr_un expr_zero
%type <nodo> tipo literal_tipo param_opt lista_param

%%

/* Programa: vazio ou lista de elementos */
programa
  : %empty                     { arvore = NULL; $$ = NULL; }
  | lista ';'                  { arvore = $1; $$ = $1; }
  ;

lista
  : elemento                     { $$ = $1; }
  | elemento ',' lista           {
      if ($1) {
        if ($3) asd_add_child($1, $3);
        $$ = $1;
      } else {
        $$ = $3;
      }
    }
  ;

/* Só funções entram na AST; declaração global é ignorada */
elemento
  : definicao_funcao           { $$ = $1; }
  | declaracao_variavel_sem_init { $$ = NULL; }
  ;

/* ---------- Funções ---------- */

definicao_funcao
  : cabecalho_funcao bloco_de_comandos {
      if ($2) asd_add_child($1, $2);
      $$ = $1;
    }
  ;

cabecalho_funcao
  : TK_ID TK_SETA tipo param_opt TK_ATRIB {
      $$ = asd_new($1->value);
      free_val($1);
    }
  ;

tipo
  : TK_DECIMAL                 { $$ = NULL; }
  | TK_INTEIRO                 { $$ = NULL; }
  ;

param_opt
  : %empty                     { $$ = NULL; }
  | TK_COM lista_param         { $$ = NULL; }
  ;

lista_param
  : TK_ID TK_ATRIB tipo                 { free_val($1); $$ = NULL; }
  | lista_param ',' TK_ID TK_ATRIB tipo { free_val($3); $$ = NULL; }
  ;

/* ---------- Bloco e comandos ---------- */

bloco_de_comandos
  : '[' lista_comandos_opt ']' { $$ = $2; }
  ;

lista_comandos_opt
  : %empty                     { $$ = NULL; }
  | lista_comandos             { $$ = $1; }
  ;

lista_comandos
  : comando_simples            { $$ = $1; }
  | comando_simples lista_comandos {
      if ($1){ asd_add_child($1, $2); $$ = $1; }
      else $$ = $2;
    }
  ;

comando_simples
  : bloco_de_comandos          { $$ = $1; }
  | declaracao_variavel        { $$ = $1; }
  | atribuicao                 { $$ = $1; }
  | chamada_funcao             { $$ = $1; }
  | retorno                    { $$ = $1; }
  | comando_se                 { $$ = $1; }
  | comando_enquanto           { $$ = $1; }
  ;

/* ---------- Declarações ---------- */

declaracao_variavel_sem_init
  : TK_VAR TK_ID TK_ATRIB tipo { free_val($2); $$ = NULL; }
  ;

declaracao_variavel
  : declaracao_variavel_sem_init               { $$ = NULL; }
  | TK_VAR TK_ID TK_ATRIB tipo TK_COM literal_tipo {
      $$ = asd_new("com");
      asd_add_child($$, asd_new($2->value));
      asd_add_child($$, $6);
      free_val($2);
    }
  ;

literal_tipo
  : TK_LI_DECIMAL               { $$ = asd_new($1->value); free_val($1); }
  | TK_LI_INTEIRO               { $$ = asd_new($1->value); free_val($1); }
  ;

/* ---------- Atribuição ---------- */

atribuicao
  : TK_ID TK_ATRIB expr         {
      $$ = asd_new(":=");
      asd_add_child($$, asd_new($1->value));
      if ($3) asd_add_child($$, $3);
      free_val($1);
    }
  ;

/* ---------- Chamadas de Função ---------- */

chamada_funcao
  : TK_ID '(' args_opt ')'          {
      char buf[1024]; snprintf(buf, sizeof(buf), "call %s", $1->value);
      $$ = asd_new(buf);
      if ($3) asd_add_child($$, $3);
      free_val($1);
    }
  ;

args_opt
  : %empty          { $$ = NULL; }
  | args            { $$ = $1; }
  ;

args
  : expr            { $$ = $1; }
  | expr ',' args   {
      $$ = $1;
      if ($3) asd_add_child($$, $3);
    }
  ;

/* ---------- Retorno ---------- */

retorno
  : TK_RETORNA expr TK_ATRIB tipo {
      $$ = asd_new("retorna");
      if ($2) asd_add_child($$, $2);
    }
  ;

/* ---------- Controle de fluxo ---------- */

comando_se
  : TK_SE '(' expr ')' bloco_de_comandos {
      $$ = asd_new("se");
      if ($3) asd_add_child($$, $3);
      if ($5) asd_add_child($$, $5);
    }
  | TK_SE '(' expr ')' bloco_de_comandos TK_SENAO bloco_de_comandos {
      $$ = asd_new("se");
      if ($3) asd_add_child($$, $3);
      if ($5) asd_add_child($$, $5);
      if ($7) asd_add_child($$, $7);
    }
  ;

comando_enquanto
  : TK_ENQUANTO '(' expr ')' bloco_de_comandos {
      $$ = asd_new("enquanto");
      if ($3) asd_add_child($$, $3);
      if ($5) asd_add_child($$, $5);
    }
  ;

/* ---------- Expressões ---------- */

expr        : expr_or ;
expr_or     : expr_or '|' expr_and      { $$ = asd_new("|");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_and                  { $$ = $1; } ;
expr_and    : expr_and '&' expr_eq      { $$ = asd_new("&");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_eq                   { $$ = $1; } ;
expr_eq     : expr_eq TK_OC_EQ expr_rel { $$ = asd_new("=="); asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_eq TK_OC_NE expr_rel { $$ = asd_new("!="); asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_rel                  { $$ = $1; } ;
expr_rel    : expr_rel '<' expr_add     { $$ = asd_new("<");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_rel '>' expr_add     { $$ = asd_new(">");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_rel TK_OC_LE expr_add{ $$ = asd_new("<="); asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_rel TK_OC_GE expr_add{ $$ = asd_new(">="); asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_add                  { $$ = $1; } ;
expr_add    : expr_add '+' expr_mul     { $$ = asd_new("+");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_add '-' expr_mul     { $$ = asd_new("-");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_mul                  { $$ = $1; } ;
expr_mul    : expr_mul '*' expr_un      { $$ = asd_new("*");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_mul '/' expr_un      { $$ = asd_new("/");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_mul '%' expr_un      { $$ = asd_new("%");  asd_add_child($$, $1); asd_add_child($$, $3); }
            | expr_un                   { $$ = $1; } ;
expr_un     : '+' expr_un               { $$ = asd_new("+");  asd_add_child($$, $2); }
            | '-' expr_un               { $$ = asd_new("-");  asd_add_child($$, $2); }
            | '!' expr_un               { $$ = asd_new("!");  asd_add_child($$, $2); }
            | expr_zero                 { $$ = $1; } ;
expr_zero   : chamada_funcao            { $$ = $1; }
            | TK_ID                     { $$ = asd_new($1->value); free_val($1); }
            | TK_LI_INTEIRO             { $$ = asd_new($1->value); free_val($1); }
            | TK_LI_DECIMAL             { $$ = asd_new($1->value); free_val($1); }
            | '(' expr ')'              { $$ = $2; } ;
%%

void yyerror (char const *mensagem) {
  fprintf(stderr, "Erro sintático na linha %d: %s\n", get_line_number(), mensagem);
}
