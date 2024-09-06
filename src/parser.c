#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"

#define DEBUG 1
#define retok 1
#define reterr 0

extern Token* globalCurrentToken;
extern FILE* globalFptr;
extern const char* STRTOKENTYPE[9];
extern int LINECOUNTER;

int identCount = 0;


/**
 * Recebe um arquivo arberto e faz análise sintática dele de acordo com a gramática JSON.
 *  - Recebe um arquivo aberto como parâmetro.
 */
int parser(FILE* fptr) {
    globalFptr = fptr;
    // currentToken = next_token(fptr);    // Pega o primeiro token do arquivo

    if ((globalCurrentToken = next_token(fptr)) != NULL) {
        json();
        // printf("LINECOUNTER: %d\n", LINECOUNTER);
    } else {
        parserError("*.json invalido");
    }
    return retok;
}

void parserError(char* dbmsg) {
    fprintf(stderr, "Parser Error: %s. currentToken{%s}\n", dbmsg, Token_token(globalCurrentToken));
    exit(EXIT_FAILURE);
}

int json(void) {
    int TOKENTYPE = Token_type(globalCurrentToken);

    if (TOKENTYPE == LEFTCURLYBRACKET) {
        object();
    } else if (TOKENTYPE == LEFTSQUAREBRACKET) {
        array();
    } else {
        parserError("*.json deve ser um object ou array");
    }
    if (globalCurrentToken != NULL)
        parserError("Unexpected token");
    return retok;
}

int object(void) {
    if (validateCurrentToken("{")) {
        if (Token_type(globalCurrentToken) == RIGHTCURLYBRACKET) {
            validateCurrentToken("}");
            return retok;
        }
        identCount += 3;
        members();
        identCount -= 3;
    } validateCurrentToken("}");
    return retok;
}

int members(void) {
    if (!key()) parserError("Esperava-se um token KEY");
    validateCurrentToken(":");
    if (!value()) parserError("Esperava-se um token VALUE");

    if (Token_type(globalCurrentToken) == COMMA) {
        validateCurrentToken(",");
        members();
    }
    return retok;
}

int key(void) {
    return (string()) ? retok : reterr;
}

int array(void) {

    validateCurrentToken("[");

    if (value()) {
        if (Token_type(globalCurrentToken) == COMMA) {
            validateCurrentToken(",");
            if (!values()) {
                parserError("Esperava-se <values>");
            }
        }
    }
    if (!validateCurrentToken("]")) {
        parserError("Token ] delimiter expected");
    }
    return 0;
}

int values(void) {
    if (!value()) {
        parserError("Esperava-se um <value>");
    }

    if (Token_type(globalCurrentToken) == COMMA) {
        validateCurrentToken(",");
        values();
    }
    return retok;
}

int value(void) {
    int TOKENTYPE = Token_type(globalCurrentToken);
    if (TOKENTYPE == LEFTCURLYBRACKET) {
        object();
    } else if (TOKENTYPE == LEFTSQUAREBRACKET) {
        array();
    } else if (TOKENTYPE == STRING) {
        string();
    } else if (TOKENTYPE == INTEGER || TOKENTYPE == FLOAT) {
        number();
    } else if (TOKENTYPE == LITERAL) {
        literal();
    }
    else {
        return reterr;
    }
    return retok;
}

int string(void) {
    if (Token_type(globalCurrentToken) == STRING) {
        validateCurrentToken(Token_token(globalCurrentToken));
        return retok;
    }
    return reterr;
}

int literal(void) {
    if(Token_type(globalCurrentToken) == LITERAL) {
        validateCurrentToken(Token_token(globalCurrentToken));
        return retok;
    } 
    return reterr;
}

int number(void) {
    if (validateCurrentToken(Token_token(globalCurrentToken)))
        return retok;

    if (validateCurrentToken(Token_token(globalCurrentToken)))
        return retok;
    
    return reterr;
}

/**
 * O token atual só é movido para o próximo token a partir dessa função.
 * 
 * O parâmetro 'tokenMustBe' é o token atual esperado, de acordo com a gramática no arquivo json.grammar.
 */
int validateCurrentToken(char* tokenMustBe) {

    if (!globalCurrentToken) return reterr;
    
    char dbmsg[128];
    if (!strcmp(Token_token(globalCurrentToken), tokenMustBe)) {
        if (DEBUG) {
            sprintf(dbmsg, "[Validando Token] TokenType{%s}, Token{%s}", STRTOKENTYPE[Token_type(globalCurrentToken)], Token_token(globalCurrentToken));
            printf("%*s\n", strlen(dbmsg)+identCount, dbmsg);
        }
        globalCurrentToken = next_token(globalFptr);
        return retok;
    }
    return reterr;
}