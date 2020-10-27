#include "typeExpression.h"
#include "tokenStream.h"

typedef struct {
    char *lexeme;
    int lineNumber;
} LeafNode;

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

typedef struct parseTreeNode {
    char *symbolName;
    SymbolType tokenType;
    Node node;
} ParseTreeNode;

void createParseTree(ParseTreeNode *root, Token *tokenStream, GrammarCell *grammar, int grammarLength);
void printParseTree(ParseTreeNode *root, int depth);
void printParseTreeRec(ParseTreeNode *root, int depth);
