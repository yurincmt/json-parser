#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"

#define false 0
#define true 1
#define DEBUG 1

extern int LINECOUNTER;
extern const char* STRTOKENTYPE[9];

Token* currentToken = NULL;
int identCount = 0;
FILE* globalFptr = NULL;

int parser(FILE* fptr) {
    globalFptr = fptr;
    // currentToken = next_token(fptr);    // Pega o primeiro token do arquivo

    if ((currentToken = next_token(fptr)) != NULL) {
        json();
        // printf("LINECOUNTER: %d\n", LINECOUNTER);
    } else {
        parserError("*.json invalido");
    }
    return 0;
}

FILE* parserFile(void) {
    return globalFptr; 
}

void parserError(char* dbmsg) {
    fprintf(stderr, "Parser Error: %s. currentToken{%s}\n", dbmsg, Token_token(currentToken));
    exit(EXIT_FAILURE);
}

int json(void) {
    int TOKENTYPE = Token_type(currentToken);

    if (TOKENTYPE == LEFTCURLYBRACKET) {
        object();
    } else if (TOKENTYPE == LEFTSQUAREBRACKET) {
        array();
    } else {
        parserError("*.json deve ser um object ou array");
    }
    return 1;
}

int object(void) {
    if (validateCurrentToken("{")) {
        if (Token_type(currentToken) == RIGHTCURLYBRACKET) {
            validateCurrentToken("}");
            return 0;
        }
        identCount += 3;
        members();
        identCount -= 3;
    } validateCurrentToken("}");
    return 1;
}

int members(void) {
    key();
    validateCurrentToken(":");
    value();
    if (Token_type(currentToken) == COMMA) {
        validateCurrentToken(",");
        members();
    }
    return 1;
}

int key(void) {
    string();
    return 1;
}

int array(void) {

    validateCurrentToken("[");

    if (value()) {
        if (Token_type(currentToken) == COMMA) {
            validateCurrentToken(",");
            if (!values()) {
                parserError("Esperava-se <values>");
            }
        }
    }
    validateCurrentToken("]");
    
    // if (validateCurrentToken("[")) {
    //     if (Token_type(currentToken) == RIGHTSQUAREBRACKET) {
    //         validateCurrentToken("]");
    //         return 0;
    //     }
    //     identCount += 3;
    //     values();
    //     identCount -= 3;
    // } validateCurrentToken("]");
    return 1;
}

int values(void) {
    if (!value()) {
        parserError("Esperava-se um <value>");
    }

    if (Token_type(currentToken) == COMMA) {
        validateCurrentToken(",");
        values();
    }
    return 1;
}

int value(void) {
    int TOKENTYPE = Token_type(currentToken);
    if (TOKENTYPE == LEFTCURLYBRACKET) {
        object();
    } else if (TOKENTYPE == LEFTSQUAREBRACKET) {
        array();
    } else if (TOKENTYPE == STRING) {
        string();
    } else if (TOKENTYPE == NUMBER) {
        number();
    } else if (TOKENTYPE == LITERAL) {
        literal();
    }
    else {
        return 0;
    } return 1;
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
            sprintf(dbmsg, "[Validando Token] TokenType: {%s}, Token: {%s}", STRTOKENTYPE[Token_type(currentToken)], Token_token(currentToken));
            printf("%*s\n", strlen(dbmsg)+identCount, dbmsg);
        }
        currentToken = next_token(parserFile());
        return true;
    }
    sprintf(dbmsg, "validateCurrentToken(); Token esperado {%s}, token recebido {%s}", tokenMustBe, Token_token(currentToken));
    parserError(dbmsg);
}