#include <stdlib.h>
#include <stdio.h>
#include "splash_compiler.h"

int
main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <input splash file> <output shortcut>\n", argv[0]);
        return EXIT_FAILURE;
    }
    return parse(argv[1], argv[2]);
}
