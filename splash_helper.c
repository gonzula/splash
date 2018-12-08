#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "splash_helper.h"
#include "utils.h"
#include "output.h"


void
append_operand(Operand *stack, OpType type, char100 operand) {
    Operand temp;
    *stack = temp;
    (*stack).type = type;

    switch (type) {
        case number:   strcpy((*stack).value.value, operand.value); break;
        case variable: strcpy((*stack).name.value, operand.value); break;
        case magicVariable:
                       strcpy((*stack).name.value, operand.value); break;
                       uuid_gen((*stack).uuid);
                       break;
    }
}

void
operation_optimization(Operand *stack, char operator, Operand op1, Operand op2) {
    fprintf(stderr, "optimizing %s %c %s\n", op1.value.value, operator, op2.value.value);
    Operand new_stack;
    new_stack.type = number;

    double v1 = atof(op1.value.value);
    double v2 = atof(op2.value.value);
    double result;

    fprintf(stderr, "v1 = %lf\n", v1);
    fprintf(stderr, "v2 = %lf\n", v2);

    switch (operator) {
        case '+': result = v1 + v2; break;
        case '-': result = v1 - v2; break;
        case '*': result = v1 * v2; break;
        case '/': result = v1 / v2; break;
        case '^': result = pow(v1, v2); break;
    }
    fprintf(stderr, "ret = %lf\n", result);

    sprintf(new_stack.value.value, "%lf", result);
    (*stack) = new_stack;
}

void
append_operation(Operand *stack, char operator, Operand op1, Operand op2) {
    if (op1.type == number && op2.type == number) { // can optimize
        operation_optimization(stack, operator, op1, op2);
        return;
    }
    switch (op1.type) {
        case number: output_number(stdout, op1.value); break;
        case variable: output_get_variable(stdout, op1.name); break;
        case magicVariable: output_get_magic_variable(stdout, op1); break;
    }

    char uuid[37];
    uuid_gen(uuid);
    output_operation(stdout, operator, op2, uuid);

    Operand new_stack;
    new_stack.type = magicVariable;

    char name[] = "Calculation Result";
    strcpy(new_stack.name.value, name);
    strcpy(new_stack.uuid, uuid);

    *stack = new_stack;
}

void
append_minus_op(Operand *stack, Operand op) {
    Operand temp;
    temp.type = number;
    char minus_one[] = "-1";
    strcpy(temp.value.value, minus_one);

    append_operation(stack, '*', temp, op);
}

void
set_variable(char100 id, Operand op) {
    switch (op.type) {
        case number: output_number(stdout, op.value); break;
        case variable: output_get_variable(stdout, op.name); break;
        case magicVariable: break;  // if it's a magic variable, its the flow
    }

    output_set_variable(stdout, id);
}
