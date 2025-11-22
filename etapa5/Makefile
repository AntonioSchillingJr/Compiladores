# Grupo S
# Antônio Schilling e Arthur Amaral

CC      := gcc
LEX     := flex
YACC    := bison

CFLAGS  := -Wall -Werror # -fsanitize=address

EXE := etapa4

OBJS := main.o parser.tab.o lex.yy.o asd.o \
        symbol.o symbolnode.o scope.o scopenode.o scopestackhandler.o \
		semantics.o utils.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Geração do parser
parser.tab.c parser.tab.h: parser.y
	$(YACC) -d -Wall -Wcounterexamples --report=all --report-file=parser.output parser.y

# Geração do scanner
lex.yy.c: scanner.l parser.tab.h
	$(LEX) scanner.l

# Fontes da etapa 3
asd.o: asd.c asd.h
	$(CC) $(CFLAGS) -c $<

# Parser gerado
parser.tab.o: parser.tab.c asd.h scopestackhandler.h semantics.h errors.h
	$(CC) $(CFLAGS) -c $<

# Scanner gerado
lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c parser.tab.h asd.h
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

semantics.o: semantics.c semantics.h errors.h asd.h symbol.h scopestackhandler.h
	$(CC) $(CFLAGS) -c $<

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c $<

# Utils
clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h parser.output $(EXE)

run: $(EXE)
	./$(EXE) < input.txt > saida.dot

.PHONY: all clean run
