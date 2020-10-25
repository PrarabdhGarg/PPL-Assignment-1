#include "typeExpression.h"
#include "parseTree.h"

#define HASH_TABLE_SIZE 20

typedef enum {
    PrimitiveDataType,
    RectangularArray,
    JaggedArray
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

typedef struct {
    TypeExpressionTableElement *element;
    TypeExpressionTableElement *next;
} HashTableElement;


typedef HashTableElement *TypeExpressionTable;

TypeExpressionTable createNewTypeExpressionTable();
void insertInTypeExpressionTable(TypeExpressionTableElement *element, TypeExpressionTable typeExpressionTable);
TypeExpressionTableElement *getElementFromTypeExpressionTable(char *symbolName , TypeExpressionTable typeExpressionTable);
void traverseParseTree(ParseTreeNode *root, TypeExpressionTable T );
void traverseDeclarationsParseTree(ParseTreeNode *declarations, TypeExpressionTable T);
void traverseAssignmentsParseTree(ParseTreeNode *assignments, TypeExpressionTable T);
void traverseDeclarationParseTree(ParseTreeNode *declaration, TypeExpressionTable T);