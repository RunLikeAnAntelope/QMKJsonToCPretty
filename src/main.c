#include <stdio.h>

#include "QJTCP.h"

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        run(argv[1], argv[2]);
    } else {
        printf("You must provide a filename as an argument\n");
        run("../test/resources/test.json", "../kb/lily58pro.txt");
    }
    return 0;
}
