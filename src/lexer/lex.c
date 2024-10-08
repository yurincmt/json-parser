#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pcre.h>
#include "lex.h"

#define retok 1
#define reter 0


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
    "\"[^\"]*\"",
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
 * Recece um arquivo aberto e faz análise léxica de cada token do arquivo de acordo com a gramática JSON.
 *  - Recebe um arquivo aberto como parâmetro.
 * 
 * A função lex() faz análise de um token a cada vez que é chamada. Se o Token é válido, retorna o Token,
 * se não é válido, retorna NULL.
 * 
 * Se o token é inválido, ou seja, não é uma string, literal, delimitador, ou número, o Lexer para e retorna
 * um status de erro.
*/
Token* lex(FILE* f) {
    globalFptr = f;

    // Enquanto o próximo token não for NULL &&
    // o próximo token não for inválido
    while ((globalCurrentToken = next_token(globalFptr)) != NULL && globalCurrentToken->tokenType != INVALID) {
        return globalCurrentToken;
    }
    if (globalCurrentToken) lexError("Invalid Token");
    return NULL;
}


/**
 * Pega o próximo token existente no arquivo e o classifica de acordo com a gramática JSON.
 *  - Recebe um arquivo aberto como parâmetro.
 */
Token* next_token(FILE* fptr) {
    char matchtoken[128];
    int matchtokenlen;

    lstrip(globalLineBuffer);

    if (strlen(globalLineBuffer) == 0 || globalLineBuffer[0] == '\n') {
        getLineBuffer(fptr);
    }

    if (matchtokenlen = match(RULES[STRING], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, STRING);

    } else if (matchtokenlen = match(RULES[COLON], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, COLON);

    } else if (matchtokenlen = match(RULES[COMMA], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, COMMA);

    } else if (matchtokenlen = match(RULES[LEFTCURLYBRACKET], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, LEFTCURLYBRACKET);

    } else if (matchtokenlen = match(RULES[RIGHTCURLYBRACKET], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, RIGHTCURLYBRACKET);

    } else if (matchtokenlen = match(RULES[LEFTSQUAREBRACKET], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, LEFTSQUAREBRACKET);

    } else if (matchtokenlen = match(RULES[RIGHTSQUAREBRACKET], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, RIGHTSQUAREBRACKET);

    } else if (matchtokenlen = match(RULES[LITERAL], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, LITERAL);

    } else if (matchtokenlen = match(RULES[FLOAT], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, FLOAT);
    } else if (matchtokenlen = match(RULES[INTEGER], globalLineBuffer)) {
        strncpy(matchtoken, globalLineBuffer, matchtokenlen);
        matchtoken[matchtokenlen] = '\0';
        memmove(globalLineBuffer, &globalLineBuffer[matchtokenlen], strlen(globalLineBuffer));
        return tokenAlloc(matchtoken, matchtokenlen, INTEGER);
    } else if (!isspace(globalLineBuffer[0]) && !feof(globalFptr)){
        strcpy(matchtoken, globalLineBuffer);
        return tokenAlloc(matchtoken, strcspn(globalLineBuffer, "\n"), INVALID);
    }
    return NULL;
}


/**
 * Apaga os caracteres não printáveis à esquerda
 */
void lstrip(char* buffer) {
    int spaceCounter = 0;
    while (isblank(buffer[spaceCounter])) ++spaceCounter;

    memmove(buffer, &buffer[spaceCounter], strlen(buffer));
}


/**
 * Pega uma linha do arquivo que será analisada pelo lexer.
 * 
 * Em vez de carregar todo o arquivo em memória, o que exigiria mais memória em execution-time,
 * essa implementação lê uma linha por vez.
 * 
 * Ignora as linhas em branco, sejam aquelas que tem só uma quebra de linha, ou sejam aquelas que
 * possuem carateres em branco e uma quebra de linha.
 */
void getLineBuffer(FILE* fptr) {
    char *s = fgets(globalLineBuffer, 256, fptr);
    int len;

    if (!s) return;

    lstrip(globalLineBuffer);   // para linhas em branco com espaços seguida de um quebra de linha
    if (!strncmp(s, "\n", 1)) {
        getLineBuffer(fptr);
    }
}


int match(const char *pattern, char *subject) {
    int erroffset;
    int ovector[2]; // Vetor de offsets para correspondência
    const char *error;

    pcre *re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if (re == NULL) {
        fprintf(stderr, "Regex erro compilation: %s at %d\n", error, erroffset);
        return reter;
    }

    // Se não encontrou o padrão, retorna...
    if (pcre_exec(re, NULL, subject, (int)strlen(subject), 0, 0, ovector, 30) <= 0) {
        return reter;
    }

    pcre_free(re);
    return ovector[1] - ovector[0];
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