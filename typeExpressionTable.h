#include "typeExpression.h"
#include "parseTree.h"

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

typedef TypeExpressionTableElement * TypeExpressionTable;

void traverseParseTree(ParseTreeNode *root, TypeExpressionTable T );
void traverseDeclarationsParseTree(ParseTreeNode *declarations, TypeExpressionTable T);
void traverseAssignmentsParseTree(ParseTreeNode *assignments, TypeExpressionTable T);
void traverseDeclarationParseTree(ParseTreeNode *declaration, TypeExpressionTable T);