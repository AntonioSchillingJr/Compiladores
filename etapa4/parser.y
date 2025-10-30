/* Grupo S */
/* Antônio Schilling e Arthur Amaral */

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "asd.h"
  
  /* Raiz global da AST */
  extern asd_tree_t *arvore;

  int yylex(void);
  void yyerror (char const *mensagem);
  int get_line_number(void);
%}

%define parse.error verbose

%code requires {
  #include "asd.h"
  #include "semantics.h"
  #include "scopestackhandler.h"
  #include "errors.h"

  typedef struct valor_lexico {
    int   line;
    int   token_kind;
    char  *value;
  } valor_lexico_t;
}

%code {
  static inline void free_val(valor_lexico_t* v){
    if (v) { free(v->value); free(v); }
  }
}

%union {
  valor_lexico_t *valor_lexico; /* TK_ID e literais */
  asd_tree_t     *nodo;         /* nodo da AST para não-terminais */
  int             tipo;         /* tipo escalar (TYPE_*) */
}

/* Apenas IDs e literais tem valor */
%token <valor_lexico> TK_ID TK_LI_INTEIRO TK_LI_DECIMAL

/* Demais tokens sem valor */
%token TK_TIPO TK_VAR TK_SENAO TK_DECIMAL TK_SE TK_INTEIRO TK_ATRIB TK_RETORNA
%token TK_SETA TK_ENQUANTO TK_COM TK_OC_LE TK_OC_GE TK_OC_EQ TK_OC_NE TK_ER

/* Tipagem de não-terminais que retornam nodos */
%type <nodo> programa lista elemento
%type <nodo> definicao_funcao cabecalho_funcao bloco_de_comandos lista_comandos lista_comandos_opt
%type <nodo> comando_simples declaracao_variavel declaracao_variavel_sem_init atribuicao
%type <nodo> chamada_funcao args args_opt
%type <nodo> retorno comando_se comando_enquanto
%type <nodo> expr expr_or expr_and expr_eq expr_rel expr_add expr_mul expr_un expr_zero
%type <nodo> literal_tipo param_opt lista_param

/* não-terminais que devolvem tipo */
%type <tipo> tipo
%%

/* Programa: vazio ou lista de elementos */
programa
  : __start_scopes lista ';' __end_scopes { arvore = $2; $$ = $2; }
  | %empty                                { arvore = NULL; $$ = NULL; }
  ;

__start_scopes
  : %empty { ScopeStackHandler_start(); }
  ;

__end_scopes
  : %empty { ScopeStackHandler_end(); }
  ;

/* Lista: função aponta para a próxima como filho extra */
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
      ScopeStackHandler_pop_scope();   /* fecha escopo da função */
      SEM_current_function = NULL;
      SEM_current_function_type = TYPE_UNTYPED;
      $$ = $1;
    }
  ;


cabecalho_funcao
  : TK_ID TK_SETA tipo
    {
      /* cria/checa símbolo de função no escopo global e abre escopo da função (para parâmetros) */
      Symbol *prev = ScopeStackHandler_get_symbol_by_key($1->value);
      if (prev) {
        SEM_error(ERR_DECLARED,
          "identificador '%s' já declarado (linha %d)",
          $1->value, prev->line);
      }

      Symbol *f = Symbol_create($1->value, $1->line, NATURE_FUNC, $3, "");
      Symbol_init_params(f);
      ScopeStackHandler_add_to_global_scope(f);

      SEM_current_function_type = $3;
      SEM_current_function = f;

      /* escopo para parâmetros e corpo da função */
      ScopeStackHandler_push_empty_scope();
    }
    param_opt TK_ATRIB
    {
      $$ = asd_new($1->value);
      free_val($1);
    }
  ;

/* Tipos não geram nodos */
tipo
  : TK_DECIMAL { $$ = TYPE_FLOAT; }
  | TK_INTEIRO { $$ = TYPE_INT; }
  ;

/* Parametros opcionais não geram nodos */
param_opt
  : %empty              { $$ = 0; }
  | lista_param         { $$ = 0; }
  | TK_COM lista_param  { $$ = 0; }
  ;

/* Lista de parâmetros: ignoramos na AST, só damos free na memória */
lista_param
  : TK_ID TK_ATRIB tipo {
      Symbol *prev = ScopeStackHandler_get_symbol_by_key($1->value);
      if (prev) {
        SEM_error(ERR_DECLARED,
          "parâmetro '%s' já declarado (linha %d)",
          $1->value, prev->line);
      }
      Symbol *p = Symbol_create($1->value, $1->line, NATURE_VAR, $3, "");
      ScopeStackHandler_add_symbol_to_current_scope(p);

      if (SEM_current_function) Symbol_add_param(SEM_current_function, $3);

      free_val($1);
      $$ = NULL;
    }
  | lista_param ',' TK_ID TK_ATRIB tipo {
      Symbol *prev = ScopeStackHandler_get_symbol_by_key($3->value);
      if (prev) {
        SEM_error(ERR_DECLARED,
          "parâmetro '%s' já declarado (linha %d)",
          $3->value, prev->line);
      }
      Symbol *p = Symbol_create($3->value, $3->line, NATURE_VAR, $5, "");
      ScopeStackHandler_add_symbol_to_current_scope(p);

      if (SEM_current_function) Symbol_add_param(SEM_current_function, $5);

      free_val($3);
      $$ = NULL;
    }
  ;

/* ---------- Bloco e comandos ---------- */

bloco_de_comandos
  : '[' { ScopeStackHandler_push_empty_scope(); }
    lista_comandos_opt
    ']' { $$ = $3; ScopeStackHandler_pop_scope(); }
  ;

lista_comandos_opt
  : %empty                     { $$ = NULL; }
  | lista_comandos             { $$ = $1; }
  ;

/* Lista de comandos: comando -> próximo comando */
lista_comandos
  : comando_simples            { $$ = $1; }
  | comando_simples lista_comandos {
      if ($1){ if ($2) asd_add_child($1, $2); $$ = $1; }
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

/* Declaração global não entra na AST, só damos free na memória */
declaracao_variavel_sem_init
  : TK_VAR TK_ID TK_ATRIB tipo {
      Symbol *prev = ScopeStackHandler_get_symbol_by_key($2->value);
      if (prev) {
        SEM_error(ERR_DECLARED,
          "identificador '%s' já declarado (linha %d)",
          $2->value, prev->line);
      }
      Symbol *s = Symbol_create($2->value, $2->line, NATURE_VAR, $4, "");
      ScopeStackHandler_add_symbol_to_current_scope(s);
      free_val($2);
      $$ = NULL;
    }
  ;

/* Declaração local com inicialização */
declaracao_variavel
  : declaracao_variavel_sem_init { $$ = NULL; }
  | TK_VAR TK_ID TK_ATRIB tipo TK_COM literal_tipo {
      Symbol *prev = ScopeStackHandler_get_symbol_by_key($2->value);
      if (prev) {
        SEM_error(ERR_DECLARED,
          "identificador '%s' já declarado (linha %d)",
          $2->value, prev->line);
      }
      Symbol *s = Symbol_create($2->value, $2->line, NATURE_VAR, $4, "");
      ScopeStackHandler_add_symbol_to_current_scope(s);
      $$ = asd_new("com");
      asd_add_child($$, asd_new($2->value));
      asd_add_child($$, $6);
      if ($6->dtype != $4)
        SEM_error(ERR_WRONG_TYPE, "inicialização incompatível: '%s' é %s e literal é %s",
          $2->value, ($4==TYPE_INT?"inteiro":"decimal"), ($6->dtype==TYPE_INT?"inteiro":"decimal"));
      $$->dtype = $4;
      free_val($2);
    }
  ;

/* Literais -> nodo folha com o lexema */
literal_tipo
  : TK_LI_DECIMAL { $$ = set_dtype(asd_new($1->value), TYPE_FLOAT); free_val($1); }
  | TK_LI_INTEIRO { $$ = set_dtype(asd_new($1->value), TYPE_INT);   free_val($1); }
  ;

/* ---------- Atribuição ---------- */

atribuicao
  : TK_ID TK_ATRIB expr {
      Symbol *s = ScopeStackHandler_get_symbol_by_key($1->value);
      if (!s) SEM_error(ERR_UNDECLARED, "identificador '%s' não declarado", $1->value);
      if (s->nature != NATURE_VAR) SEM_error(ERR_FUNCTION, "função '%s' usada como variável", $1->value);
      $$ = asd_new(":=");
      asd_add_child($$, asd_new($1->value));
      if ($3) asd_add_child($$, $3);
      if ($3 && $3->dtype != s->type)
        SEM_error(ERR_WRONG_TYPE, "atribuição incompatível: variável '%s' é %s e expressão é %s",
          $1->value, s->type==TYPE_INT?"inteiro":"decimal", $3->dtype==TYPE_INT?"inteiro":"decimal");
      $$->dtype = s->type;
      free_val($1);
    }
  ;


/* ---------- Chamadas de Função ---------- */

chamada_funcao
  : TK_ID '(' { SEM_args_begin(); } args_opt ')' {
      Symbol *s = ScopeStackHandler_get_symbol_by_key($1->value);
      if (!s) SEM_error(ERR_UNDECLARED, "função '%s' não declarada", $1->value);
      if (s->nature != NATURE_FUNC)
        SEM_error(ERR_VARIABLE, "variável '%s' usada como função", $1->value);

      char buf[1024]; snprintf(buf, sizeof(buf), "call %s", $1->value);
      $$ = asd_new(buf);
      if ($4) asd_add_child($$, $4);
      $$->dtype = s->type;

      int got = SEM_args_count();
      if (got > s->param_count) SEM_error(ERR_EXCESS_ARGS, "excesso de argumentos na chamada de '%s'", $1->value);
      if (got < s->param_count) SEM_error(ERR_MISSING_ARGS, "faltam argumentos na chamada de '%s'", $1->value);
      for (int i = 0; i < got; i++) {
        int argt = SEM_args_type_at(got - 1 - i);
        if (argt != s->param_types[i])
          SEM_error(ERR_WRONG_TYPE_ARGS, "tipo do argumento %d incompatível em '%s'", i+1, $1->value);
      }

      SEM_args_end();
      free_val($1);
    }
  ;

args_opt
  : %empty          { $$ = NULL; }
  | args            { $$ = $1; }
  ;

/* Lista de argumentos */
args
  : expr            { $$ = $1; SEM_args_push($1 ? $1->dtype : TYPE_UNTYPED); }
  | expr ',' args   {
      if ($1){ $$ = $1; if ($3) asd_add_child($$, $3); } /* cabeça é o 1º argumento e encadeia o resto como filho */
      else $$ = $3;
      SEM_args_push($1 ? $1->dtype : TYPE_UNTYPED);
    }
  ;

/* ---------- Retorno ---------- */

retorno
  : TK_RETORNA expr TK_ATRIB tipo {
      $$ = asd_new("retorna");
      if ($2) asd_add_child($$, $2);

      // anotação vs tipo da função
      if ($4 != SEM_current_function_type)
        SEM_error(ERR_WRONG_TYPE,
          "retorno anotado (%s) difere do tipo da função (%s)",
          ($4==TYPE_INT ? "inteiro" : "decimal"),
          (SEM_current_function_type==TYPE_INT ? "inteiro" : "decimal"));

      // expressão vs tipo da função
      if ($2 && $2->dtype != SEM_current_function_type)
        SEM_error(ERR_WRONG_TYPE,
          "retorno incompatível: função retorna %s e expressão é %s",
          (SEM_current_function_type==TYPE_INT ? "inteiro" : "decimal"),
          ($2->dtype==TYPE_INT ? "inteiro" : "decimal"));

      $$->dtype = $2 ? $2->dtype : TYPE_UNTYPED;
    }
  ;

/* ---------- Controle de fluxo ---------- */

comando_se
  : TK_SE '(' expr ')' bloco_de_comandos {
      $$ = asd_new("se");
      if ($3) asd_add_child($$, $3);
      if ($5) asd_add_child($$, $5);
      $$->dtype = $3?$3->dtype:TYPE_UNTYPED;
    }
  | TK_SE '(' expr ')' bloco_de_comandos TK_SENAO bloco_de_comandos {
      $$ = asd_new("se");
      if ($3) asd_add_child($$, $3);
      if ($5) asd_add_child($$, $5);
      if ($7) asd_add_child($$, $7);
      $$->dtype = $3?$3->dtype:TYPE_UNTYPED;

      int t_if   = $5 ? $5->dtype : TYPE_UNTYPED;
      int t_else = $7 ? $7->dtype : TYPE_UNTYPED;
      int t = promote_bin(t_if, t_else);
      if (t < 0) {
        SEM_error(ERR_WRONG_TYPE,
          "blocos do if e do else com tipos incompatíveis (%s vs %s)",
          (t_if==TYPE_INT?"inteiro":t_if==TYPE_FLOAT?"decimal":"<sem tipo>"),
          (t_else==TYPE_INT?"inteiro":t_else==TYPE_FLOAT?"decimal":"<sem tipo>")
        );
      }
    }
  ;

comando_enquanto
  : TK_ENQUANTO '(' expr ')' bloco_de_comandos {
      $$ = asd_new("enquanto");
      if ($3) asd_add_child($$, $3);
      if ($5) asd_add_child($$, $5);
      $$->dtype = $3?$3->dtype:TYPE_UNTYPED;
    }
  ;

/* ---------- Expressões (obedecendo associatividade e precedência) ---------- */

expr        : expr_or { $$ = $1; } ;

/* Operadores binários com pelo menos 2 filhos */
expr_or
  : expr_or '|' expr_and {
      int t = promote_bin($1 ? $1->dtype : TYPE_UNTYPED,
                          $3 ? $3->dtype : TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "operador '|' com tipos incompatíveis");
      $$ = asd_new("|");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      $$->dtype = t;
    }
  | expr_and { $$ = $1; }
  ;
expr_and
  : expr_and '&' expr_eq {
      int t = promote_bin($1 ? $1->dtype : TYPE_UNTYPED,
                          $3 ? $3->dtype : TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "operador '&' com tipos incompatíveis");
      $$ = asd_new("&");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      $$->dtype = t;
    }
  | expr_eq { $$ = $1; }
  ;
expr_eq
  : expr_eq TK_OC_EQ expr_rel {
      $$ = asd_new("==");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '=='");
      $$->dtype = t;
    }
  | expr_eq TK_OC_NE expr_rel {
      $$ = asd_new("!=");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '!='");
      $$->dtype = t;
    }
  | expr_rel             { $$ = $1; }
  ;
expr_rel
  : expr_rel '<' expr_add {
      $$ = asd_new("<");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '<'");
      $$->dtype = t;
    }
  | expr_rel '>' expr_add {
      $$ = asd_new(">");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '>'");
      $$->dtype = t;
    }
  | expr_rel TK_OC_LE expr_add {
      $$ = asd_new("<=");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '<='");
      $$->dtype = t;
    }
  | expr_rel TK_OC_GE expr_add {
      $$ = asd_new(">=");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '>='");
      $$->dtype = t;
    }
  | expr_add             { $$ = $1; }
  ;
expr_add
  : expr_add '+' expr_mul {
      $$ = asd_new("+");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '+'");
      $$->dtype = t;
    }
  | expr_add '-' expr_mul {
      $$ = asd_new("-");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '-'");
      $$->dtype = t;
    }
  | expr_mul             { $$ = $1; }
  ;
expr_mul
  : expr_mul '*' expr_un {
      $$ = asd_new("*");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '*'");
      $$->dtype = t;
    }
  | expr_mul '/' expr_un {
      $$ = asd_new("/");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '/'");
      $$->dtype = t;
    }
  | expr_mul '%' expr_un {
      $$ = asd_new("%");
      if ($1) asd_add_child($$, $1);
      if ($3) asd_add_child($$, $3);
      int t = promote_bin($1?$1->dtype:TYPE_UNTYPED, $3?$3->dtype:TYPE_UNTYPED);
      if (t < 0) SEM_error(ERR_WRONG_TYPE, "tipos incompatíveis na operação '%'");
      $$->dtype = t;
    }
  | expr_un              { $$ = $1; }
  ;

/* Operadores unários com pelo menos 1 filho */
expr_un
  : '+' expr_un { $$ = asd_new("+"); if ($2) asd_add_child($$, $2); $$->dtype = $2 ? $2->dtype : TYPE_UNTYPED; }
  | '-' expr_un { $$ = asd_new("-"); if ($2) asd_add_child($$, $2); $$->dtype = $2 ? $2->dtype : TYPE_UNTYPED; }
  | '!' expr_un { $$ = asd_new("!"); if ($2) asd_add_child($$, $2); $$->dtype = $2 ? $2->dtype : TYPE_UNTYPED; }
  | expr_zero   { $$ = $1; }
  ;

expr_zero
  : chamada_funcao            { $$ = $1; }
  | TK_ID {
      Symbol *s = ScopeStackHandler_get_symbol_by_key($1->value);
      if (!s) SEM_error(ERR_UNDECLARED, "identificador '%s' não declarado", $1->value);
      if (s->nature == NATURE_FUNC)
        SEM_error(ERR_FUNCTION, "função '%s' usada como variável", $1->value);
      $$ = asd_new($1->value);
      $$->dtype = s->type;
      free_val($1);
    }
  | TK_LI_INTEIRO { $$ = set_dtype(asd_new($1->value), TYPE_INT); free_val($1); }
  | TK_LI_DECIMAL { $$ = set_dtype(asd_new($1->value), TYPE_FLOAT); free_val($1); }
  | '(' expr ')'  { $$ = $2; }
  ;

%%

void yyerror (char const *mensagem) {
  fprintf(stderr, "Erro sintático na linha %d: %s\n", get_line_number(), mensagem);
}
