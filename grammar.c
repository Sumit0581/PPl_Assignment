#include "grammar.h"

int bufferferSize = 180;
int grammarRules = 57;

int searchNonTerminal(char *datavalue){
    
    int a = search(datavalue, 1); /*In lexer.c*/
    return a;
}

int searchTerminal(char *datavalue){
    int a = search(datavalue, 0); /*In lexer.c*/
    return a;
}


grammar* addingRule(char *buffer,int lineno, grammar* g){
    char datavalue[25];
    int i,j;
	i=1;
	j=0;
	while(buffer[i]!='>'){
		datavalue[j] = buffer[i];
        j++;
		i++;
    }
    datavalue[j] = '\0';
    g[lineno].value = searchNonTerminal(datavalue);
    g[lineno].front = NULL;
    g[lineno].rear = NULL;
    i++;
    datavalue[0] = '\0';
	j=0;
	while(buffer[i]!='\0'){
		if(buffer[i]=='<') {i++;continue;}
		else if(buffer[i]=='>')
		{
			symbolNode *s = (symbolNode*)malloc(sizeof(symbolNode));
            s->value = searchNonTerminal(datavalue);
            s->tag = nonTerminal;
            s->next = NULL;
            s->prev = NULL;
           if(g[lineno].front!=NULL){
				g[lineno].rear->next = s;
                s->prev = g[lineno].rear;
                g[lineno].rear= s;
            }
            else{
				g[lineno].front = s;
                g[lineno].rear = s;
            }
            datavalue[0] = '\0';
            j = 0;
		}
		else if(buffer[i]==' ' && buffer[i-1]!='>'){
			symbolNode *s = (symbolNode*)malloc(sizeof(symbolNode));
            s->value = searchTerminal(datavalue);
            s->tag = terminal;
            s->next = NULL;
            s->prev = NULL;
            if(g[lineno].front!=NULL){
				g[lineno].rear->next = s;
                s->prev = g[lineno].rear;
                g[lineno].rear= s;
            }
            else{
				g[lineno].front = s;
                g[lineno].rear = s;
            }
            datavalue[0] = '\0';
            j = 0;
		}
		else if(buffer[i]==' ' && buffer[i-1]=='>'){i++;continue;}
		else{
			datavalue[j] = buffer[i];
			datavalue[j+1] = '\0';
			j++;	
		}
		i++;
	}
	if(datavalue[0] !='\0'){
		symbolNode *s = (symbolNode*)malloc(sizeof(symbolNode));
		s->value = searchTerminal(datavalue);
		s->tag = terminal;
		s->next = NULL;
		s->prev = NULL;
		if(g[lineno].front!=NULL){
			g[lineno].rear->next = s;
			s->prev = g[lineno].rear;
			g[lineno].rear= s;
		}
		else{
			g[lineno].front = s;
			g[lineno].rear = s;
		}
		datavalue[0] = '\0';
		j = 0;
	}
	return g;
}
grammar* readGrammar(char *inputFile, grammar* g){
    FILE *fp;
    
    if(fp!=NULL){
        fclose(src);
    }
    fp = fopen(inputFile,"r");
    if(fp==NULL){
        printf("Could not open Grammar file: Missing or Error\n");
        fflush(stdout); //if there is anything in buffer flush it in stdout
        return NULL;
    }

	
    char bufferfer[bufferferSize];
    int i = 0;
    while(fscanf(fp," %[^\n]",bufferfer)!=EOF){
        g = addingRule(bufferfer,i,g);
        i++;
    }
    fclose(fp);
    return g;
}
void printGrammar(grammar* g){
	//printf("BB");
	printf("\n\n----PRINTING GRAMMAR--- (value=id, tag= 0:terminal, 1:non terminal) \n\n");
	for(int i=0;i<grammarRules;i++){
		printf("RuleNo=%d:, LHSNtrID=%d -> ",i+1,g[i].value);
		symbolNode *s = (symbolNode*)malloc(sizeof(symbolNode));
		s=g[i].front;
		while(s!=NULL)
		{
			if(s->next==NULL){
				printf("ID=%d tag=%d | ",s->value,s->tag);
			}
			else{
			printf("ID=%d tag=%d | ",s->value,s->tag);
			}
			s=s->next;
		}
		printf("\n");
	}
}



