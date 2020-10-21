#include "typeExpression.h"
#include "tokenStream.h"

typedef struct {
    char *lexeme;
    int lineNumber;
} LeafNode;

typedef struct parseTreeNode {
    char *symbolName;
    TokenType tokenType;
    Node node;
} ParseTreeNode;

typedef struct {
    TypeExpression typeExpression;
    int ruleNumber;
    int noOfChildren;
    struct parseTreeNode *children;
} NonLeafNode;

typedef union {
    LeafNode leafNode;
    NonLeafNode nonLeafNode;
} Node;