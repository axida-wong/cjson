ALL: lexer.o
CC = gcc
# LEXER_TEST: lexer_test

lexer_test: test_lexer.c lexer.o
	$(CC) test_lexer.c lexer.o -o lexer_test

lexer.o: lexer.c
	$(CC) -c lexer.c

clean:
	del *.o *.exe