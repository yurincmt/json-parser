#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

FILE* openFile(char const* filename) {
    FILE* fptr = NULL;

    if ((fptr  = fopen(filename, "r")) == NULL) {
        lexError("lex() function");
    } return fptr;
}

int main(int argc, char const *argv[])
{   
    char const * filename = argv[1];
    // printf("filename: %s\n", filename);

    FILE* fptr = openFile(filename);

    Token* token = NULL;
    while ((token = lex(fptr))) {
        tokenPrint(token);
    }
    
    return 0;
}