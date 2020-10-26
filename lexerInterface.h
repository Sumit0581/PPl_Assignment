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
    int tag;    // 0=terminal 1=non terminal 2=keyword
    struct HASHNODE * next;
}hashNode;

typedef struct TOKEN{
    int id; // 1,2,3,..primary key for tokens in sourceCode	//TODO : tell teammates to cover this
    int lineNum; // lite for ntr
    char lexeme[30]; // actual token in source code
    char tokenName[30]; // 
    int tag;	// 0 tr, 1 ntr, 2 keyword   
    struct TOKEN *next;
}token;


typedef struct tokens{
    token *head;
    int len;
}tokenStream;


#endif
