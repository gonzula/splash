#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "splash_helper.h"
#include "utils.h"
#include "output.h"
#include "scope.h"
#include "action.h"

void _action_release(void *obj);

void
init_parse() {
    output_header(stdout);
    scopes = htable_init();
    current_scope = scope_create("main");
}

void
end_parse() {
    scope_output(current_scope);
    output_footer(stdout);
    release(scopes);
    release(current_scope);
}

void
append_operand(Operand *stack, OpType type, char100 operand) {
    Operand temp;
    *stack = temp;
    (*stack).type = type;

    uuid_gen((*stack).uuid);

    switch (type) {
        case number:        strcpy((*stack).value.value, operand.value); break;
        case variable:      strcpy((*stack).name.value, operand.value); break;
        case magicVariable: strcpy((*stack).name.value, operand.value); break; break;
    }
}

void
operation_optimization(Operand *stack, char operator, Operand op1, Operand op2) {
    fprintf(stderr, "optimizing %s %c %s\n", op1.value.value, operator, op2.value.value);
    Operand new_stack;
    new_stack.type = number;

    uuid_gen(new_stack.uuid);

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

int
operator_is_commutative(char operator) {
    switch (operator) {
        case '+': return 1;
        case '-': return 0;
        case '*': return 1;
        case '/': return 0;
        case '^': return 0;
        default:  return 0;
    }
}

void
append_operation(Operand *stack, char operator, Operand op1, Operand op2) {
    if (op1.type == number && op2.type == number) { // can optimize
        operation_optimization(stack, operator, op1, op2);
        return;
    }
    if (operator_is_commutative(operator) && strcmp(last_uuid, op2.uuid) == 0) {
        fprintf(stderr, "Switching op's\n");
        Operand tmp = op1;
        op1 = op2;
        op2 = tmp;
    }
    switch (op1.type) {
        case number:        output_number(stdout, op1); break;
        case variable:      output_get_variable(stdout, op1); break;
        case magicVariable: output_get_magic_variable(stdout, op1); break;
    }

    Action *in_action;
    switch (op1.type) {
        case number: in_action = action_create_number(op1); break;
        case variable: in_action = action_create_get_variable(op1); break;
        case magicVariable: in_action = action_create_get_magic_variable(op1); break;
    }

    scope_add_action(current_scope, in_action);

    Action *operation_action = action_create_math_operation(operator, op2);
    output_operation(stdout, operator, op2, operation_action->uuid);
    scope_add_action(current_scope, operation_action);

    Operand new_stack;
    new_stack.type = magicVariable;

    char name[] = "Calculation Result";
    strcpy(new_stack.name.value, name);
    strcpy(new_stack.uuid, operation_action->uuid);

    *stack = new_stack;
    release(in_action);
    release(operation_action);
}

void
append_minus_op(Operand *stack, Operand op) {
    Operand temp;
    temp.type = number;
    uuid_gen(temp.uuid);
    char minus_one[] = "-1";
    strcpy(temp.value.value, minus_one);

    append_operation(stack, '*', temp, op);
}
