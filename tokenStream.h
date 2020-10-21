#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

#define MAXTOKENLENGHT 20
#define MAXLINELENGHT 100

typedef enum {  //enumration of token types
    Keyword,//0
    Identifier,//1
    Operator,//2
    IntegerConstant,//3
    Symbol//4
} TokenType;

typedef struct token{       //node of token linked list
    TokenType token;
    char lexeme[MAXTOKENLENGHT];
    int lineNumber;
    struct token *next;       
} Token;

typedef Token *TokenStream; //head of token linked list

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

TokenStream tokeniseSourcecode(char filename[], TokenStream tokenstream);  //takes filename and token pointer as 
                                                                        //input and returns token pointer to head of linked list

bool isKeyword(char* p);    //checks if given string is a Keyword

bool isOperator(char* p);   //checks if given string is an Operator

bool isSymbol(char* p);     //checks if given string is Symbol

bool isIdentifier(char* p); //checks if given string is Identifier

bool isIntegerConstant(char* p);    //checks if given string is integer constant

void printLinkedList(TokenStream tokenstream);  //takes token pointer to head of list and prints out Token type
                                                //lexeme and line number.