#include "lexer.h"


//part 1. build hash lookup table
//part 2. convert input source lexemes to token and store in tokenStream

//--------------------------------------1. starts


char *terminalFile = "tr.txt";
char *nonterminalFile = "ntr.txt";
char *keywordFile = "keywords.txt";



// extern variables from lexerInterface
int numOfTr=32, numOfNtr=30, numOfKeywords=14;

char **terminals;	// can remove everything about these three variables, even though they took a lot time
char **nonterminals;
char **keywords;
int lineNum =1;
int isHashBuilt = 0;

hashNode* hashTable[HASH_TABLE_SIZE] = {NULL};

int bufferIndex =-1; //bufferIndex points to where in buffer string we are and have to put next char
char buffer[BUFFER_SIZE];
FILE * src = NULL;



int hash(char *str){
	int hash = 0;
	int len = strlen(str);
	for(int i=0; i<len; i++){
		hash = hash*31 + str[i]; //TODO: 31? or anything else   
		hash = hash % HASH_TABLE_SIZE;
	}
	return hash;
}

void hashInsert(char *val, int id, int tag){
	int k = hash(val);
	if(hashTable[k]==NULL){
		hashTable[k] = (hashNode*) malloc(sizeof(hashNode));
		hashTable[k]->id = id;
		strcpy(hashTable[k]->value,val); 
		hashTable[k]->tag = tag;
		hashTable[k]->next = NULL;
	}
	else{
		hashNode *temp = hashTable[k];
		while(temp->next!=NULL){
			temp = temp->next;
		}
		temp->next = (hashNode*) malloc(sizeof(hashNode));
		(temp->next)->id = id;
		(temp->next)->tag = tag;
		(temp->next)->next = NULL;
		strcpy((temp->next)->value, val);
		
	}
	//printf("inserted %s %d %d\n",hashTable[k]->value, hashTable[k]->id, hashTable[k]->tag);
}


void populateHashTable(){
		if(isHashBuilt==1){
			bufferIndex=-1; //reset buffer
			lineNum = 1; // reset to first line
			return;
		}
		
		FILE *fp;
		char *name;
		size_t len;
		fp = fopen(terminalFile, "r");

		terminals = (char**) malloc(numOfTr * sizeof(char*));
		for(int i=0; i<numOfTr; i++){
			name =NULL;
			getline(&name, &len,fp);
			terminals[i] = name;  
			hashInsert(name,i,1);
			
		}
		fclose(fp);
		fp = fopen(nonterminalFile, "r");

		nonterminals = (char**) malloc(numOfNtr * sizeof(char*));
		for(int i=0; i<numOfNtr; i++){
			name =NULL;
			getline(&name, &len,fp);
			nonterminals[i] = name; 
			hashInsert(name,i,1);
			
		}
		fclose(fp);
		fp = fopen(keywordFile, "r");
		keywords = (char**) malloc(numOfKeywords * sizeof(char*));
		for(int i=0; i<numOfKeywords; i++){
			name =NULL;
			getline(&name, &len,fp); 
			keywords[i] = name;
			hashInsert(name,i,0);	// keyowrds tag=0
			
		}
		fclose(fp);	
		isHashBuilt =1;	
		//printHashTable();
}


int lexerPreReq(char *srcFile){
	if(src!=NULL){
		fclose(src);
	}
	src = fopen(srcFile,"r");
	if(src==NULL){
		printf("Could not open source file Missing or Error\n");
		fflush(stdout); //if there is anything in buffer flush it in stdout
		return -1;
	}
	populateHashTable();
}

// size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)


void printHashTable(){
	printf("PRINTING HASH TABLE \n");	
	for(int i=0; i<HASH_TABLE_SIZE; i++){
		if(hashTable[i]==NULL)
			printf("%d: --- \n", i);
		else {
			hashNode *t= hashTable[i];
			while(t!=NULL){
				printf("%d: %s ", i, t->value);
				t=t->next;
			}
			printf("\n");
			
		}
	}
}




//---------------------------------------2. starts


void printError(char lex[],int code,int len,int lno){
    /*
        lex : Is the problem string.
        Code: Error Code
        len : Length of string
        lno : Line of actual code
    */
    switch(code){
        case 0:
            printf("line: %d Lexical Error: Length of Identifier exceeded ---> Expected: Identifier of length <= 20, Got: '%s' of length %d\n", lno,lex,len);
            break;
        case 1:
            printf("line: %d Lexical Error: Invalid Floating Point Numeral '%s' ---> Missing valid character after '.'\n",lno,lex);
            break;
        case 2:
            printf("line: %d Lexical Error: Invalid Floating Point Numeral '%s' ---> Missing valid character after '%c', Expected a number between 0-9\n",lno,lex,lex[len-1]);
            break;
        case 10:
            if(strlen(lex)>=2 && lex[0]=='.' && lex[1]>='0' && lex[1]<='9'){
                printf("line: %d Lexical Error: Invalid Operator Encountered ---> '.' before %c, Add a '0' before '.' if a real number was desired\n",lno,lex[1]);
            }
            else{
		//printf("Here");
                printf("line: %d Lexical Error: Invalid Operator Encountered ---> '%c'\n",lno,lex[0]);
            }
            break;
        case 11:
            printf("line: %d Lexical Error: Invalid Character Encountered ---> '%s', No matching Tokenization Rule\n",lno,lex);
            break;
	    case 12:
	        printf("line: %d Lexical Error: Invalid Character Encountered after '!' ---> '%c', Expected: '='\n",lno,lex[0]);
            break;
    }
}


int search(char *value,int tag){
    int k = hash(value);
    hashNode * temp = hashTable[k];
    while(temp!=NULL){
        int a = strcmp(value,temp->value);
        if(a==0 && tag==temp->tag)
            return temp->id;
        temp = temp->next;
    }
    return -1;
}


void printToken(token *tk){
	printf("PRINTING A TOKEN: %d %s\b %s\b %d\n", tk->lineNum, tk->lexeme, tk->tokenName, tk->tag);
	return;
}

void printTokenStream(tokenStream *s){
	token *temp = s->head;
	while(temp!=NULL){
		printToken(temp);
		temp = temp->next;
	}
}

void tokeniseSourcecode( char * sourcecodefiletxtblahblah, tokenStream *s){
	
	if(src==NULL){
        	printf("Error: Missing Source File or Error in Opening File\n");
        	fflush(stdout);
        	return;
	}	
	
	
	token* tk = getNextToken();
	printToken(tk);
	return;
}

void loadBuffer(){
	// inputs a good chunk of 32 characters from source file and stores it into buffer (with its bufferPointer) 
		
	
	
	if(bufferIndex==-1 || bufferIndex==BUFFER_SIZE){
		for(int i=0; i<BUFFER_SIZE; i++){
			buffer[i] = 0;
		}
		fread(buffer,1, BUFFER_SIZE, src);	//src contains file pointer to source file.
		
		bufferIndex =0;
	}
	
	return;
}

token *getNextToken(){
    token *tk = (token *) malloc(sizeof(token));
    tk->tag=-1;
    loadBuffer();
    int start=0;
    char c=' ';
    int len=0; // Length of lexeme starts from 0 and not 1
    char lexeme[50] = {0};
    while(1){
        if(c==0)
            break;
        loadBuffer();
        c = buffer[bufferIndex];
        if(c==0)	//again check
            break;
        switch(c){
            case '_':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
                // The lower if is not currently of major use
                if(!start){
                    len = 0;
                    start=1;
                }
                else{
                    printf("Error : Identifier not completed earlier");
                }
                while((c>='a' && c<='z') || (c<='Z' && c>='A') || c=='_' || (c>='0' && c<='9')){
                    bufferIndex++;
                    loadBuffer();
                    lexeme[len]=c;
                    c = buffer[bufferIndex];
                    len++;
                }
                lexeme[len] = '\0';
                if(len>20)
                    printError(lexeme,0,len,lineNum);
                else{
                    //buffer_control--;
                    tk->tag=0; // for valid lexeme
                    strcpy(tk->lexeme,lexeme);
                    int a = search(lexeme,0);
                    if(a==-1)
                        strcpy(tk->tokenName,"ID");
                    else
                        strcpy(tk->tokenName,terminals[a]);
                    tk->lineNum = lineNum;
                }
                lexeme[len] = 0;
                start=0;
                break;
            case '\n':
                lineNum++;
                bufferIndex++;
                break;
            case ' ':
                //printf("In here");
                bufferIndex++;
                break;
            case '\t':
                bufferIndex++;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                while(c>='0' && c<='9'){
                    bufferIndex++;
                    loadBuffer();
                    lexeme[len]=c;
                    c = buffer[bufferIndex];
                    len++; // holds the actual length till now
                }
                /*
                if(c=='.'){
                    bufferIndex++;
                    loadBuffer();
                    lexeme[len]=c;
                    c = buff[bufferIndex];
                    if((c>='0' && c<='9')){
                        len++;
                        while(c>='0' && c<='9'){
                            bufferIndex++;
                            loadBuffer();
                            lexeme[len]=c;
                            len++; // holds the actual length till now
                            c = buff[bufferIndex];
                        }
                        if(c=='e' || c=='E'){
                            int validflag=0;
                            buffer_control++;
                            load();
                            lexeme[len] = c; // for e or E
                            len++;
                            c = buff[buffer_control];
                            if(c=='+' || c=='-' || (c>='0' && c<='9')){
                                validflag=1;
                            }
                            else{
                                lexeme[len]='\0';
                                printError(lexeme,2,len,lno);
                            }
                            int validflag2=len;
                            int plusflag=0;
                            while(c=='+' || c=='-' || (c>='0' && c<='9')){
                                if((c>='0' && c<='9')){
                                    plusflag=1;
                                }
                                buffer_control++;
                                load();
                                lexeme[len]=c;
                                len++;
                                c = buff[buffer_control];
                                if(c=='+' || c=='-'){
                                    break;
                                }
                            }
                            if(validflag2==len-1 && !plusflag){
                                validflag=0;
                                lexeme[len]='\0';
                                printError(lexeme,2,len,lno);
                            }
                            //buffer_control--;
                            if(validflag){
                                lexeme[len] = '\0';
                                tk->tag = 2;
                                strcpy(tk->lexeme,lexeme);
                                tk->lno = lno;
                                strcpy(tk->token_t,"RNUM");
                            }
                        }
                        else{
                            lexeme[len] = '\0';
                            tk->tag = 1; // For numeral
                            (tk->value).b = strtof(lexeme, (char **)NULL);
                            strcpy(tk->lexeme,lexeme);
                            tk->lno = lno;
                            strcpy(tk->token_t,"RNUM");
                        }
                    }
                    else if(c=='.'){
                        if(buffer_control==0){
                            traceback=1;
                            buff[0]='.';
                            fseek(src,-1*buff_size,SEEK_CUR);
                            buffer_control=-1;
                            load();
                        }
                        else 
                            buffer_control--;
                        
                        lexeme[len] = '\0';
                        tk->tag = 1; // For numeral
                        (tk->value).a = (int) strtol(lexeme, (char **)NULL, 10);
                        strcpy(tk->lexeme,lexeme);
                        tk->lno = lno;
                        strcpy(tk->token_t,"NUM");
                        traceback=0;
                    }
                    else{
                        len++;
                        lexeme[len] = '\0';
                        printError(lexeme,1,len,lno);
                    }
                }*/
                //else{
                    //buffer_control--;
                /*
                    lexeme[len] = '\0';
                    tk->tag = 1; // For numeral
                    (tk->tokenName).a = (int) strtol(lexeme, (char **)NULL, 10);
                    strcpy(tk->lexeme,lexeme);
                    tk->lineNum = lineNum;
                    strcpy(tk->tokenName,"NUM");
                */
                //}
                lexeme[len] = 0;
                break;

                case '+':
                    tk->tag=0;
                    strcpy(tk->lexeme,"+");
                    strcpy(tk->tokenName,"PLUS");
                    tk->lineNum = lineNum;
                    bufferIndex++;
                    break;
		
                case '-':
                    tk->tag=0;
                    strcpy(tk->lexeme,"-");
                    strcpy(tk->tokenName,"MINUS");
                    tk->lineNum = lineNum;
                    bufferIndex++;
                    break;

                case '*':
                    bufferIndex++;
                    loadBuffer();
                    c = buffer[bufferIndex];
                    int cflag=0;
                    if(c=='*'){
                        while(1){
                            //printf("Debugging");
                            bufferIndex++;
                            loadBuffer();
                            c = buffer[bufferIndex];
                            if(c==EOF){
                                break;
                            }
                            if(c=='\n')
                                lineNum++;
                            else if(cflag && c=='*' || c==0){
                                bufferIndex++;
                                break;
                            }
                            else if(c=='*')
                                cflag=1;
                            else if(cflag)
                                cflag=0;
                        }
                    }
                    else{
                        tk->tag=0;
                        strcpy(tk->lexeme,"*");
                        strcpy(tk->tokenName,"MUL");
                        tk->lineNum = lineNum;
                    }
                    break;

                case '/':
                    tk->tag=0;
                    strcpy(tk->lexeme,"/");
                    strcpy(tk->tokenName,"DIV");
                    tk->lineNum = lineNum;
                    bufferIndex++;
                    break;

                case '<':
                    bufferIndex++;
                    loadBuffer();
                    c = buffer[bufferIndex];
                    if(c=='<'){
                        bufferIndex++;
                        loadBuffer();
                        c = buffer[bufferIndex];
                        if(c=='<'){
                            tk->tag=0;
                            strcpy(tk->lexeme,"<<<");
                            strcpy(tk->tokenName,"DRIVERDEF");
                            tk->lineNum = lineNum;
                            bufferIndex++; 
                        }
                        else{
                            tk->tag=0;
                            strcpy(tk->lexeme,"<<");
                            strcpy(tk->tokenName,"DEF");
                            tk->lineNum = lineNum;
                        }
                    }
                    else if(c=='='){
                        tk->tag=0;
                        strcpy(tk->lexeme,"<=");
                        strcpy(tk->tokenName,"LE");
                        tk->lineNum = lineNum;
                        bufferIndex++;
                    }
                    else{
                        tk->tag=0;
                        strcpy(tk->lexeme,"<");
                        strcpy(tk->tokenName,"LT");
                        tk->lineNum = lineNum;
                    }
                    break;

                case '>':
                    bufferIndex++;
                    loadBuffer();
                    c = buffer[bufferIndex];
                    if(c=='>'){
                        bufferIndex++;
                        loadBuffer();
                        c = buffer[bufferIndex];
                        //printf("%c %d",c,c);
                        if(c=='>'){
                            tk->tag=0;
                            strcpy(tk->lexeme,">>>");
                            strcpy(tk->tokenName,"DRIVERENDDEF");
                            tk->lineNum = lineNum;
                            bufferIndex++; 
                        }
                        else{
                            tk->tag=0;
                            strcpy(tk->lexeme,">>");
                            strcpy(tk->tokenName,"ENDDEF");
                            tk->lineNum = lineNum;
                        }
                    }
                    else if(c=='='){
                        tk->tag=0;
                        strcpy(tk->lexeme,">=");
                        strcpy(tk->tokenName,"GE");
                        tk->lineNum = lineNum;
                        bufferIndex++;
                    }
                    else{
                        tk->tag=0;
                        strcpy(tk->lexeme,">");
                        strcpy(tk->tokenName,"GT");
                        tk->lineNum = lineNum;
                    }
                    break;

                case '=':
                    bufferIndex++;
                    loadBuffer();
                    c = buffer[bufferIndex];
                    if(c=='='){
                        tk->tag=0;
                        strcpy(tk->lexeme,"==");
                        strcpy(tk->tokenName,"EQ");
                        tk->lineNum = lineNum;
                        bufferIndex++;
                    }
                    else{
                        char arr[2]={'=','\0'};
                        printError(arr,10,1,lineNum);
                    }
                    break;
		/*
                case ':':
                    buffer_control++;
                    load();
                    c = buffer[buffer_control];
                    if(c=='='){
                        tk->tag=0;
                        strcpy(tk->lexeme,":=");
                        strcpy(tk->token_t,"ASSIGNOP");
                        tk->lno = lno;
                        buffer_control++;
                    }
                    else{
                        tk->tag=0;
                        strcpy(tk->lexeme,":");
                        strcpy(tk->token_t,"COLON");
                        tk->lno = lno;
                        
                    }
                    break;
		/*
		/*
                case ',':
                    tk->tag=0;
                    strcpy(tk->lexeme,",");
                    strcpy(tk->token_t,"COMMA");
                    tk->lno = lno;
                    buffer_control++;
                    break;
                */
                case ';':
                    tk->tag=0;
                    strcpy(tk->lexeme,";");
                    strcpy(tk->tokenName,"SEMICOL");
                    tk->lineNum = lineNum;
                    bufferIndex++;
                    break;
                case '[':
                    tk->tag=0;
                    strcpy(tk->lexeme,"[");
                    strcpy(tk->tokenName,"SQBO");
                    tk->lineNum = lineNum;
                    bufferIndex++;
                    break;
                case ']':
                    tk->tag=0;
                    strcpy(tk->lexeme,"]");
                    strcpy(tk->tokenName,"SQBC");
                    tk->lineNum = lineNum;
                    bufferIndex++;
                    break;

                case '(':
                    tk->tag=0;
                    strcpy(tk->lexeme,"(");
                    strcpy(tk->tokenName,"BO");
                    tk->lineNum = lineNum;
                    bufferIndex++;
                    break;
                case ')':
                    tk->tag=0;
                    strcpy(tk->lexeme,")");
                    strcpy(tk->tokenName,"BC");
                    tk->lineNum = lineNum;
                    bufferIndex++;
                    break;
		/*
                case '!':
                    buffer_control++;
                    load();
                    c = buffer[buffer_control];
                    if(c=='='){
                        tk->tag=0;
                        strcpy(tk->lexeme,"!=");
                        strcpy(tk->token_t,"NE");
                        tk->lno = lno;
                        buffer_control++;
                    }
                    else{
                        char arr[2]={c,'\0'};
                        printError(arr,12,1,lno);
                    }
                    break;
		*/
                case '.':
                    bufferIndex++;
                    loadBuffer();
                    c = buffer[bufferIndex];
                    if(c=='.'){
                        tk->tag=0;
                        strcpy(tk->lexeme,"..");
                        strcpy(tk->tokenName,"RANGEOP");
                        tk->lineNum = lineNum;
                        bufferIndex++;
                    }
                    else{
                        char arr[3]={'.',c,'\0'};     
                        printError(arr,10,1,lineNum);
                    }
                    break;
		
                default:
                    bufferIndex++;
                    char arr[2]={c,'\0'};
                    printError(arr,11,1,lineNum);
        }
        if(tk->tag!=-1)
            break;
    }
    if(tk->tag==-1){
        tk->lineNum = lineNum;
        strcpy(tk->lexeme,"$");
        strcpy(tk->tokenName,"DOLLAR");
    }
    return tk;
}


