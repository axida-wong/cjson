ALL: cjson.o
LEXER_TEST: lexer_test
PARSER_TEST: parser_test

cjson.o: cjson.c ./parser/parser.o ./lexer/lexer.o
	gcc -c cjson.c lexer.o parser.o -o cjson.o

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
	del *.exe