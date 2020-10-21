#include "lexer.h"


//part 1. build hash lookup table
//part 2. convert input source lexemes to token and store in tokenStream

//--------------------------------------1. starts


char *terminalFile = "tr.txt";
char *nonterminalFile = "ntr.txt";
char *keywordFile = "keywords.txt";



// extern variables from lexerInterface
int numOfTr=19, numOfNtr=30, numOfKeywords=13;

char **terminals;   // can remove everything about these three variables, even though they took a lot time
char **nonterminals;
char **keywords;
int lineNum =1;
int isHashBuilt = 0;

hashNode* hashTable[HASH_TABLE_SIZE] = {NULL};

int bufferIndex =-1; //bufferIndex points to where in buffer string we are and have to put next char
char buffer[BUFFER_SIZE];
FILE * src = NULL;



int hash(char *str){
    /*
        computes hash value for a givven string

        str: string whose hash value is to be computed

        returns: the hash value of the string
    */
    int hash = 0;
    int len = strlen(str);
    for(int i=0; i<len; i++){
        hash = hash*31 + str[i]; //TODO: 31? or anything else   
        hash = hash % HASH_TABLE_SIZE;
    }
    return (hash + HASH_TABLE_SIZE)%HASH_TABLE_SIZE;
}

void hashInsert(char *val, int id, int tag){
    /*
        inserts the hashNode in the hashtable

        val: hashNode string to be inserted
        id: id of the hashNode
        tag: tag of the hashNode

        returns: void
    */
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
    /*
        populates the hashTable

        returns: void
    */    
    if(isHashBuilt==1){
        bufferIndex=-1; //reset buffer
        lineNum = 1; // reset to first line
        return;
    }
    
    FILE *fp;
    char name[30];
    size_t len;
    fp = fopen(terminalFile, "r");

    terminals = (char**) malloc(numOfTr * sizeof(char*));
    for(int i=0; i<numOfTr; i++){
        fscanf(fp, "%s", name); 
        terminals[i] = name;
        hashInsert(name,i,1); // terminals which are not keyword are to go here (tag =1)
    }
    fclose(fp);
    fp = fopen(nonterminalFile, "r");

    nonterminals = (char**) malloc(numOfNtr * sizeof(char*));
    for(int i=0; i<numOfNtr; i++){
        fscanf(fp, "%s", name); 
        nonterminals[i] = name; 
        hashInsert(name,i,2); // non terminal tag =2
        
    }
    fclose(fp);
    fp = fopen(keywordFile, "r");
    keywords = (char**) malloc(numOfKeywords * sizeof(char*));
    for(int i=0; i<numOfKeywords; i++){
        fscanf(fp, "%s", name); 
        keywords[i] = name;
        hashInsert(name,i,0);   // keyowrds tag=0
        
    }
    fclose(fp); 
    isHashBuilt =1; 
    //printHashTable();
}


int lexerPreReq(char *srcFile){
    /*
        prepares HashTable before tokenizing

        srcFile: source code File

        error: raises error nwhen the file is missing or the user doesn't have read permissions
    */
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

    /*
        prints the hashtable

        returns: void
    */
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
        handles the error in the code

        lex : Is the problem string.
        Code: Error Code
        len : Length of string
        lno : Line of actual code

        returns: void
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
    /*
        search in the hashTable

        value: Hashnode string to be searched
        tag: tag of the Hashnode

        returns: Hashnode id (-1 if not present in hashTable)
    */
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
    /*
        prints a token on the console

        tk: token to be printed

        returns: void
    */
    printf("PRINTING A TOKEN: %d %s %s %d\n", tk->lineNum, tk->lexeme, tk->tokenName, tk->tag);
    return;
}

void printTokenStream(tokenStream *s){
    /*
        prints tokenStream on the console

        s: tokenStream to be printed

        returns: void
    */
    token *temp = s->head;
    while(temp!=NULL){
        printToken(temp);
        temp = temp->next;
    }
}

void insertInStream(tokenStream *s, token *tk){
    /*
        inserts token in the tokenStream

        s: tokenStream
        tk: token

        returns: void
    */
    token *temp=s->head;
    if(temp==NULL){
        s->head = tk;
        s->len = 1;
        return;
    }

    while(temp->next!=NULL){
        temp=temp->next;
    }

    fflush(stdout);
    temp->next = tk;
    (s->len)++;
    return;
}

token * getToken(char *word){
    /*
        returns a token for a given lexeme

        word: lexeme

        returns: token
    */
    token *tk = (token *) malloc(sizeof(token));

    // search for word == keyword (tag =0)
    int b = search(word, 0);
    
    if(b!=-1){ // token for valid keywords
        tk->lineNum = 0;
        strcpy(tk->lexeme,word);
        strcpy(tk->tokenName,word);
        tk->tag = 0;
    }
    else{ 
        if(word[0]=='a'||word[0]=='b'||word[0]=='c'||word[0]=='d'||word[0]=='e'||word[0]=='f'||word[0]=='g'||word[0]=='h'||word[0]=='i'||word[0]=='j'||word[0]=='k'||word[0]=='l'||word[0]=='m'||word[0]=='n'||word[0]=='o'||word[0]=='p'||word[0]=='q'||word[0]=='r'||word[0]=='s'||word[0]=='t'||word[0]=='u'||word[0]=='v'||word[0]=='w'||word[0]=='x'||word[0]=='y'||word[0]=='z'||word[0]=='A'||word[0]=='B'||word[0]=='C'||word[0]=='D'||word[0]=='E'||word[0]=='F'||word[0]=='G'||word[0]=='H'||word[0]=='I'||word[0]=='J'||word[0]=='K'||word[0]=='L'||word[0]=='M'||word[0]=='N'||word[0]=='O'||word[0]=='P'||word[0]=='Q'||word[0]=='R'||word[0]=='S'||word[0]=='T'||word[0]=='U'||word[0]=='V'||word[0]=='W'||word[0]=='X'||word[0]=='Y'||word[0]=='Z'||word[0]=='_'){ //token for valid variable names
            tk->lineNum = 0;
            strcpy(tk->lexeme,word);
            strcpy(tk->tokenName,"ID");
            tk->tag = 1;
        }
        else if(word[0]=='1'||word[0]=='2'||word[0]=='3'||word[0]=='4'||word[0]=='5'||word[0]=='6'||word[0]=='7'||word[0]=='8'||word[0]=='9'){ //token for valid numbers
            tk->lineNum = 0;
            strcpy(tk->lexeme,word);
            strcpy(tk->tokenName,"NUM");
            tk->tag = 1;
        }
        else{ //token for valid symbols
            tk->lineNum = 0;
            strcpy(tk->lexeme,word);
            tk->tag = 1;
            switch(word[0]){
                case ':':
                    strcpy(tk->tokenName,"COLON");
                    break;
                case ';':
                    strcpy(tk->tokenName,"SEMICOLON");
                    break;
                case '(':
                    strcpy(tk->tokenName,"BO");
                    break;
                case ')':
                    strcpy(tk->tokenName,"BC");
                    break;
                case '[':
                    strcpy(tk->tokenName,"SQRBO");
                    break;
                case ']':
                    strcpy(tk->tokenName,"SQRBC");
                    break;
                case '{':
                    strcpy(tk->tokenName,"CBO");
                    break;
                case '}':
                    strcpy(tk->tokenName,"CBC");
                    break;
                case '.':
                    strcpy(tk->tokenName,"DOTDOT");
                    break;
                case '=':
                    strcpy(tk->tokenName,"EQU");
                    break;
                case '+':
                    strcpy(tk->tokenName,"PLUS");
                    break;
                case '-':
                    printf("%c \n",word[0]);
                    fflush(stdout);
                    strcpy(tk->tokenName,"MINUS");
                    break;
                case '*':
                    strcpy(tk->tokenName,"MUL");
                    break;
                case '/':
                    strcpy(tk->tokenName,"DIV");
                    break;
                case '|':
                    strcpy(tk->tokenName,"OROP");
                    break;
                case '&':
                    strcpy(tk->tokenName,"ANDOP");
                    break;
            }
        }
    }
    
    return tk;
     
}

void tokeniseSourcecode( char * sourceCodeFile, tokenStream *s){
    /*
        tokenises the source code

        sourceCodeFile: source code
        s: tokenStream

        returns: void
    */
    if(src!=NULL){
        fclose(src);
    }
    src = fopen(sourceCodeFile,"r");
    if(src==NULL){
        printf("Could not open source file Missing or Error\n");
        fflush(stdout); //if there is anything in buffer flush it in stdout
        return;
    }
    char line[400];
    int i =0;
    int lineNumber=0;
    const char delimiter[2]= " ";
    while(fgets(line,400,src)!=NULL){
        lineNumber++;
        char* word;
        word = strtok(line,delimiter);
        while(word != NULL){
            token *tk = getToken(word);
            fflush(stdout);
            tk->lineNum = lineNumber;
            tk->next= NULL;
            insertInStream(s,tk);
            word = strtok(NULL,delimiter);
            i++;
        }
    }
    token *tk = (token *) malloc(sizeof(token));
    tk->lineNum = (lineNumber+1);
    strcpy(tk->lexeme,"EOF");
    strcpy(tk->tokenName,"EOF");
    tk->tag =1;
    tk-> next = NULL;
    insertInStream(s,tk);
    return;
}



