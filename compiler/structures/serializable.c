#include <stdio.h>
#include <string.h>


#include "serializable.h"
#include "refcnt.h"

#define isblank(c) (strchr(" \n\r", c) != NULL)
#define isprimitive(c) (strchr("-0123456789eEfalsetruenull.", c) != NULL)
#define ishex(c) (strchr("0123456789abcdefABCDEF", c) != NULL)
#define streq(a, b) (strcmp(a,b)==0)

void serializable_free(void *sobj);

Serializable *
serializable_init()
{
    Serializable *s = (Serializable *)alloc(sizeof(Serializable), serializable_free);
    s->obj = NULL;
    s->type = 0;
    return s;
}

Serializable *
serializable_create(void *obj, SerializableType type)
{
    Serializable *s = (Serializable *)alloc(sizeof(Serializable), serializable_free);
    retain(obj);
    switch (type)
    {
        case st_ht:
        case st_list:
        case st_str:
            s->obj = obj;
            break;
        default:
            break;
    }
    s->type = type;
    return s;
}

Serializable *
serializable_create_ht(HashTable *ht) {
    return serializable_create(ht, st_ht);
}

Serializable *
serializable_create_list(List *list) {
    return serializable_create(list, st_list);
}

Serializable *
serializable_create_str(String *str) {
    return serializable_create(str, st_str);
}

Serializable *
serializable_create_int(int i) {
    Serializable *s = serializable_init();
    s->type = st_int;
    s->i = i;

    return s;
}

Serializable *
serializable_create_float(float f) {
    Serializable *s = serializable_init();
    s->type = st_float;
    s->f = f;

    return s;
}

Serializable *
serializable_create_bool(int i) {
    Serializable *s = serializable_init();
    s->type = st_bool;
    s->i = i;

    return s;
}

Serializable *
serializable_create_null() {
    Serializable *s = serializable_init();
    s->type = st_null;
    s->i = 0;

    return s;
}

void
serializable_free(void *sobj)
{
    Serializable *s = (Serializable *)sobj;
    switch (s->type)
    {
        case st_ht:
        case st_list:
        case st_str:
            release(s->obj);
            break;
        default:
            break;
    }
}
