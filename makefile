ALL: cjson.o
LEXER_TEST: lexer_test
PARSER_TEST: parser_test

ifeq ($(OS), Windows_NT)
RM = del 
EXE = *.exe
else
RM = rm -f
EXE = lexer_test parser_test 
endif

cjson.o: cjson.c ./parser/parser.o ./lexer/lexer.o
	gcc -c lexer.c -o cjson.o

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