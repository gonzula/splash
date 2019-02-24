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
    for (int i = 0; s[i]; i++) {
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
            current_token->position = interpolated->str->len;
            is_inside_interpolation = 1;
            continue;
        }

        str_append_char(interpolated->str, s[i]);
    }

    return interpolated;
}

void _token_release(void *obj) {
    StringToken *token = (StringToken *)obj;
    release(token->name);
}

void _interpolated_release(void *obj) {
    Interpolated *interpolated = (Interpolated *)obj;
    release(interpolated->str);
    release(interpolated->tokens);
}
