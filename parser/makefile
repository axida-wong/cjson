ALL: parser.o
PARSER_TEST: parser_test

parser_test: test.c parser.o ../lexer/lexer.o
	gcc test.c parser.o ../lexer/lexer.o -o parser_test

parser.o: parser.c
	gcc -c parser.c

../lexer/lexer.o: ../lexer/lexer.c
	$(MAKE) -C ../lexer 

clean: 
	del *.o *.exe