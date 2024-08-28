
#ifndef WC
#define WC

int countWords(FILE* fd);
int countBytes(FILE* fd);
int countChars(FILE* fd);
int countLines(FILE* fd);
void errorMsg(const char* filename);
int optionHandler(FILE* fd, const char* option);

#endif