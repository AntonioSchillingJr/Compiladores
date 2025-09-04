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
elemento: definicao_de_funcao;		// "[...] ou uma definição de função [...]"
elemento: declaracao_de_variavel;	// "[...] ou uma declaração de variável."


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
lista_opcional_de_parametros: /*empty*/;									// definição de lista opcional
lista_opcional_de_parametros: TK_COM lista_nao_vazia_de_parametros;			// "[...] no token opcional TK_COM seguido de uma lista [...] de parâmetros."
lista_nao_vazia_de_parametros: parametro;									// definição de lista
lista_nao_vazia_de_parametros: lista_nao_vazia_de_parametros ',' parametro;	// "[...] separada por vírgula [...]"


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
bloco_de_comandos: '[' sequencia_opcional_de_comandos_simples ']';	// "[...] Definido entre colchetes [...]"
sequencia_opcional_de_comandos_simples: /*empty*/; 					// "[...] uma sequência, possivelmente vazia [...]"
sequencia_opcional_de_comandos_simples: comando_simples;			// "[...] de comandos simples."
sequencia_opcional_de_comandos_simples: sequencia_opcional_de_comandos_simples comando_simples; // DÚVIDA: sem separador entre comandos simples?


// "Os comandos simples da linguagem podem ser: [...]" 
comando_simples: bloco_de_comandos; 			// "[...] bloco de comandos, [...]" 
comando_simples: declaracao_de_variavel; 		// "[...] declaração de variável, [...]" 
comando_simples: comando_de_atribuicao; 		// "[...] comando de atribuição, [...]" 
comando_simples: chamada_de_funcao; 			// "[...] chamada de função, [...]" 
comando_simples: comando_de_retorno; 			// "[...] comando de retorno, [...]" 
comando_simples: comando_de_controle_de_fluxo; 	// "[...] e construções de fluxo de controle."



// "Declaração de variável: Esta declaração é idêntica ao comando simples de declaração de variável, sendo que a única e importante diferença é que esse elemento não pode receber valores de inicialização."
declaracao_de_variavel: ;

// Declaração de Variável: Consiste no token TK_VAR seguido do token TK_ID, que é por sua vez seguido do token TK_ATRIB e enfim seguido do tipo. O tipo pode ser ou o token TK_DECIMAL ou o token TK_INTEIRO. Uma variável pode ser opcionalmente inicializada caso sua declaração seja seguida do token TK_COM e de um literal. 


// "Um literal pode ser [...]"
literal: TK_LI_INTEIRO;		// "[...] ou o token TK_LI_INTEIRO [...]"
literal: TK_LI_DECIMAL;		// "[...] ou o token TK_LI_DECIMAL."


/*
Comando de Atribuição: O comando de atribui-
ção consiste em um token TK_ID, seguido do to-
ken TK_ATRIB e enfim seguido por uma expres-
são.
Chamada de Função: Uma chamada de função
consiste no token TK_ID, seguida de argumentos
entre parênteses, sendo que cada argumento é se-
parado do outro por vírgula. Um argumento é
uma expressão. Uma chamada de função pode
existir sem argumentos.
Comando de Retorno: Trata-se do token
TK_RETORNA seguido de uma expressão, se-
guido do token TK_ATRIB e terminado ou pelo
token TK_DECIMAL ou pelo token TK_INTEIRO.
Comandos de Controle de Fluxo: A lingua-
gem possui uma construção condicional e uma
construção iterativa para controle estruturado de
fluxo. A condicional consiste no token TK_SE se-
guido de uma expressão entre parênteses e en-
tão por um bloco de comandos obrigatório. Após
este bloco, podemos opcionalmente ter o token
TK_SENAO que, quando aparece, é seguido obri-
gatoriamente por um bloco de comandos. Temos
apenas uma construção de repetição que é o to-
ken TK_ENQUANTO seguido de uma expressão en-
tre parênteses e de um bloco de comandos.

Expressões envolvem operandos e operadores,
sendo este opcional. Os operandos podem ser
identificadores, literais e chamada de função ou
outras expressões, podendo portanto ser formadas
recursivamente pelo emprego de operadores. Elas
também permitem o uso de parênteses para forçar
uma associatividade ou precedência diferente da-
quela tradicional. A associatividade é à esquerda
(portanto implemente recursão à esquerda nas re-
gras gramaticais). Os operadores são os seguintes:
• Unários prefixados
– + número positivo
– - inverte o sinal
– ! negação lógica
• Binários infixados
– + soma
– - subtração
– * multiplicação
– / divisão
– % resto da divisão inteira
– operadores compostos
As regras de associatividade e precedência de
operadores matemáticos são aquelas tradicionais
de linguagem de programação e da matemática.
Pode-se usar esta referência da Linguagem C. Para
facilitar, abaixo temos uma tabela com uma vi-
são somente com os operadores de nossa lingua-
gem. Recomenda-se fortemente que tais regras se-
jam incorporadas na solução desta etapa através
de construções gramaticais (evitando totalmente o
emprego das diretivas %left %right do bison).
Precedência Op. Aridade
0 Chamada de Função
TK_ID
TK_LI_INTEIRO
TK_LI_DECIMAL
( expressão )
1 + Unária
- Unária
! Unária
2 * Binária
/ Binária
% Binária
3 + Binária
- Binária
4 < Binária
> Binária
TK_OC_LE Binária
TK_OC_GE Binária
5 TK_OC_EQ Binária
TK_OC_NE Binária
6 & Binária
7 | Binária
*/

%%

// função de mensagem de erro
void yyerror (char const *mensagem) {
	printf("Erro identificado na linha %d: %s\n", get_line_number(), mensagem);
}
