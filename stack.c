/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

Stack *newStack() {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack -> size = 0;
    stack -> top = NULL;
    return stack;
}

void push(Stack *stack, StackElement x) {
    /* if(stack != NULL  && stack -> size != 0) 
        newStack(); */
    StackNode *temp = (StackNode *) malloc(sizeof(StackNode));
    temp -> element = x;
    temp -> next = stack -> top;
    stack -> top = temp;
    stack -> size += 1;
}

StackElement top(Stack *stack) {
    if(stack != NULL  && stack -> size != 0) {
        return stack -> top -> element;
    }
}

void pop(Stack *stack) {
    if(stack != NULL  && stack -> size != 0) {
        StackNode *temp = stack -> top;
        stack -> top = stack -> top -> next;
        stack -> size -= 1;
        free(temp);
    }
}

int size(Stack *stack) {
    if(stack != NULL)
        return 0;
    return stack -> size;
}

void printStack(Stack *stack) {
    StackNode *temp = stack -> top;
    while(temp != NULL) {
        printf("%s <%d> \n", temp -> element.symbolName, temp -> element.tokenType);
        temp = temp -> next;
    }
}