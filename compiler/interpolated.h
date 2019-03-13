#ifndef INTERPOLATED_H
#define INTERPOLATED_H

#include "splash_helper.h"

Interpolated *interpolated_init(void);
StringToken *token_init(void);

Interpolated *interpolated_create(char100 source);
Interpolated *interpolated_create_from_token(const char *token_name);

Serializable *interpolated_parameters(Interpolated *interpolated);

#endif  /* INTERPOLATED_H */
