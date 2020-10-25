#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

char input[10];
int i,error;
int depth = 0;

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

typedef enum{
  integer,real,Boolean
}Datatype;

typedef enum{
  rectangularArray,jaggedArray
}ArrayType;

struct RectangularArray{
    ArrayType type;
    int dimensions;
    int *range[2];
    Datatype basicElementType;
  };

struct JaggedArray{
    ArrayType type;
    int dimensions;
    int range_R1[2];
    union range_R2{
      int *two_Dim;
      int **three_Dim;
    }R2;
    Datatype basicElementType;
  };
typedef union TypeExpression{
  Datatype primDt;
  struct RectangularArray rectangularArray;
  struct JaggedArray jaggedArray;
}typeExpression;


typedef struct TypeExpressionTable{
  char name[30];
  int type; //0 for primitive, 1 for rec array, 2 for jagged array
  char *arrayType; //[possible:static,dynamic,not_applicable] not made enum data type cause 'static' is a keyword in C and will cause problem.
  typeExpression typexpr;
}typeExpressionTable;


typedef struct terminal{
  char lexeme[30];
  int lno;
}tr;

typedef struct nonTerminal
{
  typeExpression typeExp;
  grammar rule;
}ntr;
typedef struct TreeNode{
  char symbol[30];
  symbolType tag;
  struct TreeNode *firstChild;
  struct TreeNode *nextSibling;
  union Data{
    tr Terminals;
    ntr Nonterminals;
  }data;
}treeNode;

typedef struct TOKEN{
    //numeral value; // to store num or rnum
    int id;
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

treeNode *new_node(char *,int);
token *expression(token *,treeNode *);
token *arithmeticExpression(token *,treeNode *);
token *arithmeticExpression2(token *,treeNode *);
token *mulExpression(token *,treeNode *);
token *mulExpression2(token *,treeNode *);
token *sumop(token *,treeNode *);
token *mulop(token *,treeNode *);
token *integerFactor(token *,treeNode *);
token *booleanExpression(token *,treeNode *);
token *booleanExpression2(token *,treeNode *);
token *andExpression(token *,treeNode *);
token *andExpression2(token *,treeNode *);
void printParseTree(treeNode *);

int main(){
  error=0;
  // printf("Enter an arithmetic expression   :  "); // Eg: a+a*a
  // gets(input);
  // treeNode *root = (treeNode *)malloc(sizeof(treeNode));
  
  tokenStream *s = (tokenStream *)malloc(sizeof(tokenStream));
  s->len = 6;
  s-> head = (token *)malloc(sizeof(token));
  s->head->id= 1;
  s->head->lineNum=1;
  strcpy(s->head->lexeme,"abcd");
  strcpy(s->head->tokenName,"ID");
  s->head->tag =1;

  token *a = (token *)malloc(sizeof(token));
  a->id=2;
  a->lineNum = 1;
  strcpy(a->lexeme,"|||");
  strcpy(a->tokenName,"PLUS");
  a->tag = 1;

  token *b = (token *)malloc(sizeof(token));
  b->id=3;
  b->lineNum = 1;
  strcpy(b->lexeme,"efgh");
  strcpy(b->tokenName,"ID");
  b->tag = 1;

  token *c = (token *)malloc(sizeof(token));
  c->id=4;
  c->lineNum = 1;
  strcpy(c->lexeme,"&&&");
  strcpy(c->tokenName,"MUL");
  c->tag = 1;

  token *d = (token *)malloc(sizeof(token));
  d->id=5;
  d->lineNum = 1;
  strcpy(d->lexeme,"2345");
  strcpy(d->tokenName,"NUM");
  d->tag = 1;

  token *e = (token *)malloc(sizeof(token));
  e->lineNum = 2;
  e->id=6;
  strcpy(e->lexeme,";");
  strcpy(e->tokenName,"SEMICOLON");
  e->tag = 1;

  s->head->next = a;
  a->next = b;
  b->next=c;
  c->next=d;
  d->next=e;
  e->next=NULL;
  
  treeNode *root = new_node("Start",1);
  printf("Hello");
  fflush(stdout);

  token *result = expression(s->head,root);
  if(!strcmp(result->tokenName,"SEMICOLON"))
    printf("\nAccepted..!!!\n");
  else printf("\nRejected..!!!\n");
  printParseTree(root);
}


treeNode *new_node(char* data,int tag){
  treeNode *new_node = malloc(sizeof(treeNode));
  strcpy(new_node->symbol,data);
  new_node-> tag = tag;
  new_node->firstChild = NULL;
  new_node->nextSibling =NULL;
  return new_node;
}

treeNode * add_sibling(treeNode *n,char *data,int tag){
  if(n==NULL) return NULL;
  while(n->nextSibling) n=n->nextSibling;
  return (n->nextSibling=new_node(data,tag));
}

treeNode *add_child(treeNode *n,char* data,int tag){
  if(n==NULL) return NULL;
  if(n->firstChild)
    return add_sibling(n->firstChild,data,tag);
  else
    return (n->firstChild=new_node(data,tag));
}


token *varName(token *node,treeNode *root){
  if(!strcmp(node->tokenName,"ID")){
    treeNode *temp = add_child(root,"varName",1);
    add_child(temp,node->lexeme,0);
    return node->next;
  } 
  else{
    return node;
  }
}

token *expression(token *node,treeNode *root){
  treeNode *temp = add_child(root,"Expression",1);
  token *a = arithmeticExpression(node,temp);
  printf("%d ",a->id);
  if(strcmp(a->tokenName,"SEMICOLON")){
    temp->firstChild = NULL;
    token *b= booleanExpression(node,temp);
    if(b->id==node->id)
      return node;
    else
      return b; 
  }
  else
    return a;
}

token *arithmeticExpression(token *node,treeNode *root){
  treeNode *temp = add_child(root,"ArithmeticExpression",1);
  token *a= mulExpression(node,temp);
  if(a->id==node->id) return node;
  token *b = arithmeticExpression2(a,temp);
  return b;
}

token *arithmeticExpression2(token *node,treeNode *root){
  treeNode *temp = add_child(root,"ArithmeticExpression2",1);
  token *a = sumop(node,temp);
  if(a->id==node->id){
    temp->firstChild = NULL;
    add_child(temp,"EPS",0);
    return node;
  }
  else{
    token *b = mulExpression(a,temp);
    token *c = arithmeticExpression2(b,temp);
    return c;
  }

}

token *mulExpression(token *node,treeNode *root){ 
  treeNode *temp = add_child(root,"MulExpression",1);
  token *a = integerFactor(node,temp);
  if(a->id==node->id) return node;
  token *b = mulExpression2(a,temp);
  return b;
}

token *mulExpression2(token *node,treeNode *root){
  treeNode *temp = add_child(root,"MulExpression2",1);
  token *a = mulop(node,temp);
  if(a->id==node->id){
    temp->firstChild = NULL;
    add_child(temp,"EPS",0);
    return node;
  }
  else{
    token *b = integerFactor(a,temp);
    if(b->id==a->id) return node;
    token *c = mulExpression2(b,temp);
    return c;
  }
}

token *sumop(token *node,treeNode *root){
  treeNode *temp = add_child(root,"SUMOP",1);
  if(!strcmp(node->tokenName,"PLUS")){
    add_child(temp,"PLUS",0);
    return node->next;
  }
  else if(!strcmp(node->tokenName,"MINUS")){
    add_child(temp,"MINUS",0);
    return node->next;
  }
  else{
    return node;
  }
  
}

token *mulop(token *node,treeNode *root){
  treeNode *temp = add_child(root,"MULOP",1);
  if(!strcmp(node->tokenName,"MUL")){
    add_child(temp,"MUL",0);
    return node->next;
  }
  else if(!strcmp(node->tokenName,"DIV")){
    add_child(temp,"DIV",0);
    return node->next;
  }
  else{
    return node;
  }
}

token *integerFactor(token *node,treeNode *root){
  treeNode *temp = add_child(root,"IntegerFactor",1);
  if(!strcmp(node->tokenName,"NUM")){
    add_child(temp,"NUM",0);
    return node->next;
  }
  else{
    token *a = varName(node,temp);
    return a;
  }
}

token *booleanExpression(token *node,treeNode *root){
  treeNode *temp = add_child(root,"BooleanExpression",1);
  token *a = andExpression(node,temp);
  if(a->id==node->id) return node;
  token *b = booleanExpression2(a,temp);
  return b;
}

token *booleanExpression2(token *node,treeNode *root){
  treeNode *temp = add_child(root,"BooleanExpression2",1);
  if(!strcmp(node->tokenName,"OROP")){
    add_child(temp,"OROP",0);
    token *a = andExpression(node->next,temp);
    if(a->id==node->id) return node;
    token *b = booleanExpression2(a,temp);
    return b;
  }
  else{
    temp->firstChild=NULL;
    add_child(temp,"EPS",0);
    return node;
  }
}

token *andExpression(token *node,treeNode *root){
  treeNode *temp = add_child(root,"AndExpression",1);
  token *a = varName(node,temp);
  if(a->id==node->id) return node;
  token *b = andExpression2(a,temp);
  return b;
}

token *andExpression2(token *node,treeNode *root){
  treeNode *temp = add_child(root,"AndExpression2",1);
  if(!strcmp(node->tokenName,"ANDOP")){
    add_child(temp,"ANDOP",0);
    token *a = varName(node->next,temp);
    if(a->id==node->id) return node;
    token *b= andExpression2(a,temp);
    return b;
  }
  else{
    temp->firstChild=NULL;
    add_child(temp,"EPS",0);
    return node;
  }
}


void printParseTree(treeNode *root){
  treeNode *temp = root;
  if(temp==NULL) return;
    printf("%s %d %d\n",temp->symbol,depth,temp->tag);
    depth++;
    printParseTree(temp->firstChild);
    depth--;
    printf(" \n");
    printParseTree(temp->nextSibling);
}

