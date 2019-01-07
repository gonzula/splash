#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include "structures/serializable.h"
#include "utils.h"
#include "splash_helper.h"


void output_header(FILE *output);
void output_footer(FILE *output);
void output_operation(FILE *output, Action *action);

void output_htable(FILE *output, HashTable *htable);

#endif /* OUTPUT_H */
