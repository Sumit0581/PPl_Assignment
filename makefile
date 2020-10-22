main: driver.o lexer.o grammar.o
	@gcc -o babyCompiler lexer.o grammar.o driver.o
driver.o: driver.c interface.h
	@gcc -c driver.c
grammar.o: grammar.c grammar.h
	@gcc -c grammar.c
lexer.o: lexer.c lexer.h lexerInterface.h
	@gcc -c lexer.c
clean:
	rm lexer.o driver.o

