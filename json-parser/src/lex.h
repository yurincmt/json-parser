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
    NUMBER,          // [0-9]+
    DELIMITER,       // [({])],:
} LexTokenType;


/**
 * Operações com o 'currentToken'
 */
char* strTokenType(Token* tk);
char* Token_token(Token* tk);
int Token_type(Token* tk);
int Token_equalc(Token* tk, const char chr);     // verifica se o token atual é iqual o caractere do parâmetro

void tokenPrint(Token* token);
void lexError(const char* dbmsg);
Token* tokenAlloc(char* tmptoken, int tokenlen, int tokenType);
Token* get_delimiter(FILE* fptr);
Token* get_string(FILE* fptr);
Token* get_literalName(FILE* fptr);
Token* get_number(FILE* fptr);
int firstTokenChar(char c);
Token* next_token(FILE* fptr);
int lex(char const * filename);

#endif