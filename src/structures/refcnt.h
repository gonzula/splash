#ifndef structures_refcnt_h
#define structures_refcnt_h

#include <stdlib.h>

typedef void (*refcnt_obj_free)(void *a_data);

void * alloc(size_t size, refcnt_obj_free freeFunc);
void retain(void *ptr);
void release(void *ptr);
int refcnt_getcnt(void *ptr);

#endif
