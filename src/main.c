#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "qfclogger.h"

void qfcReadFile(char* fileName) {
    FILE* fptr = fopen(fileName, "r");
    if (!fptr) {
        printf("Failed to open %s", fileName);
    }

    // used for reading in lines
    char* line = NULL;
    size_t len = 0;
    ssize_t line_len = 0;

    // full file as an array
    char* input_file = NULL;
    ssize_t if_size = 0;

    while ((line_len = getline(&line, &len, fptr)) != -1) {
        input_file = realloc(input_file, if_size + line_len);
        memcpy(&input_file[if_size], line, line_len);
        if_size += line_len;
    }
    fclose(fptr);

    // add extra room for null terminator
    if_size += 1;
    input_file = realloc(input_file, if_size);
    input_file[if_size - 1] = '\0';
}
int main(int argc, char* argv[]) {
    if (argc >= 2) {
        qfcReadFile(argv[1]);
    } else {
        printf("You must provide a filename as an argument");
    }
    return 0;
}
