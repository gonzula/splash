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

void
_serialize_ht(Entry *entry, int i, int count, int *stop, void *context)
{
    char *key = entry->key;
    Serializable *s = (Serializable *)entry->obj;
    String *str = (String *)context;
    str_append(str, "\"");
    String *escaped = str_escape_cstring(key);
    str_append(str, escaped->string);
    release(escaped);
    str_append(str, "\":");
    String *sout = serializable_write_json(s);
    str_append(str, sout->string);
    if (i < count - 1)
        str_append_char(str, ',');
    release(sout);
}

String *
serializable_write_json(Serializable *s)
{
    String *str = str_init();
    switch (s->type)
    {
        case st_str:
            str_append(str, "\"");
            String *escaped = str_escape(s->str);
            str_append(str, escaped->string);
            release(escaped);
            str_append(str, "\"");
            break;
        case st_list:
            str_append(str, "[");
            LIST_LOOP(s->list)
            {
                String *nodeStr = serializable_write_json(node->object);
                str_append(str, nodeStr->string);
                if (node->next)
                    str_append(str, ",");
                release(nodeStr);
            }
            str_append(str, "]");
            break;
        case st_ht:
            str_append(str, "{");
            htable_iterate(s->ht, str, _serialize_ht);
            str_append(str, "}");
            break;
        case st_int:
        {
            char num[20];
            sprintf(num, "%d", s->i);
            str_append(str, num);
            break;
        }
        case st_float:
        {
            char num[100];
            sprintf(num, "%f", s->f);
            str_append(str, num);
            break;
        }
        case st_bool:
            str_append(str, s->i?"true":"false");
            break;
        case st_null:
            str_append(str, "null");
            break;

        default:
            break;
    }
    return str;
}

Serializable * _read_json           (char *str, size_t len, int *outPos);
Serializable * _read_json_list      (char *str, size_t len, int *outPos);
Serializable * _read_json_string    (char *str, size_t len, int *outPos);
Serializable * _read_json_ht        (char *str, size_t len, int *outPos);
Serializable * _read_json_primitive (char *str, size_t len, int *outPos);

Serializable *
serializable_read_json(String *str)
{
    int op;
    return _read_json(str->string, str->len, &op);
}


Serializable *
_read_json(char *str, size_t len, int *outPos)
{
    int i;
    for (i = 0; i < len; i++)
    {
        char c = str[i];
        if (isblank(c))continue;
        if (c == '[')
        {
            return _read_json_list(&str[i+1], len-i-1, outPos);
        }
        if (c == '{')
        {
            return _read_json_ht(&str[i+1], len-i-1, outPos);
        }
        if (c == '\"')
        {
            return _read_json_string(&str[i+1], len-i-1, outPos);
        }
        if (isprimitive(c))
        {
            return _read_json_primitive(&str[i], len-i, outPos);
        }
        return NULL;
    }
    return NULL;
}

Serializable *
_read_json_primitive (char *str, size_t len, int *outPos)
{
    String *s = str_init();
    int i;
    for (i = 0; i < len; i++)
    {
        char c = str[i];
        if (!isprimitive(c))break;
        str_append_char(s, c);
    }
    Serializable *ser = serializable_init();
    if (streq(s->string, "null"))
        ser->type = st_null;
    else if (streq(s->string, "true"))
    {
        ser->type = st_bool;
        ser->i = 1;
    }
    else if (streq(s->string, "false"))
    {
        ser->type = st_bool;
        ser->i = 0;
    }
    else if (strchr(s->string, '.') || strchr(s->string, 'e') || strchr(s->string, 'E'))//float
    {
        ser->type = st_float;
        sscanf(s->string, "%f", &ser->f);
    }
    else
    {
        ser->type = st_int;
        sscanf(s->string, "%d", &ser->i);
    }

    release(s);
    *outPos = i-1;
    return ser;
}



Serializable *
_read_json_string(char *str, size_t len, int *outPos)
{
    String *s = str_init();
    int i;
    int isEscaped = 0;
    int isReadingUnicode = 0;
    char hex[5];
    hex[4] = 0;
    for (i = 0; i < len; i++)
    {
        char c = str[i];
        if (isReadingUnicode)
        {
            if (ishex(c) && isReadingUnicode < 5)
            {
                hex[isReadingUnicode-1] = c;
                isReadingUnicode++;
                continue;
            }
            else
            {
                unsigned int val = strtol(hex, NULL, 16);
                isReadingUnicode = 0;
                if (val <= 0x007F)
                {
                    str_append_char(s, val);
                }
                else if (val <= 0x07ff)
                {
                    bin8 b1, b2;
                    bin32 v;
                    v.u = val;
                    b2.b0 = v.b0;
                    b2.b1 = v.b1;
                    b2.b2 = v.b2;
                    b2.b3 = v.b3;
                    b2.b4 = v.b4;
                    b2.b5 = v.b5;
                    b2.b6 = 0;
                    b2.b7 = 1;

                    b1.b0 = v.b6;
                    b1.b1 = v.b7;
                    b1.b2 = v.b8;
                    b1.b3 = v.b9;
                    b1.b4 = v.b10;
                    b1.b5 = 0;
                    b1.b6 =
                    b1.b7 = 1;
                    str_append_char(s, b1.uc);
                    str_append_char(s, b2.uc);
                }
                else if (val <= 0xFFFF)
                {
                    bin8 b1, b2, b3;
                    bin32 v;
                    v.u = val;

                    b3.b0 = v.b0;
                    b3.b1 = v.b1;
                    b3.b2 = v.b2;
                    b3.b3 = v.b3;
                    b3.b4 = v.b4;
                    b3.b5 = v.b5;
                    b3.b6 = 0;
                    b3.b7 = 1;

                    b2.b0 = v.b6;
                    b2.b1 = v.b7;
                    b2.b2 = v.b8;
                    b2.b3 = v.b9;
                    b2.b4 = v.b10;
                    b2.b5 = v.b11;
                    b2.b6 = 0;
                    b2.b7 = 1;

                    b1.b0 = v.b12;
                    b1.b1 = v.b13;
                    b1.b2 = v.b14;
                    b1.b3 = v.b15;
                    b1.b4 = 0;
                    b1.b5 =
                    b1.b6 =
                    b1.b7 = 1;

                    str_append_char(s, b1.uc);
                    str_append_char(s, b2.uc);
                    str_append_char(s, b3.uc);
                }
            }
        }
        if (c == '\"' && !isEscaped)
        {
            break;
        }

        if (c == '\\' && !isEscaped)
        {
            isEscaped = 1;
            continue;
        }
        if (isEscaped)
        {
            if (c == 'u')
            {
                isEscaped = 0;
                isReadingUnicode = 1;
                continue;
            }
            switch (c)
            {
                case 'n': str_append_char(s, '\n');break;
                case 'r': str_append_char(s, '\r');break;
                case 'b': str_append_char(s, '\b');break;
                case 'f': str_append_char(s, '\f');break;
                case 't': str_append_char(s, '\t');break;
                case '\\':
                case '/':
                case '\"': str_append_char(s, c);break;
            }
            isEscaped = 0;
            continue;
        }
        isEscaped = 0;
        str_append_char(s, c);
    }
    *outPos = i+1;
    Serializable *ser = serializable_create(s, st_str);
    release(s);
    return ser;
}

Serializable *
_read_json_list(char *str, size_t len, int *outPos)
{
    List *list = list_init();
    int i;
    int state = 0;
    for (i = 0; i < len; i++)
    {
        char c = str[i];
        if (isblank(c))continue;
        if (state == 0)
        {
            int op;
            Serializable *s = _read_json(str + i, len - i, &op);
            list_append(list, s);
            release(s);
            i += op;
            state = 1;
            continue;
        }
        if (state == 1)
        {
            if (c == ',')
            {
                state = 0;
                continue;
            }
            if (c == ']')
            {
                break;
            }
        }
    }
    Serializable *s = serializable_create(list, st_list);
    release(list);
    *outPos = i+1;
    return s;
}

Serializable *
_read_json_ht(char *str, size_t len, int *outPos)
{
    HashTable *t = htable_init();
    int i;
    int state = 0;              // 0 - waiting key
                                // 1 - waiting colon
                                // 2 - waiting value
                                // 3 - waiting comma
    Serializable *keyHolder;
    for (i = 0; i < len; i++)
    {
        char c = str[i];
        if (isblank(c))continue;
        if (state == 0)
        {
            if (c == '\"')
            {
                int op;
                keyHolder = _read_json_string(str+i+1, len-i-1, &op);
                i += op;
                state = 1;
                continue;
            }
            else
                break;
        }
        if (state == 1)
        {
            if (c == ':')
                state = 2;
            continue;
        }
        if (state == 2)
        {
            int op;
            Serializable *s = _read_json(str+i, len-i, &op);
            htable_set(t, keyHolder->str->string, s);
            release(keyHolder);
            release(s);
            i += op;
            state = 3;
            continue;
        }
        if (state == 3)
        {
            if (c == ',')
            {
                state = 0;
                continue;
            }
            if (c == '}')
            {
                break;
            }
        }
    }
    Serializable *s = serializable_create(t, st_ht);
    release(t);
    *outPos = i+1;
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
