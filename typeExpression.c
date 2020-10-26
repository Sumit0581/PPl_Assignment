void traverseParseTree(treeNode *root,typeExpressionTable *T){
	root = root->firstChild; // moves from start node to child;
	treeNode *t = root;
	//traverse into depth 1 till node is declarationList(t)
	while(strcmp(t->symbol,"DeclarationStatement")){
		t= t->nextSibling;
	}
	resolveDeclaration(t->firstChild,T);
	resolveDeclarationList(t->firstChild->nextSibling,T);
	t = t->nextSibling;
	checkAssignment(t->firstChild,T);
	checkAssignmentList(t->firstChild->nextSibling,T);
}

void resolveDeclaration(treeNode *t,typeExpressionTable *T){
	t= t->firstChild;
	if(!strcmp(t->symbol,"varDeclaration")){
		treeNode *t1 = t->firstChild;
		t1 = t1->nextSibling; //Ignoring declare 
		char *name = getVarName(t1);
		t1 = t1->nextSibling; //Ignoring colon
		char* dt = getVarType(t1);
		//Add element to table: for this we need to store current length of T somewhere(say n) and then realloc memory of size typeExpression*(n+1) and then add this variable to T[n];
	}
	Case1:
		first child of t is declare(ignore)
		//get variable name from second child
		third child is colon(ignore)
		//get variable type
		add variable to table
	Case2:
		ignore first four childs(declare list of variables)
		//count all IDs in this subtree to ge a number of variables(simple DFS)
		// get all variables from varList
		ignore colon
		//get vartype
		add all to table
	Case3:
		depending on second child decide which declaration it is:
		case 3a:
			similar to case 1 just need to consider dimensions
		Case 3b:
			similar to case 2 just need to consider dimensions
		add single/multiple variable to table
	Case4:
		find cases as in case 3
		case 4a:
			Everything same as 3a
			//decide is 2d or 3d on basis of brackets
			Case 4aa:
				resolveJaggedInitialisation2d
				resolveJaggedInitialisationList2d;
			Case 4ab:
				resolveJaggedInitialisation3d
				resolveJaggedInitialisationList3d;
		Case 4b:
			Everything same as 3b
			//decide is 2d or 3d on basis of brackets
			Case 4aa:
				resolveJaggedInitialisation2d
				resolveJaggedInitialisationList2d;
			Case 4ab:
				resolveJaggedInitialisation3d
				resolveJaggedInitialisationList3d;
		add variable to table
}

void resolvedeclarationList(treeNode *t){
	if(not eps){
		resolvedeclaration(t->firstChild);
		resolvedeclarationList(t->firstChild->nextSibling);
	}
}

int resolvejaggedInitialisation2d{
	get size from here;
	type checking of numberList;
	return size;
}

int* resolveJaggedInitialisationList2d(treeNode *t,int*arr){
	if it is eps return arr;
	else{
		size = resolvejaggedInitialisation2d(firstChild)
		append size to arr;
		call resolveJaggedInitialisationList2d(t->firstChild->nextSibling,arr)
		return arr;
	}
}

int *resolvejaggedInitialisation3d{
	get size from 2d and 3d here from here;
	type checking of numberList;
	return size;
}

int** resolveJaggedInitialisationList2d(treeNode *t,int** arr){
	if it is eps return arr;
	else{
		size = resolvejaggedInitialisation2d(firstChild)
		append size to arr;
		call resolveJaggedInitialisationList2d(t->firstChild->nextSibling,arr)
		return arr;
	}
}


int is2dor3d(treeNode *){
	depending on rule used;
}

char *getVarName(treeNode *t){
	return t->firstChild->symbol;
}

string *getVarType(treeNode *t){
	return t->firstChild->symbol;
}

string *getVarList(count of variables){
	make a string pointer array 
	for each id node send it to get varname and populate array
	return array
}

void checkassignment(treeNode *t){
	//store typeexpression of varname(t->firstChild) in variable 'a'
	ignore EQU token
	//store typeexpression of expression (t->firstChild->nextSibing->nextSibling)in variable b
	compare a and b and return error if any;
}

typeexpression *checkVarname(treeNode *t){
	//search for variable in t->firstChild in expressionTable
	return its typeExpression
}

typeexpression *checkExpression(treeNode *t){
	Case 1: Arithmetic expression
		//check type of mul expression and store in a
		//checkArithmeticexpression2(t,a) type and store in b;
		return b;

	Case 2: Boolean expression
	similar as 1
}

typeexpression *checkmulExpression(treeNode *t){
	if integerfactor is varname
		checkvarName and store typeExpression in a
	else integerFactor is NUM and hold its typeexpression as integer
	//check type expression of mulExpression2(treeNode *t, typeExpression of a) and store in b
	return b;

}

typeexpression *checkmulExpression2(treeNode *t, typeExpression *a){
	if it is EPS return null;
	depending on mulop(mul/div) check compatibility of a and integerFactor in mulExpression2 and store in b
	//checkMulExpression2(t->nextSibling->nextSibling)and store in c
	if c is null return b
	else compare c and b and return result type whch is expected;
}

void checkAssignmentList2(){
	similaras decList2
}