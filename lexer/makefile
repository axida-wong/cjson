ALL: lexer.o
# LEXER_TEST: lexer_test

lexer_test: test_lexer.c lexer.o
	gcc test_lexer.c lexer.o -o lexer_test

lexer.o: lexer.c
	gcc -c lexer.c

clean:
	del *.o *.exe