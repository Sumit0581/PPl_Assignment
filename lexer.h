#ifndef lexerh
#define lexerh
#include "lexerInterface.h"


//part 1. build hash lookup table
//part 2. convert input source lexemes to token and store in tokenStream


//--------------------------------------1. starts

extern hashNode* hashTable[HASH_TABLE_SIZE];
//extern tokenStream *s;	// no need of gloabl variable. Just define in driver

int lexerPreReq(char *srcFile); 
	//source file name 'srcFile'
	// populates 'hashTable' for lexer token creation
	// returns 1 file is opened, else 0
void printHashTable();

//--------------------------------------2. starts

token *getNextToken();	// from source file
void printToken(token *tk);
void printTokenStream(tokenStream *s);
void tokeniseSourcecode( char * sourceFileName, tokenStream *s);
int search(char *value,int tag); // search in the hashTable, value: Hashnode string to be searched, tag: tag of the Hashnode, returns: Hashnode id (-1 if not present in hashTable)
void removeBlankspace(char *);


#endif
