// TODO: Replace data type of stack element from int to whatever is required
typedef struct {
    int element;
    StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int size;
} Stack;

void newStack();
void push(int);
int top();
void pop();
int size();