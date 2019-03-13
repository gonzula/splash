#ifndef __serializable_H_
#define __serializable_H_

#include "structures.h"

typedef enum {
    st_ht,
    st_list,
    st_str,
    st_int,
    st_float,
    st_bool,
    st_null
} SerializableType;

typedef struct {
    union {
        HashTable *ht;
        List *list;
        String *str;
        int i;
        float f;
        void *obj;
    };
    SerializableType type;
} Serializable;

Serializable *serializable_create_ht(HashTable *ht);
Serializable *serializable_create_list(List *list);
Serializable *serializable_create_str(String *str);
Serializable *serializable_create_int(int i);
Serializable *serializable_create_float(float f);
Serializable *serializable_create_bool(int i);
Serializable *serializable_create_null(void);

#endif //__serializable_H_
