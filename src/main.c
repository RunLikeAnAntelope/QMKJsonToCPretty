#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "qfclogger.h"
struct chars {
  ssize_t size;
  char *chars;
};

struct chars input_file;

void qfcReadFile(char *fileName) {
  FILE *fptr = fopen(fileName, "r");
  if (!fptr) {
    printf("Failed to open %s", fileName);
  }

  // full file as an array
  input_file.chars = NULL;
  input_file.size = 0;

  // used for reading in lines
  char *line = NULL;
  size_t len = 0;
  ssize_t line_len = 0;

  while ((line_len = getline(&line, &len, fptr)) != -1) {
    input_file.chars = realloc(input_file.chars, input_file.size + line_len);
    memcpy(&input_file.chars[input_file.size], line, line_len);
    input_file.size += line_len;
  }
  free(line);
  fclose(fptr);

  // terminate string
  input_file.size += 1;
  input_file.chars = realloc(input_file.chars, input_file.size);
  input_file.chars[input_file.size - 1] = '\0';
}

// Just getting pointer to start of layer for now
void extractLayers(char *input) {
  char *startPtr = strstr(input, "\"layers\"");
  if (startPtr == NULL) {
    printf("Json must have a \"layers\" element");
    exit(1);
  }

  startPtr = strchr(startPtr, '[');
  if (startPtr == NULL) {
    printf("Json malformed\n");
    exit(1);
  }

  bool in_str = false;
  int bracket_cntr = 1;
  size_t len = 1;

  while (bracket_cntr != 0) {
    if (startPtr[len] == '[') {
      bracket_cntr += 1;
    } else if (startPtr[len] == ']') {
      bracket_cntr -= 1;
    }
    len += 1;
  }

  // Todo: change from in place to copy to own string
  startPtr[len] = '\0';
}
int main(int argc, char *argv[]) {
  if (argc >= 2) {
    qfcReadFile(argv[1]);
  } else {
    printf("You must provide a filename as an argument");
  }
  extractLayers(input_file.chars);
  return 0;
}
