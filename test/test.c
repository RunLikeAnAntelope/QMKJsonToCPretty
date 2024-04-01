#include "QJTCP.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int test_extractLayers() {
  char *test = extractLayers("\"layers\": [\"hello\", \"one\"]");
  if (test == NULL || strcmp(test, "[\"hello\", \"one\"]") != 0) {
    printf("Failed extractLayers test 1\n");
    return 0;
  }
  free(test);

  // Test that a input without a layers input returns null
  test = extractLayers("[\"hello\", \"one\"]");
  if (test != NULL) {
    printf("Failed extractLayers test 2\n");
    return 0;
  }

  // Test that lack of closing bracket exits eventually
  test = extractLayers("\"layers\": [\"hello\", \"one\"");
  if (test != NULL) {
    printf("Failed extractLayers test 3\n");
    return 0;
  }

  return 1;
}
int main() {
  assert(test_extractLayers());
  return 0;
}
