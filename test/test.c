#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "QJTCP.h"

int layersTest1() {
    layers test = extractLayers("\"layers\": [\"hello\", \"one\"]");
    if (test.num_layers != 0) {
        printf("layers with only one depth should fail\n");
        return 0;
    }
    return 1;
}
int layersTest2() { // Test that a input without a layers input returns null
    layers test = extractLayers("[\"hello\", \"one\"]");
    if (test.num_layers != 0) {
        printf("Json with no layers should fail\n");
        return 0;
    }
    return 1;
}
int layersTest3() { // Test that lack of closing bracket exits eventually
    layers test = extractLayers("\"layers\": [\"hello\", \"one\"");
    if (test.num_layers != 0) {
        printf("Failed extractLayers test 3\n");
        return 0;
    }
    return 1;
}
int layersTest4() {
    layers test = extractLayers(
        "\"layers\": [[\"hello\", \"one\"],[\"hello\", \"two\"]]");
    if (test.num_layers != 2) {
        printf("Failed test because wrong number of layers were generted\n");
        return 0;
    }

    if (strcmp(test.layers[0], "[\"hello\", \"one\"]") != 0) {
        printf("Failed test because first layer did not match\n");
        return 0;
    }

    if (strcmp(test.layers[1], "[\"hello\", \"two\"]") != 0) {
        printf("Failed test because first layer did not match\n");
        return 0;
    }
    return 1;
}

int extractRowTest1() {
    char *test_input = "[\"hello\", \"two\"]";
    row extracted_row = extractRow(test_input);

    if (extracted_row.num_elems != 2) {
        return 0;
    }

    if (strcmp(extracted_row.elems[0], "hello") != 0) {
        return 0;
    }

    if (strcmp(extracted_row.elems[1], "two") != 0) {
        return 0;
    }
    return 1;
}

int main() {
    assert(layersTest1());
    assert(layersTest2());
    assert(layersTest3());
    assert(layersTest4());
    assert(extractRowTest1());
    return 0;
}
