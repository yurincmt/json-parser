#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wc.h"

int main(int argc, char const *argv[])
{
    int ret;

    /**
     * opção e arquivo
     * wc -w file.in
     */
    if (argc == 3) {
        FILE* fd = fopen(argv[2], "r");
        if (!fd) {
            errorMsg(argv[2]);
        }

        ret = optionHandler(fd, argv[1]);

        printf("%d %s\n", ret, argv[2]);
        fclose(fd);

    /**
     * opção e stdin
     * cat file.in | wc -c
     * 
     * OR
     * 
     * arquivo
     * wc file.in
     */
    } else if (argc == 2) {
        if (!strncmp(argv[1], "-", 1)) {
            ret = optionHandler(stdin, argv[1]);
            printf("%d\n", ret);
            exit(EXIT_SUCCESS);
        }

        FILE* fd = fopen(argv[1], "r");
        if (!fd) {
            errorMsg(argv[1]);
        }

        int retL = countLines(fd);
        int retW = countWords(fd);
        int retB = countBytes(fd);

        printf("%d %d %d %s\n", retL, retW, retB, argv[1]);
        fclose(fd);
    }

    exit(0);
}