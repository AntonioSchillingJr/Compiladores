CC = gcc

OBJ = main.o lex.yy.o
EXE = etapa1

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^

# Compila main.c
main.o: main.c tokens.h
	$(CC) -c main.c

# Gerar lex.yy.c a partir do scanner.l
lex.yy.c: scanner.l tokens.h
	flex scanner.l

# Compila lex.yy.c
lex.yy.o: lex.yy.c tokens.h
	$(CC) -c lex.yy.c

clean:
	rm -f $(OBJ) lex.yy.c $(EXE)

.PHONY: all clean
