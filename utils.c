#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"

#define CHARS "0123456789ABCDEF"

void uuid_gen(char out[]) {
    static char seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    for (int i = 0; i < 37; i++) {
        char c;
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            c = '-';
        }
        else if (i == 14) {
            c = '4';
        } else if (i == 36) {
            c = 0;
        } else {
            c = CHARS[rand() % 16];
        }

        out[i] = c;
    }
}
