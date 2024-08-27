#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"

#define false 0
#define true 1
#define DEBUG 1

Token* currentToken = NULL;
int identCount = 0;
FILE* globalFptr = NULL;

FILE* parserFile(void) {
    return globalFptr; 
}

void parserError(char* dbmsg) {
    fprintf(stderr, "Semantic error: %s\n", dbmsg);
    exit(EXIT_FAILURE);
}

int json(void) {
    value();
    return true;
}

int value(void) {
    int TOKENTYPE = Token_type(currentToken);
    if (TOKENTYPE == LEFTCURLYBRACKET) {
        object();
    } else if (TOKENTYPE == LEFTSQUAREBRACKET) {
        array();
    } else if (TOKENTYPE == STRING) {
        string();
    } else if (TOKENTYPE == LITERAL) {
        literal();
    } else if (TOKENTYPE == NUMBER) {
        number();
    } else {
        parserError("value(); Token desconhecido");
    }
}

int object(void) {
    identCount += 4;
    if (validateCurrentToken("{")) {
        members();
    } validateCurrentToken("}");
    identCount -= 4;
    return true;
}

int array(void) {
    identCount += 4;
    if (validateCurrentToken("[")) {
        values();
    } validateCurrentToken("]");
    identCount -= 4;
    return true;
}

int values(void) {
    value();
    if (Token_type(currentToken) == COMMA) {
        validateCurrentToken(",");
        values();
    }return true;
}

int members(void) {
    key();
    validateCurrentToken(":");
    value();
    if (Token_type(currentToken) == COMMA) {
        validateCurrentToken(",");
        members();
    }
    return true;
}

int key(void) {
    string();
    return true;
}

int string(void) {
    if (Token_type(currentToken) == STRING) {
        validateCurrentToken(Token_token(currentToken));
        return true;
    } parserError("Esperava-se um token do tipo STRING");
}

int literal(void) {
    if(Token_type(currentToken) == LITERAL) {
        validateCurrentToken(Token_token(currentToken));
        return true;
    } parserError("Esperava-se um token do tipo LITERAL");
}

int number(void) {
    if(Token_type(currentToken) == NUMBER) {
        validateCurrentToken(Token_token(currentToken));
        return true;
    } parserError("Esperava-se um token do tipo NUMBER");
}

/**
 * O token atual só é movido para o próximo token a partir dessa função.
 * 
 * O parâmetro 'tokenMustBe' é o token atual esperado, de acordo com a gramática no arquivo json.grammar.
 */
int validateCurrentToken(char* tokenMustBe) {
    char dbmsg[128];
    if (!strcmp(Token_token(currentToken), tokenMustBe)) {
        if (DEBUG) {
            sprintf(dbmsg, "[Validando Token] TokenType: {%s}, Token: {%s}", strTokenType(currentToken), Token_token(currentToken));
            printf("%*s\n", strlen(dbmsg)+identCount, dbmsg);
        }
        currentToken = next_token(parserFile());
        return true;
    }
    sprintf(dbmsg, "validateCurrentToken(); Token esperado {%s}, token recebido {%s}", tokenMustBe, Token_token(currentToken));
    parserError(dbmsg);
    exit(EXIT_FAILURE);
}

void parser(FILE* fptr) {
    globalFptr = fptr;
    currentToken = next_token(fptr);    // Pega o primeiro token do arquivo

    json();
}