#include "printTypeExpressionTable.h"


void printTypeExpressionTable(HashTableElement * array) {
    //array is array of hash table elements with size HASH_Table_sze which has
    // hash table elments where each hash table elements contans a linked list of 
    //typeExpressionTable elements
    printf("called printTypeExpressionTable\n");
    
    HashTableElement * temp;
    for(int i =0;i<HASH_TABLE_SIZE; i++){

        if(array[i].element!=NULL){
            temp=array;

            do
            {
                printTypeExpressionTableElement(temp->element);
                temp=temp->next;
            } while (temp!=NULL);
            
        }
        else{
            printf(" null h hash table element array index no i=%d \n",i);
        }
    }

}
void printTypeExpressionTableElement(TypeExpressionTableElement * element){
    printf("called printTypeExpressionTableElement\n");
    //make border

    //string for array type 
    char buffer[250];

    if(element->typeExpression.type==None){
        printf("| %25s | %25s | %25s | %250s |\n", element->variableName, "None","None", "None");
    }
    else if(element->typeExpression.type==Error){
        printf("| %25s | %25s | %25s | %250s |\n", element->variableName, "Error","Error", "Error");
    }

    else {
        printf("| %25s | %25s | %25s | %250s |\n", element->variableName, returnArrayType(element->arrayType, buffer), returnTypeOfRange(element->typeOfRange,buffer),returnTypeExpression(element, buffer));

    }

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


char * returnTypeExpression(TypeExpressionTableElement * element, char * s){

    if(element->typeExpression.type<3){
        //for primitive data type
        return returnPrimitiveDataType(element->typeExpression.type, s);
    }
    else{
        // for array
        char tempBuffer[100];

        strcpy(s,"<type = ");

        //Type of array
        if (element->typeExpression.type==3) //Rectangular array //ASSUMPTION:  Maybe more than 3 dimesions 
        {
            strcat(s,"rectangular_array, dimensions = ");
            //dimension
            sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions);
            strcat(s,tempBuffer); // dimensions = 2..

            //To print R1= [2,3] .. 
            for(int i=0; i<element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.dimensions;i++){
                strcat(s, ", range_R");
                sprintf(tempBuffer,"%d",i+1); // range_R1
                strcat(s,tempBuffer);
                strcat(s," = (");
                
                
                //to handle dynamic ranges
                if(element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start==-1)
                {
                    strcpy(tempBuffer,"dynamic");
                }
                else
                {
                    sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].start);

                }
                strcat(s,tempBuffer);
                strcat(s,",");

                if(element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end==-1)
                {
                    strcpy(tempBuffer,"dynamic");
                }
                else
                {
                    sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end);

                }
                //sprintf(element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.ranges[i].end,tempBuffer,10);
                strcat(s,tempBuffer);
                strcat(s,")");
            }

            //for basic elemenet type
            strcat(s, ", basicElementType = ");
            //strcpy(tempBuffer,returnBasicElementDataType(element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType, tempBuffer));
            strcat(s,returnBasicElementDataType(element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType, tempBuffer));
            strcat(s," >");
            return s;

        }
        else if(element->typeExpression.type==4) // Jagged array //ASSUMPTION: Not more than 3 dimensions
        {
            strcat(s,"jagged_array, dimensions = ");
            sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions);
            strcat(s,tempBuffer);

            // if dimensions is 2 or 1 
            if(element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions==1 || element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions==2){
                for(int i=0; i<element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions;i++){
                    strcat(s, ", range_R");
                    sprintf(tempBuffer,"%d",i+1); // range_R1
                    strcat(s,tempBuffer);
                    strcat(s," = (");

                    for(int j=0; j<element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].size; j++){

                        sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].sizes[j]);
                        strcat(s,tempBuffer);
                        strcat(s,",");
                         /* sprintf(element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[i].end,tempBuffer,10);
                         strcat(s,tempBuffer); */
                    }

                    strcat(s,")");
                }
            }
            
            // jagged array dimension = 3 
            else if(element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions==1 || element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.dimensions==3){
                
                // for range R1
                strcat(s, ", range_R1");
                //sprintf(i+1,tempBuffer,10); // range_R1
                //strcat(s,tempBuffer);
                strcat(s," = (");
                sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0]);
                strcat(s,tempBuffer);
                strcat(s,",");
                sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1]);
                strcat(s,tempBuffer);
                strcat(s,")");        

                //For Range R2
                strcat(s, ", range_R2");
                strcat(s," = ( ");
                int sizeofRangesArray =element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[1]-
                                        element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[0].sizes[0] +2;
                
                for(int j=1; j<sizeofRangesArray; j++){

                    sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[j].size);
                    strcat(s,tempBuffer);
                    strcat(s,"[ ");
                   
                    for(int k=0; k<element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[j].size; k++){

                        sprintf(tempBuffer,"%d",element->typeExpression.arrayTypeExpression.jaggedArrayTypeExpression.ranges[j].sizes[k]);
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
            //strcpy(tempBuffer,returnBasicElementDataType(element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType, tempBuffer));
            strcat(s,returnBasicElementDataType(element->typeExpression.arrayTypeExpression.rectangularArrayTypeExpression.basicElementType, tempBuffer));
            strcat(s," >");
            return s;
        }

    }

}