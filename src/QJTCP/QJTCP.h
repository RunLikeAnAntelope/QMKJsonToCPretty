#pragma once
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

int run(char *filename);
layers extractLayers(char *input);
layer extractLayer(char *raw_row);
split_layers extractSplitLayers(layers l);
void freeLayers(layers l);
void qfcReadFile(char *filename);
