#include<ctype.h>
#include "parser.h"
int c=0;
int dyn=0;
void traverseParseTree(treeNode *root,typeExpressionTable *T){
	/* Traverses the parse tree and populates the type expression table

		root: treeNode
		T: typeExpressionTable array
	*/

	root = root->firstChild; // moves from start node to child;
	treeNode *t = root->firstChild;
	//traverse into depth 1 till node is declarationList(t)
	while(strcmp(t->symbol,"declarationList")){ //keep on moving horizontally till declaration statement keyword is encountered;
		t= t->nextSibling;
	}
	typeExpressionTable *temp1 = resolveDeclaration(t->firstChild,T); //resolves the first declaration
	resolveDeclarationList(t->firstChild->nextSibling,temp1);
	t = t->nextSibling;
	// checkAssignment(t->firstChild,T);
	// checkAssignmentList(t->firstChild->nextSibling,T);
}

typeExpressionTable * next_node(typeExpressionTable *t){
	typeExpressionTable *new_node = malloc(sizeof(typeExpressionTable));
	new_node->next = NULL;

	return (t->next = new_node);
}

typeExpressionTable * resolveDeclaration(treeNode *node,typeExpressionTable *T){
	/* Traverses the declara and populates the type expression table

		root: treeNode
		T: typeExpressionTable linked list
	*/

	treeNode *t= node->firstChild; 
	if(!strcmp(t->symbol,"varDeclaration")){ //Case1: varDeclar ation
		
		treeNode *t1 = t->firstChild;
		t1 = t1->nextSibling; //Ignoring declare 
		char *name = getVarName(t1); //Stores name of the variable
		while(strcmp(t1->symbol,"varType")){
			t1 = t1->nextSibling; //Ignoring colon
		} 
		char* dt = getVarType(t1); //stores variable type of variable
		typeExpressionTable *T1 = next_node(T);
		strcpy(T1->name,name);
		T1->type = 0;
		strcpy(T1->arrayType,"not_applicable");
		if(!strcmp(dt,"integer")){
			T1->typexpr.primDt = integer;
		}
		else if(!strcmp(dt,"real")){
			T1->typexpr.primDt = real;
		}
		else{
			T1->typexpr.primDt = Boolean;
		}
		T1->next=NULL;
		return T1;
	}
	else if(!strcmp(t->symbol,"listDeclaration")){ //Case 2: listDeclaration
		treeNode *t1 = t->firstChild;
		while(strcmp(t1->symbol,"varList")){
			t1 = t1->nextSibling; //Ignoring declare list of variables
		}
		int count1=0;
		getVarCount(t1->firstChild,&count1);
		char **names;
		names = (char**) malloc(count1 * sizeof(char*));
		int index =0;
		getVarList(t1->firstChild,&index,names,count1); //get names of all variables
		while(strcmp(t1->symbol,"varType")){
			t1 = t1->nextSibling; //Ignoring colon
		}
		char* dt = getVarType(t1);//gets variable type
		typeExpressionTable *T1 = T;
		for(int i=0;i<count1;i++){
			T1 = next_node(T1);
			strcpy(T1->name,names[i]);
			T1->type = 0;
			strcpy(T1->arrayType,"not_applicable");
			if(!strcmp(dt,"integer")){
				T1->typexpr.primDt = integer;
			}
			else if(!strcmp(dt,"real")){
				T1->typexpr.primDt = real;
			}
			else{
				T1->typexpr.primDt = Boolean;
			}
			T1->next=NULL;
		}
		return T1;
	}
	else if(!strcmp(t->symbol,"arrayDeclaration")){
			treeNode *t1 = t->firstChild;
			t1 = t1->nextSibling; //Ignoring declare 
			if(!strcmp(t1->symbol,"ID")){
				char *name = getVarName(t1);
				while(strcmp(t1->symbol,"dimensions")){
					t1 = t1->nextSibling; //Ignoring colon
				} 
				dyn=0;
				int dimension = 0; //Added integer dimension to store dimensions
				getDimensions(t1,&dimension);
				char ***range;
				range=(char ***)malloc(2*sizeof(char **));
				int z;
				for(z=0;z<2;z++){
					range[z]=(char **)malloc(dimension*sizeof(char*));
					for(int i=0;i<dimension;i++){
						range[z][i]=malloc(30*sizeof(char));
					}	
				}
				c=0;
				int index =0;
				getRange(t1,range,&index);
				//add integer as datatype
			//Add element to table: for this we need to store current length of T somewhere(say n) and then realloc memory of size typeExpression*(n+1) and then add this variable to T[n];
			typeExpressionTable *T1 = next_node(T);
			strcpy(T1->name,name);
			T1->type=1;
			if(dyn==1)
				strcpy(T1->arrayType,"dynamic");
				else
					strcpy(T1->arrayType,"static");
			//T[count-1].typexpr.rectangularArray=malloc(sizeof(struct RectangularArray));
			T1->typexpr.rectangularArray.type=0;
			T1->typexpr.rectangularArray.dimensions=dimension;
			T1->typexpr.rectangularArray.range[0]=range[0];
			T1->typexpr.rectangularArray.range[1]=range[1];
			T1->typexpr.rectangularArray.basicElementType=0;
			return T1;
			}
			else{
				treeNode *t1 = t->firstChild;
				while(strcmp(t1->symbol,"varList")){
					t1 = t1->nextSibling; //Ignoring colon
				}
				int count1=0;
				getVarCount(t1->firstChild,&count1);
				char **names;
				names = (char**) malloc(count1 * sizeof(char*));
				int index =0;
				getVarList(t1->firstChild,&index,names,count1); //get names of all variables
				while(strcmp(t1->symbol,"dimensions")){
					t1 = t1->nextSibling; //Ignoring colon
				}
				int dimension = 0; //Added integer dimension to store dimensions
				dyn=0;
				getDimensions(t1,&dimension);
				char ***range;
				range=(char ***)malloc(2*sizeof(char **));
				int z;
				for(z=0;z<2;z++){
					range[z]=(char **)malloc(dimension*sizeof(char*));
					for(int i=0;i<dimension;i++){
						range[z][i]=malloc(30*sizeof(char));
					}	
				}
				c=0;
				index =0;
				
				getRange(t1,range,&index);
				

				typeExpressionTable *T1 = T;
				for(int i=0;i<count1;i++){
					T1 = next_node(T1);
					strcpy(T1->name,names[i]);
					T1->type = 1;
					if(dyn==1)
						strcpy(T1->arrayType,"dynamic");
					else
						strcpy(T1->arrayType,"static");
					T1->typexpr.primDt = integer;
					//T[count-count1+i].typexpr.rectangularArray=malloc(sizeof(struct RectangularArray));
					T1->typexpr.rectangularArray.type=0;
					T1->typexpr.rectangularArray.dimensions=dimension;
					T1->typexpr.rectangularArray.range[0]=range[0];
					T1->typexpr.rectangularArray.range[1]=range[1];
					T1->typexpr.rectangularArray.basicElementType=0;
					
				}
				return T1;
				
		}
	} 
	else{//Case 4: jaggedDeclaration

		treeNode *t1 = t->firstChild;
		t1 = t1->nextSibling; //Ignoring declare
		if(!strcmp(t1->symbol,"ID")){//Case 4a: Single Jagged array
			char *name = getVarName(t1);
			while(strcmp(t1->symbol,"singleStaticDimension")){
				t1 = t1->nextSibling; //Ignoring colon jagged array
			} 
			int range_R1[2];
			range_R1[0] = atoi(t1->firstChild->nextSibling->data.Terminals.lexeme);
			range_R1[1] = atoi(t1->firstChild->nextSibling->nextSibling->nextSibling->data.Terminals.lexeme);
			t1 = t1->nextSibling;
			int dimensions=0;
			treeNode *t2 = t1->firstChild;
			while(t2!=NULL){
				dimensions++;
				t2=t2->nextSibling;
			}
			dimensions/=2;
			dimensions++;
			while(strcmp(t1->symbol,"jaggedInitialisationList")){
				t1 = t1->nextSibling; //Ignoring of integer semicolon
			} 
			typeExpressionTable *T1 = next_node(T);
			if(dimensions==2){//Case 4aa: 2D single jagged array

				int *R2 = (int *)malloc((range_R1[1]-range_R1[0]+1)*sizeof(int));
				R2[0] = resolvejaggedInitialisation2d(t1->firstChild,range_R1[0],range_R1[1]); //returns size of first row of jagged 2D array
				t1 = t1->firstChild->nextSibling;
				resolveJaggedInitialisationList2d(t1,R2,1,range_R1[0],range_R1[1]); //populates R2 and checks for type defintition error in each initialisationList
				for(int i=0;i<(range_R1[1]-range_R1[0]+1);i++){
					if(R2[i]==0){
						T1->type = 3;//Indicator that type definition error was encountered
						break;
					}
				}
				T1->typexpr.jaggedArray.R2.two_Dim = R2;
			}
			else{//Case 4ab: 3D Single jagged array
				int **R2 = (int **)malloc((range_R1[1]-range_R1[0]+1)*sizeof(int *));
				R2[0] = resolvejaggedInitialisation3d(t1->firstChild,range_R1[0],range_R1[1]);  //returns size of first row of jagged 3D array
				t1 = t1->firstChild->nextSibling;
				resolveJaggedInitialisationList3d(t1,R2,1,range_R1[0],range_R1[1]); //populates R2 and checks for type defintition error in each initialisationList
				for(int i=0;i<(range_R1[1]-range_R1[0]+1);i++){
					if(R2[i][0]==0){
						T1->type = 3; //Indicator that type definition error was encountered
						break;
					}
				}
				T1->typexpr.jaggedArray.R2.three_Dim = R2;
			}
			strcpy(T1->name,name);
			if(T1->type!=3){T1->type = 2;}
			strcpy(T1->arrayType,"static");
			T1->typexpr.jaggedArray.type = jaggedArray;
			T1->typexpr.jaggedArray.dimensions = dimensions;
			T1->typexpr.jaggedArray.range_R1[0] = range_R1[0];
			T1->typexpr.jaggedArray.range_R1[1] = range_R1[1];
			T1->typexpr.jaggedArray.basicElementType = integer;
			return T1;
		}
		else{//Case 4b :Multiple jagged array
			while(strcmp(t1->symbol,"varList")){
				t1 = t1->nextSibling; //Ignoring list of variables
			} 
			int count1=0;
			getVarCount(t1->firstChild,&count1);
			char **names;
			names = (char**) malloc(count1 * sizeof(char*));
			int index =0;
			getVarList(t1->firstChild,&index,names,count1); //get names of all variables
			while(strcmp(t1->symbol,"singleStaticDimension")){
				t1 = t1->nextSibling; //Ignoring colon jagged array
			} 
			int range_R1[2];
			range_R1[0] = atoi(t1->firstChild->nextSibling->data.Terminals.lexeme);
			range_R1[1] = atoi(t1->firstChild->nextSibling->nextSibling->nextSibling->data.Terminals.lexeme);
			t1 = t1->nextSibling;
			int dimensions=0;
			treeNode *t2 = t1->firstChild;
			while(t2!=NULL){
				dimensions++;
				t2=t2->nextSibling;
			}
			dimensions/=2;
			dimensions++;
			while(strcmp(t1->symbol,"jaggedInitialisationList")){
				t1 = t1->nextSibling; //Ignoring colon jagged array
			} 
			typeExpressionTable *T1 = T; 
			int type = 2;
			if(dimensions==2){
				int *R2 = (int *)malloc((range_R1[1]-range_R1[0]+1)*sizeof(int));
				R2[0] = resolvejaggedInitialisation2d(t1->firstChild,range_R1[0],range_R1[1]);
				t1 = t1->firstChild->nextSibling;
				resolveJaggedInitialisationList2d(t1,R2,1,range_R1[0],range_R1[1]);

				for(int i=0;i<(range_R1[1]-range_R1[0]+1);i++){
					if(R2[0]==0){
						type = 3;
						break;
					}
				}
				for(int i=0;i<count1;i++){
					T1 = next_node(T1);
					T1->typexpr.jaggedArray.R2.two_Dim = R2;
					strcpy(T1->name,names[i]);
					T1->type = type;
					strcpy(T1->arrayType,"static");
					T1->typexpr.jaggedArray.type = jaggedArray;
					T1->typexpr.jaggedArray.dimensions = dimensions;
					T1->typexpr.jaggedArray.range_R1[0] = range_R1[0];
					T1->typexpr.jaggedArray.range_R1[1] = range_R1[1];
					T1->typexpr.jaggedArray.basicElementType = integer;
					T1->next =NULL;
				}
				return T1;
			}
			else{
				int **R2 = (int **)malloc((range_R1[1]-range_R1[0]+1)*sizeof(int *));
				R2[0] = resolvejaggedInitialisation3d(t1->firstChild,range_R1[0],range_R1[1]);
				t1 = t1->firstChild->nextSibling;

				resolveJaggedInitialisationList3d(t1,R2,1,range_R1[0],range_R1[1]);
				for(int i=0;i<(range_R1[1]-range_R1[0]+1);i++){
					if(R2[i][0]==0){
						type = 3;
						break;
					}
				}
				for(int i=0;i<count1;i++){
					T1 = next_node(T1);
					T1->typexpr.jaggedArray.R2.three_Dim = R2;
					strcpy(T1->name,names[i]);
					T1->type = type;
					strcpy(T1->arrayType,"static");
					T1->typexpr.jaggedArray.type = jaggedArray;
					T1->typexpr.jaggedArray.dimensions = dimensions;
					T1->typexpr.jaggedArray.range_R1[0] = range_R1[0];
					T1->typexpr.jaggedArray.range_R1[1] = range_R1[1];
					T1->typexpr.jaggedArray.basicElementType = integer;
					T1->next =NULL;
				}
				return T1;
			}
		}
	}
}

void resolveDeclarationList(treeNode *node,typeExpressionTable *T){
	treeNode *temp = node->firstChild;
	if(strcmp(temp->symbol,"EPS")){
		typeExpressionTable *T1 = resolveDeclaration(temp,T);
		resolveDeclarationList(temp->nextSibling,T1);
	}
}

int resolvejaggedInitialisation2d(treeNode *node,int low, int high){
	/*Returns size of a 2d jagged matrix row and checks for error if any
		
		node: treeNode containg jaggedInitialisation
		low: minimum possible allowed row index
		high: maximum possible allowed row index
	*/

	treeNode *temp = node->firstChild;
	while(strcmp(temp->symbol,"NUM")){
		temp = temp->nextSibling; //ignoring R1 SQRBO 
	}
	int lno = temp->data.Terminals.lno;
	int index = atoi(temp->data.Terminals.lexeme);
	if(index<low || index>high){ // Checks index mismatch error
		char *message = "Index mismatch error encountered: ";
		printTypeError(lno,0,"***","***","***","***","***",0,message);
		return 0;
	}

	temp = temp->nextSibling;
	while(strcmp(temp->symbol,"NUM")){
		temp = temp->nextSibling; //ignoring SQRBC COLON size
	}
	int result = atoi(temp->data.Terminals.lexeme);
	while(strcmp(temp->symbol,"numberList")){
		temp = temp->nextSibling;
	}
	int error = checkNumberList2d(temp,result); //checks 2D size mismatch error
	if(error){	
		char *message = "2D Jagged array size mismatch";
		printTypeError(lno,0,"***","***","***","***","***",0,message);
		return 0;
	}
	return result;
}

void resolveJaggedInitialisationList2d(treeNode *node,int*arr,int index,int low, int high){
	/* Returns size and checks for type definition error for each statement
		node: treeNode containing jaggedInitialisationList
		arr: contains array of size
		index: index where to store
		low: minimum range of index of jagged array
		high: maximum range of index of jagged array
	*/
	treeNode *temp = node->firstChild;
	if(strcmp(temp->symbol,"EPS")){
		int size = resolvejaggedInitialisation2d(temp,low,high);
		arr[index] = size;
		index++;
		resolveJaggedInitialisationList2d(temp->nextSibling,arr,index,low,high);
	}
	return;
}

int *resolvejaggedInitialisation3d(treeNode *node,int low, int high){
	/*Returns size of a 3d jagged matrix row and checks for error if any
		
		node: treeNode containg jaggedInitialisation
		low: minimum possible allowed row index
		high: maximum possible allowed row index
	*/
	treeNode *temp = node->firstChild;
	while(strcmp(temp->symbol,"NUM")){
		temp = temp->nextSibling;
	}
	int lno = temp->data.Terminals.lno;
	int index = atoi(temp->data.Terminals.lexeme);
	if(index<low || index>high){ // Checks index mismatch error
		char *message = "Index mismatch error encountered in 3d jagged array ";
		printTypeError(lno,0,"***","***","***","***","***",0,message);
		int *result=(int *)malloc((2)*sizeof(int));
		result[0]=0;
		return result;
	}
	temp = temp->nextSibling;
	while(strcmp(temp->symbol,"NUM")){
		temp = temp->nextSibling; //ignoring SQRBC COLON size
	}
	int size = atoi(temp->data.Terminals.lexeme);
	while(strcmp(temp->symbol,"numberList")){
		temp = temp->nextSibling;
	}
	int *result = (int *)malloc((size+1)*sizeof(int));
	result[0] = size;
	int size_check = 0;
	dfsNumberList(temp->firstChild,&size_check);
	if(size_check!=size){//Checks size mismatch error
		char *message = "3d jagged array size mismatch error";
		printTypeError(lno,0,"***","***","***","***","***",0,message);
		result[0] = 0;
		return result;
	}
	temp = temp->firstChild;
	int count = 1;
	while(temp!=NULL){
		int value = 0;
		dfsNum(temp->firstChild,&value);
		result[count] = value;
		count++;
		temp = temp->nextSibling->firstChild->nextSibling;
	}
	return result;
}

void resolveJaggedInitialisationList3d(treeNode *node,int** arr,int index,int low,int high){
	/* Returns size and checks for type definition error for each statement
		node: treeNode containing jaggedInitialisationList
		arr: contains array of size
		index: index where to store
		low: minimum range of index of jagged array
		high: maximum range of index of jagged array
	*/
	treeNode *temp = node->firstChild;
	if(strcmp(temp->symbol,"EPS")){
		int *size = resolvejaggedInitialisation3d(temp,low,high);
		arr[index] = size;
		index++;
		resolveJaggedInitialisationList3d(temp->nextSibling,arr,index,low,high);
	}
	return;
}

int checkNumberList2d(treeNode *node,int size){
	/* Checks for size mismatch error in 2d jagged array
		node: treeNode containing numList
		size: expected size

		returns 0: no error, 1: less elements than expected, 2: more than one element in a cell
	*/
	int no_of_elements = 0;
	dfsNum(node->firstChild,&no_of_elements); //finds number of NUM in numberList
	if(no_of_elements!=size){
		return 1;
	}
	int error = 0;
	dfsNumberList4(node->firstChild,&error);; //finds count of numberList4 which are not EPS
	return error;

}

void dfsNum(treeNode *node,int *count){
	/* Checks for number of NUM in a given numberList
		node: treeNode containing numberList
		count: count of NUMS
	*/
	treeNode *temp = node;
  	if(temp==NULL) return;
    if(!strcmp(temp->symbol,"NUM")){
    	*count +=1;
    }
    dfsNum(temp->firstChild,count);
    dfsNum(temp->nextSibling,count);
}

void dfsNumberList4(treeNode *node,int *error){
	/* checks if some numberList4 is expanded with non-eps rule
		node: treeNode containing numberList
		error: 0 if no error else 2
	*/
	treeNode *temp = node;
  	if(temp==NULL) return;
  	if(!strcmp(temp->symbol,"numberList4")){
    	if(temp->data.Nonterminals.rule!=27){
    		*error = 2;
    	}
    }
  	dfsNumberList4(temp->firstChild,error);
    dfsNumberList4(temp->nextSibling,error);
}

void dfsNumberList(treeNode *node,int *size_check){
	/* Checks for number of numberList2 in a given numbrList
		node: treeNode containing numberList
		size_check: count of numberList2
	*/
	treeNode *temp = node;
  	if(temp==NULL) return;
  	if(!strcmp(temp->symbol,"numberList2")){
    	*size_check +=1;
    }
  	dfsNumberList(temp->firstChild,size_check);
    dfsNumberList(temp->nextSibling,size_check);
}


char *getVarName(treeNode *t){
	/* Returns the name of individual variable name

		t: treeNode(leaf node) containing ID;
	*/
	return t->data.Terminals.lexeme;
}

char *getVarType(treeNode *t){
	/* Returns the variabletype of variable(s)

		t: treeNode(leaf node) containing varType;
	*/
	return t->firstChild->symbol;
}

void getVarList(treeNode *node,int *number,char **names,int count){
	/* Updates the array with all the variable names

		node: treeNode containing varList
		number: count of number of variables
		names: array of stringd containing the variables
	*/
	treeNode *temp = node;
	if(temp==NULL || *number==count) return;
	if(!strcmp(temp->symbol,"ID")){
		*number += 1;
		int value = *number;
		names[value-1] = temp->data.Terminals.lexeme;
	}
	getVarList(temp->firstChild,number,names,count);
	getVarList(temp->nextSibling,number,names,count);
}

void getVarCount(treeNode *node,int *count){
	treeNode *temp = node;
	if(temp==NULL) return;
	if(!strcmp(temp->symbol,"ID")){
		*count += 1;
	}
	getVarCount(temp->firstChild,count);
	getVarCount(temp->nextSibling,count);
}


void printypeExpressionTable(typeExpressionTable *T){
	while(T!=NULL){
		printf("%s %d %s ",T->name,T->type,T->arrayType);
		if(T->type == 0){
			if(T->typexpr.primDt == integer) printf("integer");
			if(T->typexpr.primDt == real) printf("real");
			if(T->typexpr.primDt == Boolean) printf("boolean");
		}
		else if(T->type ==1){
			if(T->typexpr.rectangularArray.type == rectangularArray) printf("Rectangular Array ");
			printf("Dimensions: %d ",T->typexpr.rectangularArray.dimensions);
			for(int i=0;i<T->typexpr.rectangularArray.dimensions;i++){
					printf("( %s %s ), ",T->typexpr.rectangularArray.range[0][i],T->typexpr.rectangularArray.range[1][i]);
			}
			printf("basicElementType: integer");
		}
		else if(T->type == 2){
			if(T->typexpr.jaggedArray.type == jaggedArray) printf("Jagged Array ");
			printf("Dimensions: %d ",T->typexpr.jaggedArray.dimensions);
			printf("range_R1: (%d,%d) ",T->typexpr.jaggedArray.range_R1[0],T->typexpr.jaggedArray.range_R1[1]);
			printf("range_R2: ");
			if(T->typexpr.jaggedArray.dimensions ==2){
				for(int i=0;i<(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0]+1);i++){
					printf("%d ",T->typexpr.jaggedArray.R2.two_Dim[i]);
				}
			}
			else{
				for(int i=0;i<(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0]+1);i++){
					printf("%d [", T->typexpr.jaggedArray.R2.three_Dim[i][0]);
					for(int j=1;j<(T->typexpr.jaggedArray.R2.three_Dim[i][0]+1);j++){
						printf("%d ", T->typexpr.jaggedArray.R2.three_Dim[i][j]);
					}
					printf("], ");
				}
			}
			
			printf("basicElementType: integer");

		}
		else{
			printf(" Type declaration error ");
		}
		printf("\n");
		T = T->next;
	}
}

void getDimensions(treeNode *node, int *dimension){
	treeNode *t = node;
	if(t==NULL)
		return;
	getDimensions(t->firstChild,dimension);
	if(!strcmp(t->symbol,"SQRBO"))
		*dimension +=1;
	getDimensions(t->nextSibling,dimension);
}

void getRange(treeNode *node,char ***range,int *i){
	treeNode *t = node;
	if(t==NULL)
		return;
	getRange(t->firstChild,range,i);
	//getDimensions(t->nextSibling)
	
	
	if(!strcmp(t->symbol,"ID")){
		dyn=1;
		if(c%2==0){
			c++;
			int index = *i;
			range[0][index] = t->data.Terminals.lexeme;
			//range[i][0]=t->data.Terminals.lexeme;
			//add 1st index
			getRange(t->nextSibling,range,i);
		}
		else{
			//arraycount++;
			c++;
			int index = *i;
			//add 2nd index
			range[1][index] = t->data.Terminals.lexeme;
			//range[i][1]=t->data.Terminals.lexeme;
			*i +=1;
			getRange(t->nextSibling,range,i);
		}
	}
	else if(!strcmp(t->symbol,"NUM")){
		if(c%2==0){
			c++;
			//add 1st index
			int index = *i;
			range[0][index] = t->data.Terminals.lexeme;
			getRange(t->nextSibling,range,i);
		}
		else{
			//arraycount++;
			c++;
			int index = *i;
			//add 2nd index
			range[1][index] = t->data.Terminals.lexeme;
			*i +=1;
			getRange(t->nextSibling,range,i);
		}
	}
	else
		getRange(t->nextSibling,range,i);
}

void printTypeError(int lno, int statementType, char *operator, char *firstOperand, char* firstOperandType,char *secondOperand, char* secondOperandType, int depth, char *message){
	char *statements = (char *)malloc(15*sizeof(char));
	if(statementType){
		strcpy(statements,"Assignment");	}
	else{
		strcpy(statements,"Declaration");
	}
	printf("Error encountered in Line %d. Statement Type: %s \n Operator: %5s , First operand: %5s (%3s) , Second Operand: %5s(%3s) Depth: %d \n Message: %5s \n", lno,statements,operator,firstOperand,firstOperandType,secondOperand,secondOperandType,depth,message);
}