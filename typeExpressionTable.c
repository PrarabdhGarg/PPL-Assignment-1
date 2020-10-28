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
        next = next -> next;
    }
    return NULL;
}

void traverseParseTree(ParseTreeNode *root, TypeExpressionTable T ){
    ParseTreeNode *declarations, *assignments;
    declarations = (root -> node).nonLeafNode.children + 4;
    assignments = (root -> node).nonLeafNode.children + 5;
    root -> node.nonLeafNode.typeExpression.type = None;
    traverseDeclarationsParseTree(declarations, T);
    traverseAssignmentsParseTree(assignments, T);
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
    else if(dataType -> node.nonLeafNode.ruleNumber == 8) { //RectangularArrayType
        int dimensions = 1, i;
        DataTypes type;
        ParseTreeNode *rectangularDimensions, *rectangularDimension, *startIndex, *endIndex;
        RectangularRange *ranges;

        primitiveDataType = dataType -> node.nonLeafNode.children + 3;
        if(primitiveDataType -> node.nonLeafNode.ruleNumber != 10){
            type = Error;
            //error 
            
            printf("Arrays can only have the base type of integers.\n", dataType->node.nonLeafNode.children[0].node.leafNode.lineNumber);
            
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
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.typeOfRange = 0;
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
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = atoi(endIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
            }

            if(dynamic){
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.typeOfRange = 1;
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
            if(checkInteger(startIndex->node.nonLeafNode.children->node.leafNode.lexeme, T)) {
                printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", startIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
            }
            else {
                //error not of integer type
                //TODO add error in non terminal
                printf("Error: %3d: Array ranges must be of type Integer\n", startIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
            }
            dynamic = true;
        }
        else if(startIndex -> node.nonLeafNode.ruleNumber == 42){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].start = atoi(startIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
        }

        if(endIndex -> node.nonLeafNode.ruleNumber == 41){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = -1;
            if(checkInteger(endIndex->node.nonLeafNode.children->node.leafNode.lexeme, T)){
                printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", endIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
            }
            else {
                //error not of integer type
                //TODO add error in non terminal
                printf("Error: %3d: Array ranges must be of type Integer\n", endIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
            }
            dynamic = true;
        }
        else if(endIndex -> node.nonLeafNode.ruleNumber == 42){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = atoi(endIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
        }

        if(dynamic){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.typeOfRange = 1;
        }

        //start > end error

        primitiveDataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges = ranges;
        dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges = ranges;

        declaration -> node.nonLeafNode.typeExpression.type = type;
        declaration -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType = Integer;
        declaration -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions = dimensions;
        declaration -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges = ranges;
        if(dynamic){
            declaration -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.typeOfRange = 1;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.typeOfRange = 1;
        }
        else{
            declaration -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.typeOfRange = 0;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.typeOfRange = 0;
        }
    }

    else if(dataType -> node.nonLeafNode.children[noOfChildren-1].node.nonLeafNode.ruleNumber == 9){//JaggedArrayType
        ParseTreeNode *emptyDimensions, startIndex, endIndex, *rowDefJaggedArray;
        int start, end, dimensions, size, index;
        emptyDimensions = declaration -> node.nonLeafNode.children + 7;
        startIndex = declaration -> node.nonLeafNode.children[3];
        endIndex = declaration -> node.nonLeafNode.children[5];
        start = atoi(startIndex.node.leafNode.lexeme);
        end = atoi(endIndex.node.leafNode.lexeme);
        emptyDimensions -> node.nonLeafNode.typeExpression.type = None;
        rowDefJaggedArray = dataType->node.nonLeafNode.children + 11;

        if(start > end) {
            //error
            printf("Error: %3d: Start index of range should be lesser than the end index.\n", dataType->node.nonLeafNode.children[0].node.leafNode.lineNumber);
            // I think we don't need to consider this case
        }
        else if(emptyDimensions -> node.nonLeafNode.ruleNumber == 16){
            dimensions = 3;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions = dimensions;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges = malloc((end - start + 2) * sizeof(JaggedRange));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].size = 2;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] = start;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] = end;

            for(int i = 1; i < end - start + 2; i++){
                index = atoi(rowDefJaggedArray->node.nonLeafNode.children[2].node.leafNode.lexeme);
                if(index != i + start - 1) {
                    printf("Error: %3d: The row you are trying to define is not mentioned in the range.\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    //error
                }
                size = atoi(rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lexeme);
                //check values and get size

                if(i != start - end + 1 && rowDefJaggedArray->node.nonLeafNode.ruleNumber != 18){
                    //error less declarations
                    printf("Error: %3d: The number of indices provided are lesser than expected\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    break;
                }
                if(i == end - start + 1 && rowDefJaggedArray->node.nonLeafNode.ruleNumber != 19){
                    //error more declarations
                    
                    printf("Error: %3d: The number of indices provided are greater than expected\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                }
                if(i != end - start + 1)
                    rowDefJaggedArray = rowDefJaggedArray->node.nonLeafNode.children + 12;
            }
        }
        else if(emptyDimensions -> node.nonLeafNode.ruleNumber == 17){
            dimensions = 2;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions = dimensions;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges = malloc(2 * sizeof(JaggedRange));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].size = 2;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes = malloc(2 * sizeof(int));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] = start;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] = end;
        
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].size = size;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes = malloc((size+1) * sizeof(int));
            
            
            for(int i = 0; i < end - start + 1; i++){
                index = atoi(rowDefJaggedArray->node.nonLeafNode.children[2].node.leafNode.lexeme);
                if(index != i + start - 1){
                    //error
                    printf("Error: %3d: The row you are trying to define is not mentioned in the range.\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                }
                size = atoi(rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lexeme);
                dataType->node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes[i] = size;
                //check values

                if(i != start - end + 1 && rowDefJaggedArray->node.nonLeafNode.ruleNumber != 18){
                    //error less declarations
                    printf("Error: %3d: The number of indices provided are lesser than expected\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    break;
                }
                if(i == end - start + 1 && rowDefJaggedArray->node.nonLeafNode.ruleNumber != 19){
                    //error more declarations
                    printf("Error: %3d: The number of indices provided are greater than expected\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                }
                if(i != end - start + 1)
                    rowDefJaggedArray = rowDefJaggedArray->node.nonLeafNode.children + 12;
            }
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
        traverseAssignmentsParseTree(assignments->node.nonLeafNode.children+1, T);
    }
    else if(assignments->node.nonLeafNode.ruleNumber == 26){
        traverseAssignmentParseTree(assignments->node.nonLeafNode.children, T);
    }
    return;
}

void traverseAssignmentParseTree(ParseTreeNode *assignment, TypeExpressionTable T){
    ParseTreeNode *singleTerm, *expression;

    singleTerm = assignment->node.nonLeafNode.children;
    expression = assignment->node.nonLeafNode.children + 2;

    traverseExpressionParseTree(expression, T);
    traverseSingleTerm(singleTerm, T);

    if(compareTypeExpression(singleTerm->node.nonLeafNode.typeExpression, expression->node.nonLeafNode.typeExpression)){
        assignment->node.nonLeafNode.typeExpression = singleTerm->node.nonLeafNode.typeExpression;
    }
    else{
        assignment->node.nonLeafNode.typeExpression.type = Error;
        //error type mismatch
        // TODO: Modify print statement to include type of left and right if possible
        printf("Error: %3d: Type mismatch.\n", assignment->node.nonLeafNode.children[1].node.leafNode.lineNumber);
    }
}

void traverseExpressionParseTree(ParseTreeNode *expression, TypeExpressionTable T){
    switch(expression->node.nonLeafNode.ruleNumber){
        case 28:
            traverseExpressionParseTree(expression->node.nonLeafNode.children, T);
            traverseExpressionParseTree(expression->node.nonLeafNode.children + 2, T);
            if(compareTypeExpression(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression, expression->node.nonLeafNode.children[2].node.nonLeafNode.typeExpression)){
                if(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == Integer 
                    || expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == Real
                    || expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == JaggedArray
                    || expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == RectangularArray)
                {
                    expression->node.nonLeafNode.typeExpression = expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression;
                }
                else{
                    expression->node.nonLeafNode.typeExpression.type = Error;
                    printf("Error: %3d: Arithmatic operations cannot operate on Boolean operands\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    //error
                } 
            }
            else{
                expression->node.nonLeafNode.typeExpression.type = Error;
                
                // TODO: Modify print statement to include type of left and right if possible
                printf("Error: %3d: Type mismatch.\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                //error
            }
            break;
        case 29:
            traverseExpressionParseTree(expression->node.nonLeafNode.children, T);
            expression->node.nonLeafNode.typeExpression = expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression;
            break;
        case 30:
            traverseExpressionParseTree(expression->node.nonLeafNode.children, T);
            traverseExpressionParseTree(expression->node.nonLeafNode.children + 2, T);
            if(compareTypeExpression(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression, expression->node.nonLeafNode.children[2].node.nonLeafNode.typeExpression)){
                if(expression->node.nonLeafNode.children[1].node.nonLeafNode.ruleNumber == 45){ // multiplication
                    if(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == Integer 
                        || expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == Real
                        || expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == JaggedArray
                        || expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == RectangularArray)
                    {
                        expression->node.nonLeafNode.typeExpression = expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression;
                    }
                    else{
                        expression->node.nonLeafNode.typeExpression.type = Error;
                        
                        printf("Error: %3d: Arithmatic operations cannot operate on Boolean operands\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        //error
                    } 
                }
                else if(expression->node.nonLeafNode.children[1].node.nonLeafNode.ruleNumber == 46){ // division
                    if(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == Integer 
                        || expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == Real
                        )
                    {
                        expression->node.nonLeafNode.typeExpression.type = Real;
                    }
                    else{
                        expression->node.nonLeafNode.typeExpression.type = Error;
                        
                        printf("Error: %3d: Division must have both operands of type Integer or Real\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                }
            }
            else{
                expression->node.nonLeafNode.typeExpression.type = Error;
                
                // TODO: Modify print statement to include type of left and right if possible
                printf("Error: %3d: Type mismatch.\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                //error
            }
            break;
        case 31:
            traverseExpressionParseTree(expression->node.nonLeafNode.children, T);
            expression->node.nonLeafNode.typeExpression = expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression;
            break;
        case 32:
            traverseExpressionParseTree(expression->node.nonLeafNode.children, T);
            traverseExpressionParseTree(expression->node.nonLeafNode.children + 2, T);
            if(compareTypeExpression(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression, expression->node.nonLeafNode.children[2].node.nonLeafNode.typeExpression)){
                if(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == Boolean)
                {
                    expression->node.nonLeafNode.typeExpression = expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression;
                }
                else{
                    expression->node.nonLeafNode.typeExpression.type = Error;
                    //error
                    
                    printf("Error: %3d: Boolean operations needs boolean operands\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                }
            }
            else{
                expression->node.nonLeafNode.typeExpression.type = Error;
                
                // TODO: Modify print statement to include type of left and right if possible
                printf("Error: %3d: Type mismatch.\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                //error
            }
            break;
        case 33:
            traverseExpressionParseTree(expression->node.nonLeafNode.children, T);
            expression->node.nonLeafNode.typeExpression = expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression;
            break;
        case 34:
            traverseSingleTerm(expression->node.nonLeafNode.children, T);
            traverseExpressionParseTree(expression->node.nonLeafNode.children + 2, T);
            if(compareTypeExpression(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression, expression->node.nonLeafNode.children[2].node.nonLeafNode.typeExpression)){
                if(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression.type == Boolean)
                {
                    expression->node.nonLeafNode.typeExpression = expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression;
                }
                else{
                    expression->node.nonLeafNode.typeExpression.type = Error;
                    //error
                    
                    printf("Error: %3d: Boolean operations needs boolean operands\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                }
            }
            else{
                expression->node.nonLeafNode.typeExpression.type = Error;
                
                // TODO: Modify print statement to include type of left and right if possible
                printf("Error: %3d: Type mismatch.\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                //error
            }
            break;
        case 35:
            traverseSingleTerm(expression->node.nonLeafNode.children, T);
            expression->node.nonLeafNode.typeExpression = expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression;
            break;
    }
    return;
}

void traverseSingleTerm(ParseTreeNode *singleTerm, TypeExpressionTable T){
    TypeExpression typeExpression;
    ParseTreeNode *indexList, *indexNode;
    int index;
    switch(singleTerm->node.nonLeafNode.ruleNumber){
        case 36:
            typeExpression = getElementFromTypeExpressionTable(singleTerm->node.nonLeafNode.children->node.leafNode.lexeme, T)->typeExpression;
            if(typeExpression.type != RectangularArray && typeExpression.type != JaggedArray){
                //error
                if(typeExpression.type == Integer || typeExpression.type == Real || typeExpression.type == Boolean) {
                    printf("Error: %3d: Primitive data types cannot be indexed\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                }
                else{
                    printf("Error: %3d: Error in variable declaration\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                }
                singleTerm->node.nonLeafNode.typeExpression.type = Error;
                return;
            }
            else if(typeExpression.type == RectangularArray){
                indexList = singleTerm->node.nonLeafNode.children + 2;
                for(int i = 0; i < typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions; i++){
                    if(i != typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions -1 && indexList->node.nonLeafNode.ruleNumber == 40){
                        //error less index
                        
                        printf("Error: %3d: The number of indices provided are lesser than expected. Array slices are not supported\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        indexList->node.nonLeafNode.typeExpression.type == Error;
                        indexList->node.nonLeafNode.children->node.nonLeafNode.typeExpression.type = Error;
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }
                    if(i == typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions -1 && indexList->node.nonLeafNode.ruleNumber != 40){
                        //error more index
                        
                        printf("Error: %3d: The number of indices provided are greater than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        indexList->node.nonLeafNode.typeExpression.type == Error;
                        indexList->node.nonLeafNode.children->node.nonLeafNode.typeExpression.type = Error;
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }

                    indexNode = indexList->node.nonLeafNode.children;
                    
                    if(indexNode->node.nonLeafNode.ruleNumber == 41){
                        if(checkInteger(indexNode->node.nonLeafNode.children->node.leafNode.lexeme, T)){
                            //can't be checked at run time
                            
                            printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", indexNode->node.nonLeafNode.children->node.leafNode.lineNumber);
                            continue;
                        }
                        else {
                            printf("Error : %3d : %dth dimmension must be an integer\n", indexNode->node.nonLeafNode.children->node.leafNode.lineNumber, i+1);
                            //error not of integer type
                        }
                    }
                    else if(indexNode->node.nonLeafNode.ruleNumber == 42){
                        index = atoi(indexNode->node.nonLeafNode.children->node.leafNode.lexeme);

                        if(index <= typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end 
                            && index >= typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start)
                        {
                            continue;
                        }
                        else{
                            //error;
                            printf("Error: %3d : Array index out of bounds for %dth dimmension.\n", indexList->node.nonLeafNode.children -> node.nonLeafNode.children -> node.leafNode.lineNumber, i+1);
                            indexList->node.nonLeafNode.typeExpression.type == Error;
                            indexNode->node.nonLeafNode.typeExpression.type == Error;
                            singleTerm->node.nonLeafNode.typeExpression.type = Error;
                            return;
                        }
                    }
                    indexNode->node.nonLeafNode.typeExpression.type = None;
                    indexList->node.nonLeafNode.typeExpression.type = None;
                }
                singleTerm->node.nonLeafNode.typeExpression.type = Integer;
            }
            else{ // jagged array 
                int index1, index2, index3, dimensions, start, end;
                bool dynamic = false;
                indexList = singleTerm->node.nonLeafNode.children + 2;
                indexNode = indexList->node.nonLeafNode.children;

                dimensions = singleTerm->node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions;
                if(indexList->node.nonLeafNode.ruleNumber == 40){
                    //error less 
                    printf("Error: %3d: The number of indices provided are lesser than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    indexList->node.nonLeafNode.typeExpression.type == Error;
                    indexNode->node.nonLeafNode.typeExpression.type == Error;
                    singleTerm->node.nonLeafNode.typeExpression.type = Error;
                    return;
                }
                else if(indexList->node.nonLeafNode.ruleNumber == 39){
                    if(indexNode->node.nonLeafNode.ruleNumber == 41){
                        //dynamic warning
                        printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        // check integer
                        dynamic = true;
                    }
                    else if(indexNode->node.nonLeafNode.ruleNumber == 42){
                        index1 = atoi(indexNode->node.nonLeafNode.children->node.leafNode.lexeme);
                    }
                }
                
                indexList = indexList->node.nonLeafNode.children + 1;
                indexNode = indexList->node.nonLeafNode.children;
                if(indexList->node.nonLeafNode.ruleNumber == 40){
                    if(dimensions == 3){
                        //error less 
                        
                        printf("Error: %3d: The number of indices provided are lesser than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        indexList->node.nonLeafNode.typeExpression.type == Error;
                        indexNode->node.nonLeafNode.typeExpression.type == Error;
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }
                    else{
                        if(indexNode->node.nonLeafNode.ruleNumber == 41){
                            //dynamic warning
                             
                            printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                            // check integer
                            dynamic = true;
                        }
                        else if(indexNode->node.nonLeafNode.ruleNumber == 42){
                            index2 = atoi(indexNode->node.nonLeafNode.children->node.leafNode.lexeme);
                        }   
                    }
                }
                else if(indexList->node.nonLeafNode.ruleNumber == 39){
                    if(dimensions == 2){
                        //error more
                        printf("Error: %3d: The number of indices provided are greater than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        indexList->node.nonLeafNode.typeExpression.type == Error;
                        indexNode->node.nonLeafNode.typeExpression.type == Error;
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }
                    else {
                        if(indexNode->node.nonLeafNode.ruleNumber == 41){
                            //dynamic warning
                            
                            dynamic = true;
                            printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                            // check integer
                        }
                        else if(indexNode->node.nonLeafNode.ruleNumber == 42){
                            index2 = atoi(indexNode->node.nonLeafNode.children->node.leafNode.lexeme);
                        }
                        indexList = indexList->node.nonLeafNode.children + 1;
                        indexNode = indexList->node.nonLeafNode.children;
                    }
                }
                
                if(indexList->node.nonLeafNode.ruleNumber == 40){
                    if(indexNode->node.nonLeafNode.ruleNumber == 41){
                        
                        printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        //dynamic warning
                        // check integer
                        dynamic = true;
                    }
                    else if(indexNode->node.nonLeafNode.ruleNumber == 42){
                        index3 = atoi(indexNode->node.nonLeafNode.children->node.leafNode.lexeme);
                    }
                }
                else if(indexList->node.nonLeafNode.ruleNumber == 39){
                    //error more 
                    
                    printf("Error: %3d: The number of indices provided are greater than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    indexList->node.nonLeafNode.typeExpression.type == Error;
                    indexNode->node.nonLeafNode.typeExpression.type == Error;
                    singleTerm->node.nonLeafNode.typeExpression.type = Error;
                    return;
                }

                start = typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0];
                end = typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1];
                
                if(index1 <= end && index1 >= start)
                {
                    //do nothing
                }
                else{
                    //error not in range
                    
                    printf("Error: %3d: Array index not in range\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    return;
                }
                
                if(dimensions == 2){
                    if(index2 <= typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes[index1 - start] && index2 >= 0){
                        singleTerm->node.nonLeafNode.typeExpression.type = Integer;
                    }
                    else{
                        //error out of bound
                        
                        printf("Error: %3d: Array index out of bounds\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }
                }
                else if(dimensions == 3){
                    if(index2 <= typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[index1-start+1].size && index2 >= 0){
                        //do nothing
                    }
                    else{
                        //error out of bounds
                        
                        printf("Error: %3d: Array index out of bounds\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        return;
                    }

                    if(index3 >= 0 && index3 <= typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[index1-start+1].sizes[index2]){
                        singleTerm->node.nonLeafNode.typeExpression.type = Integer;
                    }
                    else{
                        //error out of bound
                        
                        printf("Error: %3d: Array index out of bounds\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }
                }
            }
            
            break;
        case 37:
            singleTerm->node.nonLeafNode.typeExpression = getElementFromTypeExpressionTable(singleTerm->node.nonLeafNode.children->node.leafNode.lexeme, T)->typeExpression;
            break;
        case 38:
            singleTerm->node.nonLeafNode.typeExpression.type = Integer;
            break;
    }
    return;
}

bool checkInteger(char *name, TypeExpressionTable T){
    TypeExpression typeExpression;
    typeExpression = getElementFromTypeExpressionTable(name, T)->typeExpression;

    if(typeExpression.type == Integer){
        return true;
    }
    else 
        return false;
}

bool compareTypeExpression(TypeExpression t1, TypeExpression t2) {
    if(t1.type == Error || t2.type == Error)
        return false;
    if(t1.type == None || t2.type == None)
        return false;
    if(t1.type == t2.type) {
        if(t1.type == Integer || t1.type == Real || t1.type == Boolean) {
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
