#ifndef structures_htable_h
#define structures_htable_h

#include "refcnt.h"
#include <stdbool.h>

typedef unsigned long Hash;

typedef struct {
    char *key;
    void *obj;
} Entry;

typedef struct _tableBucket {
    Entry *entry;
    struct _tableBucket *next;

} TableBucket;

typedef struct {
    TableBucket **table;
    size_t count;
} HashTable;


HashTable *htable_init(void);
HashTable *htable_set(HashTable *t, const char *key, void *obj);
void *htable_retrieve(HashTable *t, const char *key, bool remove);
void htable_iterate(HashTable *t, void *context, void (*obj_iteration)(Entry *entry, int i, size_t count, bool *stop, void *context));
Hash string_hash(const char *string);


#endif
