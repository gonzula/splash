#include <stdlib.h>
#include <string.h>
#include "htable.h"

#define TABLE_SIZE (1 << 10)
#define streq(a, b) (strcmp(a,b)==0)

Entry * entry_create(const char *key, void *obj);
void entry_free(Entry *e);
Hash string_hash(const char * string);

void
_htable_release(void *t);

HashTable *
htable_init() {
    HashTable * t = (HashTable *)alloc(sizeof(HashTable), _htable_release);
    t->table = (TableBucket**)calloc(TABLE_SIZE, sizeof(TableBucket*));
    t->count = 0;
    return t;
}


HashTable *
htable_set(HashTable *t, const char *key, void *obj) {
    Entry *e = entry_create(key, obj);
    Hash hash = string_hash(key);
    TableBucket * bucket = *(t->table + (hash & (TABLE_SIZE - 1)));
    TableBucket * previous_bucket = NULL;

    /* check if we have a collision or the key already existed in the table */
    while (bucket && !streq(bucket->entry->key, key)) {
        previous_bucket = bucket;
        bucket = bucket->next;
    }
    if (!bucket) { /* new Entry */
        bucket = (TableBucket*)malloc(sizeof(TableBucket));
        bucket->next = NULL;
        bucket->entry = NULL;
        t->count++;
    } else { /* updating Entry */
        entry_free(bucket->entry);
        bucket->entry = NULL;
    }

    if (previous_bucket) {
        previous_bucket->next = bucket;
    }

    bucket->entry = e;

    if (!t->table[hash & (TABLE_SIZE - 1)]) {
        t->table[hash & (TABLE_SIZE - 1)] = bucket;
    }

    return t;
}

void *
htable_retrieve(HashTable *t, const char * key, int remove) {
    Hash hash = string_hash(key);
    TableBucket **top_bucket = &(t->table[hash & (TABLE_SIZE - 1)]);
    TableBucket *bucket = *top_bucket;
    TableBucket *previous_bucket = NULL;
    while (bucket && !streq(bucket->entry->key, key)) {
        previous_bucket = bucket;
        bucket = bucket->next;
    }
    if (bucket) { /* Found */
        Entry *entry = bucket->entry;
        void *obj = entry->obj;
        if (remove) {
            if (previous_bucket) {
                previous_bucket->next = bucket->next;
            }
            else {
                *top_bucket = bucket->next;
            }
            retain(entry->obj);
            entry_free(entry);
            free(bucket);
            t->count--;
        }
        return obj;
    }
    return NULL; /* Not found */
}

void
htable_iterate(HashTable *t, void *context, void (*obj_iteration)(Entry *entry, int i, size_t count, int *stop, void *context)) {
    int stop = 0;
    for (int i = 0, j = 0; j < t->count && !stop; i++)
    {
        if (t->table[i])
        {
            TableBucket * bucket = t->table[i];
            while (bucket && !stop)
            {
                obj_iteration(bucket->entry, j, t->count, &stop, context);
                j++;
                bucket = bucket->next;
            }
        }
    }
}

void
htable_release(HashTable *t) {
    for (int i = 0; t->count; i++)
    {
        if (t->table[i])
        {
            TableBucket * bucket = t->table[i];
            while (bucket)
            {
                TableBucket * bucketToFree = bucket;
                entry_free(bucket->entry);
                bucket = bucket->next;
                free(bucketToFree);
                t->count--;
            }
        }
    }
    free(t->table);
}

void
_htable_release(void *t) {
    htable_release(t);
}

Entry *
entry_create(const char *key, void *obj) {
    Entry * e = (Entry *)malloc(sizeof(Entry));
    e->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(e->key, key);
    e->obj = obj;
    retain(obj);
    return e;
}

void
entry_free(Entry *e) {
    release(e->obj);
    free(e->key);
    free(e);
}

Hash
string_hash(const char * string) {
    unsigned i = 0;
    unsigned long x = string[i] << 7;
    while (string[i])
        x = (1000003 * x) ^ string[i++];
    x ^= i;
    return x;
}
