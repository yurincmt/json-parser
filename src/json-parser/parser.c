#include <stdio.h>
#include "parser.h"

#define DEBUG 1
#define false 0
#define true 1

Token* currentToken = NULL;
int identCount = 0;
FILE* fptr = NULL;

void ParserError(char* dbmsg) {
    fprintf(stderr, "Semantic error: %s\n", dbmsg);
    exit(EXIT_FAILURE);
}

int json(void) {
    value();
    return true;
}

int value(void) {
    if (currentTokenIs("{")) {
        object();
    } else if (currentTokenIs("[")) {
        array();
    } else if (currentToken->tokenType == STRING) {
        string();
    } else if (currentToken->tokenType == LITERAL) {
        literal();
    } else if (currentToken->tokenType == NUMBER) {
        number();
    } else {
        ParserError("value(); Token desconhecido");
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
    if (currentTokenIs(",")) {
        validateCurrentToken(",");
        values();
    }return true;
}

int members(void) {
    key();
    validateCurrentToken(":");
    value();
    if (currentTokenIs(",")) {
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
    if (currentToken->tokenType == STRING) {
        validateCurrentToken(currentToken->token);
        return true;
    } ParserError("Esperava-se um token do tipo STRING");
}

int literal(void) {
    if(currentToken->tokenType == LITERAL) {
        validateCurrentToken(currentToken->token);
        return true;
    } ParserError("Esperava-se um token do tipo LITERAL");
}

int number(void) {
    if(currentToken->tokenType == NUMBER) {
        validateCurrentToken(currentToken->token);
        return true;
    } ParserError("Esperava-se um token do tipo NUMBER");
}

/**
 * O token atual só é movido para o próximo token a partir dessa função.
 * 
 * O parâmetro 'tokenMustBe' é o token atual esperado, de acordo com a gramática no arquivo json.grammar.
 */
int validateCurrentToken(char* tokenMustBe) {
    char dbmsg[128];
    if (!strcmp(currentToken->token, tokenMustBe)) {
        if (DEBUG) {
            sprintf(dbmsg, "[Validando Token] TokenType: {%s}, Token: {%s}", StrTokenType[currentToken->tokenType], currentToken->token);
            printf("%*s\n", strlen(dbmsg)+identCount, dbmsg);
        }
        currentToken = next_token(fptr);
        return true;
    }
    sprintf(dbmsg, "validateCurrentToken(); Token esperado {%s}, token recebido {%s}", tokenMustBe, currentToken->token);
    ParserError(dbmsg);
    exit(EXIT_FAILURE);
}

int currentTokenIs(char const * token) {
    return !strncmp(currentToken->token, token, 1);
}

void parser(FILE* fptr) {
    currentToken = next_token(fptr);    // Pega o primeiro token do arquivo

    json();
}

int main(int argc, char const *argv[])
{
    char const * filename = argv[1];
    // filename = "input2.json";
    fptr = openFile(filename);

    parser(fptr);

    return 0;
}
