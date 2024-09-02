#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"

const char* STRTOKENTYPE[9] = {
    "LEFTCURLYBRACKET",
    "RIGHTCURLYBRACKET",
    "LEFTSQUAREBRACKET",
    "RIGHTSQUAREBRACKET",
    "COMMA",
    "COLON",
    "STRING",
    "LITERAL",
    "NUMBER",
};

Token* globalCurrentToken = NULL;
FILE* globalFptr = NULL;
int globalFileCounter = 0;

 struct token {
    char* token;
    LexTokenType tokenType;
};

char* Token_token(Token* tk) {
    return (!tk) ? NULL : tk->token;
}
int Token_type(Token* tk) {
    return (!tk) ? -1 : tk->tokenType;
}

/**
 * 
*/
int lex(char const * filename) {
    FILE* fptr = NULL;

    if ((fptr  = fopen(filename, "r")) == NULL) {
        lexError("lex() function");
    } else {
        // enquando houver token para ver visto, pegar o próximo token;

        while ((globalCurrentToken = next_token(fptr))) {
            tokenPrint(globalCurrentToken);
        }
    }
    return 0;
}

/**
 * Pega o próximo token existente no arquivo
 */
Token* next_token(FILE* fptr) {
    char c = fgetc(fptr);

    if (c == '\n') ++globalFileCounter;
 
    // ignora os caracteres não printáveis
    while (isspace(c))
        c = fgetc(fptr);
    
    ungetc(c, fptr);

    switch (firstTokenChar(c)) {
        case DELIMITER:
            return get_delimiter(fptr);
            break;
        case STRING:
            return get_string(fptr);
            break;
        case LITERAL:
            return get_literalName(fptr);
            break;
        case NUMBER:
            return get_number(fptr);
            break;
        default:
            return get_invalidToken(fptr);
    }
}

void tokenPrint(Token* token) {
    printf("Token: %s, Type: %s\n", token->token, STRTOKENTYPE[token->tokenType]);
}

void lexError(const char* dbmsg) {
    fprintf(stderr, "Lexer Error: %s\n", dbmsg);
    exit(EXIT_FAILURE);
}

Token* tokenAlloc(char* tmptoken, int tokenlen, int tokenType) {
    Token* newTokenPtr = malloc(sizeof(Token));
    newTokenPtr->token = malloc(tokenlen * sizeof(char));
    strncpy(newTokenPtr->token, tmptoken, tokenlen);
    newTokenPtr->tokenType = tokenType;
    return newTokenPtr;
}

Token* get_delimiter(FILE* fptr){
    int TYPE = 0;
    char c = fgetc(fptr);
    switch (c) {
    case '{':
        TYPE = LEFTCURLYBRACKET; break;
    case '}':
        TYPE = RIGHTCURLYBRACKET; break;
    case '[':
        TYPE = LEFTSQUAREBRACKET; break;
    case ']':
        TYPE = RIGHTSQUAREBRACKET; break;
    case ',':
        TYPE = COMMA; break;
    case ':':
        TYPE = COLON; break;
    default:
        break;
    }
    return tokenAlloc(&c, 1, TYPE);
}

Token* get_string(FILE* fptr){
    char tmptoken[128], c;
    int tokenlen = 0;

    tmptoken[tokenlen++] = fgetc(fptr);
    while ((c = fgetc(fptr)) != '"') {
        tmptoken[tokenlen++] = c;
    }
    tmptoken[tokenlen++] = c;
    tmptoken[tokenlen++] = '\0';

    return tokenAlloc(tmptoken, tokenlen, STRING);
}

Token* get_literalName(FILE* fptr){
    char tmptoken[32], c;
    int tokenlen = 0;

    while (islower(c = fgetc(fptr)) && isalpha(c)) {
        tmptoken[tokenlen++] = c;
    } ungetc(c, fptr);
    tmptoken[tokenlen++] = '\0';

    if (!strncmp(tmptoken, "true", tokenlen)  ||
        !strncmp(tmptoken, "false", tokenlen) ||
        !strncmp(tmptoken, "null", tokenlen)) {
        return tokenAlloc(tmptoken, tokenlen, LITERAL);
    } lexError("get_literalName() fucntion");
    return NULL;
}

Token* get_number(FILE* fptr){
    char tmptoken[64], c;
    int tokenlen = 0;

    while (isdigit(c = fgetc(fptr)) || strchr("+-.", c)) {
        tmptoken[tokenlen++] = c;
    } ungetc(c, fptr);
    tmptoken[tokenlen++] = '\0';

    if (tmptoken[0] == '0')
        return tokenAlloc(tmptoken, tokenlen, INVALID);


    return tokenAlloc(tmptoken, tokenlen, NUMBER);
}

Token* get_invalidToken(FILE* fptr) {
    char buffer[128], c;
    int tokenlen = 0;

    while (!isspace(c = fgetc(fptr)) && c != EOF) {
        buffer[tokenlen++] = c;
    }
    buffer[tokenlen++] = '\0';

    if (strlen(buffer) == 0) return NULL;

    return tokenAlloc(buffer, tokenlen, INVALID);
}

/**
 * Verifica o primeiro char do token e retorn o seu tipo;
 * 
 * Função chamada exclusivamente pela next_token()
 */
int firstTokenChar(char c) {
    switch (c) {
        case '{':
        case '}':
        case '[':
        case ']':
        case ':':
        case ',':
            return DELIMITER;
            break;

        case '"':
            return STRING;
            break;

        case 'f':
        case 'n':
        case 't':
            return LITERAL;
            break;

        case '.':
        case '-':
        case '+':
        case 'e':
        case 'E':
        case '0' ... '9' :
            return NUMBER;
            break;

        default:
            return -1;
    }
}