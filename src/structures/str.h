#ifndef __str_H_
#define __str_H_

typedef struct
{
    union {
        char *string;
        unsigned char *ustring;
    };
    size_t len;
    size_t bufferSize;
} String;

String *str_init();
String *str_create(const char * original);
String *str_escape_cstring(char * string);
String *str_escape(String *str);
void str_append(String *str, const char * toAppend);
void str_append_char(String *str, const unsigned char c);
void str_center(String *str, int size);
int str_unicode_len(String *str);

#endif //__str_H_
