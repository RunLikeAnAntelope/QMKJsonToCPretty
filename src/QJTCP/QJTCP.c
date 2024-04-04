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
        input_file.chars =
            realloc(input_file.chars, input_file.size + line_len);
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
            return kb_layers;
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
        kb_layers
            .layers[i][all_layers[i].endIndex - all_layers[i].startIndex + 1] =
            '\0';
    }

    kb_layers.num_layers = num_layers;

    return kb_layers;
}

layer extractLayer(char *raw_row) {
    layer ret_row = {NULL, 0};

    int i = 0;
    bool in_quote = false;

    char temp[80];
    unsigned int temp_len = 0;
    while (raw_row[i] != '\0') {
        if (in_quote == false && raw_row[i] == '\"') {
            in_quote = true;
        } else if (in_quote == true && raw_row[i] != '\"') {
            temp[temp_len] = raw_row[i];
            temp_len++;
        } else if (in_quote == true && raw_row[i] == '\"') {
            in_quote = false;
            temp[temp_len] = '\0';
            temp_len++;
            ret_row.num_elems++;

            ret_row.elems =
                realloc(ret_row.elems, ret_row.num_elems * sizeof(char *));
            ret_row.elems[ret_row.num_elems - 1] = malloc(strlen(temp));
            strcpy(ret_row.elems[ret_row.num_elems - 1], temp);
            temp_len = 0;
        }
        i++;
    }
    return ret_row;
}
split_layers extractSplitLayers(layers l) {
    split_layers slt_layers;
    slt_layers.rows = malloc(sizeof(layer) * l.num_layers);
    slt_layers.num_rows = l.num_layers;
    int i = 0;
    for (i = 0; i < l.num_layers; i++) {
        slt_layers.rows[i] = extractLayer(l.layers[i]);
    }
    return slt_layers;
}

void freeLayers(layers l) {
    int i = 0;
    for (i = 0; i < l.num_layers; i++) {
        free(l.layers[i]);
    }

    free(l.layers);
}

bool splitLayersInvalid(split_layers sl) {
    unsigned int layer_len = sl.rows[0].num_elems;
    int i;
    for (i = 1; i < sl.num_rows; i++) {
        if (sl.rows[i].num_elems != layer_len) {
            printf("Error. All layers need to be the same length\n");
            return false;
        }
    }
    return true;
}

int largestElemInLayer(layer l) {
    int i;
    int max_length = 0;
    for (i = 0; i < l.num_elems; i++) {
        int new_len = strlen(l.elems[i]);
        if (new_len > max_length) {
            max_length = new_len;
        }
    }
    return max_length;
}

int run(char *filename) {
    qfcReadFile(filename);
    layers my_layers = extractLayers(input_file.chars);

    if (my_layers.num_layers == 0) {
        return 1;
    }

    split_layers slt_layers = extractSplitLayers(my_layers);
    freeLayers(my_layers);

    if (splitLayersInvalid(slt_layers)) {
        return 1;
    }

    return 0;
}
