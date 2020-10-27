#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHARLINE 500
#define MAXCHARWORD 100
#define INITSIZE 50

typedef enum {
    Terminal,
    NonTerminal
} SymbolType;

typedef struct grammarNode {
    char symbolName[MAXCHARWORD];
    SymbolType symbolType;
    struct grammarNode *next; 
}GrammarNode; // Node of Linked List

typedef struct grammarCell {
    char nonTerminalName[MAXCHARWORD];
    GrammarNode *rule;
}GrammarCell; // an element of array grammar

typedef struct GrammarCell* Grammar; // array

int readGrammar( char* filename, GrammarCell* grammar);
void printGrammar(int lineCount, GrammarCell* grammar);
