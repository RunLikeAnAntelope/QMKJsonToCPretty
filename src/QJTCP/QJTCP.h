#pragma once
typedef struct layers {
    char **layers;
    unsigned int num_layers;
} layers;

typedef struct row {
    char **elems;
    unsigned int num_elems;
} row;

typedef struct split_layers {
    row *rows;
    unsigned int num_rows;
} split_layers;

void run(char *filename);
layers extractLayers(char *input);
row extractRow(char *raw_row);
split_layers extractSplitLayers(layers l);
void freeLayers(layers l);
void qfcReadFile(char *filename);
