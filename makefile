main: driver.o lexer.o grammar.o parserFinal.o
	@gcc -o babyCompiler lexer.o grammar.o parserFinal.o driver.o
driver.o: driver.c interface.h
	@gcc -c driver.c
grammar.o: grammar.c grammar.h
	@gcc -c grammar.c
lexer.o: lexer.c lexer.h lexerInterface.h
	@gcc -c lexer.c
parserFinal.o: parserFinal.c parser.h	
	@gcc -c parserFinal.c
clean:
	rm lexer.o driver.o grammar.o parserFinal.o

