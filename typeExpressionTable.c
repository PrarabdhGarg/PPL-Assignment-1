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
    root -> node.nonLeafNode.typeExpression.type = None;
    traverseDeclarationsParseTree(declarations, T);
    //traverseAssignmentsParseTree(assignments, T);
}

void traverseDeclarationsParseTree(ParseTreeNode *declarations, TypeExpressionTable T){
    declarations -> node.nonLeafNode.typeExpression.type = None;
    
    if(declarations -> node.nonLeafNode.ruleNumber == 2)
        traverseDeclarationParseTree(declarations -> node.nonLeafNode.children, T);
        

    else if(declarations -> node.nonLeafNode.ruleNumber == 1){
        traverseDeclarationParseTree(declarations -> node.nonLeafNode.children, T);
        traverseDeclarationsParseTree(declarations -> node.nonLeafNode.children + 1, T);
    }
    return;
}

void traverseDeclarationParseTree(ParseTreeNode *declaration, TypeExpressionTable T){
    int noOfChildren;
    ParseTreeNode *dataType, *primitiveDataType;
    bool dynamic = false;

    noOfChildren = declaration -> node.nonLeafNode.noOfChildren;
    dataType = declaration -> node.nonLeafNode.children + noOfChildren-1;
    primitiveDataType = dataType -> node.nonLeafNode.children;
    
    if(dataType -> node.nonLeafNode.ruleNumber == 7){ //PrimitiveDataType
        if(primitiveDataType -> node.nonLeafNode.ruleNumber == 10){
            declaration -> node.nonLeafNode.typeExpression.type = Integer;
            dataType -> node.nonLeafNode.typeExpression.type = Integer;
            primitiveDataType -> node.nonLeafNode.typeExpression.type = Integer;            
        }
        else if(primitiveDataType -> node.nonLeafNode.ruleNumber == 12){
            declaration -> node.nonLeafNode.typeExpression.type = Real;
            dataType -> node.nonLeafNode.typeExpression.type = Real;
            primitiveDataType -> node.nonLeafNode.typeExpression.type = Real;
        }
        else if(primitiveDataType -> node.nonLeafNode.ruleNumber == 11){
            declaration -> node.nonLeafNode.typeExpression.type = Boolean;
            dataType -> node.nonLeafNode.typeExpression.type = Boolean;
            primitiveDataType -> node.nonLeafNode.typeExpression.type = Boolean;
        }
    }
    else if(dataType -> node.nonLeafNode.children[noOfChildren-1].node.nonLeafNode.ruleNumber == 8){ //RectangularArrayType
        int dimensions = 1, i;
        DataTypes type;
        ParseTreeNode *rectangularDimensions, *rectangularDimension, *startIndex, *endIndex;
        RectangularRange *ranges;

        primitiveDataType = dataType -> node.nonLeafNode.children + 3;
        if(primitiveDataType -> node.nonLeafNode.ruleNumber != 10){
            type = Error;
            //error 
            //not integer
        }
        else{
            type = RectangularArray;
        }
        primitiveDataType -> node.nonLeafNode.typeExpression.type = type;
        dataType -> node.nonLeafNode.typeExpression.type = type;

        rectangularDimensions = dataType -> node.nonLeafNode.children + 1;
        while(rectangularDimensions -> node.nonLeafNode.ruleNumber == 13){
            rectangularDimension = rectangularDimensions -> node.nonLeafNode.children;
            rectangularDimension -> node.nonLeafNode.typeExpression.type = type;
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType = Integer;
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions = dimensions;
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges = malloc(sizeof(RectangularRange)*dimensions);

            for(i = 0; i < dimensions-1; i++){
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start = ranges[i].start;
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end = ranges[i].end;
            }
            ranges = rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges;
            startIndex = rectangularDimension -> node.nonLeafNode.children + 1;
            endIndex = rectangularDimension -> node.nonLeafNode.children + 3;
            
            if(startIndex -> node.nonLeafNode.ruleNumber == 41){
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].start = -1;
                // check for integer???
                dynamic = true;
            }
            else if(startIndex -> node.nonLeafNode.ruleNumber == 42){
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].start = atoi(startIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
            }

            if(endIndex -> node.nonLeafNode.ruleNumber == 41){
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = -1;
                // check for integer???
                dynamic = true;
            }
            else if(endIndex -> node.nonLeafNode.ruleNumber == 42){
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = atoi(startIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
            }

            rectangularDimensions = rectangularDimensions -> node.nonLeafNode.children + 1;
            dimensions++;
        }

        rectangularDimension = rectangularDimensions -> node.nonLeafNode.children;
        rectangularDimension -> node.nonLeafNode.typeExpression.type = type;
        rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType = Integer;
        rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions = dimensions;
        rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges = malloc(sizeof(RectangularRange)*dimensions);

        for(i = 0; i < dimensions-1; i++){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start = ranges[i].start;
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end = ranges[i].end;
        }
        ranges = rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges;
        startIndex = rectangularDimension -> node.nonLeafNode.children + 1;
        endIndex = rectangularDimension -> node.nonLeafNode.children + 3;
            
        if(startIndex -> node.nonLeafNode.ruleNumber == 41){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].start = -1;
            // check for integer???
            dynamic = true;
        }
        else if(startIndex -> node.nonLeafNode.ruleNumber == 42){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].start = atoi(startIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
        }

        if(endIndex -> node.nonLeafNode.ruleNumber == 41){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = -1;
            // check for integer???
            dynamic = true;
        }
        else if(endIndex -> node.nonLeafNode.ruleNumber == 42){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = atoi(startIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
        }

        primitiveDataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges = ranges;
        dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges = ranges;

        declaration -> node.nonLeafNode.typeExpression.type = type;
        declaration -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType = Integer;
        declaration -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions = dimensions;
        declaration -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges = ranges;
    }

    else if(dataType -> node.nonLeafNode.children[noOfChildren-1].node.nonLeafNode.ruleNumber == 9){//JaggedArrayType
        ParseTreeNode *emptyDimensions, startIndex, endIndex, *rowDefJaggedArray;
        int start, end, dimensions, size;
        emptyDimensions = declaration -> node.nonLeafNode.children + 7;
        startIndex = declaration -> node.nonLeafNode.children[3];
        endIndex = declaration -> node.nonLeafNode.children[5];
        start = atoi(startIndex.node.leafNode.lexeme);
        end = atoi(endIndex.node.leafNode.lexeme);
        emptyDimensions -> node.nonLeafNode.typeExpression.type = None;

        if(start > end){
            //error
        }
        else if(emptyDimensions -> node.nonLeafNode.ruleNumber == 16){
            dimensions = 3;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions = dimensions;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges = malloc((end - start + 2) * sizeof(JaggedRange));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].size = 2;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] = start;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] = end;

            /*for(int i = 1; i < start - end + 2; i++){

            }*/
        }
        else if(emptyDimensions -> node.nonLeafNode.ruleNumber == 17){
            dimensions = 2;
            size = end - start + 1;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions = dimensions;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges = malloc(2 * sizeof(JaggedRange));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].size = 2;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes = malloc(2 * sizeof(int));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] = start;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] = end;
        
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].size = size;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes = malloc(size * sizeof(int));
            
            rowDefJaggedArray = dataType->node.nonLeafNode.children + 11;
            /*for(int i = 0; i < size; i++){
                dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes[i] =  
            }*/
        }
    }

    if(declaration -> node.nonLeafNode.ruleNumber == 3){
        populateSymbolTable(declaration->node.nonLeafNode.children+1, declaration, T);
    }
    else if(declaration -> node.nonLeafNode.ruleNumber == 4){
        ParseTreeNode *multipleVariables;
        multipleVariables = declaration->node.nonLeafNode.children + 4;

        while(multipleVariables->node.nonLeafNode.ruleNumber == 5){
            populateSymbolTable(multipleVariables->node.nonLeafNode.children, declaration, T);
            multipleVariables = multipleVariables->node.nonLeafNode.children + 1;
        }
        populateSymbolTable(multipleVariables->node.nonLeafNode.children, declaration, T);
        populateSymbolTable(multipleVariables->node.nonLeafNode.children + 1, declaration, T);
    }
}

void populateSymbolTable(ParseTreeNode *terminal, ParseTreeNode *nonTerminal, TypeExpressionTable typeExpressionTable){
    TypeExpressionTableElement *element = (TypeExpressionTableElement *) malloc(sizeof(TypeExpressionTableElement));
    element->variableName = terminal->node.leafNode.lexeme;
    
    switch (nonTerminal -> node.nonLeafNode.typeExpression.type) {
        case Integer:
        case Real:
        case Boolean:
            element->arrayType = PrimitiveDataType;
            element->typeOfRange = NotApplicable;
            break;
        case RectangularArray:
            element->arrayType = RectangularArrayDataType;
            element->typeOfRange = nonTerminal->node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.typeOfRange;
            break;
        case JaggedArray:
            element->arrayType = JaggedArrayDataType;
            element->typeOfRange = Static;
            break;
        default:
            printf("Error\n");
            break;
    }
    
    element->typeExpression = nonTerminal->node.nonLeafNode.typeExpression;

    insertInTypeExpressionTable(element, typeExpressionTable);
}

void traverseAssignmentsParseTree(ParseTreeNode *assignments, TypeExpressionTable T){
    assignments->node.nonLeafNode.typeExpression.type = None;

    if(assignments->node.nonLeafNode.ruleNumber == 25){
        traverseAssignmentParseTree(assignments->node.nonLeafNode.children, T);
        traverseAssignmentsParseTree(assignments+1, T);
    }
    else if(assignments->node.nonLeafNode.ruleNumber == 26){
        traverseAssignmentParseTree(assignments->node.nonLeafNode.children, T);
    }
    return;
}

void traverseAssignmentParseTree(ParseTreeNode *assignment, TypeExpressionTable T){
    ParseTreeNode *singleNode, *expression;

    singleNode = assignment->node.nonLeafNode.children;
    expression = assignment->node.nonLeafNode.children + 2;

    traverseExpressionParseTree(expression, T);

    //compare type expression of epression and singlenode
}

void traverseExpressionParseTree(ParseTreeNode *expression, TypeExpressionTable T){
    if(expression->node.nonLeafNode.ruleNumber == 29){ //term

    }
    else if(expression->node.nonLeafNode.ruleNumber == 28){ //term plusminus expression

    }
}

bool compare(TypeExpression t1, TypeExpression t2) {
    if(t1.type == t2.type) {
        if(t1.type == PrimitiveDataType) {
            return true;
        } else if(t1.type == RectangularArray) {
             if(t1.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType == t2.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType) {
                if(t1.arrayTypeExpression.rectangularArrayTypeExpression.dimensions == t2.arrayTypeExpression.rectangularArrayTypeExpression.dimensions) {
                   for(int i  = 0; i < t1.arrayTypeExpression.rectangularArrayTypeExpression.dimensions; i++) {
                       if(t1.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start == t2.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start
                       && t1.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end == t2.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end)
                        continue;
                    return false;
                   }
                   return true;
                } else {
                    return false;
                }
            } else {
                 return false;
             }
        } else {
            if(t1.arrayTypeExpression.jaggedArrayTypeExpression.basicDataType == t2.arrayTypeExpression.jaggedArrayTypeExpression.basicDataType) {
                if(t1.arrayTypeExpression.jaggedArrayTypeExpression.dimensions == t2.arrayTypeExpression.jaggedArrayTypeExpression.dimensions) {
                    if(t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] 
                    && t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1]) {
                        int size = t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] - t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] + 2;
                        for(int i = 1; i < size; i++) {
                            if(t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size) {
                                for(int j = 0; j < t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size; j++) {
                                    if(t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].sizes[j] == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].sizes[j]) {
                                        continue;
                                    }
                                    return false;
                                }
                            } else {
                                return false;
                            }
                        }
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else {
                return false;
            }
            return true;
        }
    } else {
        return false;
    }
}