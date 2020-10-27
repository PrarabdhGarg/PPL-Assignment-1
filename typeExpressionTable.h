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
} TypeExpressionTableElement;

typedef struct hashTableElement {
    TypeExpressionTableElement *element;
    struct hashTableElement *next;
} HashTableElement;


typedef HashTableElement *TypeExpressionTable; // array of hashtable element where each array element contains a linkedlist of type expressiontable elements

TypeExpressionTable createNewTypeExpressionTable();
void insertInTypeExpressionTable(TypeExpressionTableElement *element, TypeExpressionTable typeExpressionTable);
TypeExpressionTableElement *getElementFromTypeExpressionTable(char *symbolName , TypeExpressionTable typeExpressionTable);
void traverseParseTree(ParseTreeNode *root, TypeExpressionTable T);
void traverseDeclarationsParseTree(ParseTreeNode *declarations, TypeExpressionTable T);
void traverseAssignmentsParseTree(ParseTreeNode *assignments, TypeExpressionTable T);
void traverseAssignmentParseTree(ParseTreeNode *assignment, TypeExpressionTable T);
void traverseDeclarationParseTree(ParseTreeNode *declaration, TypeExpressionTable T);
void traverseExpressionParseTree(ParseTreeNode *expression, TypeExpressionTable T);
void populateSymbolTable(ParseTreeNode *terminal, ParseTreeNode *nonTerminal, TypeExpressionTable typeExpressionTable);