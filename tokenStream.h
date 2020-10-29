/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include "grammar.h"
#include<stdbool.h>

#define MAXTOKENLENGHT 20
#define MAXLINELENGHT 500

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

TokenStream tokeniseSourcecode(char filename[], TokenStream tokenstream);  //takes filename and token pointer as 
                                                                        //input and returns token pointer to head of linked list

bool isKeyword(char* p);    //checks if given string is a Keyword

bool isOperator(char* p);   //checks if given string is an Operator

bool isSymbol(char* p);     //checks if given string is Symbol

bool isIdentifier(char* p); //checks if given string is Identifier

bool isIntegerConstant(char* p);    //checks if given string is integer constant

void printLinkedList(TokenStream tokenstream);  //takes token pointer to head of list and prints out Token type
                                                //lexeme and line number.