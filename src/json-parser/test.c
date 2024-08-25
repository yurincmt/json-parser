#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* openfile(char const* filename) {
    FILE* fptr = NULL;

    if ((fptr  = fopen(filename, "r")) == NULL) {
        puts("error openfile()");
        exit(EXIT_FAILURE);
    } return fptr;
}

void readFileChar(char const* filename) {
    FILE* fptr = openfile(filename);
    char c;
    while ((c = fgetc(fptr)) != EOF) {
        printf("%c ", c);
    }
}

int main(int argc, char const *argv[])
{
    char name[32] = "\0";

    strncpy(name, "yurincmt", 8);

    printf("%s, len: %d\n", name, strlen(name));
    
    return 0;
}
