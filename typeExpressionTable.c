#include "typeExpressionTable.h"

int calculateHash(char *str) {
    int hash = 0;
    for(int i = 0; i < strlen(str); i++) {
        hash += (int) str[i];
    }
    return hash % HASH_TABLE_SIZE;
}

TypeExpressionTable createNewTypeExpressionTable() {
    TypeExpressionTable typeExpressionTable = (TypeExpressionTable) malloc(sizeof(HashTableElement) * HASH_TABLE_SIZE);
    for(int i = 0; i < HASH_TABLE_SIZE; i++) {
        typeExpressionTable[i].next = NULL;
        typeExpressionTable[i].element =NULL;
    }
    return typeExpressionTable;
}

void insertInTypeExpressionTable(TypeExpressionTableElement *element, TypeExpressionTable typeExpressionTable) {
    HashTableElement *temp = (HashTableElement *) malloc(sizeof(HashTableElement));
    temp -> element = element;
    temp -> next = NULL;
    int hash = calculateHash(element -> variableName);
    HashTableElement *next = (typeExpressionTable + hash);
    if(next -> element == NULL) {
        next -> element = element;
        free(temp);
    } else {
        while(next -> next != NULL) {
            next = next -> next;
        }
        next -> next = temp;
    }
}

TypeExpressionTableElement *getElementFromTypeExpressionTable(char *symbolName , TypeExpressionTable typeExpressionTable) {
    int hash = calculateHash(symbolName);
    HashTableElement *next = (typeExpressionTable + hash);
    while(next != NULL) {
        if(strcmp(next -> element -> variableName, symbolName) == 0) {
            return next -> element;
        }
    }
    return NULL;
}

void traverseParseTree(ParseTreeNode *root, TypeExpressionTable T ){
    ParseTreeNode *declarations, *assignments;
    declarations = (root -> node).nonLeafNode.children + 4;
    assignments = (root -> node).nonLeafNode.children + 5;

    traverseDeclarationsParseTree(declarations, T);
    traverseAssignmentsParseTree(assignments, T);
}

void traverseDeclarationsParseTree(ParseTreeNode *declarations, TypeExpressionTable T){
    if(declarations -> node.nonLeafNode.ruleNumber == 2)
        traverseDeclarationParseTree(declarations -> node.nonLeafNode.children, T);
    
    else if(declarations -> node.nonLeafNode.ruleNumber == 3){
        traverseDeclarationParseTree(declarations -> node.nonLeafNode.children, T);
        traverseDeclarationsParseTree(declarations -> node.nonLeafNode.children + 1, T);
    }
    return;
}

void traverseDeclarationParseTree(ParseTreeNode *declaration, TypeExpressionTable T){
    int noOfChildren;
    ParseTreeNode *dataType, *primitiveDataType;

    noOfChildren = declaration -> node.nonLeafNode.noOfChildren;
    dataType = declaration -> node.nonLeafNode.children + noOfChildren-1;
    primitiveDataType = dataType -> node.nonLeafNode.children;
    
    if(dataType -> node.nonLeafNode.ruleNumber == 8){
        if(primitiveDataType -> node.nonLeafNode.ruleNumber == 12){
            declaration -> node.nonLeafNode.typeExpression.type = 0;
            dataType -> node.nonLeafNode.typeExpression.type = 0;
            primitiveDataType -> node.nonLeafNode.typeExpression.type = 0;            
        }
        else if(primitiveDataType -> node.nonLeafNode.ruleNumber == 13){
            declaration -> node.nonLeafNode.typeExpression.type = 2;
            dataType -> node.nonLeafNode.typeExpression.type = 2;
            primitiveDataType -> node.nonLeafNode.typeExpression.type = 2;
        }
        else if(primitiveDataType -> node.nonLeafNode.ruleNumber == 14){
            declaration -> node.nonLeafNode.typeExpression.type = 1;
            dataType -> node.nonLeafNode.typeExpression.type = 1;
            primitiveDataType -> node.nonLeafNode.typeExpression.type = 1;
        }
    }
    else if(declaration -> node.nonLeafNode.children[noOfChildren-1].node.nonLeafNode.ruleNumber == 9){

    }

    
    if(declaration -> node.nonLeafNode.ruleNumber == 4){

    }
    else if(declaration -> node.nonLeafNode.ruleNumber == 5){

    }
}

void traverseAssignmentsParseTree(ParseTreeNode *assignments, TypeExpressionTable T){

}