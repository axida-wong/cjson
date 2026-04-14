ALL: parser.o
CC = gcc
# PARSER_TEST: parser_test

parser_test: test_parser.c parser.o ../lexer/lexer.o
	$(CC) test_parser.c parser.o ../lexer/lexer.o -o parser_test

parser.o: parser.c
	$(CC) -c parser.c

../lexer/lexer.o: ../lexer/lexer.c
	$(MAKE) -C ../lexer 

clean: 
	del *.o *.exe