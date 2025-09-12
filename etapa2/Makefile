CC := gcc
LEX := flex
YACC := bison
CFLAGS := -Wall -Werror

EXE := etapa2

all: $(EXE)

# Gera parser.tab.c e parser.tab.h com relatório detalhado em parser.output
parser.tab.c parser.tab.h: parser.y
	$(YACC) -d --report=all --report-file=parser.output parser.y

# Gera o scanner (usa parser.tab.h pelos tokens)
lex.yy.c: scanner.l parser.tab.h
	$(LEX) scanner.l

# Compilação
parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c $<

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $<

main.o: main.c parser.tab.h
	$(CC) $(CFLAGS) -c $<

$(EXE): main.o parser.tab.o lex.yy.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h parser.output $(EXE)

run: $(EXE)
	./$(EXE) < input.txt

.PHONY: all clean run