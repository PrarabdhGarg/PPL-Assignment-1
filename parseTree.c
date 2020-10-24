#include "stack.h"

Token *nextToken = NULL;

int createParseTreeReccursive(ParseTreeNode *root, GrammarCell *grammar, int grammarLength) {
    if(root != NULL) {
        if(root -> tokenType == NonTerminal) {
            Token *tempToken = nextToken;
            // int result = -1;
            for(int i = 0; i < grammarLength; i++) {
                if(root -> node.nonLeafNode.noOfChildren != 0) {
                    free(root -> node.nonLeafNode.children);
                    root -> node.nonLeafNode.noOfChildren = 0;
                }
                if(strcmp(root -> symbolName, grammar[i].nonTerminalName) == 0) {
                    nextToken = tempToken;
                    GrammarNode *rule = grammar[i].rule;
                    Stack *tempStack = newStack();
                    int count = 0;
                    while(rule != NULL) {
                        count++;
                        /* ParseTreeNode temp;
                        temp.symbolName = (char *) malloc(sizeof(char) * strlen(rule -> symbolName));
                        strcpy(temp.symbolName, rule -> symbolName);
                        temp.tokenType = rule -> symbolType;
                        push(tempStack, temp); */
                        rule = rule -> next;
                    }
                    root -> node.nonLeafNode.noOfChildren = count;
                    root -> node.nonLeafNode.ruleNumber = i;
                    root -> node.nonLeafNode.children = (ParseTreeNode *) malloc(sizeof(ParseTreeNode) * count);
                    rule = grammar[i].rule;
                    int result = 0;
                    for(int j = 0; j < count && result >= 0; j++) {
                        ParseTreeNode temp;
                        temp.symbolName = (char *) malloc(sizeof(char) * strlen(rule -> symbolName));
                        strcpy(temp.symbolName, rule -> symbolName);
                        temp.tokenType = rule -> symbolType;
                        temp.node.nonLeafNode.noOfChildren = 0;
                        rule = rule -> next;
                        root -> node.nonLeafNode.children[j] = temp;
                        result = createParseTreeReccursive(&(root -> node.nonLeafNode.children[j]), grammar, grammarLength);
                        if(result < 0) {
                            break;
                        } else {
                            pop(tempStack);
                        }
                    }
                    if(result < 0) {
                        continue;
                    }
                    return 1;
                }
            }
            printf("Couldn't find a suitable rule for %s. \n", root -> symbolName);
            return -1;
        } else if(root -> tokenType == Terminal) {
            if(nextToken != NULL) {
                if(nextToken -> token == Keyword) {
                    if(strcmp(nextToken -> lexeme, root -> symbolName) == 0) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        printf("Token Consumed = %s\n", nextToken -> lexeme);
                        nextToken = nextToken -> next;
                        return 1;
                    }
                } else if(nextToken -> token == Operator) {
                    // TODO: Replace with symbol check
                    if(isOperator(root -> symbolName) == true && strcmp(nextToken -> lexeme, root -> symbolName) == 0) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        printf("Token Consumed = %s\n", nextToken -> lexeme);
                        nextToken = nextToken -> next;
                        return 1;
                    }
                } else if(nextToken -> token == IntegerConstant) {
                    // TODO: Replace with symbol check
                    if(strcmp(root -> symbolName, "integerConstant") == 0) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;\
                        printf("Token Consumed = %s\n", nextToken -> lexeme);
                        nextToken = nextToken -> next;
                        return 1;
                    }
                } else if(nextToken -> token == Symbol) {
                    // TODO: Replace with symbol check
                    if(isSymbol(root -> symbolName) == true && strcmp(nextToken -> lexeme, root -> symbolName) == 0) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        printf("Token Consumed = %s\n", nextToken -> lexeme);
                        nextToken = nextToken -> next;
                        return 1;
                    }
                } else {
                    // TODO: Replace with symbol check
                    if(strcmp("identifier", root -> symbolName) == 0) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        printf("Token Consumed = %s\n", nextToken -> lexeme);
                        nextToken = nextToken -> next;
                        return 1;
                    }
                }
                printf("Token Mismatch. BackTracking........\n");
                return -1;
            } else {
                printf("Token Stream Empty. BackTracking......\n");
                return -1;
            }
        } else {
            printf("Error occoured as symbol was neither Terminal nor NonTerminal");
            return -1;
        }
    } else {
        if(nextToken == NULL) 
            return 1;
        return -1;
    }
}

void createParseTree(ParseTreeNode *root, Token *tokenStream, GrammarCell *grammar, int grammarLength) {
    root -> symbolName = "Start";
    root -> tokenType = NonTerminal;
    root -> node.nonLeafNode.noOfChildren = 0;
    nextToken = tokenStream;
    createParseTreeReccursive(root, grammar, grammarLength);
}

/* void createParseTree(ParseTreeNode *root, Token *tokenStream, GrammarCell *grammar, int grammarLength) {
    Token *nextToken = tokenStream;
    StackElement baseElement;
    baseElement.symbolName = "Start";
    baseElement.SymbolType = NonTerminal;
    root -> tokenType = NonTerminal;
    root -> symbolName = "Start";
    ParseTreeNode *focusNode = root;
    Stack *mainStack = newStack();
    push(mainStack, baseElement);
    while(nextToken != NULL) {
        StackElement topElement = top(mainStack);
        pop(mainStack);
        if(topElement.SymbolType == Terminal) {
            
        } 
        else {
            for(int i = 0; i < grammarLength; i++) {
                if(strcmp(grammar[i].nonTerminalName, topElement.symbolName) == 0) {
                    Stack *tempStack = newStack();
                    GrammarNode *rule = grammar[i].rule;
                    int noOfChildren = 0;
                    while(rule != NULL) {
                        noOfChildren++;
                        StackElement temp;
                        temp.symbolName = (char *) malloc(sizeof(char) * strlen(rule -> symbolName));
                        strcpy(temp.symbolName, rule -> symbolName);
                        temp.SymbolType = rule -> symbolType;
                        push(tempStack, temp);
                        rule = rule -> next;
                    }
                    // ParseTreeNode *node;
                    // node->symbolName = (char *) malloc(sizeof(char) * strlen(topElement.symbolName));
                    // strcpy(node -> symbolName, topElement.symbolName);
                    // node -> tokenType = NonTerminal;
                    focusNode -> node.nonLeafNode.noOfChildren = noOfChildren;
                    focusNode -> node.nonLeafNode.children = (ParseTreeNode *) malloc(sizeof(ParseTreeNode) * noOfChildren);
                    focusNode -> node.nonLeafNode.ruleNumber = i;
                    int j = 0;
                    while(tempStack -> size > 0) {
                        StackElement topTemp = top(tempStack);
                        ParseTreeNode *temp = ((focusNode -> node).nonLeafNode.children)[j];
                        temp -> symbolName = (char *) malloc(sizeof(char) * strlen(topTemp.symbolName));
                        strcpy(temp -> symbolName, topTemp.symbolName);
                        temp -> tokenType = topTemp.SymbolType;
                        push(mainStack, topTemp);
                        pop(tempStack);
                    }
                    focusNode = ((focusNode -> node).nonLeafNode.children)[0];
                }
            }
        }
    }
} */


void printParseTree(ParseTreeNode *root, int depth){
    if(root == NULL)
        return;

    printf("%s | ", root -> symbolName);
    
    switch(root -> tokenType){
        case 0 : printf("Terminal | ");
                 printf("%s | ", (root -> node).leafNode.lexeme);
                 printf("%d | ", (root -> node).leafNode.lineNumber);
                 break;
        case 1 : printf("Non-Terminal | ");
                // TODO: Handle Type Expressions
                 // printTypeExpression((root -> node).nonLeafNode.typeExpression);
                 printf(" | ");
                 printf("%d | ", (root -> node).nonLeafNode.ruleNumber);
                 break;
    }

    printf("%d \n", depth);

    if(root -> tokenType == 0)
        return;

    for(int i = 0; i < (root -> node).nonLeafNode.noOfChildren; i++){
        printParseTree(&(root -> node).nonLeafNode.children[i], depth + 1);
    }
    return;
}