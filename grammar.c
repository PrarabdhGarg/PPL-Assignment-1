/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include"grammar.h" 
//#include "typeExpressionTable.h"

int readGrammar( char* filename, GrammarCell* grammar){

    FILE *fptr;
    if ((fptr = fopen("grammar.txt", "r")) == NULL) {
        printf("Error! opening file");
       return 0;
    }

    char *p = NULL;
    int i=0;
    char line[MAXCHARLINE];
    int size=INITSIZE;
    int lineCount=0;

    // GrammarCell* grammar = (GrammarCell*)malloc(INITSIZE*sizeof(GrammarCell));
    GrammarNode* currentGrammarNode = NULL;
    GrammarNode* previousGrammarNode = NULL;
    while(fgets(line,MAXCHARLINE,fptr) != NULL){
        p = strtok(line," ");
        i=0;
        lineCount++;
        if (lineCount>size){
            grammar=(GrammarCell*)realloc(grammar, 2*size*sizeof(GrammarCell));
            size=2*size;
        }
        
        while(p != NULL){
            if(p[strlen(p) - 1] == '\n') {
                p[strlen(p) - 1] = '\0';
            }
            
            if(i==0){
                //first elemenet of array
                strcpy(grammar[lineCount-1].nonTerminalName,p);
                grammar[lineCount-1].rule= (GrammarNode*)malloc(sizeof(GrammarNode));
                grammar[lineCount-1].rule->next=NULL;
                currentGrammarNode=grammar[lineCount-1].rule;
                i++;
            }
            else if(i==1){
                currentGrammarNode->next=NULL;
                strcpy(currentGrammarNode->symbolName,p);
                if(p[0]>=65&&p[0]<=90){
                    currentGrammarNode->symbolType= NonTerminal;
                }
                else{
                    currentGrammarNode->symbolType= Terminal;
                }
                if(strcmp(currentGrammarNode -> symbolName, "R1") == 0) {
                    currentGrammarNode -> symbolType = Terminal;
                }
                previousGrammarNode=currentGrammarNode;
                i++;
                //elements in linked List
            }
            else{
                currentGrammarNode=(GrammarNode*)malloc(sizeof(GrammarNode));
                previousGrammarNode->next=currentGrammarNode;
                currentGrammarNode->next=NULL;
                strcpy(currentGrammarNode->symbolName,p);
                if(p[0]>=65&&p[0]<=90){
                    currentGrammarNode->symbolType= NonTerminal;
                }
                else{
                    currentGrammarNode->symbolType= Terminal;
                }
                previousGrammarNode=currentGrammarNode;
                i++;

            }

            //printf("%s ",p); /* your word */
            p = strtok(NULL," ");
        }
    }
    // printf("Line count %d\n",lineCount);
    fclose(fptr);
    // printGrammar(lineCount,grammar);
    return lineCount;

}

void printGrammar(int lineCount, GrammarCell* grammar)
{

    for(int i=0; i<lineCount; i++){
       
      printf(" %d %s --> ",i,grammar[i].nonTerminalName);
      GrammarNode * tempNode= grammar[i].rule;
      while (tempNode!=NULL)
      {
            printf("%s ",tempNode->symbolName);
            tempNode=tempNode->next;
      }
      printf("\n");      

    }
}