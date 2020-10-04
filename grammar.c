typedef enum {
    Terminal,
    NonTerminal
} SymbolType;

typedef struct {
    char *symbolName;
    SymbolType symbolType;
    GrammarNode *next; 
} GrammarNode;

typedef struct {
    char *nonTerminalName;
    GrammarNode *rule;
} GrammarCell;

typedef GrammarCell* Grammar;