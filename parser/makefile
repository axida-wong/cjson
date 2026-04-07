ALL: parser.o
PARSER_TEST: parser_test

parser_test: test.c parser.o
	gcc test.c parser.o -o parser_test

parser.o: parser.c
	gcc -c parser.c

clean: 
	del *.o *.exe