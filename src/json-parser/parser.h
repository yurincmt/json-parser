#include "lex.c"

int json(void);
int value(void);
int values(void);
int object(void);
int array(void);
int members(void);
int key(void);
int string(void);
int literal(void);
int number(void);
int currentTokenIs(char const * token);
int validateCurrentToken(char* tokenMustBe);
void parser(FILE* fptr);