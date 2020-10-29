/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include "typeExpressionTable.h"

bool printErrors = true;

char *getType(TypeExpression typeExpression) {
    char *type = (char *) malloc(sizeof(char) * 30);
    switch (typeExpression.type) {
    case Integer:
        type = "Integer";
        break;
    case Real:
        type = "Real";
        break;
    case Boolean:
        type = "Boolean";
        break;
    case RectangularArray:
        type = "Rectangular Array";
        break;
    case JaggedArray:
        type = "Jagged Array";
        break;
    default:
        type = "Error";
        break;
    }
}

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
            if(strcmp(next -> element -> variableName, temp -> element -> variableName) == 0) {
                free(temp);
                return ;
            }
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

void traverseParseTree(ParseTreeNode *root, TypeExpressionTable T, bool b){
    printErrors = b;
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
            
            if(printErrors) {
                printf("Error: %3d: Arrays can only have the base type of integers.\n", dataType->node.nonLeafNode.children[0].node.leafNode.lineNumber);
            }
            
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
                if(checkInteger(startIndex->node.nonLeafNode.children->node.leafNode.lexeme, T)){
                    //can't be checked at run time
                    
                    printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", startIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
                    //continue;
                }
                else {
                    printf("Error : %3d : %dth dimmension must be an integer\n", startIndex->node.nonLeafNode.children->node.leafNode.lineNumber, i+1);
                    //error not of integer type
                }
                dynamic = true;
            }
            else if(startIndex -> node.nonLeafNode.ruleNumber == 42){
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].start = atoi(startIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
            }

            if(endIndex -> node.nonLeafNode.ruleNumber == 41){
                rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = -1;
                if(checkInteger(endIndex->node.nonLeafNode.children->node.leafNode.lexeme, T)){
                    //can't be checked at run time
                    
                    printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", endIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
                    //continue;
                }
                else {
                    printf("Error : %3d : %dth dimmension must be an integer\n", endIndex->node.nonLeafNode.children->node.leafNode.lineNumber, i+1);
                    //error not of integer type
                }
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
                if(printErrors) {
                    printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", startIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
                }
            }
            else {
                //error not of integer type
                //TODO add error in non terminal
                if(printErrors) {
                    printf("Error: %3d: Array ranges must be of type Integer\n", startIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
                }
            }
            dynamic = true;
        }
        else if(startIndex -> node.nonLeafNode.ruleNumber == 42){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].start = atoi(startIndex -> node.nonLeafNode.children -> node.leafNode.lexeme);
        }

        if(endIndex -> node.nonLeafNode.ruleNumber == 41){
            rectangularDimension -> node.nonLeafNode.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[dimensions-1].end = -1;
            if(checkInteger(endIndex->node.nonLeafNode.children->node.leafNode.lexeme, T)){
                if(printErrors) {
                    printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", endIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
                }
            }
            else {
                //error not of integer type
                //TODO add error in non terminal
                if(printErrors) {
                    printf("Error: %3d: Array ranges must be of type Integer\n", endIndex->node.nonLeafNode.children->node.leafNode.lineNumber);
                }
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

    else if(dataType->node.nonLeafNode.ruleNumber == 9){//JaggedArrayType
        ParseTreeNode *emptyDimensions, startIndex, endIndex, *rowDefJaggedArray, *valuesList, *numbersList;
        int start, end, dimensions, size, index, size1;
        bool error = false;
        emptyDimensions = dataType -> node.nonLeafNode.children + 7;
        startIndex = dataType -> node.nonLeafNode.children[3];
        endIndex = dataType -> node.nonLeafNode.children[5];
        start = atoi(startIndex.node.leafNode.lexeme);
        end = atoi(endIndex.node.leafNode.lexeme);
        emptyDimensions -> node.nonLeafNode.typeExpression.type = None;
        rowDefJaggedArray = dataType->node.nonLeafNode.children + 10;

        if(start > end) {
            //error
            if(printErrors) {
                printf("Error: %3d: Start index of range should be lesser than the end index.\n", dataType->node.nonLeafNode.children[0].node.leafNode.lineNumber);
            }
            // I think we don't need to consider this case
        }
        else if(emptyDimensions -> node.nonLeafNode.ruleNumber == 16){
            dimensions = 3;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions = dimensions;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges = malloc((end - start + 2) * sizeof(JaggedRange));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].size = 2;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes = malloc(2 * sizeof(int));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] = start;
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] = end;
 
            for(int i = 1; i < end - start + 2; i++){
                index = atoi(rowDefJaggedArray->node.nonLeafNode.children[2].node.leafNode.lexeme);
                if(index != i + start - 1) {
                    if(printErrors) {
                        printf("Error: %3d: Invalid index of row.\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                    //error
                    error = true;
                    break;
                }
                size = atoi(rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lexeme);
                if(size <= 0){
                    //error
                    if(printErrors) {
                        printf("Error: %3d: Size of row should be a positive integer.\n", rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lineNumber);
                    }
                    error = true;
                    break;
                }
                dataType->node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size = size;
                dataType->node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].sizes = malloc(size * sizeof(int));
                
                valuesList = rowDefJaggedArray->node.nonLeafNode.children + 10;

                for(int j = 0; j < size; j++){
                    size1 = 0;
                    if(j != size-1 && valuesList->node.nonLeafNode.ruleNumber == 21){
                            //error
                            if(printErrors) {
                                printf("Error: %3d: Number of rows are less than exprected.\n", rowDefJaggedArray->node.nonLeafNode.children->node.leafNode.lineNumber);
                            }
                            error = true;
                            break;
                    }
                    numbersList = valuesList->node.nonLeafNode.children;
                    while(numbersList->node.nonLeafNode.ruleNumber != 24){
                        size1++;
                        if(numbersList->node.nonLeafNode.ruleNumber == 22){
                            numbersList = numbersList->node.nonLeafNode.children+1;
                        }
                        else{
                            break;
                        }
                    }
                    if(size1 == 0){
                        //error
                        if(printErrors) {
                            printf("Error: %3d: Dimensions of a row cannot be zero in length.\n", rowDefJaggedArray->node.nonLeafNode.children->node.leafNode.lineNumber);
                        }
                        error = true;
                        break;
                    }
                    else{
                        dataType->node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].sizes[j] = size1;
                    }
                    if(valuesList->node.nonLeafNode.ruleNumber == 20)
                        valuesList = valuesList->node.nonLeafNode.children + 1;
                }

                if(i != end - start + 1 && rowDefJaggedArray->node.nonLeafNode.ruleNumber != 18){
                    //error less declarations
                    error = true;
                    if(printErrors) {
                        printf("Error: %3d: The number of indices provided are lesser than expected\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                    break;
                }
                if(i == end - start + 1 && rowDefJaggedArray->node.nonLeafNode.ruleNumber != 19){
                    //error more declarations
                    error = true;
                    if(printErrors) {
                         printf("Error: %3d: The number of indices provided are greater than expected\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
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
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes = malloc((end - start + 1) * sizeof(int));
            dataType -> node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].size = end - start + 1;

            for(int i = 0; i < end - start + 1; i++){
                index = atoi(rowDefJaggedArray->node.nonLeafNode.children[2].node.leafNode.lexeme);
                if(index != i + start){
                    //error
                    error = true;
                    if(printErrors) {
                        printf("Error: %3d: The row you are trying to define is not mentioned in the range.\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                }
                size = atoi(rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lexeme);               
                dataType->node.nonLeafNode.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes[i] = size;
                
                if(size <= 0){
                    //error
                    if(printErrors) {
                        printf("Error: %3d: Size of row should be a positive integer.\n", rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lineNumber);
                    }
                    error = true;
                }
                else{
                    valuesList = rowDefJaggedArray->node.nonLeafNode.children + 10;
                    for(int j = 0; j < size; j++){
                        if(j != size-1 && valuesList->node.nonLeafNode.ruleNumber == 21){
                            //error
                            if(printErrors) {
                                printf("Error: %3d: Number of entries less than expected.\n", rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lineNumber);
                            }
                            error = true;
                            break;
                        }
                        numbersList = valuesList->node.nonLeafNode.children;
                        if(j != size -1 && numbersList->node.nonLeafNode.ruleNumber == 23){
                            //error
                            if(printErrors) {
                                printf("Error: %3d: Number of entries less than expected.\n", rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lineNumber);
                            }
                            error = true;
                            break;
                        }
                        if(j == size - 1 && numbersList->node.nonLeafNode.ruleNumber != 23) {
                            // error
                            if(printErrors) {
                                printf("Error: %3d: Number of entries greater than expected.\n", rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lineNumber);
                            }
                            error = true;
                            break;
                        }
                        else if(numbersList->node.nonLeafNode.ruleNumber == 24){
                            //error
                            if(printErrors) {
                                printf("Error: %3d: Number of entries less than expected.\n", rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lineNumber);
                            }
                            error = true;
                            break;
                        }
                        else if(numbersList->node.nonLeafNode.ruleNumber == 22){
                            numbersList = numbersList->node.nonLeafNode.children + 1;
                            if(numbersList->node.nonLeafNode.ruleNumber != 24){
                                //error
                                if(printErrors) {
                                    printf("Error: %3d: Number of entries more than expected.\n", rowDefJaggedArray->node.nonLeafNode.children[6].node.leafNode.lineNumber);
                                }
                                error = true;
                                break;
                            }
                        }
                        if(valuesList->node.nonLeafNode.ruleNumber == 20)
                            valuesList = valuesList->node.nonLeafNode.children + 1;
                    }
                }

                if(i != end - start && rowDefJaggedArray->node.nonLeafNode.ruleNumber != 18){
                    //error less declarations
                    error = true;
                    if(printErrors) {
                        printf("Error: %3d: The number of indices provided are lesser than expected\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                    break;
                }
                if(i == end - start && rowDefJaggedArray->node.nonLeafNode.ruleNumber != 19){
                    //error more declarations
                    error = true;
                    if(printErrors) {
                        printf("Error: %3d: The number of indices provided are greater than expected\n", rowDefJaggedArray->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                }
                if(i != end - start)
                    rowDefJaggedArray = rowDefJaggedArray->node.nonLeafNode.children + 12;
            }
        }
        if(error){
            dataType->node.nonLeafNode.typeExpression.type = Error;
        }
        else{
            dataType->node.nonLeafNode.typeExpression.type = JaggedArray;
        }
        declaration->node.nonLeafNode.typeExpression = dataType->node.nonLeafNode.typeExpression;
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
    element->lineNumber = terminal->node.leafNode.lineNumber;
    
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
        case Error:
            element->typeExpression.type = Error;
            element->typeOfRange = NotApplicable;
            break;
        default:
            if(printErrors) {
                printf("Error\n");
            }
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
        if(printErrors) {
            char *type1 = getType(singleTerm->node.nonLeafNode.typeExpression);
            char *type2 = getType(expression->node.nonLeafNode.typeExpression);
            printf("Error: %3d: Value of type %s cannot be assigned to %s.\n", assignment->node.nonLeafNode.children[1].node.leafNode.lineNumber, type2, type1);
        }
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
                    if(printErrors) {
                        printf("Error: %3d: Arithmatic operations cannot operate on Boolean operands\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                    //error
                } 
            }
            else{
                expression->node.nonLeafNode.typeExpression.type = Error;
                
                // TODO: Modify print statement to include type of left and right if possible
                if(printErrors) {
                    char *type1 = getType(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression);
                    char *type2 = getType(expression->node.nonLeafNode.children[2].node.nonLeafNode.typeExpression);
                    if(strcmp(expression->node.nonLeafNode.children[1].node.nonLeafNode.children->symbolName, "+") == 0)
                        printf("Error: %3d: Value of type %s cannot be added with value of type %s.\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber, type1, type2);
                    else
                        printf("Error: %3d: Value of type %s cannot be subtracted from value of type %s.\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber, type2, type1);
                }
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
                        
                        if(printErrors) {
                            printf("Error: %3d: Arithmatic operations cannot operate on Boolean operands\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
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
                        
                        if(printErrors) {
                            printf("Error: %3d: Division must have both operands of type Integer or Real\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
                    }
                }
            }
            else{
                expression->node.nonLeafNode.typeExpression.type = Error;
                
                // TODO: Modify print statement to include type of left and right if possible
                if(printErrors) {
                    char *type1 = getType(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression);
                    char *type2 = getType(expression->node.nonLeafNode.children[2].node.nonLeafNode.typeExpression);
                    if(strcmp(expression->node.nonLeafNode.children[1].node.nonLeafNode.children->symbolName, "*") == 0)
                        printf("Error: %3d: Value of type %s cannot be multiplied with value of type %s.\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber, type1, type2);
                    else
                        printf("Error: %3d: Value of type %s cannot be divided from value of type %s.\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber, type2, type1);
                }
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
                    
                    if(printErrors) {
                        printf("Error: %3d: Boolean operations needs boolean operands\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                }
            }
            else{
                expression->node.nonLeafNode.typeExpression.type = Error;
                
                // TODO: Modify print statement to include type of left and right if possible
                if(printErrors) {
                    char *type1 = getType(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression);
                    char *type2 = getType(expression->node.nonLeafNode.children[2].node.nonLeafNode.typeExpression);
                    printf("Error: %3d: Logical Or Operation cannot be applied between values of type %s and %s.\n", expression->node.nonLeafNode.children[1].node.leafNode.lineNumber, type1, type2);
                }
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
                    
                    if(printErrors) {
                        printf("Error: %3d: Boolean operations needs boolean operands\n", expression->node.nonLeafNode.children[1].node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                }
            }
            else{
                expression->node.nonLeafNode.typeExpression.type = Error;
                
                // TODO: Modify print statement to include type of left and right if possible
                if(printErrors) {
                    char *type1 = getType(expression->node.nonLeafNode.children[0].node.nonLeafNode.typeExpression);
                    char *type2 = getType(expression->node.nonLeafNode.children[2].node.nonLeafNode.typeExpression);
                    printf("Error: %3d: Logical And Operation cannot be applied between values of type %s and %s.\n", expression->node.nonLeafNode.children[1].node.leafNode.lineNumber, type1, type2);
                }
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
                    if(printErrors) {
                        printf("Error: %3d: Primitive data types cannot be indexed\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                }
                else{
                    if(printErrors) {
                        printf("Error: %3d: Error in variable declaration\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                }
                singleTerm->node.nonLeafNode.typeExpression.type = Error;
                return;
            }
            else if(typeExpression.type == RectangularArray){
                indexList = singleTerm->node.nonLeafNode.children + 2;
                for(int i = 0; i < typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions; i++){
                    if(i != typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions -1 && indexList->node.nonLeafNode.ruleNumber == 40){
                        //error less index
                        
                        if(printErrors) {
                            printf("Error: %3d: The number of indices provided are lesser than expected. Array slices are not supported\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
                        indexList->node.nonLeafNode.typeExpression.type == Error;
                        indexList->node.nonLeafNode.children->node.nonLeafNode.typeExpression.type = Error;
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }
                    if(i == typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions -1 && indexList->node.nonLeafNode.ruleNumber != 40){
                        //error more index
                        if(printErrors) {
                            printf("Error: %3d: The number of indices provided are greater than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
                        indexList->node.nonLeafNode.typeExpression.type == Error;
                        indexList->node.nonLeafNode.children->node.nonLeafNode.typeExpression.type = Error;
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }

                    indexNode = indexList->node.nonLeafNode.children;
                    
                    if(indexNode->node.nonLeafNode.ruleNumber == 41){
                        if(checkInteger(indexNode->node.nonLeafNode.children->node.leafNode.lexeme, T)){
                            //can't be checked at run time
                            
                            if(printErrors) {
                                printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", indexNode->node.nonLeafNode.children->node.leafNode.lineNumber);
                            }
                            //continue;
                        }
                        else {
                            if(printErrors) {
                                printf("Error : %3d : %dth dimmension must be an integer\n", indexNode->node.nonLeafNode.children->node.leafNode.lineNumber, i+1);
                            }
                            //error not of integer type
                        }
                    }
                    else if(indexNode->node.nonLeafNode.ruleNumber == 42){
                        index = atoi(indexNode->node.nonLeafNode.children->node.leafNode.lexeme);

                        if(index <= typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end 
                            && index >= typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start)
                        {
                            
                        }
                        else{
                            //error;
                            if(printErrors) {
                                 printf("Error: %3d : Array index out of bounds for %dth dimmension.\n", indexList->node.nonLeafNode.children -> node.nonLeafNode.children -> node.leafNode.lineNumber, i+1);
                            }
                            indexList->node.nonLeafNode.typeExpression.type == Error;
                            indexNode->node.nonLeafNode.typeExpression.type == Error;
                            singleTerm->node.nonLeafNode.typeExpression.type = Error;
                            return;
                        }
                    }
                    indexNode->node.nonLeafNode.typeExpression.type = None;
                    indexList->node.nonLeafNode.typeExpression.type = None;

                    if(indexList->node.nonLeafNode.ruleNumber == 39) {
                        indexList = indexList->node.nonLeafNode.children+1;
                    }
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
                    if(printErrors) {
                        printf("Error: %3d: The number of indices provided are lesser than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                    indexList->node.nonLeafNode.typeExpression.type == Error;
                    indexNode->node.nonLeafNode.typeExpression.type == Error;
                    singleTerm->node.nonLeafNode.typeExpression.type = Error;
                    return;
                }
                else if(indexList->node.nonLeafNode.ruleNumber == 39){
                    if(indexNode->node.nonLeafNode.ruleNumber == 41){
                        //dynamic warning
                        if(printErrors) {
                            printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
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
                        if(printErrors) {
                            printf("Error: %3d: The number of indices provided are lesser than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
                        indexList->node.nonLeafNode.typeExpression.type == Error;
                        indexNode->node.nonLeafNode.typeExpression.type == Error;
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }
                    else{
                        if(indexNode->node.nonLeafNode.ruleNumber == 41){
                            //dynamic warning
                             
                            if(printErrors) {
                                printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                            }
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
                        if(printErrors) {
                            printf("Error: %3d: The number of indices provided are greater than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
                        indexList->node.nonLeafNode.typeExpression.type == Error;
                        indexNode->node.nonLeafNode.typeExpression.type == Error;
                        singleTerm->node.nonLeafNode.typeExpression.type = Error;
                        return;
                    }
                    else {
                        if(indexNode->node.nonLeafNode.ruleNumber == 41){
                            //dynamic warning
                            
                            dynamic = true;
                            if(printErrors) {
                                printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                            }
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
                        
                        if(printErrors) {
                            printf("Warning: %3d: Dynamic range of array. Cannot be checked at compile time\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
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
                    
                    if(printErrors) {
                        printf("Error: %3d: The number of indices provided are greater than expected\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
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
                    if(printErrors) {
                        printf("Error: %3d: Array index not in range\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                    }
                   
                    return;
                }
                
                if(dimensions == 2){
                    if(index2 <= typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes[index1 - start] && index2 >= 0){
                        singleTerm->node.nonLeafNode.typeExpression.type = Integer;
                    }
                    else{
                        //error out of bound
                        
                        if(printErrors) {
                            printf("Error: %3d: Array index out of bounds\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
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
                        
                        if(printErrors) {
                            printf("Error: %3d: Array index out of bounds\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
                        return;
                    }

                    if(index3 >= 0 && index3 <= typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[index1-start+1].sizes[index2]){
                        singleTerm->node.nonLeafNode.typeExpression.type = Integer;
                    }
                    else{
                        //error out of bound
                        
                        if(printErrors) {
                            printf("Error: %3d: Array index out of bounds\n", singleTerm->node.nonLeafNode.children[0].node.leafNode.lineNumber);
                        }
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
    if (t1.type == Error || t2.type == Error)
        return false;
    if (t1.type == None || t2.type == None)
        return false;
    if (t1.type == t2.type)
    {
        if (t1.type == Integer || t1.type == Real || t1.type == Boolean)
        {
            return true;
        }
        else if (t1.type == RectangularArray)
        {
            if (t1.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType == t2.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType)
            {
                if (t1.arrayTypeExpression.rectangularArrayTypeExpression.dimensions == t2.arrayTypeExpression.rectangularArrayTypeExpression.dimensions)
                {
                    for (int i = 0; i < t1.arrayTypeExpression.rectangularArrayTypeExpression.dimensions; i++)
                    {
                        if (t1.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start == t2.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start && t1.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end == t2.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end)
                            continue;
                        return false;
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (t1.arrayTypeExpression.jaggedArrayTypeExpression.basicDataType == t2.arrayTypeExpression.jaggedArrayTypeExpression.basicDataType)
            {
                if (t1.arrayTypeExpression.jaggedArrayTypeExpression.dimensions == t2.arrayTypeExpression.jaggedArrayTypeExpression.dimensions)
                {
                    if(t1.arrayTypeExpression.jaggedArrayTypeExpression.dimensions == 2){
                        if(t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] && t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1]) {
                            for(int i = 0; i < t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] - t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] + 1; i++) {
                                if(t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes[i] == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[1].sizes[i]) {
                                    continue;
                                } else {
                                    return false;
                                }
                            }
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                    else{
                        if(t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] && t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1]) {
                            for(int i = 1; i < t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1] - t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] + 2; i++) {
                                if(t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size == t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size) {
                                    for(int j = 0; j < t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size; j++) {
                                        if(t1.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].sizes[j] != t2.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].sizes[j]) {
                                            return false;
                                        }
                                    }
                                    return true;
                                } else {
                                    return false;
                                }
                            }
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
            return true;
        }
    }
    else
    {
        return false;
    }
}