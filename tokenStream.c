#include "tokenStream.h"

/* int main(){     //sample driver main for code-testing
    char filename[]="test.txt";
    TokenStream tokenstream=NULL;
    
    tokenstream=tokeniseSourcecode(filename,tokenstream);
    printLinkedList(tokenstream);

    return 0;
} */

char KEYWORDS[12][10]={     //list of keywords for comparision 
    "program",
    "declare",
    "list",
    "of",
    "variables",
    "array",
    "size",
    "values",
    "jagged",
    "integer",
    "real",
    "boolean"
};

char SYMBOLS[9]={'(',')','{','}','[',']',':',';','.'};  //list of symbols for comparision

char OPERATORS[7]={'+','-','*','/','=','&','|'};    //list of operators for comparision

TokenStream tokeniseSourcecode(char filename[], TokenStream tokenstream)
{
	FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("Error! opening file");
       return;
    }
    
    char* p;
    char in_line[MAXLINELENGHT];
    int lineCount=0;

    // tokenstream = (Token*)malloc(sizeof(Token));
    Token* currentToken=tokenstream;
    Token* prevToken=NULL;
    while (fgets(in_line,MAXLINELENGHT,fptr) != NULL){  //loop to read one line and input it to in_line
        p = strtok(in_line," \n");
        lineCount++;

        while(p!=NULL){  //loop to tokenise one line
            strcpy(currentToken->lexeme,p);
            currentToken->lineNumber=lineCount;
            if(isKeyword(p))
                currentToken->token=Keyword;
            else if(isOperator(p))
                currentToken->token=Operator;
            else if(isSymbol(p))
                currentToken->token=Symbol;
            else if(isIntegerConstant(p))
                currentToken->token=IntegerConstant;
            else if(isIdentifier(p))
                currentToken->token=Identifier;
            
            prevToken=currentToken;
            
            currentToken = (Token*)malloc(sizeof(Token));

            prevToken->next=currentToken;
            
            p = strtok(NULL," \n");
        }
    }
    free(currentToken);
    prevToken->next=NULL;  //freeing one last extra node and setting pointer of last node to NULL
    fclose(fptr);
    return tokenstream;
}

bool isKeyword(char* p){
    int i=0;
    for(;i<12;i++){
        if(strcmp(p,KEYWORDS+i)==0)
            break;
    }
    if(i==12)
        return false;
    else
        return true;
}

bool isOperator(char* p){
    int i=0;
    for(;i<7;i++){
        if(OPERATORS[i]==p[0])
            break;
    }
    if(i==7)
        return false;
    else
        return true;    
}

bool isSymbol(char* p){
    int i=0;
    for(;i<9;i++){
        if(SYMBOLS[i]==p[0])
            break;
    }
    if(i==9)
        return false;
    else
        return true;    
}

bool isIntegerConstant(char* p){
    if(p[0]>=48&&p[0]<=57)
        return true;
    else
    return false;    
}

bool isIdentifier(char* p){
    return true;
}

void printLinkedList(TokenStream tokenstream){
    Token* current=tokenstream;

    while(current!=NULL){
        printf("String:%s,Token:%d,LineNUmber:%d\n",current->lexeme,current->token,current->lineNumber);
        current=current->next;
    }
}