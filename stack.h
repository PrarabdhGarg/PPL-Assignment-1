/*  ID:	2018A7PS0198P 		Name:	Prarabdh Nilesh Garg
    ID:	2018A7PS0223P 		Name:	Nalin Deepak
    ID:	2018A7PS0252P 		Name:	Akshat Gupta
    ID:	2018A7PS0257P 		Name:	Prakhar Mishra*/

#include "parseTree.h"

typedef ParseTreeNode StackElement;

typedef struct stackNode {
    StackElement element;
    struct stackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int size;
} Stack;

Stack *newStack();
void push(Stack *, StackElement);
StackElement top(Stack *);
void pop(Stack *);
int size(Stack *);
void printStack(Stack *);