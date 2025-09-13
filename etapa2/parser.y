/* Grupo S */
/* Antônio Schilling e Arthur Amaral */

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

// "Um programa na linguagem é composto por [...]"
programa: %empty 							// "[...] lista opcional de elementos."
		| lista ';' ;						// "[...] a lista é terminada pelo operador ponto-e-vírgula."
lista: elemento | lista ',' elemento ; 		// "[...] elementos da lista são separados pelo operador vírgula [...]"

// "Cada elemento dessa lista é [...]"
elemento: declaracao_variavel_sem_init 		// "[...] ou uma definição de função [...]"
		| definicao_funcao;					// "[...] ou uma declaração de variável."

// "Definição de Função: Ela possui um cabeçalho e um corpo."  
definicao_funcao: cabecalho_funcao bloco_de_comandos ;

// "Declaração de variável: [...] comando simples de declaração de variável, sendo que a [...] diferença é que [...] não pode receber valores de inicialização."
declaracao_variavel_sem_init: TK_VAR TK_ID TK_ATRIB tipo ;

// Declaração de Variável: Consiste [...]" 
// "[...] no token TK_VAR [...]"
// "[...] seguido do token TK_ID [...]" 
// "[...] seguido do token TK_ATRIB [...]" 
// "[...] seguido do tipo. O tipo pode ser ou o token TK_DECIMAL ou o token TK_INTEIRO." 
declaracao_variavel: declaracao_variavel_sem_init | declaracao_variavel_sem_init TK_COM literal_tipo ;

// "O cabeçalho consiste [...]"
// "[...] no token TK_ID [...]"
// "[...] seguido do token TK_SETA [...]" 
// "[...] seguido ou do token TK_DECIMAL ou do token TK_INTEIRO [...]" 
// "[...] seguido por uma lista opcional de parâmetros [...]"
// "[...] seguido do token TK_ATRIB."
cabecalho_funcao: TK_ID TK_SETA tipo param_opt TK_ATRIB ;

// "A lista de parâmetros, quando presente, consiste [...] 
param_opt: %empty | lista_param | TK_COM lista_param ;	// "[...] no token opcional TK_COM seguido de uma lista [...] de parâmetros."

// "Cada parâmetro consiste [...]" 
// "[...] no token TK_ID [...]"
// "[...] seguido do token TK_ATRIB [...]" 
// "[...] seguido ou do token TK_INTEIRO ou do token TK_DECIMAL."
lista_param: TK_ID TK_ATRIB tipo | lista_param ',' TK_ID TK_ATRIB tipo ;	// "[...] separada por vírgula [...]"

// "Bloco de Comandos: [...] "  
bloco_de_comandos: '[' lista_comandos_opt ']' ;						// "[...] Definido entre colchetes [...] uma sequência [...]"
lista_comandos_opt: %empty | lista_comandos ;						// "[...] possivelmente vazia [...]"
lista_comandos: comando_simples | lista_comandos comando_simples ;	// "[...] de comandos simples."

// "Os comandos simples da linguagem podem ser: [...]" 
comando_simples: bloco_de_comandos		// "[...] bloco de comandos, [...]" 
               | declaracao_variavel	// "[...] declaração de variável, [...]" 
               | atribuicao				// "[...] comando de atribuição, [...]" 
               | chamada_funcao			// "[...] chamada de função, [...]" 
               | retorno				// "[...] comando de retorno, [...]" 
               | comando_se				// "[...] e construções de fluxo de controle."
               | comando_enquanto ;

// "Comando de Atribuição: O comando de atribuição consiste [...]" 
// "[...] em um token TK_ID [...]"
// "[...] seguido do token TK_ATRIB [...]" 
// "[...] seguido por uma expressão.
atribuicao: TK_ID TK_ATRIB expr ;

// "Chamada de Função: Uma chamada de função consiste [...]"
// "[...] no token TK_ID [...]" 
// "[...] seguida de argumentos entre parênteses [...]"
chamada_funcao: TK_ID '(' args ')' ;
args: %empty 			// "[...] pode existir sem argumentos."
    | expr 				// "Um argumento é uma expressão."
	| args ',' expr ;	// "[...] cada argumento é separado do outro por vírgula."

// "Comando de Retorno: Trata-se do [...]
// "[...] token TK_RETORNA [...]"
// "[...] seguido de uma expressão [...]"
// "[...] seguido do token TK_ATRIB [...]"
// "[...] terminado ou pelo token TK_DECIMAL ou pelo token TK_INTEIRO."
retorno: TK_RETORNA expr TK_ATRIB tipo ;

// "Comandos de Controle de Fluxo: A linguagem possui uma construção condicional e uma construção iterativa para controle estruturado de fluxo."
// A condicional consiste [...]"
// "[...] no token TK_SE [...]"
// "[...] seguido de uma expressão entre parênteses [...]"
// "[...] e então por um bloco de comandos obrigatório."
comando_se: TK_SE '(' expr ')' bloco_de_comandos | TK_SE '(' expr ')' bloco_de_comandos TK_SENAO bloco_de_comandos ;

// "[...] construção de repetição é [...]" 
// "[...] o token TK_ENQUANTO [...]"
// "[...] seguido de uma expressão entre parênteses [...]" 
// "[...] e de um bloco de comandos."
comando_enquanto: TK_ENQUANTO '(' expr ')' bloco_de_comandos ;

// "[...] ou do token TK_DECIMAL ou do token TK_INTEIRO [...]"
tipo: TK_DECIMAL | TK_INTEIRO ;

// "Um literal pode ser [...]"
literal_tipo: TK_LI_DECIMAL 	// "[...] ou o token TK_LI_DECIMAL."
			| TK_LI_INTEIRO ;	// "[...] ou o token TK_LI_INTEIRO [...]"

// "Expressões envolvem [...]"
expr: expr_or ;

// "[...] Binários infixados [...]"
expr_or: expr_or '|' expr_and | expr_and ;
expr_and: expr_and '&' expr_eq | expr_eq ;
expr_eq: expr_eq TK_OC_EQ expr_rel | expr_eq TK_OC_NE expr_rel | expr_rel ;
expr_rel: expr_rel '<' expr_add | expr_rel '>' expr_add
        | expr_rel TK_OC_LE expr_add | expr_rel TK_OC_GE expr_add
        | expr_add ;
expr_add: expr_add '+' expr_mul | expr_add '-' expr_mul | expr_mul ;
expr_mul: expr_mul '*' expr_un | expr_mul '/' expr_un | expr_mul '%' expr_un | expr_un ;

// "[...] Unários prefixados [...]"
expr_un: '+' expr_un | '-' expr_un | '!' expr_un | expr_zero ;

// "Os operandos podem ser [...]"
// "[...] identificadores [...]"
// "[...] literais [...]"
// "[...] chamada de função [...]"
// "[...] ou outras expressões [...]"
expr_zero: chamada_funcao | TK_ID | TK_LI_INTEIRO | TK_LI_DECIMAL | '(' expr ')' ;

%%
void yyerror (char const *mensagem) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", get_line_number(), mensagem);
}