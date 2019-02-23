#ifndef UTILS_H
#define UTILS_H

#ifdef DEBUG
#   define DEBUGPRINT(...) fprintf(stderr, __VA_ARGS__)
#else
#   define DEBUGPRINT(...)
#endif

void uuid_gen(char out[]);
char *xml_escape(char *str);

#endif /* UTILS_H */
