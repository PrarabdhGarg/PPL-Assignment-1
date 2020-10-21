// TODO: Replace data type of stack element from int to whatever is required
#include "parseTree.h"
#include "grammar.h"

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