/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include "printParseTree.h"

void main(int argc, char *argv[]) {

    GrammarCell *grammar = (GrammarCell *) malloc(INITSIZE * sizeof(GrammarCell));
    TokenStream tokenStream = (TokenStream) malloc(sizeof(Token));
    TypeExpressionTable typeExpressionTable = createNewTypeExpressionTable();
    int length = readGrammar("grammar.txt", grammar);
    if(argv[1]!=NULL){
        tokeniseSourcecode(argv[1], tokenStream);
    }
    else{
        tokeniseSourcecode("test.txt", tokenStream);
    }

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
        break;
    case 2:
        createParseTree(root, tokenStream, grammar, length);
        traverseParseTree(root, typeExpressionTable, true);
        printf("Parse Tree Traversal Complete\n");
        break;
    case 3:
        printGrammar(length, grammar);
        createParseTree(root, tokenStream, grammar, length);
        traverseParseTree(root, typeExpressionTable, false);
        printParseTree(root, 0);
        break;
    case 4:
        createParseTree(root, tokenStream, grammar, length);
        traverseParseTree(root, typeExpressionTable, false);
        printTypeExpressionTable(typeExpressionTable);
        break;        
    default:
        printf("Please enter a valid option!!!\n");
        break;
    }
    } while(option!=0);
}