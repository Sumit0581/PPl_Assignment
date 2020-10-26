main: driver.o lexer.o grammar.o parser.o
	@gcc -o babyCompiler lexer.o grammar.o parser.o driver.o
driver.o: driver.c interface.h
	@gcc -c driver.c
grammar.o: grammar.c grammar.h
	@gcc -c grammar.c
lexer.o: lexer.c lexer.h lexerInterface.h
	@gcc -c lexer.c
parser.o: parser.c parser.h	
	@gcc -c parser.c
clean:
	rm lexer.o driver.o grammar.o parser.o parser.o

