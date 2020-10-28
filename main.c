#include "printTypeExpressionTable.h"

void main(int argc, char *argv[]) {

    GrammarCell *grammar = (GrammarCell *) malloc(INITSIZE * sizeof(GrammarCell));
    TokenStream tokenStream;
    TypeExpressionTable typeExpressionTable = createNewTypeExpressionTable();
    int length = readGrammar("grammar.txt", grammar);
    printf("Grammar Read Sucessfully\n");
    printf(" argc : %d v %s\n",argc,argv[1]);
    //tokeniseSourcecode("test.txt", tokenStream);
    if(argv[1]!=NULL){
        tokeniseSourcecode(argv[1], tokenStream);
        //printf(" argc : %d v  YIPEEEE%s\n",argc,argv[1]);
    }
    else{
        tokeniseSourcecode("test.txt", tokenStream);
    }
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