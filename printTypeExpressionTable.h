#include "typeExpressionTable.h"
#include <stdlib.h>
#include <stdio.h>


void printTypeExpressionTable(HashTableElement * T);
void printTypeExpressionTableElement(TypeExpressionTableElement * element);

char * returnArrayType(int i, char * s);
char * returnTypeOfRange(int i, char * s);
char * returnPrimitiveDataType(int i, char * s);
char * returnBasicElementDataType(int i, char * s);
char * returnTypeExpression(TypeExpressionTableElement * element, char * s);