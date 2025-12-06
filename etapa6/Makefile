# Grupo S
# Antônio Schilling e Arthur Amaral

CC      := gcc
LEX     := flex
YACC    := bison

CFLAGS  := -Wall -Werror -fsanitize=address

EXE := etapa6

OBJS := main.o parser.tab.o lex.yy.o asd.o \
        symbol.o symbolnode.o scope.o scopenode.o scopestackhandler.o \
        semantics.o utils.o \
        iloccode.o ilocinstruction.o list.o label.o \
		codegen.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Geração do parser
parser.tab.c parser.tab.h: parser.y
	$(YACC) -d -Wall -Wcounterexamples --report=all --report-file=parser.output parser.y

# Geração do scanner
lex.yy.c: scanner.l parser.tab.h
	$(LEX) scanner.l

# AST
asd.o: asd.c asd.h
	$(CC) $(CFLAGS) -c $<

# Parser gerado
parser.tab.o: parser.tab.c asd.h scopestackhandler.h semantics.h errors.h iloccode.h label.h
	$(CC) $(CFLAGS) -c $<

# Scanner gerado
lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c $<

# Main
main.o: main.c parser.tab.h asd.h iloccode.h label.h
	$(CC) $(CFLAGS) -c $<

# Símbolos e tabela
symbol.o: symbol.c symbol.h utils.h semantics.h
	$(CC) $(CFLAGS) -c $<

symbolnode.o: symbolnode.c symbolnode.h symbol.h utils.h
	$(CC) $(CFLAGS) -c $<

# Escopos
scope.o: scope.c scope.h symbolnode.h symbol.h utils.h
	$(CC) $(CFLAGS) -c $<

scopenode.o: scopenode.c scopenode.h scope.h symbolnode.h symbol.h utils.h
	$(CC) $(CFLAGS) -c $<

scopestackhandler.o: scopestackhandler.c scopestackhandler.h scopenode.h scope.h symbolnode.h symbol.h utils.h
	$(CC) $(CFLAGS) -c $<

# Semântica
semantics.o: semantics.c semantics.h errors.h asd.h symbol.h scopestackhandler.h
	$(CC) $(CFLAGS) -c $<

# Utils
utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c $<

list.o: list.c list.h
	$(CC) $(CFLAGS) -c $<

# ILOC
iloccode.o: iloccode.c iloccode.h ilocinstruction.h list.h
	$(CC) $(CFLAGS) -c $<

ilocinstruction.o: ilocinstruction.c ilocinstruction.h
	$(CC) $(CFLAGS) -c $<

# Labels
label.o: label.c label.h
	$(CC) $(CFLAGS) -c $<

# Geração de código assembly
codegen.o: codegen.c codegen.h iloccode.h ilocinstruction.h semantics.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h parser.output $(EXE)

run: $(EXE)
	./$(EXE) < input.txt > saida.s

.PHONY: all clean run
