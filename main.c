#include "printParseTree.h"

void main(int argc, char *argv[]) {

    GrammarCell *grammar = (GrammarCell *) malloc(INITSIZE * sizeof(GrammarCell));
    TokenStream tokenStream;
    TypeExpressionTable typeExpressionTable = createNewTypeExpressionTable();
    int length = readGrammar("grammar.txt", grammar);
    printf("Grammar Read Sucessfully\n");

    printf("argc: %d argv[1] %s", argc, argv[1]);
    argv[1]!=NULL?tokeniseSourcecode(argv[1], tokenStream) :tokeniseSourcecode("sourcecode.txt", tokenStream);
    
        
    printf("Token Stream Created Sucessfully Length = %d\n", length);
    ParseTreeNode *root = (ParseTreeNode *) malloc(sizeof(ParseTreeNode));
    createParseTree(root, tokenStream, grammar, length);
    printf("Parse Tree Creation complete\n");
    traverseParseTree(root, typeExpressionTable);
    printf("traversed\n");
    printParseTree(root, 0);
    //To print the typeExpressionTable
    printf("\n\n");
    printTypeExpressionTable(typeExpressionTable);

}