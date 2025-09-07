%{
#include <stdio.h>
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


// Definição da gramática, passo a passo, de acordo com a especificação ---------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Documentação de padrões de gramática utilizados no trabalho:
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		1. "Lista" / "Sequência" de "elementos":
//				lista: elemento;		// lista ou sequência contendo apenas 1 elemento ou final da recursão
//				lista: lista elemento;	// recursão à esquerda dos elementos 
//			- Adição de separador de elementos ou de sinalizador de final de lista a critério da definição (omitido se não especificado).
//
//		2. "Lista" / "Sequência" "opcional" de "elementos":
//				lista: /*empty*/;		// lista ou sequência vazia
//				lista: elemento;		// lista ou sequência contendo apenas 1 elemento ou final da recursão
//				lista: lista elemento;	// recursão à esquerda dos elementos 
//			- Adição de separador de elementos ou de sinalizador de final de lista a critério da definição (omitido se não especificado).
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// "Um programa na linguagem é composto por [...]"
programa: /*empty*/;  									// "[...] lista opcional [...]"
programa: lista_de_elementos ';';						// "[...] de elementos. [...] e a lista é terminada pelo operador ponto-e-vírgula."
lista_de_elementos: elemento; 							// definição de lista 
lista_de_elementos: lista_de_elementos ',' elemento;	// "[...] elementos da lista são separados pelo operador vírgula [...]"


// "Cada elemento dessa lista é [...]"
elemento: definicao_de_funcao;						// "[...] ou uma definição de função [...]"
elemento: declaracao_de_variavel_nao_inicializavel;	// "[...] ou uma declaração de variável."


// "Definição de Função: Ela possui um cabeçalho e um corpo."  
definicao_de_funcao: cabecalho_da_funcao corpo_da_funcao;


// "O cabeçalho consiste [...]"
// "[...] no token TK_ID [...]"
// "[...] seguido do token TK_SETA [...]" 
// "[...] seguido ou do token TK_DECIMAL ou do token TK_INTEIRO [...]" 
// "[...] seguido por uma lista opcional de parâmetros [...]"
// "[...] seguido do token TK_ATRIB."
cabecalho_da_funcao: TK_ID TK_SETA token_decimal_ou_inteiro lista_opcional_de_parametros TK_ATRIB;


// "[...] ou do token TK_DECIMAL ou do token TK_INTEIRO [...]"
token_decimal_ou_inteiro: TK_DECIMAL | TK_INTEIRO;


// "A lista de parâmetros, quando presente, consiste [...] 
lista_opcional_de_parametros: /*empty*/;					// definição de lista opcional
lista_opcional_de_parametros: TK_COM lista_de_parametros;	// "[...] no token opcional TK_COM seguido de uma lista [...] de parâmetros."
lista_de_parametros: parametro;								// definição de lista
lista_de_parametros: lista_de_parametros ',' parametro;		// "[...] separada por vírgula [...]"


// "Cada parâmetro consiste [...]" 
// "[...] no token TK_ID [...]"
// "[...] seguido do token TK_ATRIB [...]" 
// "[...] seguido ou do token TK_INTEIRO ou do token TK_DECIMAL."
parametro: TK_ID TK_ATRIB token_decimal_ou_inteiro;


// "O corpo de uma função é um bloco de comandos"
corpo_da_funcao: bloco_de_comandos;



// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------ ADIÇÕES FEITAS SEM RODAR O BISON PARA TESTAGEM AINDA ------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// "Bloco de Comandos: [...] "  
bloco_de_comandos: '[' sequencia_opcional_de_comandos_simples ']';	// "[...] Definido entre colchetes [...] uma sequência [...]"
sequencia_opcional_de_comandos_simples: /*empty*/; 					// "[...] possivelmente vazia [...]"
sequencia_opcional_de_comandos_simples: comando_simples;			// "[...] de comandos simples."
sequencia_opcional_de_comandos_simples: sequencia_opcional_de_comandos_simples comando_simples; // Dúvida: sem separador entre comandos simples?


// "Os comandos simples da linguagem podem ser: [...]" 
comando_simples: bloco_de_comandos; 			// "[...] bloco de comandos, [...]" 
comando_simples: declaracao_de_variavel; 		// "[...] declaração de variável, [...]" 
comando_simples: comando_de_atribuicao; 		// "[...] comando de atribuição, [...]" 
comando_simples: chamada_de_funcao; 			// "[...] chamada de função, [...]" 
comando_simples: comando_de_retorno; 			// "[...] comando de retorno, [...]" 
comando_simples: comando_de_controle_de_fluxo; 	// "[...] e construções de fluxo de controle."


// "Declaração de variável: [...] comando simples de declaração de variável, sendo que a [...] diferença é que [...] não pode receber valores de inicialização."
declaracao_de_variavel_nao_inicializavel: TK_VAR TK_ID TK_ATRIB token_decimal_ou_inteiro;


// Declaração de Variável: Consiste [...]" 
// "[...] no token TK_VAR [...]"
// "[...] seguido do token TK_ID [...]" 
// "[...] seguido do token TK_ATRIB [...]" 
// "[...] seguido do tipo. O tipo pode ser ou o token TK_DECIMAL ou o token TK_INTEIRO." 
declaracao_de_variavel: TK_VAR TK_ID TK_ATRIB token_decimal_ou_inteiro inicilizacao_opcional_de_variavel;


// "Uma variável pode ser [...] inicializada caso [...]" 
// "[...] seja seguida do token TK_COM [...]"
// "[...] e de um literal. 
inicilizacao_opcional_de_variavel: /*empty*/;
inicilizacao_opcional_de_variavel: TK_COM literal;


// "Um literal pode ser [...]"
literal: TK_LI_INTEIRO;		// "[...] ou o token TK_LI_INTEIRO [...]"
literal: TK_LI_DECIMAL;		// "[...] ou o token TK_LI_DECIMAL."


// "Comando de Atribuição: O comando de atribuição consiste [...]" 
// "[...] em um token TK_ID [...]"
// "[...] seguido do token TK_ATRIB [...]" 
// "[...] seguido por uma expressão.
comando_de_atribuicao: TK_ID TK_ATRIB expressao;


// "Chamada de Função: Uma chamada de função consiste [...]"
// "[...] no token TK_ID [...]" 
// "[...] seguida de argumentos entre parênteses [...]"
chamada_de_funcao: TK_ID '(' sequencia_opcional_de_argumentos ')';
sequencia_opcional_de_argumentos: /*empty*/;										// "[...] pode existir sem argumentos."
sequencia_opcional_de_argumentos: argumento;										// definição de sequência
sequencia_opcional_de_argumentos: sequencia_opcional_de_argumentos ',' argumento;	// "[...] cada argumento é separado do outro por vírgula."
argumento: expressao; 																// "Um argumento é uma expressão."
// Dúvida: definição de argumento desnecessária? Já que "argumento == expressão" sempre


// "Comando de Retorno: Trata-se do [...]
// "[...] token TK_RETORNA [...]"
// "[...] seguido de uma expressão [...]"
// "[...] seguido do token TK_ATRIB [...]"
// "[...] terminado ou pelo token TK_DECIMAL ou pelo token TK_INTEIRO."
comando_de_retorno: TK_RETORNA expressao TK_ATRIB token_decimal_ou_inteiro;


// "Comandos de Controle de Fluxo: A linguagem possui uma construção condicional e uma construção iterativa para controle estruturado de fluxo."
comando_de_controle_de_fluxo: construcao_condicional | construcao_iterativa;


// A condicional consiste [...]"
// "[...] no token TK_SE [...]"
// "[...] seguido de uma expressão entre parênteses [...]"
// "[...] e então por um bloco de comandos obrigatório." 
construcao_condicional: TK_SE '(' expressao ')' bloco_de_comandos senao_opcional;
senao_opcional: /*empty*/;						// "[...] opcionalmente [...]"
senao_opcional: TK_SENAO bloco_de_comandos; 	// "[...] token TK_SENAO [...] seguido [...] por um bloco de comandos."


// "[...] construção de repetição é [...]" 
// "[...] o token TK_ENQUANTO [...]"
// "[...] seguido de uma expressão entre parênteses [...]" 
// "[...] e de um bloco de comandos."
construcao_iterativa: TK_ENQUANTO '(' expressao ')' bloco_de_comandos;


// "Expressões envolvem [...]"
expressao: operador_unario_opcional operando_nao_recursivo;  	// "[...] operadores [...] opcional."
expressao: '(' expressao ')';									// "[...] parênteses para [...] associatividade ou precedência diferente [...]"				
expressao: expressao operador_binario operando_nao_recursivo;	// "A associatividade é à esquerda [...] portanto [...] recursão à esquerda [...]"
// Dúvida: uma sequência de operadores unários como "+-inteiro" é válida?
// Dúvida: uma sequência de operadores binários e unários como "inteiro+-inteiro" é válida?


// "Os operandos podem ser [...]"
// "[...] identificadores [...]"
// "[...] literais [...]"
// "[...] chamada de função [...]"
operando_nao_recursivo: TK_ID | literal | chamada_de_funcao;


// "Os operadores são os seguintes: [...]"
// "[...] Unários prefixados [...]"
operador_unario_opcional: /*empty*/ | '+' | '-' | '!';
// "[...] Binários infixados [...]"
operador_binario: '*' | '/' | '%' | '+' | '-' | '<' | '>' | TK_OC_LE | TK_OC_GE | TK_OC_EQ | TK_OC_NE | '&' | '|';

/* 
	Todas as construções de expressões esperadas pelo programa:
		TK_ID 	
		literal 
		chamada_de_funcao 
		'(' operando ')'	 
		'+' operando
		'-' operando
		'!' operando
		operando '*' operando
		operando '/' operando
		operando '%' operando
		operando '+' operando
		operando '-' operando
		operando '<' operando
		operando '>' operando
		operando TK_OC_LE operando
		operando TK_OC_GE operando
		operando TK_OC_EQ operando
		operando TK_OC_NE operando
		operando '&' operando
		operando '|' operando
	Obs: atualmente, sequência de operadores entre ou antes de operandos não é permitida pela gramática.
*/

// As regras de associatividade e precedência [...] são aquelas tradicionais de linguagem de programação e da matemática.
operador_precedencia_1: '+' | '-' | '!'; 					// unária
operador_por_nivel_de_precedencia_2: '*' | '/' | '%'; 					// binária
operador_por_nivel_de_precedencia_3: '+' | '-' ; 						// binária
operador_por_nivel_de_precedencia_4: '<' | '>' | TK_OC_LE | TK_OC_GE; 	// binária
operador_por_nivel_de_precedencia_5: TK_OC_EQ | TK_OC_NE; 				// binária
operador_por_nivel_de_precedencia_6: '&'; 								// binária
operador_por_nivel_de_precedencia_7: '|'; 								// binária

expressao_precedencia_6:
expressao_precedencia_7: expressao_precedencia_6 | expressao_precedencia_6 '|' expressao_precedencia_6;


Expression : Term;
Expression : Expression ('+' | '-') Term;

Term : Factor;
Term : Term ('*' | '/') Factor;

expressao_precedencia_7 : expressao_precedencia_0 | expressao_precedencia_0 operador_precedencia_7 expressao_precedencia_0;
expressao_precedencia_6 : expressao_precedencia_7 | expressao_precedencia_7 operador_precedencia_6 expressao_precedencia_7;
expressao_precedencia_5 : expressao_precedencia_6 | expressao_precedencia_6 operador_precedencia_5 expressao_precedencia_6;
expressao_precedencia_4 : expressao_precedencia_5 | expressao_precedencia_5 operador_precedencia_4 expressao_precedencia_5;
expressao_precedencia_3 : expressao_precedencia_4 | expressao_precedencia_4 operador_precedencia_3 expressao_precedencia_4;

expressao_precedencia_2 : expressao_precedencia_3 | expressao_precedencia_2 operador_precedencia_2 expressao_precedencia_3;

expressao_precedencia_1 : expressao_precedencia_2 | operador_precedencia_1 expressao_precedencia_2;	// não aceita 2 operadores unários seguidos

expressao_precedencia_0 : operando_nao_recursivo  | expressao_precedencia_1 | '(' expressao_precedencia_0 ')';


%%

// função de mensagem de erro
void yyerror (char const *mensagem) {
	printf("Erro identificado na linha %d: %s\n", get_line_number(), mensagem);
}
