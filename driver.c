//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include "interface.h"

char* sourceFileName = "sourcecode.txt";  //"testcases/t1.txt"; //TODO: use "sourcecode.txt"
char* grammarFile = "grammar.txt";
int numOfRules = 70;
int main(int argc, char* argv[]){
	
	/*
	Option 0: exit
	Option 1: Create parse tree
	Option 2: Traverse the parse tree to construct typeExpressionTable. Also print the type errors while
traversing the parse tree and accessing the typeExpressionTable.
	Option 3: Print parse tree in the specified format
	Option 4: Print typeExpressionTable in the specified format.
	*/
	
	/*
	TODO:ensure that all options are independent of each other. For example, option 2 must first create the parse
tree and then traverse appropriately. Similarly the Option 1 created earlier should be independent from other
options.
	*/
	
	// IN ALL OPTIONS - need to implement lexer(populate tokenStream) , create parse tree, etc 


	int option=-1;
	while(1){
		
		printf("\n\n----------------Welcome to the world of PPL Assignment by Group theta-----\n\n");
		printf("Enter your option 1(Create Tree), 2(Traverse tree to create TypeExp Table), 3(Print Parse Tree), or 4(Print TypeExp Table)\n"); //TODO: print what they do?
		scanf("%d", &option);

		if(option==0){
			printf("Exiting\n");
			break;	
		}
		else if(option==1){
			printf("\nOption 1: Create Parse tree:\n");
			lexerPreReq(sourceFileName); /* in lexer.c */ //hashTable available globally 
			//printHashTable();

			tokenStream *s = (tokenStream *)malloc(sizeof(tokenStream));
			s->head= NULL;
			s->len = 0; //len captures the len of source file as # of lexemes
			tokeniseSourcecode( sourceFileName, s); /* in lexer.c */
			
			// printTokenStream(s);	
			
			/*
			grammar* g = (grammar *)malloc(numOfRules* sizeof(grammar));
			g = readGrammar(grammarFile, g);	// in grammar.c 
			printGrammar(g);
			*/
			
			token *h = s->head;
			
			token *t = createTokenNtr("Start");
			treeNode *root = new_node(t,1, -1);
			
			token *result = program(h,root);
			
			
			//printParseTree(root);
			//printParseTreeFancy(root);
			
			
			//printf("OUTSIDE: %s %s\n", result->tokenName, result->lexeme);
			
			if(!strcmp(result->tokenName,"EOF"))
				printf("\nSourceCode is Accepted..!!!\n");
			else printf("\nSourcecode Rejected..!!!\n");

			//printf("Hello fflush");
			//fflush(stdout);
			
			if(strcmp(result->tokenName,"program")==0) { printf("\n\nThere are some unwanted/unreadable characters in the source code.... \nPARTIAL PARSE TREE CREATED\nTERMINATING\n");}
			else printf("PARSE TREE SUCCESFULLY CREATED");
			continue;
		}
		else if(option==2){
			printf("\nOption 2: Traverse the parse tree to construct typeExpressionTable. Also print the type errors whiletraversing the parse tree and accessing the typeExpressionTable\n");
			
		
			continue;		
		}
		else if(option==3){
			printf("\nOption 3: Print Parse Tree in specified format:\n");
	
			lexerPreReq(sourceFileName); /* in lexer.c */ //hashTable available globally 
			//printHashTable();
			tokenStream *s = (tokenStream *)malloc(sizeof(tokenStream));
			s->head= NULL;
			s->len = 0; //len captures the len of source file as # of lexemes
			tokeniseSourcecode( sourceFileName, s); /* in lexer.c */
			// printTokenStream(s);	
			/*
			grammar* g = (grammar *)malloc(numOfRules* sizeof(grammar));
			g = readGrammar(grammarFile, g);	// in grammar.c 
			printGrammar(g);
			*/
			token *h = s->head;
			token *t = createTokenNtr("Start");
			treeNode *root = new_node(t,1, -1);
			token *result = program(h,root);
			printParseTreeFancy(root);	//TODO remove this?
			printParseTree(root);
			if(!strcmp(result->tokenName,"EOF"))
				printf("\nSourceCode is Accepted..!!!\n");
			else printf("\nSourcecode Rejected..!!!\n");
			if(strcmp(result->tokenName,"program")==0) { printf("\n\nThere are some unwanted/unreadable characters in the source code.... \nPARTIAL PARSE TREE CREATED\nTERMINATING\n");}
			
			continue;		
		}
		else if(option==4){
			printf("Option 4: Print typeExpressionTable in the specified format: \n");
			continue;		
		}
		else {
			printf("Invalid option\n");
		}
	}
	
	
	return 0;
}



