#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef lexeri
#define lexeri

#define BUFFER_SIZE 32  //TODO: how to decide this
#define HASH_TABLE_SIZE 96 //TODO: how to decide this

extern int numOfTr, numOfNtr, numOfKeywords;
extern char **terminals;
extern char **nonterminals;
extern char **keywords;
extern FILE * src;
extern int lineNum;
extern char buffer[BUFFER_SIZE];
extern int isHashBuilt;

typedef struct HASHNODE{
    int id;
    char value[30];
    int tag;    // 0=keyword, 1=non keyword terminal, 2 = non terminals
    struct HASHNODE * next;
}hashNode;

typedef struct TOKEN{
    //numeral value; // to store num or rnum
    int lineNum;
    char lexeme[30]; // actual token in source code
    char tokenName[30]; // 
    int tag;   // 0 : Valid Lexeme
               // 1 : Valid Int or floating point number
               // 2 : Valid Floating Point no. with E stored in lexeme
    struct TOKEN *next;
}token;


typedef struct tokens{
    token *head;
    int len;
}tokenStream;


#endif
