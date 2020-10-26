#include<ctype.h>
#include "parser.h"

// ----------------------------------------------------------------------------------------- UTILITY

treeNode *new_node(token *t,int tag){
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
  	new_node->data->tr = n; 
  }
  // non terminal
  else{
    	ntr *n = (ntr *) malloc(sizeof(ntr));
  	n->typeExp = NULL;	// to be filled while traversal
  	n->rule = rule;
  	new_node->data->ntr =
  }
  return new_node;
}

treeNode * add_sibling(treeNode *n,token *t,int tag){
  if(n==NULL) return NULL;
  while(n->nextSibling) n=n->nextSibling;
  return (n->nextSibling=new_node(data,tag));
}

treeNode *add_child(treeNode *n,token *t,int tag){
  if(n==NULL) return NULL;
  if(n->firstChild)
    return add_sibling(n->firstChild,data,tag);
  else
    return (n->firstChild=new_node(data,tag));
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

token* assignment(token* node, treeNode* root){ for(int i=0; i<3; i++) node = node->next; return node; }
token *varType(token *node, treeNode *root2){
	treeNode *root = add_child(root2, "varType", 1);
	if(strcmp(node->tokenName, "integer")==0){
			add_child(root, node->tokenName, 0);
	}
	else if(strcmp(node->tokenName, "real")==0){
			add_child(root, node->tokenName, 0);
	}
	else if(strcmp(node->tokenName, "boolean")==0){
			add_child(root, node->tokenName, 0);
	} 
	node = node->next;
	return node;

}
token* varList(token* node, treeNode* root){ for(int i=0; i<3; i++) node = node->next; return node; }
token* dimesnions(token* node, treeNode* root){ for(int i=0; i<5; i++) node = node->next; return node; }


// ----------------------------------------------------------------------------------------- KANAV's WORK


token* program(token* node, treeNode* root){
  add_child(root, node->tokenName, 0); // "program" added
  node = node->next;

  treeNode* temp = add_child(root, node->tokenName, 1); // "BO" added
  add_child(temp, node->lexeme, 0); // "(" added
  node = node->next;

  temp = add_child(root, node->tokenName, 1); // "BC" added
  add_child(temp, node->lexeme, 0); // ")" added
  node = node->next;

  temp = add_child(root, node->tokenName, 1); // "CBO" added
  add_child(temp, node->lexeme, 0); // "{" added
  node = node->next;

  token* tokenPtr = declarationList(node, root);
  tokenPtr = assignmentList(tokenPtr, root);

  temp = add_child(root, tokenPtr->tokenName, 1); // "CBC" added
  temp = add_child(root, tokenPtr->lexeme, 0); // "}" added

  return tokenPtr;
}

token* declarationList(token *node, treeNode *root){
  treeNode* temp = add_child(root, "declarationList", 1);
  token* tptr = declaration(node, temp);
  tptr = declarationList2(tptr, temp);
  return tptr;
}

token* declaration(token *node, treeNode *root){
  treeNode* temp = add_child(root, "declaration", 1);
  token* tokenPtr = NULL;

  if((tokenPtr = varDeclaration(node, temp)) != node) {node = tokenPtr;}
  else if((tokenPtr = listDeclaration(node, temp)) != node) {node = tokenPtr;}
  else if((tokenPtr = arrayDeclaration(node, temp)) != node) {node = tokenPtr;}
  // else node = jaggedDeclaration(node, temp);

  return node;
}

token* declarationList2(token *node, treeNode *root){
  token* tokenPtr = node;
  treeNode* temp = add_child(root, "declarationList2", 1);

  if(!strcmp(node->tokenName, "declare")){
    tokenPtr = declaration(node, temp);
    if(tokenPtr == node) return node;
    tokenPtr = declarationList2(tokenPtr, temp);
  }
  else {
    add_child(temp, "EPS", 0);
  }

  return tokenPtr;
}

token* assignmentList(token* node, treeNode* root){
  treeNode* temp = add_child(root, "assignmentList", 1);
  token* tptr = assignment(node, temp);
  tptr = assignmentList2(tptr, temp);
  return tptr;
}

token* assignmentList2(token *node, treeNode *root){
  token* tokenPtr = node;
  treeNode* temp = add_child(root, "assignmentList2", 1);

  if(!strcmp(node->tokenName, "ID")){
    tokenPtr = assignment(node, temp);
    if(tokenPtr == node) return node;
    tokenPtr = assignmentList2(tokenPtr, temp);
  }
  else {
    add_child(temp, "EPS", 0);
  }

  return tokenPtr;
}

token* varDeclaration(token* node, treeNode* root){
  treeNode* temp1 = add_child(root, "varDeclaration", 1);
  token* tokenPtr;

  add_child(temp1, node->tokenName, 0); // "declare" added
  node = node->next;

  treeNode* temp2 = add_child(temp1, node->tokenName, 1); // "ID" added
  add_child(temp2, node->lexeme, 0);
  node = node->next;

  temp2 = add_child(temp1, node->tokenName, 1); // "COLON" added
  add_child(temp2, node->lexeme, 0); // ":" added
  node = node->next;

  tokenPtr = varType(node, temp1);
//  if(tokenPtr == node)  return node;

  temp2 = add_child(temp1, node->tokenName, 1); // "SEMICOLON" added
  add_child(temp2, node->lexeme, 0); // ";" added
  node = node->next;

  return node;
}

token* listDeclaration(token* node, treeNode* root){
  treeNode* temp1 = add_child(root, "listDeclaration", 1);
  token* tokenPtr;

  add_child(temp1, node->tokenName, 0); // "declare" added
  node = node->next;

  add_child(temp1, node->tokenName, 1); // "list" added
  node = node->next;

  add_child(temp1, node->tokenName, 1); // "of" added
  node = node->next;

  add_child(temp1, node->tokenName, 1); // "variables" added
  node = node->next;

  tokenPtr = varList(node, temp1);
  if(tokenPtr == node)  return node;

  treeNode* temp2 = add_child(temp1, node->tokenName, 1); // "COLON" added
  add_child(temp2, node->lexeme, 0); // ":" added
  node = node->next;

  tokenPtr = varType(node, temp1);
  if(tokenPtr == node)  return node;

  temp2 = add_child(temp1, node->tokenName, 1); // "SEMICOLON" added
  add_child(temp2, node->lexeme, 0); // ";" added
  node = node->next;

  return node;
}


token* arrayDeclaration(token* node, treeNode* root){
  treeNode* temp1 = add_child(root, "varDeclaration", 1);
  token* tokenPtr;
  treeNode* temp2 = NULL;

  add_child(temp1, node->tokenName, 0); // "declare" added
  node = node->next;

  if(!strcmp(node->tokenName, "list")){
    add_child(temp1, node->tokenName, 1); // "list" added
    node = node->next;

    add_child(temp1, node->tokenName, 1); // "of" added
    node = node->next;

    add_child(temp1, node->tokenName, 1); // "variables" added
    node = node->next;

    tokenPtr = varList(node, temp1);
    if(tokenPtr == node)  return node;

    temp2 = add_child(temp1, node->tokenName, 1); // "COLON" added
    add_child(temp2, node->lexeme, 0); // ":" added
    node = node->next;
  }

  else{
    temp2 = add_child(temp1, node->tokenName, 1); // "ID" added
    add_child(temp1, node->lexeme, 0);
    node = node->next;

    temp2 = add_child(temp1, node->tokenName, 1); // "COLON" added
    add_child(temp1, node->lexeme, 0); // ":" added
    node = node->next;
  }

  add_child(temp1, node->tokenName, 1); // "array" added
  node = node->next;

  // tokenPtr = dimensions(node, temp1);
  // if(tokenPtr == node)  return node;

  add_child(temp1, node->tokenName, 1); // "of" added
  node = node->next;

  add_child(temp1, node->tokenName, 1); // "integer" added
  node = node->next;

  temp2 = add_child(temp1, node->tokenName, 1); // "SEMICOLON" added
  add_child(temp2, node->lexeme, 0); // ";" added
  node = node->next;

  return node;
}




