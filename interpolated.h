#ifndef INTERPOLATED_H
#define INTERPOLATED_H

#include "splash_helper.h"

Interpolated *interpolated_init();
StringToken *token_init();

Interpolated *interpolated_create(char100 source);

HashTable *interpolated_dict(Interpolated *interpolated);

#endif  /* INTERPOLATED_H */
