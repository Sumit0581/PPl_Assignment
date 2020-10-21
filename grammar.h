#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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