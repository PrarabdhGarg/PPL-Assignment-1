#include "printParseTree.h"

void main(int argc, char *argv[]) {

    GrammarCell *grammar = (GrammarCell *) malloc(INITSIZE * sizeof(GrammarCell));
    TokenStream tokenStream = (TokenStream) malloc(sizeof(Token));
    TypeExpressionTable typeExpressionTable = createNewTypeExpressionTable();
    int length = readGrammar("grammar.txt", grammar);
    printf("Grammar Read Sucessfully\n");
    //printf(" argc : %d v %s\n",argc,argv[1]);
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

    int option;
    do{
    printf("Enter your option : ");
    scanf("%d",&option);
    switch (option){
    case 0:
        break;
    case 1:
        createParseTree(root, tokenStream, grammar, length);
        printf("Parse Tree Creation complete\n");
        break;
    case 2:
        createParseTree(root, tokenStream, grammar, length);
        printf("Parse Tree Creation complete\n");
        traverseParseTree(root, typeExpressionTable);
        printf("Parse Tree Traversal Complete\n");
        break;
    case 3:
        createParseTree(root, tokenStream, grammar, length);
        printf("Parse Tree Creation complete\n");
        traverseParseTree(root, typeExpressionTable);
        printf("Parse Tree Traversal Complete\n");
        printParseTree(root, 0);
        break;
    case 4:
        createParseTree(root, tokenStream, grammar, length);
        printf("Parse Tree Creation complete\n");
        traverseParseTree(root, typeExpressionTable);
        printf("Parse Tree Traversal Complete\n");
        printTypeExpressionTable(typeExpressionTable);
        break;        
    default:
        printf("Please enter a valid option!!!\n");
        break;
    }
    }while(option!=0);
}