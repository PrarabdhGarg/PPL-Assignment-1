#include "tokenStream.h"

void main() {
    GrammarCell *grammar = (GrammarCell *) malloc(INITSIZE * sizeof(GrammarCell));
    TokenStream tokenStream;
    int length = readGrammar("grammar.txt", grammar);
    printf("Grammar Read Sucessfully");
    tokeniseSourcecode("test.txt", tokenStream);
    printf("Token Stream Created Sucessfully");
}