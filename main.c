#include "parseTree.h"

void main() {
    GrammarCell *grammar = (GrammarCell *) malloc(INITSIZE * sizeof(GrammarCell));
    TokenStream tokenStream;
    int length = readGrammar("grammar.txt", grammar);
    printf("Grammar Read Sucessfully\n");
    tokeniseSourcecode("test.txt", tokenStream);
    printf("Token Stream Created Sucessfully Length = %d\n", length);
    ParseTreeNode *root = (ParseTreeNode *) malloc(sizeof(ParseTreeNode));
    createParseTree(root, tokenStream, grammar, length);
    printf("Parse Tree Creation complete\n");
}