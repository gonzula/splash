#ifndef INTERPOLATED_H
#define INTERPOLATED_H

#include "splash_helper.h"

Interpolated *interpolated_init(void);
StringToken *token_init(void);

Interpolated *interpolated_create(char100 source);

HashTable *interpolated_dict(Interpolated *interpolated);

#endif  /* INTERPOLATED_H */
