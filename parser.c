#include<ctype.h>
#include "parser.h"

// ---------------------------------------------------------------------------------------- USELESS AS OF NOW

void printError2(char *c){
	printf("In printError2() %s\n", c);
}



// ------------------------------------------------------------------------------------------ OLD

void fContainer(token *start, grammar *g){
	printf("trying %s %s ",start->lexeme, start->tokenName);
	f(start, g);
	putchar('\n');
	return;
}

int f(token *ntr, grammar *g){
	/*
	ntr: the non terminal to be expanded via rules
	*/
	//printf("%lu \n",sizeof(g));
	int id  = search(ntr->tokenName, 1);
	int found =0;
	for(int i=0; i<grammarRules; i++){ /*from grammar.h, grammar.c*/
		if(g[i].value==id){
			found =1;
			printf(": %s,%s- found one rule \n", ntr->tokenName, ntr->lexeme);	
		}		
		
	}
	//if(!found) printf("%s | %s rule not found\n", ntr->tokenName, ntr->lexeme);
	return 0; // no match found
}

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

//RISHAV's works start rule no-32 to 50


token * varList(token *node,treeNode *root){
	token *prev=node;
	token *temp,*temp1,*temp2;
	if(!strcmp(node->tokenName,"ID")){
		temp=node;
		node=node->next;
		if(!strcmp(node->tokenName,"ID")){
			temp1=node;
			node=node->next;
			treeNode *child=add_child(root,createTokenNtr("varList"),1,31);
			add_child(child,temp,0,31);
			add_child(child,temp1,0,31);
			token *move=varList2(node,child);
			return move;
		}
		else if(!strcmp(node->tokenName,"COMMA")){
			temp1=node;
			node=node->next;
			if(!strcmp(node->tokenName,"ID")){
				temp2=node;
				node=node->next;
				treeNode *child=add_child(root,createTokenNtr("varList"),1,32);
				add_child(child,temp,0,32);
				add_child(child,temp1,0,32);
				add_child(child,temp2,0,32);
				token *move =varList2(node,child);
				return move;
			}
		}
	}
	return prev;
}
//<varList2> ID <varList2>
//<varList2> COMMA ID <varList2>
//<varList2> EPS
token * varList2(token *node,treeNode *root){
		token *prev=node;
		token *temp,*temp1,*temp2;
	if(!strcmp(node->tokenName,"ID")){
		temp=node;
		node=node->next;
		treeNode *child=add_child(root,createTokenNtr("varList2"),1,33);
			add_child(child,temp,0,33);
			//add_child(child,temp1->lexeme);
			token *move=varList2(node,child);
			return move;
		
		
	}
	else if(!strcmp(node->tokenName,"COMMA"))
		{
			temp=node;
			node=node->next;
		treeNode *child=add_child(root,createTokenNtr("varList2"),1,34);
			add_child(child,temp,0,34);
			add_child(child,node,0,34);
			node=node->next;
			//add_child(child,temp1->lexeme);
			token *move=varList2(node,child);
			return move;
		}
	else
	{
		treeNode *child=add_child(root,createTokenNtr("varList2"),1,35);
		add_child(child,createTokenEPS(),0,35);
		return node;
	}
}
//<dimensions> SQRBO <range> DOTDOT <range> SQRBC <dimensions2>
//<dimensions2> SQRBO <range> DOTDOT <range> SQRBC <dimensions2>
//<dimensions2> EPS
//<singleDimension> SQRBO <range> DOTDOT <range> SQRBC
token * dimensions(token *node,treeNode *root){
	if(!strcmp(node->tokenName,"SQRBO")){
		
			treeNode *child=add_child(root,createTokenNtr("dimensions"),1,36);
			add_child(child,node,0,36);
			node=node->next;
			token *move=range(node,child);
			move=move->next;
			add_child(child,move,0,36);
			move=move->next;
			move=range(move,child);
			move=move->next;
			add_child(child,move,0,36);
			move=move->next;
			move=dimensions2(move,child);
			//move=move->next;
			return move;
		}
		else{
			return node;
		}
	}
token * dimensions2(token *node,treeNode *root){
			if(!strcmp(node->tokenName,"SQRBO")){
		
			treeNode *child=add_child(root,createTokenNtr("dimensions2"),1,37);
			add_child(child,node,0,37);
			node=node->next;
			token *move=range(node,child);
			move=move->next;
			add_child(child,move,0,37);
			move=move->next;
			move=range(move,child);
			move=move->next;
			add_child(child,move,0,37);
			move=move->next;
			move=dimensions2(move,child);
			//move=move->next;
			return move;
		}
		else{
			
			treeNode *child=add_child(root,createTokenNtr("dimensions2"),1,38);
			add_child(child,createTokenEPS(),0,38);
			return node;
		}
			
}
token* range(token *node,treeNode *root){
	if(!strcmp(node->tokenName,"ID")){
		treeNode *child=add_child(root,createTokenNtr("range"),1,39);
			add_child(child,node,0,39);
			return node;
	}
	if(!strcmp(node->tokenName,"NUM")){
		treeNode *child=add_child(root,createTokenNtr("range"),1,40);
			add_child(child,node,0,40);
			return node;
	}
	return node;
}
//<singleStaticDimension> SQRBO NUM DOTDOT NUM SQRBC
token * singleStaticDimension(token *node,treeNode *root){
	if(!strcmp(node->tokenName,"SQRBO")){
		
			treeNode *child=add_child(root,createTokenNtr("singleStaticDimension"),1,41);
			add_child(child,node,0,41);
			node=node->next;
			add_child(child,node,0,41);
			node=node->next;
			add_child(child,node,0,41);
			node=node->next;
			add_child(child,node,0,41);
			node=node->next;
			add_child(child,node,0,41);
			node=node->next;
			return node;
		}
		else{
	
			return node;
		}
}
//<index> <range> <index2>
//<index2> <range> <index2>
//<index2> EPS
token * index1(token *node,treeNode *root){
		treeNode *child=add_child(root,createTokenNtr("index"),1,42);
			token *move=range(node,child);
			move=move->next;
			move=index2(move,child);
			return move;
}
token * index2(token *node,treeNode *root){
		
			if((!strcmp(node->tokenName,"ID")) || (!strcmp(node->tokenName,"NUM"))){
			treeNode *child=add_child(root,createTokenNtr("index2"),1,43);
			token *move=range(node,child);
			move=move->next;
			move=index2(move,child);
			
			return move;
			}
			else{
				treeNode *child=add_child(root,createTokenNtr("index2"),1,44);
					add_child(child,createTokenEPS(),0,44);
					return node;
					
			}
}
//<brackets> SQRBO SQRBC SQRBO SQRBC
//<brackets> SQRBO SQRBC
token * brackets(token *node,treeNode *root){
	token *temp=node;
	temp=temp->next->next;
	if(!strcmp(temp->tokenName,"SQRBO"))
	{
		treeNode *child=add_child(root,createTokenNtr("brackets"),1,45);
		add_child(child,node,0,45);
		node=node->next;
		add_child(child,node,0,45);
		node=node->next;
		add_child(child,node,0,45);
		node=node->next;
		add_child(child,node,0,45);
		node=node->next;
			return node;
	}
	else{
	treeNode *child=add_child(root,createTokenNtr("brackets"),1,46);
		add_child(child,node,0,46);
		node=node->next;
		add_child(child,node,0,46);
		node=node->next;
			return node;
		}
}
//<assignment> <varName> EQU <expression> SEMICOLON
token * assignment(token *node,treeNode *root){
		treeNode *child=add_child(root,createTokenNtr("assignment"),1,47);
		token *move=varName(node,child);
		add_child(child,move,0,47);
		move=move->next;
		//move=expression(move,child);
		add_child(child,move,0,47);
		move=move->next;
		return move;
}
//<varName> ID
//<varName> ID SQRBO <index> SQRBC
token * varName(token *node,treeNode *root){
	token *temp=node;
	temp=temp->next;
	if(!strcmp(temp->tokenName,"SQRBO")){
	treeNode *child=add_child(root,createTokenNtr("varName"),1,49);
	add_child(child,node,0,49);
	node=node->next;
	add_child(child,node,0,49);
	node=node->next;
	token *move=index1(node,child);
	add_child(child,move,0,49);
	move=move->next;
	return move;
	}
	else{
		treeNode *child=add_child(root,createTokenNtr("varName"),1,48);
	add_child(child,node,0,48);
	node=node->next;
	return node;
	}
}
//RISHAV's works end rule no-32 to 50


// ----------------------------------------------------------------------------------------- ANIKET's REQUIREMENT
/*
token * singleStaticDimension(token *node, treeNode *root){
	// dummy 
	add_child(root, createTokenNtr("singleStaticDimension<??"), 1, 169);
	return node->next;
}
token * brackets(token *node, treeNode *root){
	// dummy 
	add_child(root, createTokenNtr("brackets<??"), 1, 169);
	for(int i=0; i<8; i++) node = node->next;
	return node;
}


token * varList(token *node, treeNode *root){
	// dummy
	add_child(root, createTokenNtr("varList<??"), 1, 169);
	return node->next->next->next;			//TODO: HARDCODED here (have faith that this func will be alright)
}	
*/
// ----------------------------------------------------------------------------------------- ANIKET's WORK


token *numberList(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("numberList"), 1, 21);
	token *a = numberList2(node, root);
	node = a;
	a = numberList3(node, root);
	node = a;
	return node;
}

token *numberList2(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("numberList2"), 1, 24);
	if(strcmp(node->tokenName, "NUM")==0){
		add_child(root, node , 0, 99);
		node = node->next;
		token *a = numberList4(node, root);
		return a;
	}
	else {
		root->data.Nonterminals.rule = 25;
		add_child(root,createTokenEPS(),0, 169);		//TODO:
		return node;
	}
}

token *numberList4(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("numberList4"), 1, 26);
	if(strcmp(node->tokenName, "NUM")==0){
		add_child(root, node, 0, 99);
		node = node->next;
		token *a = numberList4(node, root);
		return a;
	}
	else {
		add_child(root,createTokenEPS(), 0, 169);
		return node;
	}
}

token *numberList3(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("numberList3"), 1, 22);
	if(strcmp(node->tokenName, "SEMICOLON")==0){
		add_child(root, node, 0, 99);
		node = node->next;
		token *a = numberList2(node, root);
		node = a;
		a = numberList3(node, root);
		return a;
	}
	else {
		add_child(root, createTokenEPS(), 0, 169);
		return node;
	}
}

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

token *jaggedInitialisationList(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("jaggedInitialisationList"), 1, 17);
	
	token *a =jaggedInitialisation(node, root);
	node = a;
	a = jaggedInitialisationList2(node, root);
	node = a;
	return node;

}

token *jaggedInitialisationList2(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("jaggedInitialisationList2"), 1, 18);
	if(strcmp(node->tokenName, "R1")==0){
		token *a =jaggedInitialisation(node, root);
		node = a;
		a = jaggedInitialisationList2(node, root);
		node = a;
		return node;
	}
	else {
		root->data.Nonterminals.rule = 19;
		add_child(root,createTokenEPS(),0, 169);
		return node;
	}
}


token *jaggedInitialisation(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("jaggedInitialisation"), 1, 20);
	token *prev = node;
	if(strcmp(node->tokenName, "R1")==0){
		add_child(root, node, 0, 99);
		node= node->next;
		if(strcmp(node->tokenName, "SQRBO")==0){
			add_child(root, node, 0, 99);
			node= node->next;
			if(strcmp(node->tokenName, "NUM")==0){
				add_child(root, node, 0, 99);
				node= node->next;
				if(strcmp(node->tokenName, "SQRBC")==0){
					add_child(root, node, 0, 99);
					node= node->next;
					if(strcmp(node->tokenName, "COLON")==0){
						add_child(root, node, 0, 99);
						node= node->next;
						if(strcmp(node->tokenName, "size")==0){
							add_child(root, node, 0, 99);
							node= node->next;
							if(strcmp(node->tokenName, "NUM")==0){
								add_child(root, node, 0, 99);
								node= node->next;
								if(strcmp(node->tokenName, "COLON")==0){
									add_child(root, node, 0, 99);
									node= node->next;
									if(strcmp(node->tokenName, "values")==0){
										add_child(root, node, 0, 99);
										node= node->next;
										if(strcmp(node->tokenName, "CBO")==0){
											add_child(root, node, 0, 99);
											node= node->next;
											
											token *a = numberList(node, root);
											if(a==node) {printf("Parsing Error Inside Jagged Array declaration | SYNTACTICALLY Wrong | (Not assumed)\n"); return prev;}
											node =a;
											if(strcmp(node->tokenName, "CBC")==0){
												add_child(root, node, 0, 99);
												node= node->next;
												return node;
											}	
										}
									}									
								}
							}
						}
					}
				}
			}
		}
	}
	
	return prev;	
}


token * jaggedDeclaration(token *node, treeNode *root2){
	treeNode *root = add_child(root2, createTokenNtr("jaggedDeclaration"), 1, 15);
	token *prev = node;
	if(strcmp(node->tokenName, "declare")==0){
		add_child(root, node, 0, 99);
		node= node->next;
		
		// rule 1
		if(strcmp(node->tokenName, "ID")==0){
			/*
			add_child(root, "ID", 0);
						
			node = node->next;
			add_child(root, "COLON", 0);			
			
			node = node->next;
			add_child(root, "jagged", 0));			
			
			node = node->next;
			add_child(root, "array", 0);
			
			node = node->next;
			token *a = singleStaticDimension();
			*/
					
			
			add_child(root, node, 0, 99);		//added "ID"-no, the lexeme say "i1"-yes			
			node = node->next;
			if(strcmp(node->tokenName, "COLON")==0){
				add_child(root, node, 0, 99);	// "COLON"
				node = node->next;
				if(strcmp(node->tokenName, "jagged")==0){
					add_child(root, node, 0, 99); // "jagged"
					node = node->next;
					if(strcmp(node->tokenName, "array")==0){	
						add_child(root, node, 0, 99); //"array"
						//printf("YesB %d %s %s\n", node->id, node->tokenName , "hmm");
						
						node = node->next;
						
						//treeNode *child = add_child(root, "singleStaticDimension", 1);	// "singleStaticDimension" node will be added by the func itself
						token *a =  singleStaticDimension(node, root);
						
						if(a == node) {printf("Couldn't complete singleStaticDimension() but this should be must\n"); return prev;} //this must not happen
						//printf("YesA %d %s %s\n", node->id, node->tokenName , "hmm");
						node = a;
						//child = add_child(root, "brackets", 1);
						//printf("Yes %d %s %s\n", node->id, node->tokenName , "hmm");
						a = brackets(node, root);
						if(a == node) {printf("Couldn't complete brackets() but this should be must\n"); return prev;} //this must not happen
						
						node =a;
						//printf("%s", node->tokenName);
						if(strcmp(node->tokenName, "of")==0){
							//printf("Yes\n");
							add_child(root, node, 0, 99);
							node = node->next;
							if(strcmp(node->tokenName, "integer")==0){
								add_child(root, node, 0, 99);
								node = node->next;
								if(strcmp(node->tokenName, "SEMICOLON")==0){
									add_child(root, node, 0, 99);
									node = node->next;
									
									//child = add_child(root, "jaggedInitialisationList", 1);
									a = jaggedInitialisationList(node, root);  
									if(a == node) {printf("Couldn't complete jaggedInitialisationList() but this should be must\n"); return prev;} //this must not happen
									node =a;
									return node;
								}
							}
						}					
					}
				}
			}
		}
		
		
		// rule 2
		else if(strcmp(node->tokenName, "list")==0){
			root->data.Nonterminals.rule = 16;
			add_child(root, node, 0, 99);
			node = node->next;
			if(strcmp(node->tokenName, "of")==0){
				add_child(root, node, 0, 99);
				node = node->next;
				if(strcmp(node->tokenName, "variables")==0){
					add_child(root, node, 0, 99);
					node = node->next;
					
					//treeNode *child = add_child(root, "varList", 1);
					token *a =  varList(node, root);
					if(a == node) { printf("Couldn't complete varList() but this should be must\n"); return prev;} //this must not happen
					node = a;
					//printf("\n\nhi token is: %s %d\n", node->tokenName, node->id);
					
					if(strcmp(node->tokenName, "COLON")==0){
						//printf("\n\nhi\n");	
						add_child(root, node, 0, 99);
						node = node->next;
						if(strcmp(node->tokenName, "jagged")==0){
							add_child(root, node, 0, 99);
							node = node->next;
							if(strcmp(node->tokenName, "array")==0){
								add_child(root, node, 0, 99);
								node = node->next;
								
								//child = add_child(root, "singleStaticDimension", 1);
								a =  singleStaticDimension(node, root);
								if(a == node) {printf("Couldn't complete singleStaticDimension() but this should be must as syntactically correct\n"); return prev;} //this must not happen
								
								node = a;
								//child = add_child(root, "brackets", 1);
								a = brackets(node, root);
								if(a == node) {printf("Couldn't complete brackets() but this should be must\n"); return prev;} //this must not happen
								//printf("\n\nhi token is: %s %d\n", a->tokenName, a->id);								
								node =a;
								if(strcmp(node->tokenName, "of")==0){
									add_child(root, node, 0, 99);
									node = node->next;
									if(strcmp(node->tokenName, "integer")==0){
										add_child(root, node, 0, 99);
										node = node->next;
										//printf("%s\n", node->tokenName);
										if(strcmp(node->tokenName, "SEMICOLON")==0){
											add_child(root, node, 0, 99);
											node = node->next;
											//printf("%s\n", node->tokenName);
											//child = add_child(root, "jaggedInitialisationList", 1);
											a = jaggedInitialisationList(node, root);
											if(a == node) {printf("Couldn't complete jaggedInitialisationList() but this should be must\n"); return prev;} //this must not happen
											//printf("\n\nhiya the token is: %s %d\n", a->tokenName, a->id);
											node =a;
											return node;  
										}
									}
								}					
							}
						}
					}	
				}
			}
		}
		
		else
		printError2("jaggedDeclaration");
	}
	return prev;
}


// ------------------------------------------------------------------------------------------------- FANCY

int ENN=40;
void printNTree(treeNode* x, 
    int flag[ENN],  
    int depth, int isLast ) 
{ 
    // Condition when node is None 
    if (x == NULL)  
        return; 
      
    // Loop to print the depths of the 
    // current node 
    for (int i = 1; i < depth; ++i) { 
          
        // Condition when the depth  
        // is exploring 
        if (flag[i] == 1) { 
            /*cout << "| "
                << " "
                << " "
                << " ";*/ 
        	printf("|           ");
        } 
          
        // Otherwise print  
        // the blank spaces 
        else { 
            /*cout << " "
                << " "
                << " "
                << " "; */
        	printf("            ");	//12 spaces
        } 
    } 
      
    // Condition when the current 
    // node is the root node 
    if (depth == 0) 
        //cout << x->n << '\n'; 
      	printf("%s\n",x->symbol);
    // Condtion when the node is  
    // the last node of  
    // the exploring depth 
    else if (isLast) { 
        //cout << "+--- " << x->n << '\n';
        printf("+-----------%s\n", x->symbol);  
        // No more childrens turn it  
        // to the non-exploring depth 
        flag[depth] = 0; 
    } 
    else { 
        printf("+-----------%s\n", x->symbol);
    } 
  
    int it = 0; 
//    for (auto i = x->root.begin();  
//    i != x->root.end(); ++i, ++it) 

//	for(int i=0; i<10; i++) //TODO  
        // Recursive call for the 
        // children nodes 
//        printNTree(*i, flag, depth + 1,  
 //           it == (x->root.size()) - 1); 
    flag[depth] = 1; 
} 

