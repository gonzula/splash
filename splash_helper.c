#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "splash_helper.h"


void
append_operand(char **stack, const char *operand) {
    *stack = (char *)malloc(sizeof(char) * (strlen(operand) + 1));
    strcpy(*stack, operand);
}

void
append_operation(char **stack, char operator, char *op1, char *op2) {
    puts(op1);
    printf("# %c %s\n", operator, op2);
    puts("AUX = #");
    char aux[] = "AUX";
    strcpy(*stack, aux);
}
