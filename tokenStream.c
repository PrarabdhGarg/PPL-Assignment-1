typedef enum {
    // TODO: Fill this list
    AssignmentOperator
} TokenType;

typedef struct {
    TokenType token;
    char *lexeme;
    Token *next;       
} Token;

typedef Token *TokenStream;