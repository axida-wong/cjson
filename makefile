.PHONY: all parser lexer clean lexer_test parser_test
CC = gcc
all: test

ifeq ($(OS), Windows_NT)
RM = del 
EXE = *.exe
else
RM = rm -f
EXE = lexer_test parser_test 
endif

test: test.o cjson.o ./parser/parser.o ./lexer/lexer.o
	$(CC) -o test test.o cjson.o ./parser/parser.o ./lexer/lexer.o 

test.o: test.c
	$(CC) -c test.c

cjson.o: cjson.c 
	$(CC) -c cjson.c

lexer_test:  
	$(MAKE) -C lexer lexer_test

parser_test: 
	$(MAKE) -C parser parser_test

lexer: 
	$(MAKE) -C lexer

parser:
	$(MAKE) -C parser

clean: 
	$(MAKE) -C lexer clean
	$(MAKE) -C parser clean
	$(RM) *.o $(EXE)