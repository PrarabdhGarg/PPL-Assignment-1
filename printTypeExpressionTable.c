/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include "printTypeExpressionTable.h"


void printTypeExpressionTable(HashTableElement * array) {
    //array is array of hash table elements with size HASH_Table_sze which has
    // hash table elments where each hash table elements contans a linked list of 
    //typeExpressionTable elements
    //printf("called printTypeExpressionTable\n");

    

    writeToFile=1;

   // use appropriate location if you are using MacOS or Linux
   fileptr = fopen("typeExpressionTableOutput.txt","w");

   if(fileptr == NULL)
   {
      printf("Error! in writing to typeExpressionTableOutput.txt");   
      writeToFile=0;           
   }

   

   
   
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("| Line | %-20s | %-20s | %-20s | %-80s |\n","Variable Name","Data Type", "Type of Range", "Type Expression");
    printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    //write to file
    if(writeToFile){
        fprintf(fileptr,"+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
        fprintf(fileptr,"| Line | %-20s | %-20s | %-20s | %-150s |\n","Variable Name","Data Type", "Type of Range", "Type Expression");
        fprintf(fileptr,"+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    }

    HashTableElement * temp;
    for(int i =0;i<HASH_TABLE_SIZE; i++){

        if(array[i].element!=NULL){
            temp=array+i;

            do
            {
                printTypeExpressionTableElement(temp->element);
                temp=temp->next;
            } while (temp!=NULL);
            
        }
        else{
            //printf(" null h hash table element array index no i=%d \n\n",i);
        }
    }

    
    if(writeToFile){
        fclose(fileptr);
        writeToFile=0;
    }

}
void printTypeExpressionTableElement(TypeExpressionTableElement * element){
    //printf("called printTypeExpressionTableElement\n");
    //make border

    //string for array type 
    char buffer1[250];
    char buffer2[250];
    char buffer3[250];
    char buffer4[250];

    if(element->typeExpression.type==None){
        printf("| %-4d | %-20s | %-20s | %-20s | %-80s |\n",element->lineNumber, element->variableName, "None","not_applicable", "not_applicable");
        printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

        //write to file
        if(writeToFile){
            fprintf(fileptr,"| %-4d | %-20s | %-20s | %-20s | %-150s |\n", element->lineNumber, element->variableName, "None","not_applicable", "not_applicable");
            fprintf(fileptr,"+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
 
        }
    }
    else if(element->typeExpression.type==Error){
        printf("| %-4d | %-20s | %-20s | %-20s | %-80s |\n", element->lineNumber, element->variableName, "Error","not_applicable", "not_applicable");
        printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

        //writeToFile
        if (writeToFile)
        {
            fprintf(fileptr,"| %-4d | %-20s | %-20s | %-20s | %-150s |\n", element->lineNumber,element->variableName, "Error","not_applicable", "not_applicable");
            fprintf(fileptr,"+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

        }
        
    }

    
    else {
        
        //printf("| %20s | %20s | %20s | %150s |\n\n", element->variableName, returnArrayType(element->arrayType, buffer), returnTypeOfRange(element->typeOfRange,buffer ),returnTypeExpression(element, buffer));
        returnArrayType(element->arrayType, buffer1);
        returnTypeOfRange(element->typeOfRange,buffer2 );
        returnTypeExpression(element, buffer3);
        printf("| %-4d | %-20s | %-20s | %-20s | %-80s |\n", element->lineNumber,element->variableName,buffer1,buffer2,buffer3);
        printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

        //write to File
        if (writeToFile)
        {
            fprintf(fileptr,"| %-4d | %-20s | %-20s | %-20s | %-150s |\n",element->lineNumber, element->variableName,buffer1,buffer2,buffer3);
            fprintf(fileptr,"+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

        }
        
    }

}


char * returnArrayType(int i , char * s ){

   // printf(" //1 ");
    switch (i)
    {
    case 0:
        strcpy(s,"primitive_data_type");
        break;
    
    case 1:
        strcpy(s,"rectangular_array");
        break;
    
    case 2:
        strcpy(s,"jagged_array");
        
        break;
    
    default:
        strcpy(s,"Error Occured: returnArrayType()");
        break;

    return s;
    }
}

char * returnTypeOfRange(int i , char * s ){
   //printf(" //2 ");
    switch (i)
    {
    case 0:
        strcpy(s,"static");
        
        break;
    
    case 1:
        strcpy(s,"dynamic");
        
        break;
    
    case 2:
        strcpy(s,"not_applicable");
        
        break;
    
    default:
        strcpy(s,"Error Occured: returnTypeofRange()");
        break;


    }

     return s;
}

char * returnPrimitiveDataType(int i, char * s){
    //printf(" //3 ");
    switch (i)
    {
    case 0:
        strcpy(s,"<type = integer>");
        break;
    
    case 1:
        strcpy(s,"<type = real>");
        break;
    
    case 2:
        strcpy(s,"<type = Boolean>");
        break;
    
    default:
        strcpy(s,"Error Occured: returnPrimitiveDataType()");
        break;
    
    }

    return s;
}


char * returnBasicElementDataType(int i, char * s){
    //printf(" //4 ");
    switch (i)
    {
        case 0:
            strcpy(s,"integer");
            break;
 
        case 1:
            strcpy(s,"real");
            break;
    
        case 2:
            strcpy(s,"Boolean");
            break;
    
        default:
            strcpy(s,"Error Occured: returnBasicElementDataType()");
            break;
    }

    return s;

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