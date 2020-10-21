typedef enum {
    Keyword,
    Identifier,
    Operator,
    IntegerConstant
} TokenType;

typedef struct token {
    TokenType token;
    char *lexeme;
    struct token *next; 
    int lineNumber;      
} Token;

typedef Token *TokenStream;