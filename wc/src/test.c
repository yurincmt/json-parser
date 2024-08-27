#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <wchar.h>


int main(int argc, char const *argv[])
{
    char* ptr;

    if ((ptr = strchr(argv[1], '-')) == NULL) {
        printf("NULL\n");
    } else {
        puts(ptr);
    }
    // printf("%s\n", strchr(str, '-'));
    return 0;
}