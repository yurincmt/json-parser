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
    Token* a;
    char const * filename = (argv[1] ? argv[1] : "input.1"); 
    printf("filename: %s\n", filename);

    lex(filename);
    return 0;
}