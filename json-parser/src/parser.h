#ifndef PARSER
#define PARSER

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
void parser(FILE* fptr);
void parserError(char* dbmsg);
int validateCurrentToken(char* tokenMustBe);
FILE* parserFile(void);

#endif