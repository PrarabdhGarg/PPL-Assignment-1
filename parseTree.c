#include "parseTree.h"
#include "grammar.h"
#include "stack.h"
#include "string.h"

Token *nextToken = NULL;

int createParseTreeReccursive(ParseTreeNode *root, GrammarCell *grammar, int grammarLength) {
    if(root != NULL) {
        if(root -> tokenType == NonTerminal) {
            for(int i = 0; i < grammarLength; i++) {
                if(strcmp(root -> symbolName, grammar[i].nonTerminalName) == 0) {
                    GrammarNode *rule = grammar[i].rule;
                    Stack *tempStack = newStack();
                    int count = 0;
                    while(rule != NULL) {
                        count++;
                        ParseTreeNode temp;
                        temp.symbolName = (char *) malloc(sizeof(char) * strlen(rule -> symbolName));
                        strcpy(temp.symbolName, rule -> symbolName);
                        temp.tokenType = rule -> symbolType;
                        push(tempStack, temp);
                        rule = rule -> next;
                    }
                    root -> node.nonLeafNode.noOfChildren = count;
                    root -> node.nonLeafNode.ruleNumber = i;
                    root -> node.nonLeafNode.children = (ParseTreeNode *) malloc(sizeof(ParseTreeNode) * count);
                    for(int i = 0; i < count; i++) {
                        root -> node.nonLeafNode.children[i] = top(tempStack);
                        int result = createParseTreeReccursive(&(root -> node.nonLeafNode.children[i]), grammar, grammarLength);
                        if(result < 0) {
                            continue;
                        } else {
                            pop(tempStack);
                        }
                    }
                    return 1;
                }
            }
            return -1;
        } else if(root -> tokenType == Terminal) {
            if(nextToken != NULL) {
                if(nextToken -> token == Keyword) {
                    if(strcmp(nextToken -> lexeme, root -> symbolName) == 0) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        nextToken = nextToken -> next;
                        return 1;
                    }
                } else if(nextToken -> token == Operator) {
                    // TODO: Replace with symbol check
                    if(1 == 1) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        nextToken = nextToken -> next;
                        return 1;
                    }
                } else if(nextToken -> token == IntegerConstant) {
                    // TODO: Replace with symbol check
                    if(1 == 1) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        nextToken = nextToken -> next;
                        return 1;
                    }
                } else if(nextToken -> token == Symbol) {
                    // TODO: Replace with symbol check
                    if(1 == 1) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        nextToken = nextToken -> next;
                        return 1;
                    }
                } else {
                    // TODO: Replace with symbol check
                    if(1 == 1) {
                        root -> node.leafNode.lexeme = nextToken -> lexeme;
                        root -> node.leafNode.lineNumber = nextToken -> lineNumber;
                        nextToken = nextToken -> next;
                        return 1;
                    }
                }
            } else {
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