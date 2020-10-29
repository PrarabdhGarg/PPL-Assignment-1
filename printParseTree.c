/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include "printParseTree.h"

void printParseTree(ParseTreeNode *root, int depth){
    printf("+-----------------------+-------------------+------------------------------------------------------------------------------------------------------+----------------+------+\n");
    printf("| %21s |%18s | %100s |%15s |%4s |\n" , "SymbolName", "TokenType", "Lexeme/DataType", "Line/RuleNumber", "Depth");
    printf("+-----------------------+-------------------+------------------------------------------------------------------------------------------------------+----------------+------+\n");
    printParseTreeRec(root, 0);
}

void printParseTreeRec(ParseTreeNode *root, int depth){
    if(root == NULL)
        return;

    printf("| %21s | ", root -> symbolName);
    
    switch(root -> tokenType){
        case 0 : printf("%20s", "Terminal | ");
                 printf("%100s | ", (root -> node).leafNode.lexeme);
                 printf("LineNumber %3d | ", (root -> node).leafNode.lineNumber);
                 break;
        case 1 : printf("%20s", "Non-Terminal | ");
                 /* switch ((root->node).nonLeafNode.typeExpression.type)
                 {
                 case 0 :
                     printf("             Integer | ");
                     break;
                 case 1 :
                     printf("                Real | ");
                     break;    
                 case 2 :
                     printf("             Boolean | ");
                     break;
                 case 3 :
                     printf("    RectangularArray | ");
                     break;
                 case 4 :
                     printf("         JaggedArray | ");
                     break;
                 case 5 :
                     printf("                None | ");
                     break;
                 case 6 :
                     printf("               Error | ");
                     break;
                 default :
                     printf("    UnknownDataType  | ");
                     break;    
                 } */
                 TypeExpressionTableElement temp;
                 temp.typeExpression = root -> node.nonLeafNode.typeExpression;
                 if(temp.typeExpression.type == None) {
                     printf("%100s | ", "None");
                 } else if(temp.typeExpression.type == Error) {
                     printf("%100s | ", "Error");
                 } else {
                     char *result = (char *) malloc(sizeof(char) * 200);
                    returnTypeExpression(&temp, result);
                    printf("%100s | ", result);
                    free(result);
                 }
                 printf("RuleNumber %3d | ", (root -> node).nonLeafNode.ruleNumber);
                 break;
    }

    printf("%4d |\n", depth);

    printf("+-----------------------+-------------------+------------------------------------------------------------------------------------------------------+----------------+------+\n");
    if(root -> tokenType == Terminal)
        return;

    for(int i = 0; i < (root -> node).nonLeafNode.noOfChildren; i++){
        printParseTreeRec((root -> node).nonLeafNode.children + i, depth + 1);
    }
    return;
}