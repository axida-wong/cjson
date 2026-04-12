ALL: test
LEXER_TEST: lexer_test
PARSER_TEST: parser_test

ifeq ($(OS), Windows_NT)
RM = del 
EXE = *.exe
else
RM = rm -f
EXE = lexer_test parser_test 
endif

test: test.c cjson.o ./parser/parser.o ./lexer/lexer.o 
	gcc -o test test.c cjson.o ./parser/parser.o ./lexer/lexer.o 

cjson.o: cjson.c 
	gcc -c cjson.c

lexer_test:  
	$(MAKE) -C lexer lexer_test

parser_test: 
	$(MAKE) -C parser parser_test

./lexer/lexer.o: 
	$(MAKE) -C lexer

./parser/parser.o: 
	$(MAKE) -C parser

clean: 
	$(MAKE) -C lexer clean
	$(MAKE) -C parser clean
	$(RM) *.o $(EXE)