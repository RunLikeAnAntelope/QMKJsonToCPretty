#include "QJTCP.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "qfclogger.h"

#define MAX_LAYERS 20
struct chars {
  ssize_t size;
  char *chars;
};

typedef struct substring {
  ssize_t startIndex;
  ssize_t endIndex;
} substring;

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
layers extractLayers(char *input) {
  struct layers kb_layers = {NULL, 0};

  char *startPtr = strstr(input, "\"layers\"");
  if (startPtr == NULL) {
    printf("Json must have a \"layers\" element");
    return kb_layers;
  }

  startPtr = strchr(startPtr, '[');
  if (startPtr == NULL) {
    printf("Json malformed\n");
    return kb_layers;
  }
  int bracket_cntr = 1;
  size_t len = 1;

  substring *all_layers = NULL;
  size_t num_layers = 0;
  bool in_layer = false;

  while (bracket_cntr != 0 && startPtr[len] != '\0') {
    if (startPtr[len] == '[') {
      bracket_cntr += 1;
    } else if (startPtr[len] == ']') {
      bracket_cntr -= 1;
    }

    if (bracket_cntr > 2) {
      printf("Malformed layers element. Structure too deep\n");
    }

    if (!in_layer && bracket_cntr == 2) {
      in_layer = true;
      num_layers += 1;
      all_layers = realloc(all_layers, sizeof(substring) * num_layers);
      all_layers[num_layers - 1].startIndex = len;
    } else if (in_layer && bracket_cntr == 1) {
      in_layer = false;
      all_layers[num_layers - 1].endIndex = len;
    }

    len += 1;
  }

  if (bracket_cntr != 0) {
    printf("Unbalanced brackets in your layers entry.\n");
    return kb_layers;
  }

  kb_layers.layers = malloc(sizeof(char *) + num_layers);

  unsigned long i = 0;
  for (i = 0; i < num_layers; i++) {
    kb_layers.layers[i] =
        malloc(all_layers[i].endIndex - all_layers[i].startIndex + 2);
    kb_layers.layers[i] =
        memcpy(kb_layers.layers[i], &startPtr[all_layers[i].startIndex],
               all_layers[i].endIndex - all_layers[i].startIndex + 1);
    kb_layers.layers[i][all_layers[i].endIndex - all_layers[i].startIndex + 1] =
        '\0';
  }

  kb_layers.num_layers = num_layers;

  return kb_layers;
}

void run(char *filename) {
  qfcReadFile(filename);
  extractLayers(input_file.chars);
}
