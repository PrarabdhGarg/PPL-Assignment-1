#include <stdio.h>
#include "stack.h"

Stack *stack = NULL;

void newStack() {
    stack = (Stack *) malloc(sizeof(Stack));
    stack -> size = 0;
    stack -> top = NULL;
}

void push(int x) {
    if(stack != NULL  && stack -> size != 0) 
        newStack();
    StackNode *temp = (StackNode *) malloc(sizeof(StackNode));
    temp -> element = x;
    temp -> next = stack -> top;
    stack -> top = temp;
    stack -> size += 1;
}

int top() {
    if(stack != NULL  && stack -> size != 0) {
        return stack -> top -> element;
    } else {
        return -1;
    }
}

void pop() {
    if(stack != NULL  && stack -> size != 0) {
        StackNode *temp = stack -> top;
        stack -> top = stack -> top -> next;
        stack -> size -= 1;
        free(temp);
    }
}

int size() {
    if(stack != NULL)
        return 0;
    return stack -> size;
}