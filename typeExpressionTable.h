#include "typeExpression.h"

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

void printTypeExpressionTable(TypeExpressionTableElement * T);
void printTypeExpressionTableElement(TypeExpressionTableElement element);

