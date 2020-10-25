#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

char input[10];
int i,error;

typedef struct TreeNode{
  char symbol[30];
  struct TreeNode *parent;
  struct TreeNode *firstChild;
  struct TreeNode *nextSibling;
}treeNode;

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

treeNode *new_node(char *);
treeNode * add_sibling(treeNode *,char *);
treeNode *add_child(treeNode *,char *);
token * varList(token *,int ,treeNode *);
token * varList2(token *,int,treeNode *);
token * dimensions(token *,int,treeNode *);
token * dimensions2(token *,int ,treeNode *);
token * range(token *,int,treeNode *);
token * singleStaticDimension(token *node,int len,treeNode *root)


treeNode *new_node(char* data){
  treeNode *new_node = malloc(sizeof(treeNode));
  strcpy(new_node->symbol,data);
  new_node->firstChild = NULL;
  new_node->nextSibling =NULL;
  new_node->parent =NULL;
  return new_node;
}

treeNode * add_sibling(treeNode *n,char *data){
  if(n==NULL) return NULL;
  while(n->nextSibling) n=n->nextSibling;
  return (n->nextSibling=new_node(data));
}

treeNode *add_child(treeNode *n,char* data){
  if(n==NULL) return NULL;
  if(n->firstChild)
    return add_sibling(n->firstChild,data);
  else
    return (n->firstChild=new_node(data));
}
//treeNode *removeNode(treeNode *n){
	




token * varList(token *node,int len,treeNode *root){
	token *prev=node;
	token *temp,*temp1,*temp2;
	if(!strcmp(node->tokenName,"ID")){
		temp=node;
		node=node->next;
		if(!strcmp(node->tokenName,"ID")){
			temp1=node;
			node=node->next;
			treeNode *child=add_child(root,"varList");
			add_child(child,temp->lexeme);
			add_child(child,temp1->lexeme);
			token *move=varList2(node,len,child);
			return move;
		}
		else if(!strcmp(node->tokenName,"COMMA")){
			temp1=node;
			node=node->next;
			if(!strcmp(node->tokenName,"ID")){
				temp2=node;
				node=node->next;
				treeNode *child=add_child(root,"varList");
				add_child(child,temp->lexeme);
				add_child(child,temp1->lexeme);
				add_child(child,temp2->lexeme);
				token *move =varList2(node,len,child);
				return move;
			}
		}
	}
	return prev;
}
//<varList2> ID <varList2>
//<varList2> COMMA ID <varList2>
//<varList2> EPS
token * varList2(token *node,int len,treeNode *root){
		token *prev=node;
		token *temp,*temp1,*temp2;
	if(!strcmp(node->tokenName,"ID")){
		temp=node;
		node=node->next;
		treeNode *child=add_child(root,"varList2");
			add_child(child,temp->lexeme);
			//add_child(child,temp1->lexeme);
			token *move=varList2(node,len,child);
			return move;
		
		
	}
	else if(!strcmp(node->tokenName,"COMMA"))
		{
			temp=node;
			node=node->next;
		treeNode *child=add_child(root,"varList2");
			add_child(child,temp->lexeme);
			add_child(child,node->lexeme);
			node=node->next;
			//add_child(child,temp1->lexeme);
			token *move=varList2(node,len,child);
			return move;
		}
	else
	{
		treeNode *child=add_child(root,"varList2");
		add_child(child,"EPS");
		return node;
	}
}
//<dimensions> SQRBO <range> DOTDOT <range> SQRBC <dimensions2>
//<dimensions2> SQRBO <range> DOTDOT <range> SQRBC <dimensions2>
//<dimensions2> EPS
//<singleDimension> SQRBO <range> DOTDOT <range> SQRBC
token * dimensions(token *node,int len,treeNode *root){
	if(!strcmp(node->tokenName,"SQRBO")){
		
			treeNode *child=add_child(root,"dimensions");
			add_child(child,node->lexeme);
			node=node->next;
			token *move=range(node,len,child);
			move=move->next;
			add_child(child,move->lexeme);
			move=move->next;
			move=range(move,len,child);
			move=move->next;
			add_child(child,move->lexeme);
			move=move->next;
			move=dimensions2(move,len,child);
			//move=move->next;
			return move;
		}
		else{
			return node;
		}
	}
token * dimensions2(token *node,int len,treeNode *root){
			if(!strcmp(node->tokenName,"SQRBO")){
		
			treeNode *child=add_child(root,"dimensions2");
			add_child(child,node->lexeme);
			node=node->next;
			token *move=range(node,len,child);
			move=move->next;
			add_child(child,move->lexeme);
			move=move->next;
			move=range(move,len,child);
			move=move->next;
			add_child(child,move->lexeme);
			move=move->next;
			move=dimensions2(move,len,child);
			//move=move->next;
			return move;
		}
		else{
			treeNode *child=add_child(root,"dimensions2");
			add_child(child,"EPS");
			return node;
		}
			
}
token* range(token *node,int len,treeNode *root){
	if(!strcmp(node->tokenName,"ID")){
		treeNode *child=add_child(root,"range");
			add_child(child,node->lexeme);
			return node;
	}
	if(!strcmp(node->tokenName,"NUM")){
		treeNode *child=add_child(root,"range");
			add_child(child,node->lexeme);
			return node;
	}
}
//<singleStaticDimension> SQRBO NUM DOTDOT NUM SQRBC
token * singleStaticDimension(token *node,int len,treeNode *root){
	if(!strcmp(node->tokenName,"SQRBO")){
		
			treeNode *child=add_child(root,"singleStaticDimension");
			add_child(child,node->lexeme);
			node=node->next;
			add_child(child,node->lexeme);
			node=node->next;
			add_child(child,node->lexeme);
			node=node->next;
			add_child(child,node->lexeme);
			node=node->next;
			add_child(child,node->lexeme);
			//move=move->next;
			return node;
		}
		else{
	
			return node;
		}
}
//<index> <range> <index2>
//<index2> <range> <index2>
//<index2> EPS
token * index(token *node,int len,treeNode *root){
		treeNode *child=add_child(root,"index");
			token *move=range(node,len,child);
			move=move->next;
			move=index2(node,len,child);
}
token * index2(token *node,int len,treeNode *root){
		treeNode *child=add_child(root,"index2");
			if(!strcmp(node->tokenName,"ID") || !strcmp(node->tokenName,"NUM")){
			token *move=range(node,len,child);
			move=move->next;
			move=range(node,len,child);
			}
			else{
				add_child(child,"EPS");
			}
}
//<brackets> SQRBO SQRBC SQRBO SQRBC
//<brackets> SQRBO SQRBC
token * brackets(token *node,int len,treeNode *root){
	treeNode *child=add_child(root,"brackets");
		add_child(child,node->lexeme);
		node=node->next;
		add_child(child,node->lexeme);
		node=node->next;
		if(!strcmp(node->tokenName,"SQRBO"))
		{
			add_child(child,node->lexeme);
			node=node->next;
			add_child(child,node->lexeme);
			node=node->next;
			return node;
		}
		else{
			return node;
		}
}
//<assignment> <varName> EQU <expression> SEMICOLON
token * assignment(token *node,int len,treeNode *root){
		treeNode *child=add_child(root,"assignments");
		token *move=varName(node,len,child);
		move=move->next;
		add_child(child,move->lexeme);
		move=move->next;
		move=expression(move,len,child);
		move=move->next;
		add_child(child,move->lexeme);
}
//<varName> ID
//<varName> ID SQRBO <index> SQRBC
token * varName(token *node,int len,treeNode *root){
	treeNode *child=add_child(root,"varName");
		add_child(child,node->lexeme);
		node=node->next;
		if(!strcmp(node->tokenName,"SQRBO"))
		{
			token *move=varName(node,len,child);
			move=move->next;
			add_child(child,move->lexeme);
			move=move->next;
			return move;
		}
		else{
			return node;
		}
}
			
		


		
		
		
		
	
				

			
		

	
void printParseTree(treeNode *root,int i){
  treeNode *temp = root;
  if(temp==NULL) return;
    printf("%s %d \n",temp->symbol,i);
    printParseTree(temp->firstChild,i+1);
    printf(" \n");
    printParseTree(temp->nextSibling,i);
}
int main(){
  error=0;
  // printf("Enter an arithmetic expression   :  "); // Eg: a+a*a
  // gets(input);
  // treeNode *root = (treeNode *)malloc(sizeof(treeNode));
 /*
  tokenStream *s = (tokenStream *)malloc(sizeof(tokenStream));
  s->len = 5;
  s-> head = (token *)malloc(sizeof(token));
  s->head->lineNum=1;
  strcpy(s->head->lexeme,"abcd");
  strcpy(s->head->tokenName,"ID");
  s->head->tag =1;

  token *a = (token *)malloc(sizeof(token));
  a->lineNum = 1;
  strcpy(a->lexeme,"&&&");
  strcpy(a->tokenName,"ANDOP");
  a->tag = 1;

  token *b = (token *)malloc(sizeof(token));
  b->lineNum = 1;
  strcpy(b->lexeme,"efgh");
  strcpy(b->tokenName,"ID");
  b->tag = 1;

  token *c = (token *)malloc(sizeof(token));
  c->lineNum = 1;
  strcpy(c->lexeme,"&&&");
  strcpy(c->tokenName,"ANDOP");
  c->tag = 1;

  token *d = (token *)malloc(sizeof(token));
  d->lineNum = 1;
  strcpy(d->lexeme,"efgh");
  strcpy(d->tokenName,"ID");
  d->tag = 1;

  token *e = (token *)malloc(sizeof(token));
  e->lineNum = 2;
  strcpy(e->lexeme,"EOF");
  strcpy(e->tokenName,"EOF");
  e->tag = 1;

  s->head->next = a;
  a->next = b;
  b->next=c;
  c->next=d;
  d->next=e;
  e->next=NULL;
  */
 
 /* tokenStream *s = (tokenStream *)malloc(sizeof(tokenStream));
  s->len = 5;
  s-> head = (token *)malloc(sizeof(token));
  s->head->lineNum=1;
  strcpy(s->head->lexeme,"abcd");
  strcpy(s->head->tokenName,"ID");
  s->head->tag =1;

  token *a = (token *)malloc(sizeof(token));
  a->lineNum = 1;
  strcpy(a->lexeme,"cdef");
  strcpy(a->tokenName,"ID");
  a->tag = 1;

  token *b = (token *)malloc(sizeof(token));
  b->lineNum = 1;
  strcpy(b->lexeme,",");
  strcpy(b->tokenName,"COMMA");
  b->tag = 1;

  token *c = (token *)malloc(sizeof(token));
  c->lineNum = 1;
  strcpy(c->lexeme,"ghij");
  strcpy(c->tokenName,"ID");
  c->tag = 1;

  token *d = (token *)malloc(sizeof(token));
  d->lineNum = 1;
  strcpy(d->lexeme,"efgh");
  strcpy(d->tokenName,"ID");
  d->tag = 1;

  token *e = (token *)malloc(sizeof(token));
  e->lineNum = 2;
  strcpy(e->lexeme,"EOF");
  strcpy(e->tokenName,"EOF");
  e->tag = 1;

  s->head->next = a;
  a->next = b;
  b->next=c;
  c->next=d;
  d->next=e;
  e->next=NULL;
  */
  tokenStream *s = (tokenStream *)malloc(sizeof(tokenStream));
   s->len = 5;
  s-> head = (token *)malloc(sizeof(token));
  s->head->lineNum=1;
  strcpy(s->head->lexeme,"[");
  strcpy(s->head->tokenName,"SQRBO");
  s->head->tag =1;

  token *a = (token *)malloc(sizeof(token));
  a->lineNum = 1;
  strcpy(a->lexeme,"abc");
  strcpy(a->tokenName,"ID");
  a->tag = 1;

  token *b = (token *)malloc(sizeof(token));
  b->lineNum = 1;
  strcpy(b->lexeme,"..");
  strcpy(b->tokenName,"DOTDOT");
  b->tag = 1;

  token *c = (token *)malloc(sizeof(token));
  c->lineNum = 1;
  strcpy(c->lexeme,"1234");
  strcpy(c->tokenName,"NUM");
  c->tag = 1;

  token *d = (token *)malloc(sizeof(token));
  d->lineNum = 1;
  strcpy(d->lexeme,"]");
  strcpy(d->tokenName,"SQRBC");
  d->tag = 1;

  token *e = (token *)malloc(sizeof(token));
  e->lineNum = 2;
  strcpy(e->lexeme,"EOF");
  strcpy(e->tokenName,"EOF");
  e->tag = 1;

  s->head->next = a;
  a->next = b;
  b->next=c;
  c->next=d;
  d->next=e;
  e->next=NULL;
 
  
  treeNode *root = new_node("Start");

  //int result = andExpression(s->head,s->len,root);
  token * ans=dimensions(s->head,s->len,root);
  printf("%s\n",ans->lexeme);
  //if(result==0)
    //printf("\nAccepted..!!!\n");
  //else printf("\nRejected..!!!\n");
  printParseTree(root,0);
}

