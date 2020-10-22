#include "addgrammar.h"
int Rules=57;
int main(){
	//printf("AAAAA\n");
	grammar* g = (grammar*)malloc(Rules*sizeof(grammar));
	g=readGrammar("All_Grammar_Rules.txt",g);
	printGrammar(g);
	return 0;
}