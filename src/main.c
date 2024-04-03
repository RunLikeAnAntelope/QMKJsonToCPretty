#include <stdio.h>

#include "QJTCP.h"

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        run(argv[1]);
    } else {
        printf("You must provide a filename as an argument");
    }
    return 0;
}
