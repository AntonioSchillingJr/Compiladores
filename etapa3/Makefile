# Grupo S
# Antônio Schilling e Arthur Amaral

CC      := gcc
LEX     := flex
YACC    := bison
CFLAGS  := -Wall -Wextra -Werror -fsanitize=address

EXE := etapa3

all: $(EXE)

parser.tab.c parser.tab.h: parser.y
	$(YACC) -d -Wall -Wcounterexamples --report=all --report-file=parser.output parser.y

lex.yy.c: scanner.l parser.tab.h
	$(LEX) scanner.l

asd.o: asd.c asd.h
	$(CC) $(CFLAGS) -c $<

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c $<

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -Wno-sign-compare -c $<

main.o: main.c parser.tab.h asd.h
	$(CC) $(CFLAGS) -c $<

$(EXE): main.o parser.tab.o lex.yy.o asd.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o lex.yy.c parser.tab.c parser.tab.h parser.output $(EXE)

run: $(EXE)
	./$(EXE) < input.txt > saida.dot

.PHONY: all clean run
