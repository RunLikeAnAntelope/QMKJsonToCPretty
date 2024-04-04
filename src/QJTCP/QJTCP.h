#pragma once
#include <stdio.h>
typedef struct chars {
    ssize_t size;
    char *chars;
} chars;
typedef struct layers {
    char **layers;
    unsigned int num_layers;
} layers;

typedef struct layer {
    char **elems;
    unsigned int num_elems;
} layer;

typedef struct split_layers {
    layer *rows;
    unsigned int num_rows;
} split_layers;

int run(char *input_file_name, char *format_file_name);
layers extractLayers(char *input);
layer extractLayer(char *raw_row);
split_layers extractSplitLayers(layers l);
void freeLayers(layers l);
chars qfcReadFile(char *filename);
