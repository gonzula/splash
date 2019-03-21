#include <stdlib.h>
#include "refcnt.h"

typedef struct {
    unsigned int retainCnt;
    refcnt_obj_free freeFunc;
    void *data;
} _refcnt_objwrapper;


void *
alloc(size_t size, refcnt_obj_free freeFunc) {
    _refcnt_objwrapper *ow;
    void *ptr;

    ow = (_refcnt_objwrapper *)calloc(1, sizeof(_refcnt_objwrapper) + size);
    ptr = ow;
    ptr += sizeof(_refcnt_objwrapper);
    ow->retainCnt = 1;
    ow->data = ptr;
    ow->freeFunc = freeFunc;

    return ( void * )ptr;
}

void
retain(void *ptr) {
    if (!ptr) return;
    _refcnt_objwrapper * ow;
    void *cptr;
    cptr  = ptr;
    cptr -= sizeof(_refcnt_objwrapper);
    ow = (_refcnt_objwrapper *)cptr;
    ow->retainCnt++;
}

void
release(void *ptr) {
    if (!ptr) return;
    _refcnt_objwrapper * ow;
    char *cptr;
    cptr  = (char *)ptr;
    cptr -= sizeof(_refcnt_objwrapper);
    ow = (_refcnt_objwrapper *)cptr;
    ow->retainCnt--;
    if(ow->retainCnt == 0) {
        if (ow->freeFunc)
            ow->freeFunc(ptr);
        free(ow);
    }
}

int
refcnt_getcnt(void *ptr) {
    _refcnt_objwrapper * ow;
    char *cptr;
    cptr  = (char *)ptr;
    cptr -= sizeof(_refcnt_objwrapper);
    ow = (_refcnt_objwrapper *)cptr;
    return ow->retainCnt;
}
