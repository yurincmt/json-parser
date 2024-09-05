#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pcre.h>
#include "lex.h"

struct token {
    char* token;
    LexTokenType tokenType;
};

const char* STRTOKENTYPE[] = {
    "LEFTCURLYBRACKET",
    "RIGHTCURLYBRACKET",
    "LEFTSQUAREBRACKET",
    "RIGHTSQUAREBRACKET",
    "COMMA",
    "COLON",
    "STRING",
    "LITERAL",
    "INTEGER",
    "FLOAT"
};

const char* RULES[] = {
    "^\\{",
    "^\\}",
    "^\\[",
    "^\\]",
    "^\\,",
    "^\\:",
    "^\"[A-Za-zÀ-ü0-9\\,\\:\\_\\{\\}\\[\\]\\-.! ]*\"",
    "^true|false|null",
    "^[1-9]\\d*",
    "^\\d+\\.\\d+"
};

Token* globalCurrentToken = NULL;
FILE* globalFptr = NULL;
int globalFileCounter = 0;
static char globalLineBuffer[256];

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
        while (!feof(fptr) && (globalCurrentToken = next_token(fptr))) {
            tokenPrint(globalCurrentToken);
        }
    }
    return 0;
}

/**
 * Apaga os caracteres não printáveis à esquerda
 */
void lstrip(char* buffer) {
    int spaceCounter = 0;
    while (isspace(buffer[spaceCounter])) ++spaceCounter;

    memmove(buffer, &buffer[spaceCounter], strlen(buffer));
}

void getLineBuffer(FILE* fptr) {
    char *s = fgets(globalLineBuffer, 256, fptr);
    int len;

    if (!s) return;

    if (!strncmp(s, "\n", 1)) {
        getLineBuffer(fptr);
    } else {
        len = strlen(s) -1;
        if (globalLineBuffer[len] == '\n') globalLineBuffer[len] = '\0';
    }

}

int match(const char *pattern, char *subject) {
    char matchbuffer[32];
    int erroffset;
    int ovector[2]; // Vetor de offsets para correspondência
    const char *error;

    // puts(subject);

    pcre *re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if (re == NULL) {
        fprintf(stderr, "Regex erro compilation: %s at %d\n", error, erroffset);
        return 0;
    }

    // Se não encontrou o padrão, retorna...
    if (pcre_exec(re, NULL, subject, (int)strlen(subject), 0, 0, ovector, 30) <= 0) {
        return 0;
    }

    pcre_free(re);
    return ovector[1] - ovector[0];
}

/**
 * Pega o próximo token existente no arquivo
 */
Token* next_token(FILE* fptr) {
    char matchtoken[128];
    int matchtokenlen;

    if (strlen(globalLineBuffer) == 0) {
        getLineBuffer(fptr);
    }

    lstrip(globalLineBuffer);

    if (matchtokenlen = match(RULES[STRING], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, STRING);

    } else if (matchtokenlen = match(RULES[COLON], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, COLON);

    } else if (matchtokenlen = match(RULES[COMMA], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, COMMA);

    } else if (matchtokenlen = match(RULES[LEFTCURLYBRACKET], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, LEFTCURLYBRACKET);

    } else if (matchtokenlen = match(RULES[RIGHTCURLYBRACKET], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, RIGHTCURLYBRACKET);

    } else if (matchtokenlen = match(RULES[LEFTSQUAREBRACKET], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, LEFTSQUAREBRACKET);

    } else if (matchtokenlen = match(RULES[RIGHTSQUAREBRACKET], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, RIGHTSQUAREBRACKET);

    } else if (matchtokenlen = match(RULES[LITERAL], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, LITERAL);

    } else if (matchtokenlen = match(RULES[FLOAT], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, FLOAT);
    } else if (matchtokenlen = match(RULES[INTEGER], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        // strcpy(globalLineBuffer, &globalLineBuffer[matchtokenlen]);
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, INTEGER);
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