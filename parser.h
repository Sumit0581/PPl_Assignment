#ifndef parserh
#define parserh
 
#include "grammar.h"
#include "stdbool.h"
#define maxChild 10

//TODO index => index1 in grammar as well
/*
typedef struct node{
	char symbolName[30];
	int isNtr; //0=tr, 1=ntr
	
	int ntrId;	// can use search() but better would have been enum only just if enum creation could be done dynamic
	int depth; //root's depth =0
	struct node *children[maxChild];	//all set to null
	
	// if non leaf
	int grammarRule;	//which grammar rule is used to expand this ntr
	char *typeExpression;
	
	//if leaf
	int lineNum;	// takes from lineNum. which line in source text
	
} parseNode;

typedef struct tree{
	parseNode *root;
} parseTree;
*/

// ----------------------------------------------------------------------- Data structures


/*
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
*/
// already defined in grammar.h
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
  //grammar rule;	//TODO: grammar *rule
  int rule;	// grammar rule nos. starting from 0 index
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

// check TOKEN and tokenStream in lexerInterface.h

// ----------------------------------------------------------------------- end Data structures

void printParseTree(treeNode *);
void printParseTreeFancy(treeNode *);	//fancy

// t* add_child(treeNode *, char *data, tag);
//1. terminals - COLON add_child(,node,, rule) //node->tokenName
//2/ add_child(, NTR("varList"), rule )

treeNode * add_sibling(treeNode *n,token *t,int tag,int rule);
treeNode *new_node(token *t,int tag, int rule);
treeNode *add_child(treeNode *n,token *t,int tag, int rule);

token * createTokenNtr(char *data);
token * createTokenEPS();
//aniket
token * jaggedDeclaration(token *node, treeNode *root2);
token *jaggedInitialisation(token *node, treeNode *root);
token *numberList(token *node, treeNode *root);
token *numberList2(token *node, treeNode *root2);
token *numberList3(token *node, treeNode *root2);
token *numberList4(token *node, treeNode *root2);
token *varType(token *node, treeNode *root2);
token *jaggedInitialisationList(token *node, treeNode *root2);
token *jaggedInitialisationList2(token *node, treeNode *root2);


//sumit
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


//kanav
token* program(token* node, treeNode* root);
token* declarationList(token *node, treeNode *root);
token* declaration(token *node, treeNode *root);
token* declarationList2(token *node, treeNode *root);
token* assignmentList(token* node, treeNode* root);
token* assignmentList2(token *node, treeNode *root);
token* varDeclaration(token* node, treeNode* root);
token* listDeclaration(token* node, treeNode* root);
token* arrayDeclaration(token* node, treeNode* root);
//token* assignment(token* node, treeNode* root);
//token* varType(token* node, treeNode* root);
//token* varList(token* node, treeNode* root);
//token* dimensions(token* node, treeNode* root);


//rishav
token * varList(token *,treeNode *);
token * varList2(token *, treeNode *);
token * dimensions(token *, treeNode *);
token * dimensions2(token *, treeNode *);
token * range(token *, treeNode *);
token * singleStaticDimension(token *, treeNode *);

token * index1(token *node,treeNode *root);
token * index2(token *node,treeNode *root);
token * brackets(token *node,treeNode *root);
token * assignment(token *node, treeNode *root);
token * varName(token *node, treeNode *root);




#endif
