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
    int value;	// id number of terminal / ntr
    symbolType tag; // 0=tr, 1=ntr
    struct sym *next;
    struct sym *prev;
}symbolNode;

typedef struct{
    int value;	// id number of the ntr (lhs of a rule)
    symbolNode *front;
    symbolNode *rear;
}grammar;
// end result grammar: g[grammarRules], starting index from 0

extern int grammarRules; // change in grammar.c

int searchNonTerminal(char *datavalue);
int searchTerminal(char *datavalue);
grammar* addingRule(char *buffer,int lineno, grammar* g);
grammar* readGrammar(char *inputfile, grammar* g);
void printGrammar(grammar* g);

#endif
