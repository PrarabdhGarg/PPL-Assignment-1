#include "printParseTree.h"

void printParseTree(ParseTreeNode *root, int depth){
    printf("| %30s | %20s | %100s | %30s | %4s |\n" , "SymbolName", "TokenType", "Lexeme/DataType", "Line/RuleNumber", "Depth");
    printParseTreeRec(root, 0);
}

void printParseTreeRec(ParseTreeNode *root, int depth){
    if(root == NULL)
        return;

    printf("%30s | ", root -> symbolName);
    
    switch(root -> tokenType){
        case 0 : printf("%20s", "Terminal | ");
                 printf("%100s | ", (root -> node).leafNode.lexeme);
                 printf("LineNumber %19d | ", (root -> node).leafNode.lineNumber);
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
                 printf("RuleNumber %19d | ", (root -> node).nonLeafNode.ruleNumber);
                 break;
    }

    printf("%4d |\n", depth);

    if(root -> tokenType == Terminal)
        return;

    for(int i = 0; i < (root -> node).nonLeafNode.noOfChildren; i++){
        printParseTreeRec((root -> node).nonLeafNode.children + i, depth + 1);
    }
    return;
}