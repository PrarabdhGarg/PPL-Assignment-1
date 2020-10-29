/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include "typeExpressionTable.h"
#include <stdlib.h>
#include <stdio.h>

int writeToFile;
FILE *fileptr;
void printTypeExpressionTable(HashTableElement * T);
void printTypeExpressionTableElement(TypeExpressionTableElement * element);

char * returnArrayType(int i , char * s) ;
char * returnTypeOfRange(int i , char * s);
char * returnPrimitiveDataType(int i, char * s);
char * returnBasicElementDataType(int i, char * s);
char * returnTypeExpression(TypeExpressionTableElement * element, char * s);