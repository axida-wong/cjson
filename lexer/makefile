ALL: lexer.o
CC = gcc

ifeq ($(OS), Windows_NT)
RM = del
EXE = *.exe
else
RM = rm -f
EXE = lexer_test
endif

# LEXER_TEST: lexer_test

lexer_test: test_lexer.c lexer.o
	$(CC) test_lexer.c lexer.o -o lexer_test

lexer.o: lexer.c
	$(CC) -c lexer.c

clean:
	$(RM) *.o $(EXE)
