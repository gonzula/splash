#include <string.h>
#include "interpolated.h"

void _token_release(void *obj);
void _interpolated_release(void *obj);

StringToken *
token_init() {
    StringToken *token = (StringToken *)alloc(sizeof(StringToken), _token_release);
    token->name = str_init();

    return token;
}

Interpolated *
interpolated_init() {
    Interpolated *interpolated = (Interpolated *)alloc(sizeof(Interpolated), _interpolated_release);
    interpolated->str = str_init();
    interpolated->tokens = list_init();

    return interpolated;
}

char
_unescape_char(char c) {
    switch (c) {
        case 'b': return '\b';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        default: return c;
    }
}

Interpolated *
interpolated_create(char100 source) {
    Interpolated *interpolated = interpolated_init();
    char *s = source.value;

    int is_escaped = 0;
    int is_inside_interpolation = 0;
    StringToken *current_token = NULL;
    size_t len = strlen(s);
    for (int i = 1; i < len - 1; i++) {
        unsigned char c = s[i];
        if (is_escaped) {
            str_append_char(interpolated->str, _unescape_char(c));
            is_escaped = 0;
            continue;
        } else if (c == '\\') {
            is_escaped = 1;
            continue;
        }

        if (is_inside_interpolation) {
            if (c == '}') {
                list_append(interpolated->tokens, current_token);
                release(current_token);
                current_token = NULL;
                is_inside_interpolation = 0;
            } else {
                str_append_char(current_token->name, c);
            }
            continue;
        } else if (c == '{') {
            current_token = token_init();
            current_token->position = str_unicode_len(interpolated->str);
            //EFBFBC
            str_append_char(interpolated->str, 0xEF);
            str_append_char(interpolated->str, 0xBF);
            str_append_char(interpolated->str, 0xBC);
            is_inside_interpolation = 1;
            continue;
        }

        str_append_char(interpolated->str, s[i]);
    }

    return interpolated;
}

Interpolated *
interpolated_create_from_token(const char *token_name) {
    char100 text;
    sprintf(text.value, "\"{%s}\"", token_name);

    Interpolated *interpolated = interpolated_create(text);
    return interpolated;
}

Serializable *
interpolated_parameters(Interpolated *interpolated) {
    HashTable *dict = htable_init();

    HashTable *value = htable_init();
    Serializable *s2 = serializable_create_ht(value);
    htable_set(dict, "Value", s2);

    HashTable *attachments = htable_init();
    Serializable *s3 = serializable_create_ht(attachments);
    htable_set(value, "attachmentsByRange", s3);

    LIST_LOOP(interpolated->tokens) {
        StringToken *token = (StringToken *)node->object;

        HashTable *dict = htable_init();
        Serializable *s1 = serializable_create_ht(dict);

        char range[100];
        sprintf(range, "{%d, 1}", token->position);
        htable_set(attachments, range, s1);

        String *type = str_create("Variable");
        Serializable *s2 = serializable_create_str(type);
        htable_set(dict, "Type", s2);

        Serializable * s3 = serializable_create_str(token->name);
        htable_set(dict, "VariableName", s3);

        release(s1);
        release(dict);
        release(type);
        release(s2);
        release(s3);
    }

    Serializable *s4 = serializable_create_str(interpolated->str);
    htable_set(value, "string", s4);

    String *serialization_type = str_create("WFTextTokenString");
    Serializable *s5 = serializable_create_str(serialization_type);
    htable_set(dict, "WFSerializationType", s5);

    release(attachments);
    release(s3);
    release(value);
    release(s2);
    release(s4);
    release(serialization_type);
    release(s5);

    Serializable *s = serializable_create_ht(dict);
    release(dict);
    return s;
}

void
_token_release(void *obj) {
    StringToken *token = (StringToken *)obj;
    release(token->name);
}

void
_interpolated_release(void *obj) {
    Interpolated *interpolated = (Interpolated *)obj;
    release(interpolated->str);
    release(interpolated->tokens);
}
