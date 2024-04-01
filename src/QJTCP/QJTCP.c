#include "QJTCP.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "qfclogger.h"

// I know this is probably not a good idea. Just having some fun with macros
#define TIMEOUT_LEN 30000
#define RESET_TIMEOUT() (timeout = 0)
#define TIMED_OUT ((timeout > TIMEOUT_LEN && timeout++))
unsigned long timeout = 0;

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

// Returns new string with just the layers. Callee reponsible for freeing.
char *extractLayers(char *input) {
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

  int bracket_cntr = 1;
  size_t len = 1;

  RESET_TIMEOUT();
  while (bracket_cntr != 0 && !TIMED_OUT) {
    if (startPtr[len] == '[') {
      bracket_cntr += 1;
    } else if (startPtr[len] == ']') {
      bracket_cntr -= 1;
    }
    len += 1;
  }

  char *layers = malloc(len + 1);
  strncpy(layers, startPtr, len);
  layers[len] = '\0';
  printf(layers);
  return layers;
}

void run(char *filename) {
  qfcReadFile(filename);
  extractLayers(input_file.chars);
}
