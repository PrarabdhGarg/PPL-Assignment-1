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