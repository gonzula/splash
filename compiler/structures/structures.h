#ifndef structures_combined_h
#define structures_combined_h

#include "refcnt.h"

#include "list.h"
#include "htable.h"
#include "str.h"
#include "serializable.h"

#include <stdbool.h>

typedef union {
    unsigned char uc;
    char c;
    struct {
        int b0:1;
        int b1:1;
        int b2:1;
        int b3:1;
        int b4:1;
        int b5:1;
        int b6:1;
        int b7:1;
    };
} bin8;

typedef union {
    unsigned int u;
    int i;
    struct {
        int b0:1;
        int b1:1;
        int b2:1;
        int b3:1;
        int b4:1;
        int b5:1;
        int b6:1;
        int b7:1;
        int b8:1;
        int b9:1;
        int b10:1;
        int b11:1;
        int b12:1;
        int b13:1;
        int b14:1;
        int b15:1;
        int b16:1;
        int b17:1;
        int b18:1;
        int b19:1;
        int b20:1;
        int b21:1;
        int b22:1;
        int b23:1;
        int b24:1;
        int b25:1;
        int b26:1;
        int b27:1;
        int b28:1;
        int b29:1;
        int b30:1;
        int b31:1;
    };
} bin32;

#endif
