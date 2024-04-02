#pragma once
typedef struct layers {
  char **layers;
  unsigned int num_layers;
} layers;

void run(char *filename);
layers extractLayers(char *input);
void qfcReadFile(char *filename);
