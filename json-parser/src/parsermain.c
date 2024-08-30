#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int LINECOUNTER = 0;

FILE* openFile(char const* filename) {
    FILE* fptr = NULL;

    if ((fptr  = fopen(filename, "r")) == NULL) {
        parserError("openFile() function");
    } return fptr;
}

int main(int argc, char const *argv[])
{
    char const * filename = argv[1];
    // filename = "input1.json";
    FILE* fptr = openFile(filename);

    parser(fptr);
    return 0;
}