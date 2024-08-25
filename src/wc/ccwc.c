#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int countWords(FILE* fd) {
    int wordCounter = 0;
    char buffer[128];

    rewind(fd);
    while (fscanf(fd, "%128s", buffer) != EOF) {
        // printf("%s:%d\n", buffer, strlen(buffer));
        wordCounter += 1;
    }
    return wordCounter;
}

int countBytes(FILE* fd) {
    int byteCounter = 0;
    char chr;

    rewind(fd);
    while (fscanf(fd, "%c", &chr) != EOF) {
        // printf("%c ", chr);
        byteCounter += 1;
    }
    return byteCounter;
}

int countChars(FILE* fd) {
    int charCounter = 0;
    char chr;

    rewind(fd);
    /**
     * ler byte a byte e verifica se é um próximo
     * byte de um codificação de múltiplos bytes.
     * 0xc0 = 11000000
     * 0x80 = 10000000
     * 10xxxxxx = o identifcador de multibyte encoding do UFT-8. O que quer dizer que é um byte restante de um caracter.
     * ---------------
     * O 0xc0 é usado para pegar os dois bits mais signicativos do byte atual. Daí, se
     * os dois bits mais significativos do byte atual não forem '10', então não é um multibyte encoding do UFT-8.
     */

    while ((chr = fgetc(fd)) != EOF) {
        charCounter += (chr & 0xc0) != 0x80;
    }
    return charCounter;
}

int countLines(FILE* fd) {
    int lineCounter = 0;
    char chr;
    char buffer[128];

    rewind(fd);
    while (fscanf(fd, "%c", &chr) != EOF) {
    // while (fscanf(fd, "%128s", buffer) != EOF) {
        // chr = fgetc(fd);
        if (chr == '\n') {
            lineCounter += 1;
        }
    }
    return lineCounter;
}

void errorMsg(const char* filename) {
    printf("ccwc: %s: No such file or directory\n", filename);
    exit(EXIT_FAILURE);
}

int optionHandler(FILE* fd, const char* option) {

    if(!strcmp(option, "-c")) {
        return countBytes(fd);

    } else if(!strcmp(option, "-m")) {
        return countChars(fd);

    } else if(!strcmp(option, "-l")) {
        return countLines(fd);

    } else if(!strcmp(option, "-w")) {
        return countWords(fd);
    } else {
        printf("ccwc: invalid option -- '%c'\n", option[1]);
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char const *argv[])
{
    int ret;

    // opção e arquivo
    if (argc == 3) {
        FILE* fd = fopen(argv[2], "r");
        if (!fd) {
            errorMsg(argv[2]);
        }

        ret = optionHandler(fd, argv[1]);

        printf("%d %s\n", ret, argv[2]);
        fclose(fd);

    } else if (argc == 2) {
        if (strchr(argv[1], '-') != NULL) {
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