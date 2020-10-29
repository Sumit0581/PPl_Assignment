//group no 34
/*
ID:             2017B3A71005P                  Name:              Aniket Upadhyay 
ID:             2017B3A70305P                  Name:                    Kanav 
ID:             2017B3A70557P                  Name:               Rishav Mishra
ID:             2017B4A70581P                  Name:                 Sumit Bisht 
*/

#include<ctype.h>
#include "parser.h"
int c=0;
int dyn=0;

typeExpressionTable *tempTable;
int firstError = 0;
Datatype data = integer;




//--------------------------------------------------------------------------------------------------------------ANIKET & K



typeExpressionTable compatibleTypeBool(typeExpressionTable t1, typeExpressionTable t2);
typeExpressionTable compatibleTypeArith(typeExpressionTable t1, typeExpressionTable t2);
typeExpressionTable divCompatibleTypeArith(typeExpressionTable t1, typeExpressionTable t2); //compatible for div operations

// use printypeExpressionTable

// typeExpression t1;
// t1.primDt = ERROR;	print ERROR
// t1.primDt = NIL;	print --

// are these useful?
bool isError(typeExpression t1);
bool isNil(typeExpression t2);

void printTypeErrorKanav(treeNode *t){return;}



typeExpressionTable nilTable(){
  typeExpressionTable nil_rep;
  nil_rep.type = 0;
  nil_rep.typexpr.primDt = 4;
  return nil_rep;
}


//aniket
void printTypeTable(typeExpressionTable *T){
		printf("here-> %s, %d, %s, ",T->name,T->type,T->arrayType);
		if(T->type == 0){
			if(T->typexpr.primDt == 3) printf("ERROR");
			if(T->typexpr.primDt == 4) printf("NIL");
			
			if(T->typexpr.primDt == 0) printf("integer");
			if(T->typexpr.primDt == 1) printf("real");
			if(T->typexpr.primDt == 2) printf("boolean");
			
			
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
			printf(" Type declaration error ");	//TODO: type=0, primDt=3
		}
		//printf("\n");
}

char * printTypeTable2(typeExpressionTable *T){
		//printf("%25s %25s %25s %25s \n","Field 1","Field 2","Field 3","Field 4");
	
		//printf("%23s ",T->name);
		//printf("  <");
		char *str = (char *) malloc(200 * sizeof(char));
		//char str[200]="<";
		strcat(str,"<");
		if(T->type == 0){
			//printf("%s,%14s, ","Primitive","not_applicable");
			strcat(str,"arrType=N/A,");
			//printf("%9s","type = ");
			strcat(str," type=");
			if(T->typexpr.primDt == integer) strcat(str," integer");//printf(" integer");
			if(T->typexpr.primDt == real) strcat(str," real");
			if(T->typexpr.primDt == Boolean) strcat(str," boolean");
			//printf(">");
			if(T->typexpr.primDt == NIL) strcat(str," NOT REQUIRED");
			if(T->typexpr.primDt == ERROR) strcat(str," TYPE ERROR in this subtree");
			strcat(str,">");
		}
		else if(T->type ==1){
			//printf("%17s,%14s, ","Rectangular Array",T->arrayType);
			//strcat(str,"arrType=N/A");
			strcat(str,T->arrayType);
			//printf("%9s","type = ");
			strcat(str,", type=");
			if(T->typexpr.rectangularArray.type == rectangularArray) strcat(str," rectangularArray,");
			strcat(str,"dimensions=");
			//printf("=%d,",T->typexpr.rectangularArray.dimensions);
			char buff[10];
			sprintf(buff, "%d",T->typexpr.rectangularArray.dimensions );
			strcat(str,buff );
			strcat(str,",");
			for(int i=0;i<T->typexpr.rectangularArray.dimensions;i++){
				//printf("range_R%d=",i+1);
				strcat(str,"range_R");
				char buffer[10];
				sprintf(buffer, "%d", i+1);
				strcat(str,buffer);
				strcat(str,"=(");
				//printf("(%s,%s)",T->typexpr.rectangularArray.range[0][i],T->typexpr.rectangularArray.range[1][i]);
				strcat(str,T->typexpr.rectangularArray.range[0][i]);
				strcat(str,",");
				strcat(str, T->typexpr.rectangularArray.range[1][i]);
				strcat(str, "),");
				//printf(",");
			}
			strcat(str, "basicElementType=integer>");
		}
		else if(T->type == 2){
			//printf("%s,%14s, ","Jagged Array",T->arrayType);
			//strcat(str,"Rect Array ,");
			strcat(str,T->arrayType);
			//printf("%9s","<type = ");
			strcat(str,", type=");
			if(T->typexpr.jaggedArray.type == jaggedArray) strcat(str," jaggedArray,");
			strcat(str, "dimensions");
			char buffer[100];
			sprintf(buffer, "=%d,",T->typexpr.jaggedArray.dimensions);
			strcat(str, buffer);
			sprintf(buffer,"range_R1=(%d,%d),",T->typexpr.jaggedArray.range_R1[0],T->typexpr.jaggedArray.range_R1[1]);
			strcat(str, buffer);
			sprintf(buffer, "range_R2=(");
			strcat(str, buffer);
			
			if(T->typexpr.jaggedArray.dimensions ==2){
				for(int i=0;i<(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0]+1);i++){
					sprintf(buffer, "%d",T->typexpr.jaggedArray.R2.two_Dim[i]);
					strcat(str, buffer);
					if(i!=(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0])) strcat(str, ",");
				}
			}
			else{
				for(int i=0;i<(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0]+1);i++){
					sprintf(buffer, "%d[", T->typexpr.jaggedArray.R2.three_Dim[i][0]);
					strcat(str,buffer);
					for(int j=1;j<(T->typexpr.jaggedArray.R2.three_Dim[i][0]+1);j++){
						sprintf(buffer,"%d", T->typexpr.jaggedArray.R2.three_Dim[i][j]);
						strcat(str, buffer);
						if(j!=(T->typexpr.jaggedArray.R2.three_Dim[i][0])) strcat(str,",");
					}
					strcat(str,"]");
					if(i!=(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0])) strcat(str,",");
				}
			}
			
			strcat(str,"),basicElementType=integer>");

		}
		else{
			strcat(str, "--- ,  ---");
			strcat(str," Type declaration error ");
		}
		//printf("\n \n");
		return str;
}


void driverfunc(typeExpressionTable *T){

	typeExpressionTable tError;
	tError.type = 0;
	strcpy(tError.arrayType,"not_applicable");
	strcpy(tError.name, "ERROR");
	tError.typexpr.primDt = ERROR;

	typeExpressionTable tNIL;
	tNIL.type = 0;
	strcpy(tNIL.arrayType,"not_applicable");
	strcpy(tNIL.name,"NIL");
	tNIL.typexpr.primDt = NIL;

	
	
	typeExpressionTable U[26];
	for(int i=0; i<26; i++){
		U[i] = *T;
		T = T->next;
	}
	
	typeExpressionTable V1 = U[14], V2 =U[15] ;
	
	typeExpressionTable x, y, z;
	x = compatibleTypeBool(V1, V2);
	y = compatibleTypeArith(V1,V2);
	z = divCompatibleTypeArith(V1,V2);
	printf("\nPrinting\n");
	printTypeTable(&V1);
	printTypeTable(&x);
	printTypeTable(&y);
	printTypeTable(&z);
	
	/*typeExpressionTable t[11], u[11];
	strcpy(t[0].name, "v1-int");
	strcpy(t[1].name, "v2-bool");
	strcpy(t[2].name, "v3-real");
	strcpy(t[3].name, "v4-erro");
	strcpy(t[4].name, "v5-nil");
	strcpy(t[5].name, "v6-recAr1");
	strcpy(t[6].name, "v7-recAr2");
	strcpy(t[7].name, "v8-recAr3");
	strcpy(t[8].name, "v9-jagArr2");
	strcpy(t[9].name, "v10-jagArr3");
	strcpy(t[10].name, "v11");
	
	t[0].type = 0; //int
	t[1].type = 0;	//bool
	t[2].type = 0;	//real
	t[3].type = 0;	//error
	t[4].type = 0; //NILL
	t[5].type = 1;	//a[]
	t[6].type = 1;	//a[][]
	t[7].type = 2;	//a[][][]
	t[8].type = 3;	//j[][]
	t[9].type = 3;	//j[][][]
	t[10].type = 3;
	
	t[0].typexpr.primDt = integer;
	t[1].typexpr.primDt = Boolean;
	t[2].typexpr.primDt = real;
	t[3].typexpr.primDt = ERROR;
	t[4].typexpr.primDt = NIL;
	
	t[5].typexpr.rectangularArray.dimensions = 1;
	t[5].typexpr.rectangularArray.range[1][2] = {
						1,2
						};
	t[6].typexpr.rectangularArray.dimensions = 2;
	t[6].typexpr.rectangularArray.range = { 
						{1,3},
						{2,5}
						};	//{ {1,2,3},{4,5,6} };
	t[7].typexpr.rectangularArray.dimensions = 3;
	t[7].typexpr.rectangularArray.range = { 
						{1,3},
						{2,5},
						{4,6}
						};	//{ {{1,2,3},{4,5,6},{7,8,9}} , {{10,11,12},{13,14,15},{16,17,18}} };
	
	t[8].typexpr.jaggedArray.dimensions = 2; 
	t[8].typexpr.jaggedArray.range_R1 = {2,5};
	t[8].typexpr.jaggedArray.R2.two_Dim = { 5,3,6,5 };//{ {1,2,3},{4,5,6} };
	
	t[9].typexpr.jaggedArray.dimensions = 3;
	t[9].typexpr.jaggedArray.range_R1 = {2,3};
	t[9].typexpr.jaggedArray.R2.three_Dim = { 
						{3,4,1,3},
						{1,3}
						};
	*/
	/*
	for(int i=0; i<5; i++){
		printTypeTable(t[i]);
	}

	u[0].type = 0; //int
	u[1].type = 0;	//bool
	u[2].type = 0;	//real
	u[3].type = 0;	//error
	u[4].type = 0; //NILL
	
	u[0].typexpr.primDt = integer;
	u[1].typexpr.primDt = Boolean;
	u[2].typexpr.primDt = real;
	u[3].typexpr.primDt = ERROR;
	u[4].typexpr.primDt = NIL;
	
	strcpy(u[0].name, "u1-int");
	strcpy(u[1].name, "u2-bool");
	strcpy(u[2].name, "u3-real");
	strcpy(u[3].name, "u4-error");
	strcpy(u[4].name, "u5-nil");
	
	
	typeExpressionTable tError;
	tError.type = 0;
	//strcpy(tError.arrayType,"not_applicable");
	strcpy(tError.name,"ERROR");
	tError.typexpr.primDt = ERROR;
	
	typeExpressionTable tNIL;
	tNIL.type = 0;
	//strcpy(tNIL.arrayType,"not_applicable");
	strcpy(tNIL.name,"NIL");
	tNIL.typexpr.primDt = NIL;

	
	
	printTypeTable(tError);
	printTypeTable(tNIL);
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			printf("\n\n");
			printTypeTable(compatibleTypeBool(u[i],t[j]));
			printTypeTable(compatibleTypeArith(u[i],t[j]));
			printTypeTable(divCompatibleTypeArith(u[i],t[j]));
			printf("\n\n");
		}
	}
	*/
}

typeExpressionTable compatibleTypeBool(typeExpressionTable t1, typeExpressionTable t2){
	typeExpressionTable tError;
	tError.type = 0;
	strcpy(tError.arrayType,"not_applicable");
	strcpy(tError.name, "ERROR");
	tError.typexpr.primDt = ERROR;

	typeExpressionTable tNIL;
	tNIL.type = 0;
	strcpy(tNIL.arrayType,"not_applicable");
	strcpy(tNIL.name,"NIL");
	tNIL.typexpr.primDt = NIL;

	//printf("Inside Bool: %s %d %d, %s %d %d\n",t1.name, t1.type, t1.typexpr.primDt, t2.name, t2.type, t2.typexpr.primDt);
	if( t1.type==0 && t1.typexpr.primDt==Boolean && t2.type==0 && t2.typexpr.primDt==Boolean) { strcpy(t1.name, ""); return t1; }
	else if( t1.type==0 && t1.typexpr.primDt==NIL && t2.type==0 && t2.typexpr.primDt==Boolean) {
		//nil OP bool
		return t2;
	}
	else if( t1.type==0 && t1.typexpr.primDt==Boolean && t2.type==0 && t2.typexpr.primDt==NIL) {
		//nil OP bool
		return t1;
	}
	else if( t1.type==0 && t1.typexpr.primDt==NIL && t2.type==0 && t2.typexpr.primDt==NIL) {
		//nil OP nil
		strcpy(t1.name, "tNIL");
		return t1;
	}
	else return tError;

}

typeExpressionTable compatibleTypeArith(typeExpressionTable t1, typeExpressionTable t2){	
	typeExpressionTable tError;
	tError.type = 0;
	strcpy(tError.arrayType,"not_applicable");
	strcpy(tError.name, "ERROR");
	tError.typexpr.primDt = ERROR;

	typeExpressionTable tNIL;
	tNIL.type = 0;
	strcpy(tNIL.arrayType,"not_applicable");
	strcpy(tNIL.name,"NIL");
	tNIL.typexpr.primDt = NIL;


	if( (t1.type==0 && t1.typexpr.primDt==Boolean) || ( t2.type==0 && t2.typexpr.primDt==Boolean) )return tError;
	
	
	if(t1.type==0 && t1.typexpr.primDt==NIL) {strcpy(t2.name, ""); return t2;}
	if(t2.type==0 && t2.typexpr.primDt==NIL) {strcpy(t1.name, ""); return t1;}
		
	if(t1.type!=t2.type) {return tError;}	//TODO upadte if we can add a jagged array and rect array of same size 
	else if(t1.type == 0){
	
		if(t1.typexpr.primDt == t2.typexpr.primDt) {strcpy(t1.name, ""); return t1;}
		else return tError;
	}
	else if(t1.type == 1){ // rect array
		//check if both dynamic or static
		if(strcmp(t1.arrayType,t2.arrayType)!=0) return tError;
		// checking dimension and range only
		if(t1.typexpr.rectangularArray.dimensions == t1.typexpr.rectangularArray.dimensions){
			for(int i=0; i<t1.typexpr.rectangularArray.dimensions; i++){
				// range diff must be same not necessarily range start and end point	//TODO asked Mam: she responded with:start and end must also be identical
				if(strcmp(t1.typexpr.rectangularArray.range[0][i], t2.typexpr.rectangularArray.range[0][i])!=0) return tError;
				if(strcmp(t1.typexpr.rectangularArray.range[1][i], t2.typexpr.rectangularArray.range[1][i])!=0) return tError;

			}
			strcpy(t1.name, ""); return t1;
		}
		else return tError;
	
	}
	else if(t1.type == 2){ 
		//jaggedArray

		//check if both dynamic or static
		//if(strcmp(t1.arrayType,t2.arrayType)!=0) return tError;	// jegged array is always static

		// range diff must be same: necessarily range start and end point same	//TODO asked Mam
		if(t1.typexpr.jaggedArray.range_R1[0] != t2.typexpr.jaggedArray.range_R1[0]) return tError;
		if(t1.typexpr.jaggedArray.range_R1[1] != t2.typexpr.jaggedArray.range_R1[1]) return tError;		
		
		if(t1.typexpr.jaggedArray.dimensions == t1.typexpr.jaggedArray.dimensions){
			if(t1.typexpr.jaggedArray.dimensions ==2){
				for(int i=0; i<(t1.typexpr.jaggedArray.range_R1[1]-t1.typexpr.jaggedArray.range_R1[0]+1); i++){
					if(t1.typexpr.jaggedArray.R2.two_Dim[i] != t2.typexpr.jaggedArray.R2.two_Dim[i]) return tError;
				}
				strcpy(t1.name, ""); return t1;
			}
			
			else if(t1.typexpr.jaggedArray.dimensions ==3){
				for(int i=0; i<(t1.typexpr.jaggedArray.range_R1[1]-t1.typexpr.jaggedArray.range_R1[0]+1); i++){
					//int t1diff2 = sizeof(t1.typexpr.jaggedArray.R2.three_Dim[i]) / sizeof(int); 
					//int t2diff2 = sizeof(t2.typexpr.jaggedArray.R2.three_Dim[i]) / sizeof(int);
					//if(t1diff2 != t2diff2 ) return tError;
					if(t1.typexpr.jaggedArray.R2.three_Dim[i][0] == t2.typexpr.jaggedArray.R2.three_Dim[i][0]){
						for(int j=1; j<t1.typexpr.jaggedArray.R2.three_Dim[i][0]+1 ; j++){
							if(t1.typexpr.jaggedArray.R2.three_Dim[i][j] != t2.typexpr.jaggedArray.R2.three_Dim[i][j]) return tError;
						}
					}
					else return tError;
				}
				strcpy(t1.name, ""); return t1;
			}
		
		}
		else return tError;
	
	
	}
}

typeExpressionTable divCompatibleTypeArith(typeExpressionTable t1, typeExpressionTable t2){ //compatible for div operations
	
	typeExpressionTable tError;
	tError.type = 0;
	strcpy(tError.arrayType,"not_applicable");
	strcpy(tError.name, "ERROR");
	tError.typexpr.primDt = ERROR;

	typeExpressionTable tNIL;
	tNIL.type = 0;
	strcpy(tNIL.arrayType,"not_applicable");
	strcpy(tNIL.name,"NIL");
	tNIL.typexpr.primDt = NIL;	
	
	//if(t1==NULL) t1=tNIL;
	//if(t2==NULL) t2=tNIL;
	
	
	
	if( (t1.type==0 && t1.typexpr.primDt==Boolean) || ( t2.type==0 && t2.typexpr.primDt==Boolean) )return tError;
	
	if(t1.type==0 && t1.typexpr.primDt==NIL) {
		//strcpy(t2.name, ""); 
		return t2;}
	if(t2.type==0 && t2.typexpr.primDt==NIL){
		//strcpy(t1.name, ""); 
		return t1;}
		
	if(t1.type!=t2.type) {return tError;}	//TODO upadte if we can add a jagged array and rect array of same size 
	else if(t1.type == 0){ 
	
		if(t1.typexpr.primDt == t2.typexpr.primDt) {
			if(t1.typexpr.primDt == integer) {
				t1.typexpr.primDt = real;
			}
			//strcpy(t1.name, ""); 
			return t1;
		}
		else return tError;
	}
	else if(t1.type == 1){ // rect array
		// checking dimension and range only
		if(t1.typexpr.rectangularArray.dimensions == t1.typexpr.rectangularArray.dimensions){
			for(int i=0; i<t1.typexpr.rectangularArray.dimensions; i++){
				// range diff must be same not necessarily range start and end point	//TODO asked Mam
				int t1diff = t1.typexpr.rectangularArray.range[i][1] - t1.typexpr.rectangularArray.range[i][0];
				int t2diff = t2.typexpr.rectangularArray.range[i][1] - t2.typexpr.rectangularArray.range[i][0];
				if(t1diff != t2diff) return tError;
			}
			// TODO NOTE: since basic type of an array is always integer, even after array el-wise division will yield an integer array not real array
			//strcpy(t1.name, ""); 
			return t1;
		}
		else return tError;
	
	}
	else if(t1.type == 2){ 
		//jaggedArray
		// range diff must be same not necessarily range start and end point	//TODO asked Mam
		int t1diff = t1.typexpr.jaggedArray.range_R1[1] - t1.typexpr.jaggedArray.range_R1[0];
		int t2diff = t2.typexpr.jaggedArray.range_R1[1] - t2.typexpr.jaggedArray.range_R1[0];
		
		if(t1.typexpr.jaggedArray.dimensions == t1.typexpr.jaggedArray.dimensions && t1diff==t2diff){
			if(t1.typexpr.jaggedArray.dimensions ==2){
				for(int i=0; i<t1diff; i++){
					if(t1.typexpr.jaggedArray.R2.two_Dim[i] != t2.typexpr.jaggedArray.R2.two_Dim[i]) return tError;
				}
				strcpy(t1.name, ""); return t1;
			}
			
			else if(t1.typexpr.jaggedArray.dimensions ==3){
				for(int i=0; i<t1diff; i++){
					int t1diff2 = sizeof(t1.typexpr.jaggedArray.R2.three_Dim[i]) / sizeof(int); 
					int t2diff2 = sizeof(t2.typexpr.jaggedArray.R2.three_Dim[i]) / sizeof(int);
					if(t1diff2 != t2diff2 ) return tError;
					for(int j=0; j<t1diff2 ; j++){
						if(t1.typexpr.jaggedArray.R2.three_Dim[i][j] != t2.typexpr.jaggedArray.R2.three_Dim[i][j]) return tError;
					}
				}
				//strcpy(t1.name, ""); 
				return t1;
			}	
		}
		else return tError;
	
	
	}
}

















//--------------------------------------------------------------------------------------------------------------ANIKET & K (end)

void traverseParseTree(treeNode *root,typeExpressionTable *T){
	/* Traverses the parse tree and populates the type expression table

		root: treeNode
		T: typeExpressionTable array
	*/
	tempTable = T;
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
	treeNode *decStatement = t;
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
		decStatement->data.Nonterminals.typeExp = *T1;
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
		decStatement->data.Nonterminals.typeExp = *T1;
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
			if(data==ERROR) T1->type=3;
			if(dyn==1)
				strcpy(T1->arrayType,"dynamic");
				else
					strcpy(T1->arrayType,"static");
			//T[count-1].typexpr.rectangularArray=malloc(sizeof(struct RectangularArray));
			T1->typexpr.rectangularArray.type=0;
			T1->typexpr.rectangularArray.dimensions=dimension;
			T1->typexpr.rectangularArray.range[0]=range[0];
			T1->typexpr.rectangularArray.range[1]=range[1];
			T1->typexpr.rectangularArray.basicElementType=data;
			decStatement->data.Nonterminals.typeExp = *T1;
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
					if(data==ERROR) T1->type=3;
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
					T1->typexpr.rectangularArray.basicElementType=data;
					
				}
				decStatement->data.Nonterminals.typeExp = *T1;
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
			decStatement->data.Nonterminals.typeExp = *T1;
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
				decStatement->data.Nonterminals.typeExp = *T1;
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
				decStatement->data.Nonterminals.typeExp = *T1;
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
		printTypeError(lno,0,"***","***","***","***","***",temp->depth,message);
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
		printTypeError(lno,0,"***","***","***","***","***",error,message);
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
		char message[30] = "Index mismatch error 3D JA";
		printTypeError(lno,0,"***","***","***","***","***",temp->depth,message);
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
		char message[30] = "3D JA size mismatch";
		printTypeError(lno,0,"***","***","***","***","***",temp->depth,message);
		result[0] = 0;
		return result;
	}
	temp = temp->firstChild;
	int count = 1;
	while(temp!=NULL){
		int value = 0;
		dfsNum(temp->firstChild,&value);
		result[count] = value;
		if(value == 0){
			char message[30] = "3D JA R3 size mismatch";
			printTypeError(lno,0,"***","***","***","***","***",temp->depth,message);
			result[0] = 0;
			return result;
		}
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
    		*error = temp->depth+1;
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
	printf("%25s %25s %25s %25s \n","Field 1","Field 2","Field 3","Field 4");
	while(T!=NULL){
		printf("%23s ",T->name);
		if(T->type == 0){
			printf("%28s %28s ","Primitive","not_applicable");
			printf("%10s","<type = ");
			if(T->typexpr.primDt == integer) printf("integer");
			if(T->typexpr.primDt == real) printf("real");
			if(T->typexpr.primDt == Boolean) printf("boolean");
			printf(">");
		}
		else if(T->type ==1){
			printf("%28s %28s ","Rectangular Array",T->arrayType);
			printf("%10s","<type = ");
			if(T->typexpr.rectangularArray.type == rectangularArray) printf("rectangularArray,");
			printf("dimensions");
			printf("=%d,",T->typexpr.rectangularArray.dimensions);
			for(int i=0;i<T->typexpr.rectangularArray.dimensions;i++){
				printf("range_R%d=",i+1);
				printf("(%s,%s)",T->typexpr.rectangularArray.range[0][i],T->typexpr.rectangularArray.range[1][i]);
				printf(",");
			}
			printf("basicElementType=integer>");
		}
		else if(T->type == 2){
			printf("%28s %28s ","Jagged Array",T->arrayType);
			printf("%10s","<type = ");
			if(T->typexpr.jaggedArray.type == jaggedArray) printf("jaggedArray,");
			printf("dimensions");
			printf("=%d,",T->typexpr.jaggedArray.dimensions);
			printf("range_R1=(%d,%d),",T->typexpr.jaggedArray.range_R1[0],T->typexpr.jaggedArray.range_R1[1]);
			printf("range_R2=(");
			if(T->typexpr.jaggedArray.dimensions ==2){
				for(int i=0;i<(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0]+1);i++){
					printf("%d",T->typexpr.jaggedArray.R2.two_Dim[i]);
					if(i!=(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0])) printf(",");
				}
			}
			else{
				for(int i=0;i<(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0]+1);i++){
					printf("%d[", T->typexpr.jaggedArray.R2.three_Dim[i][0]);
					for(int j=1;j<(T->typexpr.jaggedArray.R2.three_Dim[i][0]+1);j++){
						printf("%d", T->typexpr.jaggedArray.R2.three_Dim[i][j]);
						if(j!=(T->typexpr.jaggedArray.R2.three_Dim[i][0])) printf(",");
					}
					printf("]");
					if(i!=(T->typexpr.jaggedArray.range_R1[1]-T->typexpr.jaggedArray.range_R1[0])) printf(",");
				}
			}
			
			printf("),basicElementType=integer>");

		}
		else{
			printf("%28s %28s ","---","---");
			printf(" Type declaration error ");
		}
		printf("\n \n");
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
		if(declcheck(t)){
			data=ERROR;
			char message[35] = "Array Index undeclared/not integer";
			printTypeError(t->data.Terminals.lno,0,"***","***","***","***","***",t->depth,message);
		}
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
	printf("Error encountered in Line %d (%5s statement)  Operator: %5s , First operand: %5s (%3s) , Second Operand: %5s (%3s) Depth: %5d , Message: %5s \n", lno,statements,operator,firstOperand,firstOperandType,secondOperand,secondOperandType,depth,message);
}


// ------------------------------------------------------------------------- KANAVTYPE

typeExpressionTable checkArithmeticExpression(treeNode *node,typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;  // tempNode points to <mulExpression>
  treeNode* tempNode2;
  typeExpressionTable a, c;

  a = checkmulExpression(tempNode, T);
  
  tempNode2 = tempNode;  // tempNode2 points to <mulExpression>
  tempNode = tempNode->nextSibling;  // tempNode points to <arithmeticExpression2>
  c = checkArithmeticExpression2(tempNode, T);
  if(c.typexpr.primDt == 4) {
    node->data.Nonterminals.typeExp = a;
    return a;
  }

  node->data.Nonterminals.typeExp = compatibleTypeArith(a, c);

  if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
      printTypeErrorKanav(tempNode);
      firstError = 1;  // make firstError = 0 when it returns back to assignment.
  }

  return node->data.Nonterminals.typeExp;
}


typeExpressionTable checkArithmeticExpression2(treeNode *node, typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;
  treeNode* tempNode2;
  treeNode* tempNode3;
  typeExpressionTable a, c;
  
  node->firstChild->data.Nonterminals.typeExp = nilTable();
  
  if(!strcmp(tempNode->symbol, "EPS")){
    node->data.Nonterminals.typeExp.type = 0;
    node->data.Nonterminals.typeExp.typexpr.primDt = 4;
    return nilTable();
  }
  else{ // tempNode points to <sumop>
    tempNode2 = tempNode; // tempNode2 points to <sumop>
    tempNode = tempNode->nextSibling;   // tempNode points to <mulExpression>
    a = checkmulExpression(tempNode, T);

    tempNode3 = tempNode;  // tempNode3 points to <mulExpression>
    tempNode = tempNode->nextSibling;  // tempNode points to <arithmeticExpression2>
    c = checkArithmeticExpression2(tempNode, T);
    if(c.typexpr.primDt == 4) {
      node->data.Nonterminals.typeExp = a;
      return a;
    }

    node->data.Nonterminals.typeExp = compatibleTypeArith(a, c);

    if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
        printTypeErrorKanav(tempNode);
        firstError = 1;  // make firstError = 0 when it returns back to assignment.
    }
    return node->data.Nonterminals.typeExp;
  }
}


typeExpressionTable checkmulExpression(treeNode *node,typeExpressionTable *T){
    treeNode* tempNode = node->firstChild;  // tempNode points to <IntegerFactor>
    treeNode* tempNode2;
    typeExpressionTable a, c;

    a = checkIntegerFactor(tempNode, T);

    tempNode2 = tempNode;  // tempNode2 points to <IntegerFactor>
    tempNode = tempNode->nextSibling;  // tempNode points to <mulExpression2>
    c = checkmulExpression2(tempNode, T);
    if(c.typexpr.primDt == 4) {
      node->data.Nonterminals.typeExp = a;
      return a;
    }

    node->data.Nonterminals.typeExp = compatibleTypeArith(a, c);

    if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
        printTypeErrorKanav(tempNode);
        firstError = 1;  // make firstError = 0 when it returns back to assignment.
    }
    return node->data.Nonterminals.typeExp;
}


typeExpressionTable checkmulExpression2(treeNode *node, typeExpressionTable *T){
    treeNode* tempNode = node->firstChild;
    treeNode* tempNode2;
    treeNode* tempNode3;
    typeExpressionTable a, c;
	node->firstChild->data.Nonterminals.typeExp = nilTable();
    if(!strcmp(tempNode->symbol, "EPS")){
      node->data.Nonterminals.typeExp.type = 0;
      node->data.Nonterminals.typeExp.typexpr.primDt = 4;
      return nilTable();
    }
    else{ // tempNode points to <mulop>
      tempNode2 = tempNode; // tempNode2 points to <mulop>
      tempNode = tempNode->nextSibling;   // tempNode points to <IntegerFactor>
      a = checkIntegerFactor(tempNode, T);

      tempNode3 = tempNode;  // tempNode3 points to <IntegerFactor>
      tempNode = tempNode->nextSibling;  // tempNode points to <mulExpression2>
      c = checkmulExpression2(tempNode, T);
      if(c.typexpr.primDt == 4) {
        node->data.Nonterminals.typeExp = a;
        return a;
      }

      if(!strcmp(tempNode->firstChild->firstChild->symbol, "MUL"))      node->data.Nonterminals.typeExp = compatibleTypeArith(a, c);
      else    node->data.Nonterminals.typeExp = divCompatibleTypeArith(a, c);

      if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
          printTypeErrorKanav(tempNode);
          firstError = 1;  // make firstError = 0 when it returns back to assignment.
      }
      return node->data.Nonterminals.typeExp;
    }
}

typeExpressionTable checkIntegerFactor(treeNode *node,typeExpressionTable *T){
  typeExpressionTable a;
  if(!strcmp(node->firstChild->symbol, "NUM")){  // checking if <IntegerFactor> goes to NUM
    a.type = 0;
    a.typexpr.primDt = 0;
  }
  else a = checkVarName(node->firstChild,T); // sending <varName>
  node->data.Nonterminals.typeExp = a;
  printf("\nINside varname%s %d \n",node->data.Nonterminals.typeExp.name, node->data.Nonterminals.typeExp.type);
  printf("\nINside varname%s %d \n",a.name, a.type);

  return a;
}


typeExpressionTable checkVarName(treeNode *node,typeExpressionTable *T){
  
	typeExpressionTable tError;
	tError.type = 0;
	strcpy(tError.arrayType,"not_applicable");
	strcpy(tError.name, "ERROR");
	tError.typexpr.primDt = ERROR;
	
	typeExpressionTable tInteger;
	tInteger.type = 0;
	strcpy(tInteger.arrayType,"not_applicable");
	strcpy(tInteger.name, "integer");
	tInteger.typexpr.primDt = integer;
	
	typeExpressionTable a;
	a = getVariableTypeExpression(node->firstChild->data.Terminals.lexeme, T);
	node->data.Nonterminals.typeExp = a;
	
  	//printf("\n\n\nTODO: CHECKING array if Out of bounds\n\n");	 
  	// there are  '[' ']'
  	if(node->firstChild->nextSibling!=NULL){
	  	int cNum=0, cID=0;
	  	treeNode *tempNode = node->firstChild->nextSibling->nextSibling;  //  <index>>
	  	getVarsAndNums(tempNode, &cNum, &cID);
	  	
	  	int dimensions=0;
		//printf("\n\n\nTodo2: CHECKING array if Out of bounds  %d %d \n\n", cNum, cID);	  				

	  	if(a.type==1){
	  		dimensions = a.typexpr.rectangularArray.dimensions;
	  		if(dimensions==(cNum+cID)){
	  			//printf("in 3rd loop\n\n");
	  			if(cNum==dimensions && strcmp(a.arrayType, "static")==0){ //do static bound checking
	  				int *arr = (int *) malloc(sizeof(int)*dimensions);
	  				//a [ 2 3 4]
	  				//printf("in 3rd loop\n\n");
	  				int *index = (int *) malloc(sizeof(int));
	  				*index=0;
	  				getNumValues(arr, tempNode, index);
	  				
	  				for(int i=0; i<dimensions; i++){
	  					//printf(" %d,%d ",i,arr[i]);
	  					if(arr[i]<atoi(a.typexpr.rectangularArray.range[0][i]) || arr[i]>atoi(a.typexpr.rectangularArray.range[1][i])){
	  						// out of bounds error
	  						printf("\n Rectangular Array out of bounds\n");	//TODO printError
	  						//printTypeError(node.);
	  						node->data.Nonterminals.typeExp = tError;
	  						return tError;
	  					}
	  				}
					//printf("\n\n\nTODO: array NOT Out of bounds\n\n");
					node->data.Nonterminals.typeExp = tInteger;	  				
	  				return tInteger;	  				
	  			}
	  			node->data.Nonterminals.typeExp = tInteger;
	  			return tInteger;
	  		}
	  		else {
	  			node->data.Nonterminals.typeExp = tError;
	  			return tError;	//less or more dimnesions
	  		}
	  	
	  	}
	  	else if(a.type==2){
	  		dimensions = a.typexpr.jaggedArray.dimensions;	
	  		if(dimensions==(cNum+cID)){
	  			if(cNum==dimensions && strcmp(a.arrayType, "static")==0){ //do static bound checking
	  				
	  				int *arr = (int *) malloc(sizeof(int)*dimensions);
	  				//a [ 2 3 4]
	  				//printf("in 3rd loop\n\n");
	  				int *index = (int *) malloc(sizeof(int));
	  				*index=0;
	  				getNumValues(arr, tempNode, index);
	  							
	  				if(dimensions ==2){
	  						if(arr[0]<a.typexpr.jaggedArray.range_R1[0] || arr[0]>a.typexpr.jaggedArray.range_R1[1]) {
	  							printf("\n Jagged Array out of bounds\n");	//TODO printError
	  							node->data.Nonterminals.typeExp = tError;
	  							return tError;
	  							// jagged array 1st index out of bounds
	  						}
	  						
	  						//int size = sizeof(a.typexpr.jaggedArray.R2.two_Dim)/sizeof(int);
	  						int d = arr[0] - a.typexpr.jaggedArray.range_R1[0];
	  						
	  						
	  						if(arr[1]>=a.typexpr.jaggedArray.R2.two_Dim[d]) {
	  							// copy above's
	  							printf("\n Jagged Array out of bounds\n");	//TODO printError
	  							// jagged array 2nd index out of bounds
	  							node->data.Nonterminals.typeExp = tError;
	  							return tError;
	  						}
	  						node->data.Nonterminals.typeExp = tInteger;
	  						return tInteger;	// no error
	  				}
	  				else if(dimensions ==3){
	  						// arr [0 1 2]
	  						if(arr[0]<a.typexpr.jaggedArray.range_R1[0] || arr[0]>a.typexpr.jaggedArray.range_R1[1]) {
	  							printf("\n Jagged Array out of bounds\n");	//TODO printError
	  							node->data.Nonterminals.typeExp = tError;
	  							return tError;
	  							// jagged array 1st index out of bounds
	  						}
	  						
	  						int d = arr[0] - a.typexpr.jaggedArray.range_R1[0];
	  						
	  						if(arr[1]>= a.typexpr.jaggedArray.R2.three_Dim[d][0] ) {
	  							// copy above's
	  							printf("\n Jagged Array out of bounds\n");	//TODO printError
	  							// jagged array 2nd index out of bounds
	  							node->data.Nonterminals.typeExp = tError;
	  							return tError;
	  						}
	  						
	  						int e = arr[1];
	  						if(arr[2]>=a.typexpr.jaggedArray.R2.three_Dim[d][e+1] ) {
	  							// copy above's
	  							printf("\n Jagged Array out of bounds\n");	//TODO printError
	  							// jagged array 3rd index out of bounds
	  							node->data.Nonterminals.typeExp = tError;
	  							return tError;
	  						}
	  						node->data.Nonterminals.typeExp = tInteger;	
							return tInteger;
	  					
	  				} 				
	  				//return tInteger;
	  			}
	  			node->data.Nonterminals.typeExp = tInteger;
	  			return tInteger;
	  		}
	  			//TODO printError
	  		else{
	  			printf("\n Jagged Array more/less dimesnions\n");
	  			node->data.Nonterminals.typeExp = tError;
	  		 return tError; //less or more dimnesions
	  		}
	  	}
	  	
	  	
	  	 
  	}
	else { 
	
	  	node->data.Nonterminals.typeExp = a;
	  	return a;
	}
}

void getVarsAndNums(treeNode *node,int *countNum, int *countID){
	treeNode *temp = node;
	if(temp==NULL) return;
	if(!strcmp(temp->symbol,"ID") && temp->tag==0){
		*countID += 1;
	}
	else if(!strcmp(temp->symbol,"NUM") && temp->tag==0){
		*countNum +=1;
	}
	getVarsAndNums(temp->firstChild,countNum, countID);
	getVarsAndNums(temp->nextSibling,countNum, countID);
}

void getNumValues(int *arr, treeNode *node, int *i){
	treeNode *temp = node;
	if(temp==NULL) return;
	getNumValues(arr, temp->firstChild, i);
	if(!strcmp(temp->symbol,"NUM") && temp->tag==0){
		arr[(*i)] = atoi(temp->data.Terminals.lexeme);
		//printf(" H:%s,%d,%d,%d ",temp->data.Terminals.lexeme, atoi(temp->data.Terminals.lexeme), arr[(*i)],(*i));
		(*i)++;
	}

	getNumValues(arr, temp->nextSibling, i);
	
}


typeExpressionTable getVariableTypeExpression(char *name, typeExpressionTable *T){
  	//for(int i=0; i<c && T!=NULL; i++){
  	
  	typeExpressionTable tNIL;
	tNIL.type = 0;
	strcpy(tNIL.arrayType,"not_applicable");
	strcpy(tNIL.name,"NIL");
	tNIL.typexpr.primDt = 4;
  	
  	while(T!=NULL)	
  	{	
 		if(!strcmp((*T).name,name)){
 			return *T;
 		}
 		T = T->next;
 	}
 	return tNIL;
}


typeExpressionTable checkAssignmentList(treeNode *node,typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;   // tempNode points to <assignment>
  checkAssignment(tempNode, T);
  
  tempNode = tempNode->nextSibling;  // tempNode points to <assignmentList2>
  checkAssignmentList2(tempNode, T);
  
  node->data.Nonterminals.typeExp.type = 0;
  node->data.Nonterminals.typeExp.typexpr.primDt = 4;
  return node->data.Nonterminals.typeExp;
}


typeExpressionTable checkAssignmentList2(treeNode *node, typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;   
  if(!strcmp(tempNode->symbol, "EPS")){
    node->data.Nonterminals.typeExp.type = 0;
    node->data.Nonterminals.typeExp.typexpr.primDt = 4;
    return nilTable();
  }
  else{ // tempNode points to <assignment>
   checkAssignment(tempNode, T);
   tempNode = tempNode->nextSibling;  // tempNode points to <assignmentList2>
   checkAssignmentList2(tempNode, T);
  }
  
  node->data.Nonterminals.typeExp.type = 0;
  node->data.Nonterminals.typeExp.typexpr.primDt = 4;
  return node->data.Nonterminals.typeExp;
}


typeExpressionTable checkAssignment(treeNode *node, typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;   // tempNode points to <varName>
  treeNode* tempNode2;
  typeExpressionTable a, c;
  a = checkVarName(tempNode, T);

  tempNode2 = tempNode;  // tempNode2 points to <varName>
  tempNode = tempNode->nextSibling;  // tempNode points to <EQU>
  tempNode = tempNode->nextSibling;  // tempNode points to <expression>

  c = checkExpression(tempNode, T);
  if(strcmp(tempNode->firstChild->symbol,"arithmeticExpression")==0)	node->data.Nonterminals.typeExp = compatibleTypeArith(a, c);
  else node->data.Nonterminals.typeExp = compatibleTypeBool(a, c);

  if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
      printTypeErrorKanav(tempNode);
      firstError = 1;  // make firstError = 0 when it returns back to assignment.
  }
  return node->data.Nonterminals.typeExp;
}


typeExpressionTable checkExpression(treeNode *node,typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;   // tempNode points to <arithmeticExpression> or <booleanExpression>
  treeNode* tempNode2;
  typeExpressionTable a, c;
  firstError = 0;
  if(!strcmp(tempNode->symbol, "arithmeticExpression")){
    node->data.Nonterminals.typeExp = checkArithmeticExpression(tempNode, T);
    printf("\n\nPrinting type EXPR: %s %d\n\n",node->data.Nonterminals.typeExp.name, node->data.Nonterminals.typeExp.typexpr.primDt);
    firstError = 0;
    return node->data.Nonterminals.typeExp;
  }
  else if(!strcmp(tempNode->symbol, "booleanExpression")){
    node->data.Nonterminals.typeExp = checkBooleanExpression(tempNode, T);
    firstError = 0;
    return node->data.Nonterminals.typeExp;
  }
  else{
    printf("ERROR ---------couldn't find both : arithmeticExpression and booleanExpression-------------\n");
    exit(0);
  }
}


typeExpressionTable checkBooleanExpression(treeNode *node,typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;  // tempNode points to <andExpression>
  treeNode* tempNode2;
  typeExpressionTable a, c;

  a = checkAndExpression(tempNode, T);

  tempNode2 = tempNode;  // tempNode2 points to <andExpression>
  tempNode = tempNode->nextSibling;  // tempNode points to <booleanExpression2>
  c = checkBooleanExpression2(tempNode, T);
  if(c.typexpr.primDt == 4) {
    node->data.Nonterminals.typeExp = a;
    return a;
  }

  node->data.Nonterminals.typeExp = compatibleTypeBool(a, c);

  if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
      printTypeErrorKanav(tempNode);
      firstError = 1;  // make firstError = 0 when it returns back to assignment.
  }
  return node->data.Nonterminals.typeExp;
}


typeExpressionTable checkBooleanExpression2(treeNode *node, typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;
  treeNode* tempNode2;
  treeNode* tempNode3;
  typeExpressionTable a, c;

  if(!strcmp(tempNode->symbol, "EPS")){
    node->data.Nonterminals.typeExp.type = 0;
    node->data.Nonterminals.typeExp.typexpr.primDt = 4;
    return nilTable();
  }
  else{ // tempNode points to OROP
    tempNode2 = tempNode; // tempNode2 points to OROP
    tempNode = tempNode->nextSibling;   // tempNode points to <andExpression>
    a = checkAndExpression(tempNode, T);

    tempNode3 = tempNode;  // tempNode3 points to <andExpression>
    tempNode = tempNode->nextSibling;  // tempNode points to <booleanExpression2>
    c = checkBooleanExpression2(tempNode, T);
    if(c.typexpr.primDt == 4) {
      node->data.Nonterminals.typeExp = a;
      return a;
    }

    node->data.Nonterminals.typeExp = compatibleTypeBool(a, c);

    if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
        printTypeErrorKanav(tempNode);
        firstError = 1;  // make firstError = 0 when it returns back to assignment.
    }
    return node->data.Nonterminals.typeExp;
  }
}


typeExpressionTable checkAndExpression(treeNode *node,typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;  // tempNode points to <varName>
  treeNode* tempNode2;
  typeExpressionTable a, c;

  a = checkVarName(tempNode, T);

  tempNode2 = tempNode;  // tempNode2 points to <varName>
  tempNode = tempNode->nextSibling;  // tempNode points to <andExpression2>
  c = checkAndExpression2(tempNode, T);
  if(c.typexpr.primDt == 4) {
    node->data.Nonterminals.typeExp = a;
    return a;
  }

  node->data.Nonterminals.typeExp = compatibleTypeBool(a, c);

  if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
      printTypeErrorKanav(tempNode);
      firstError = 1;  // make firstError = 0 when it returns back to assignment.
  }
  return node->data.Nonterminals.typeExp;
}


typeExpressionTable checkAndExpression2(treeNode *node, typeExpressionTable *T){
  treeNode* tempNode = node->firstChild;
  treeNode* tempNode2;
  treeNode* tempNode3;
  typeExpressionTable a, c;

  if(!strcmp(tempNode->symbol, "EPS")){
    node->data.Nonterminals.typeExp.type = 0;
    node->data.Nonterminals.typeExp.typexpr.primDt = 4;
    return nilTable();
  }
  else{ // tempNode points to ANDOP
    tempNode2 = tempNode; // tempNode2 points to ANDOP
    tempNode = tempNode->nextSibling;   // tempNode points to <varName>
    a = checkVarName(tempNode, T);

    tempNode3 = tempNode;  // tempNode3 points to <varName>
    tempNode = tempNode->nextSibling;  // tempNode points to <andExpression2>
    c = checkAndExpression2(tempNode, T);
    if(c.typexpr.primDt == 4) {
      node->data.Nonterminals.typeExp = a;
      return a;
    }

    node->data.Nonterminals.typeExp = compatibleTypeBool(a, c);

    if(node->data.Nonterminals.typeExp.type == 0 && node->data.Nonterminals.typeExp.typexpr.primDt == ERROR && firstError == 0){
        printTypeErrorKanav(tempNode);
        firstError = 1;  // make firstError = 0 when it returns back to assignment.
    }
    return node->data.Nonterminals.typeExp;
  }
}

int declcheck(treeNode* t){
	typeExpressionTable *temp=tempTable;
	printf("%s ",tempTable->name);
	while(temp!=NULL){
	if(temp->type==0){
		if(!strcmp(t->data.Terminals.lexeme,temp->name)){
			if(temp->typexpr.primDt==0)
				return 0;
		}
	}
	temp=temp->next;
	}
	return 1;
}



