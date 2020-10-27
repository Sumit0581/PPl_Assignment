#include<ctype.h>
#include "parser.h"

// ----------------------------------------------------------------------------------------- UTILITY


token * createTokenNtr(char *data){
	token *tk = (token *) malloc(sizeof(token));
	tk->tag =1;
	strcpy(tk->tokenName, data);
	return tk;
}

token * createTokenEPS(){
	token *tk = (token *) malloc(sizeof(token));
	tk->tag = 0;
	strcpy(tk->tokenName, "EPS");
	strcpy(tk->lexeme, "EPS");
	tk->lineNum = -100;
	return tk;
}


treeNode *new_node(token *t,int tag, int rule){
  treeNode *new_node = malloc(sizeof(treeNode));
  strcpy(new_node->symbol,t->tokenName);
  new_node-> tag = tag;
  new_node->firstChild = NULL;
  new_node->nextSibling =NULL;
  //terminal
  if(tag==0){
  	tr *n = (tr *) malloc(sizeof(tr));
  	strcpy(n->lexeme, t->lexeme);
  	n->lno = t->lineNum;
  	new_node->data.Terminals = *n;
  }
  // non terminal
  else{
    	ntr *n = (ntr *) malloc(sizeof(ntr));
  	//n->typeExp = NULL;	// to be filled while traversal
  	n->rule = rule;
  	new_node->data.Nonterminals = *n;
  }
  return new_node;
}

treeNode * add_sibling(treeNode *n,token *t,int tag, int rule){
  if(n==NULL) return NULL;
  while(n->nextSibling) n=n->nextSibling;
  return (n->nextSibling=new_node(t,tag, rule));
}

treeNode *add_child(treeNode *n,token *t,int tag,int rule ){
  if(n==NULL) return NULL;
  if(n->firstChild)
    return add_sibling(n->firstChild,t ,tag, rule);
  else
    return (n->firstChild=new_node(t,tag, rule));
}

int depth =0;	// yes, a global variable
void printRecur(treeNode *root){
  treeNode *temp = root;

  if(temp==NULL) return;
    printf("%s %d %d\n",temp->symbol,depth,temp->tag);
    depth++;
    printRecur(temp->firstChild);
    depth--;
    printf(" \n");
    printRecur(temp->nextSibling);
}

void printParseTree(treeNode *root){
	printf("\n\n-------PRINTING PARSE TREE--------(symbol, depth, tag (0/1))\n\n");
	printRecur(root);
}

// ----------------------------------------------------------------------------------------- KANAV's REQUIREMENT

token* assignment(token* node, treeNode* root){ add_child(root, createTokenEPS(), 1, -1); return node; }
token *varType(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("varType"), 1, 28);
	if(strcmp(node->tokenName, "integer")==0){
			add_child(root, node, 0, 99);
	}
	else if(strcmp(node->tokenName, "real")==0){
			add_child(root, node, 0, 99);
			root->data.Nonterminals.rule = 29;
	}
	else if(strcmp(node->tokenName, "boolean")==0){
			add_child(root, node, 0, 99);
			root->data.Nonterminals.rule = 30;
	}
	node = node->next;
	return node;
}
token* varList(token* node, treeNode* root){ for(int i=0; i<4; i++) node = node->next; return node; }
token* dimensions(token* node, treeNode* root){ for(int i=0; i<5; i++) node = node->next; return node; }


// ----------------------------------------------------------------------------------------- KANAV's WORK


token* program(token* node, treeNode* root){
  treeNode* temp = add_child(root, createTokenNtr("program"), 1, 0);
  root = temp;

  add_child(root, node, 0, -1); // "program" added
  node = node->next;

  add_child(root, node, 0, -1); // "BO" added
  node = node->next;

  add_child(root, node, 0, -1); // "BC" added
  node = node->next;

  add_child(root, node, 0, -1); // "CBO" added
  node = node->next;

  token* tokenPtr = declarationList(node, root);
  tokenPtr = assignmentList(tokenPtr, root);

  //temp = add_child(root, tokenPtr->tokenName, 1); // "CBC" added

  return tokenPtr;
}

token* declarationList(token *node, treeNode *root){
  treeNode* temp = add_child(root, createTokenNtr("declarationList"), 1, 1);
  token* tptr = declaration(node, temp);
  tptr = declarationList2(tptr, temp);
  return tptr;
}

token* declaration(token *node, treeNode *root){
  treeNode* temp = add_child(root, createTokenNtr("declaration"), 1, 7);
  token* tokenPtr = NULL;

  tokenPtr = varDeclaration(node, temp);
  if(tokenPtr == node) {
    free(temp->firstChild);
    temp->firstChild = NULL;
  }
  else return tokenPtr;

  temp->data.Nonterminals.rule = 8;
  tokenPtr = listDeclaration(node, temp);
  if(tokenPtr == node) {
    free(temp->firstChild);
    temp->firstChild = NULL;
  }
  else return tokenPtr;

  temp->data.Nonterminals.rule = 9;
  tokenPtr = arrayDeclaration(node, temp);
  if(tokenPtr == node) {
    free(temp->firstChild);
    temp->firstChild = NULL;
  }
  else return tokenPtr;
/*
  temp->data.Nonterminals.rule = 10;
  tokenPtr = jaggedDeclaration(node, temp);
  if(tokenPtr != node) {
    free(temp->firstChild);
    temp->firstChild = NULL;
  }
  else return tokenPtr;
*/
  return node;
}

token* declarationList2(token *node, treeNode *root){
  token* tokenPtr = node;
  treeNode* temp = add_child(root, createTokenNtr("declarationList2"), 1, 2);

  if(!strcmp(node->tokenName, "declare")){
    tokenPtr = declaration(node, temp);
    if(tokenPtr == node) return node;
    tokenPtr = declarationList2(tokenPtr, temp);
  }
  else {
    temp->data.Nonterminals.rule = 3;
    add_child(temp, createTokenEPS(), 0, -1);
  }

  return tokenPtr;
}

token* assignmentList(token* node, treeNode* root){
  treeNode* temp = add_child(root, createTokenNtr("assignmentList"), 1, 4);
  token* tptr = assignment(node, temp);
  tptr = assignmentList2(tptr, temp);
  return tptr;
}

token* assignmentList2(token *node, treeNode *root){
  token* tokenPtr = node;
  treeNode* temp = add_child(root, createTokenNtr("assignmentList2"), 1, 5);

  if(!strcmp(node->tokenName, "ID")){
    tokenPtr = assignment(node, temp);
    if(tokenPtr == node) return node;
    tokenPtr = assignmentList2(tokenPtr, temp);
  }
  else {
    temp->data.Nonterminals.rule = 6;
    add_child(temp, createTokenEPS(), 0, -1);
  }

  return tokenPtr;
}

token* varDeclaration(token* node, treeNode* root){
  treeNode* temp1 = add_child(root, createTokenNtr("varDeclaration"), 1, 11);
  root = temp1;
  token* tokenPtr = NULL;
  token* tptr = node;

  if(strcmp(node->tokenName, "declare"))  return tptr;
  add_child(root, node, 0, -1); // "declare" added
  node = node->next;

  if(strcmp(node->tokenName, "ID"))  return tptr;
  add_child(root, node, 0, -1); // "ID" added
  node = node->next;

  if(strcmp(node->tokenName, "COLON"))  return tptr;
  add_child(root, node, 0, -1); // "COLON" added
  node = node->next;

  if(!strcmp(node->tokenName, "array") || !strcmp(node->tokenName, "jagged"))  return tptr;
  tokenPtr = varType(node, root);
  if(tokenPtr == node)  return node;

  node = tokenPtr;
  if(strcmp(node->tokenName, "SEMICOLON"))  return tptr;
  add_child(root, node, 0, -1); // "SEMICOLON" added
  node = node->next;

  return node;
}

token* listDeclaration(token* node, treeNode* root){
  treeNode* temp1 = add_child(root, createTokenNtr("listDeclaration"), 1, 12);
  root = temp1;
  token* tokenPtr = NULL;
  token* tptr = node;

  if(strcmp(node->tokenName, "declare"))  return tptr;
  add_child(root, node, 0, -1); // "declare" added
  node = node->next;

  if(strcmp(node->tokenName, "list"))  return tptr;
  add_child(root, node, 0, -1); // "list" added
  node = node->next;

  if(strcmp(node->tokenName, "of"))  return tptr;
  add_child(root, node, 0, -1); // "of" added
  node = node->next;

  if(strcmp(node->tokenName, "variables"))  return tptr;
  add_child(root, node, 0, -1); // "variables" added
  node = node->next;

  if(strcmp(node->tokenName, "ID"))  return tptr;
  tokenPtr = varList(node, root);
  if(tokenPtr == node)  return node;

  node = tokenPtr;
  if(strcmp(node->tokenName, "COLON"))  return tptr;
  add_child(root, node, 0, -1); // "COLON" added
  node = node->next;

  if(!(!strcmp(node->tokenName, "integer") || !strcmp(node->tokenName, "real") || !strcmp(node->tokenName, "boolean")))  return tptr;
  tokenPtr = varType(node, root);
  if(tokenPtr == node)  return node;

  node = tokenPtr;
  if(strcmp(node->tokenName, "SEMICOLON"))  return tptr;
  add_child(root, node, 0, -1); // "SEMICOLON" added
  node = node->next;

  return node;
}

token* arrayDeclaration(token* node, treeNode* root){
  treeNode* temp1 = add_child(root, createTokenNtr("arrayDeclaration"), 1, 14);
  root = temp1;
  token* tokenPtr = NULL;
  token* tptr = node;

  if(strcmp(node->tokenName, "declare"))  return tptr;
  add_child(root, node, 0, -1); // "declare" added
  node = node->next;

  if(!strcmp(node->tokenName, "list")){
    add_child(root, node, 0, -1); // "list" added
    node = node->next;

    if(strcmp(node->tokenName, "of"))  return tptr;
    add_child(root, node, 0, -1); // "of" added
    node = node->next;

    if(strcmp(node->tokenName, "variables"))  return tptr;
    add_child(root, node, 0, -1); // "variables" added
    node = node->next;

    if(strcmp(node->tokenName, "ID"))  return tptr;
    tokenPtr = varList(node, root);
    if(tokenPtr == node)  return node;

    node = tokenPtr;
    if(strcmp(node->tokenName, "COLON"))  return tptr;
    add_child(root, node, 0, -1); // "COLON" added
    node = node->next;
  }

  else{
    root->data.Nonterminals.rule = 14;
    if(strcmp(node->tokenName, "ID"))  return tptr;
    add_child(root, node, 0, -1); // "ID" added
    node = node->next;

    if(strcmp(node->tokenName, "COLON"))  return tptr;
    add_child(root, node, 0, -1); // "COLON" added
    node = node->next;
  }

  if(strcmp(node->tokenName, "array"))  return tptr;
  add_child(root, node, 0, -1); // "array" added
  node = node->next;

  if(strcmp(node->tokenName, "SQRBO"))  return tptr;
  tokenPtr = dimensions(node, root);
  if(tokenPtr == node)  return node;

  node = tokenPtr;
  if(strcmp(node->tokenName, "of"))  return tptr;
  add_child(root, node, 0, -1); // "of" added
  node = node->next;

  if(strcmp(node->tokenName, "integer"))  return tptr;
  add_child(root, node, 0, -1); // "integer" added
  node = node->next;

  if(strcmp(node->tokenName, "SEMICOLON"))  return tptr;
  add_child(root, node, 0, -1); // "SEMICOLON" added
  node = node->next;

  return node;
}
