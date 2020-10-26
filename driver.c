//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include "interface.h"

char* sourceFileName = "trash2.txt"; //TODO: use "sourcecode.txt"
char* grammarFile = "grammar.txt";
int numOfRules = 57;
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
		printf("Enter your option 1, 2, 3, or 4\n"); //TODO: print what they do?
		scanf("%d", &option);

		if(option==0){
			printf("Exiting\n");
			break;	
		}
		else if(option==1){
			lexerPreReq(sourceFileName); /* in lexer.c */ //hashTable available globally 
			printHashTable();

			tokenStream *s = (tokenStream *)malloc(sizeof(tokenStream));
			s->head= NULL;
			s->len = 0; //len captures the len of source file as # of lexemes
			tokeniseSourcecode( sourceFileName, s); /* in lexer.c */
			printTokenStream(s);	
			/*
			grammar* g = (grammar *)malloc(numOfRules* sizeof(grammar));
			g = readGrammar(grammarFile, g);	// in grammar.c 
			printGrammar(g);
			*/
			token *h = s->head;
			
			//for(int i=0; i<(s->len); i++){
				//fContainer(h, g);
			//	printToken(h);
			//	h = h->next;
			//}
			token *t = createTokenNtr("Start");
			treeNode *root = new_node(t,1, 99);
			
			//token *result = numberList2(h,root);
			//token *result = numberList3(h,root);
			//token *result = numberList4(h,root);
			//token *result = numberList(h,root);
			//token *result = jaggedInitialisation(h, root);
			//token *result = jaggedInitialisation(h,root);
			//token *result = varType(h,root);
			//token *result = jaggedInitialisationList(h,root);
			//token *result = jaggedInitialisationList2(h,root);

			token *result = jaggedDeclaration(h,root);
			
			//token *result = program(h, root);
			
			if(!strcmp(result->tokenName,"EOF"))
				printf("\nAccepted..!!!\n");
			else printf("\nRejected..!!!\n");

			printf("Hello fflush");
			fflush(stdout);
			
			printParseTree(root);
			
			
			
			continue;
		}
		else if(option==2){
			continue;		
		}
		else if(option==3){
			continue;		
		}
		else if(option==4){
			continue;		
		}
		else {
			printf("Invalid option\n");
		}
	}
	
	
	return 0;
}



