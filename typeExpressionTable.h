/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/
    //trying out github in VSCode

#include "parseTree.h"

#define HASH_TABLE_SIZE 3

typedef enum {
    PrimitiveDataType,
    RectangularArrayDataType,
    JaggedArrayDataType
} ArrayType;

typedef enum {
    Static,
    Dynamic,
    NotApplicable
} TypeOfRange;

typedef struct {
    char *variableName;
    ArrayType arrayType;
    TypeOfRange typeOfRange;
    TypeExpression typeExpression;
    int lineNumber;
} TypeExpressionTableElement;

typedef struct hashTableElement {
    TypeExpressionTableElement *element; // A1 
    struct hashTableElement *next;// A1 -> A2 
} HashTableElement;


typedef HashTableElement *TypeExpressionTable;

TypeExpressionTable createNewTypeExpressionTable();
void insertInTypeExpressionTable(TypeExpressionTableElement *element, TypeExpressionTable typeExpressionTable);
TypeExpressionTableElement *getElementFromTypeExpressionTable(char *symbolName , TypeExpressionTable typeExpressionTable);
void traverseParseTree(ParseTreeNode *root, TypeExpressionTable T, bool printErrors);
void traverseDeclarationsParseTree(ParseTreeNode *declarations, TypeExpressionTable T);
void traverseAssignmentsParseTree(ParseTreeNode *assignments, TypeExpressionTable T);
void traverseAssignmentParseTree(ParseTreeNode *assignment, TypeExpressionTable T);
void traverseDeclarationParseTree(ParseTreeNode *declaration, TypeExpressionTable T);
void traverseExpressionParseTree(ParseTreeNode *expression, TypeExpressionTable T);
void traverseSingleTerm(ParseTreeNode *singleTerm, TypeExpressionTable T);
void populateSymbolTable(ParseTreeNode *terminal, ParseTreeNode *nonTerminal, TypeExpressionTable typeExpressionTable);
bool compareTypeExpression(TypeExpression t1, TypeExpression t2);
bool checkInteger(char *name, TypeExpressionTable T);