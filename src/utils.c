#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utils.h"

#define CHARS "0123456789ABCDEF"

void
uuid_gen(char out[]) {
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

char *
xml_escape(char *str) {
    /*    https://stackoverflow.com/a/1091953/1186116     */
    /*     "   &quot;       */
    /*     '   &apos;       */
    /*     <   &lt;         */
    /*     >   &gt;         */
    /*     &   &amp;        */
    char *escaped = (char *)calloc(strlen(str) * 6 + 1, sizeof(char));  // 6 is the longest escape sequence's length
    int j = 0;
    for (int i = 0; str[i]; i++) {
        char to_replace[10];
        to_replace[0] = 0;
        if (str[i] == '\"') {
            strcpy(to_replace, "&quot;");
        } else if (str[i] == '\'') {
            strcpy(to_replace, "&apos;");
        } else if (str[i] == '<') {
            strcpy(to_replace, "&lt;");
        } else if (str[i] == '>') {
            strcpy(to_replace, "&gt;");
        } else if (str[i] == '&') {
            strcpy(to_replace, "&amp;");
        }

        if (to_replace[0]) {
            strcpy(escaped + j, to_replace);
            j += strlen(to_replace);
        } else {
            escaped[j] = str[i];
            j++;
        }
    }
    escaped[j] = 0;

    return escaped;
}
