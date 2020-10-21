main: driver.o lexer.o
	@gcc -o babyCompiler lexer.o driver.o
driver.o: driver.c interface.h
	@gcc -c driver.c
lexer.o: lexer.c lexer.h lexerInterface.h
	@gcc -c lexer.c
clean:
	rm lexer.o driver.o

