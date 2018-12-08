#ifndef OUTPUT_H
#define OUTPUT_H

#include "utils.h"
#include "splash_helper.h"

char last_uuid[37];

void output_header(FILE *output);
void output_footer(FILE *output);
void output_operand(FILE *output, Operand op);
void output_number(FILE *output, Operand op);
void output_set_variable(FILE *output, char100 name);
void output_get_variable(FILE *output, Operand op);
void output_get_magic_variable(FILE *output, Operand op);
void output_math_operand(FILE *output, Operand op);
void output_math_simple_operation_parameters(FILE *output, char operator, Operand operand);
void output_math_scientific_operation_parameters(FILE *output, char operator, Operand operand);
void output_operation(FILE *output, char operator, Operand operand, char *uuid);

#endif /* OUTPUT_H */
