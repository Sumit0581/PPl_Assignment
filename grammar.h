#ifndef grammarh
#define grammarh

//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
#include "lexer.h"

typedef enum{
    terminal,nonTerminal
}symbolType;

typedef struct sym{
    int value;
    symbolType tag;
    struct sym *next;
    struct sym *prev;
}symbolNode;

typedef struct{
    int value;
    symbolNode *front;
    symbolNode *rear;
}grammar;
int searchNonTerminal(char *datavalue);
int searchTerminal(char *datavalue);
grammar* addingRule(char *buffer,int lineno, grammar* g);
grammar* readGrammar(char *inputfile, grammar* g);
void printGrammar(grammar* g);

#endif
