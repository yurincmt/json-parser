#ifndef LEX
#define LEX

typedef struct token Token;

typedef enum lextokentype {
    LEFTCURLYBRACKET = 0,
    RIGHTCURLYBRACKET,
    LEFTSQUAREBRACKET,
    RIGHTSQUAREBRACKET,
    COMMA,
    COLON,
    STRING,          // "..."
    LITERAL,         // true false null
    INTEGER,          // [0-9]+
    FLOAT,
    DELIMITER,       // [({])],:
    INVALID,
} LexTokenType;


/**
 * Operações com o 'currentToken'. Necessárias no Parser.
 */
char* Token_token(Token* tk);
int Token_type(Token* tk);

Token* lex(FILE* f);
Token* next_token(FILE* fptr);
int match(const char *pattern, char *subject);
void tokenPrint(Token* token);
void lexError(const char* dbmsg);
Token* tokenAlloc(char* tmptoken, int tokenlen, int tokenType);
Token* get_invalidToken(FILE* fptr);
void lstrip(char* buffer);
void getLineBuffer(FILE* fptr);

#endif