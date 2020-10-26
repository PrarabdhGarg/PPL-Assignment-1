#include "typeExpressionTable.h"

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

void printTypeExpressionTable(TypeExpressionTableElement * T) {

}
void printTypeExpressionTableElement(TypeExpressionTableElement element){

    //make border

    //string for array type 
    char buffer[250];

    printf("| %25s | %25s | %25s | %250s |\n", element.variableName, returnArrayType(element.arrayType, buffer), returnTypeOfRange(element.typeOfRange,buffer),returnTypeExpression(element, buffer));

}

char * returnArrayType(int i, char * s){

    switch (i)
    {
    case 0:
        strcpy(s,"primitive_data_type");
        return s;
        break;
    
    case 1:
        strcpy(s,"rectangular_array");
        return s;
        break;
    
    case 2:
        strcpy(s,"jagged_array");
        return s;
        break;
    
    default:
        break;
    }
}

char * returnTypeOfRange(int i, char * s){
    
    switch (i)
    {
    case 0:
        strcpy(s,"static");
        return s;
        break;
    
    case 1:
        strcpy(s,"dynamic");
        return s;
        break;
    
    case 2:
        strcpy(s,"not_applicable");
        return s;
        break;
    
    default:
        break;
    }
}

char * returnPrimitiveDataType(int i, char * s){
    switch (i)
    {
    case 0:
        strcpy(s,"<type = integer>");
        return s;
        break;
    
    case 1:
        strcpy(s,"<type = real>");
        return s;
        break;
    
    case 2:
        strcpy(s,"<type = Boolean>");
        return s;
        break;
    
    default:
        break;
    }
}


char * returnBasicElementDataType(int i, char * s){
    switch (i)
    {
        case 0:
        strcpy(s,"integer");
        return s;
        break;
 
        case 1:
        strcpy(s,"real");
        return s;
        break;
    
        case 2:
        strcpy(s,"Boolean");
        return s;
        break;
    
        default:
        break;
    }

}


char * returnTypeExpression(TypeExpressionTableElement element, char * s){

    if(element.typeExpression.type<3){
        //for primitive data type
        return returnPrimitiveDataType(element.typeExpression.type, s);
    }
    else{
        // for array
        char tempBuffer[100];

        strcpy(s,"<type = ");

        //Type of array
        if (element.typeExpression.type==3) //Rectangular array //ASSUMPTION:  Maybe more than 3 dimesions 
        {
            strcat(s,"rectangular_array, dimensions = ");
            //dimension
            itoa (element.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions,tempBuffer,10);
            strcat(s,tempBuffer); // dimensions = 2..

            //To print R1= [2,3] .. 
            for(int i=0; i<element.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions;i++){
                strcat(s, ", range_R");
                itoa(i+1,tempBuffer,10); // range_R1
                strcat(s,tempBuffer);
                strcat(s," = (");
                itoa(element.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start,tempBuffer,10);
                strcat(s,tempBuffer);
                strcat(s,",");
                itoa(element.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end,tempBuffer,10);
                strcat(s,tempBuffer);
                strcat(s,")");
            }

            //for basic elemenet type
            strcat(s, ", basicElementType = ");
            //strcpy(tempBuffer,returnBasicElementDataType(element.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType, tempBuffer));
            strcat(s,returnBasicElementDataType(element.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType, tempBuffer));
            strcat(s," >");
            return s;

        }
        else if(element.typeExpression.type==4) // Jagged array //ASSUMPTION: Not more than 3 dimensions
        {
            strcat(s,"jagged_array, dimensions = ");
            itoa (element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions,tempBuffer,10);
            strcat(s,tempBuffer);

            // if dimensions is 2 or 1 
            if(element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions==1 || element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions==2){
                for(int i=0; i<element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions;i++){
                    strcat(s, ", range_R");
                    itoa(i+1,tempBuffer,10); // range_R1
                    strcat(s,tempBuffer);
                    strcat(s," = (");

                    for(int j=0; j<element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size; j++){

                        itoa(element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].sizes[j],tempBuffer,10);
                        strcat(s,tempBuffer);
                        strcat(s,",");
                         /* itoa(element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].end,tempBuffer,10);
                         strcat(s,tempBuffer); */
                    }

                    strcat(s,")");
                }
            }
            
            // jagged array dimension = 3 
            else if(element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions==1 || element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions==3){
                
                // for range R1
                strcat(s, ", range_R1");
                //itoa(i+1,tempBuffer,10); // range_R1
                //strcat(s,tempBuffer);
                strcat(s," = (");
                itoa(element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0],tempBuffer,10);
                strcat(s,tempBuffer);
                strcat(s,",");
                itoa(element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1],tempBuffer,10);
                strcat(s,tempBuffer);
                strcat(s,")");        

                //For Range R2
                strcat(s, ", range_R2");
                strcat(s," = ( ");
                int sizeofRangesArray =element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1]-
                                        element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] +2;
                
                for(int j=1; j<sizeofRangesArray; j++){

                    itoa(element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[j].size,tempBuffer,10);
                    strcat(s,tempBuffer);
                    strcat(s,"[ ");
                   
                    for(int k=0; k<element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[j].size; k++){

                        itoa(element.typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[j].sizes[k],tempBuffer,10);
                        strcat(s,tempBuffer);

                        strcat(s,",");
                    }
                    
                    strcat(s," ], ");
                    /* TODO: Watch out for extra commas at the end */
                }

               // s[strlen[s]-2]='\0';
                strcat(s,")");
            }
            
            //for basic element type
            strcat(s, ", basicElementType = ");
            //strcpy(tempBuffer,returnBasicElementDataType(element.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType, tempBuffer));
            strcat(s,returnBasicElementDataType(element.typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType, tempBuffer));
            strcat(s," >");
            return s;
        }

    }

}
