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

FILE *
init_parse(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s (input splash file) (output shortcut)\n", argv[0]);
        return NULL;
    }

    input_file = fopen(argv[1], "r");
    char output_name[1000];
    sprintf(output_name, "%s.shortcut", argv[2]);
    output_file = fopen(output_name, "w");
    output_header(output_file);
    scopes = htable_init();
    current_scope = scope_create("main");
    if_count = 0;

    return input_file;
}

void
end_parse() {
    scope_output(current_scope, output_file);
    output_footer(output_file);
    fclose(output_file);
    fclose(input_file);
    release(scopes);
    release(current_scope);
}

void
increment_if_count() {
    if_count++;
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
        case magicVariable: strcpy((*stack).name.value, operand.value); break;
        case string:        strcpy((*stack).value.value, operand.value); break;
        case ask_number: break;
        case null: break;
    }
}

void
append_null_operand(Operand *stack) {
    Operand temp;
    *stack = temp;
    (*stack).type = null;
}

void
append_func_call(Operand *stack, char100 name, Operand parameter) {
    if (strcmp(name.value, "AskNumber") == 0) {
        Operand temp;
        *stack = temp;
        (*stack).type = ask_number;
        uuid_gen((*stack).uuid);
    } else if (strcmp(name.value, "ShowResult") == 0) {
        append_null_operand(stack);
        Action *action = action_create_show_result(parameter);
        scope_add_action(current_scope, action);

        release(action);
    } else {
        DEBUGPRINT("uninplemented function");
    }
}

void
operation_optimization(Operand *stack, char operator, Operand op1, Operand op2) {
    DEBUGPRINT("optimizing %s %c %s\n", op1.value.value, operator, op2.value.value);
    Operand new_stack;
    new_stack.type = number;

    uuid_gen(new_stack.uuid);

    double v1 = atof(op1.value.value);
    double v2 = atof(op2.value.value);
    double result;

    DEBUGPRINT("v1 = %lf\n", v1);
    DEBUGPRINT("v2 = %lf\n", v2);

    switch (operator) {
        case '+': result = v1 + v2; break;
        case '-': result = v1 - v2; break;
        case '*': result = v1 * v2; break;
        case '/': result = v1 / v2; break;
        case '^': result = pow(v1, v2); break;
    }
    DEBUGPRINT("ret = %lf\n", result);

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

    if (operator_is_commutative(operator) && strcmp(current_scope->last_uuid, op2.uuid) == 0) {
        DEBUGPRINT("Switching op's\n");
        Operand tmp = op1;
        op1 = op2;
        op2 = tmp;
    }

    place_operand(op1);

    Action *operation_action = action_create_math_operation(operator, op2);
    scope_add_action(current_scope, operation_action);

    Operand new_stack;
    new_stack.type = magicVariable;

    char name[] = "Calculation Result";
    strcpy(new_stack.name.value, name);
    strcpy(new_stack.uuid, operation_action->uuid);

    *stack = new_stack;
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

void
append_cond_control() {
    List *actions = action_create_cond_control(0, if_count);
    scope_add_actions(current_scope, actions);
    release(actions);
}

void
append_conditional(Comparison comp) {
    place_operand(comp.op1);

    Action *action = action_create_comp(comp);
    scope_add_action(current_scope, action);
    action->cond_should_close_control = 1;

    action->sub_scope->parent_name = current_scope->name;
    current_scope = action->sub_scope;
    action->cond_control_count = if_count;

    release(action);
}

void
append_else() {
    Comparison comp;
    comp.operator = CompOpEQ;

    Operand op1;
    op1.type = variable;

    uuid_gen(op1.uuid);
    sprintf(op1.name.value, "$splash_if_%d", if_count);

    Operand op2;
    op2.type = number;
    strcpy(op2.value.value, "0");

    comp.op1 = op1;
    comp.op2 = op2;

    place_operand(comp.op1);
    Action *action = action_create_comp(comp);
    scope_add_action(current_scope, action);
    action->cond_should_close_control = 0;

    action->sub_scope->parent_name = current_scope->name;
    current_scope = action->sub_scope;
    action->cond_control_count = if_count;

    release(action);
}

void
close_scope() {
    String *uuid = current_scope->name;
    current_scope = htable_retrieve(scopes, current_scope->parent_name->string, 0);
    scope_clear_last_uuid(current_scope);
}

void
append_comparison(Comparison *stack, CompOp operator, Operand op1, Operand op2) {
    Comparison comp;
    comp.operator = operator;
    comp.op1 = op1;
    comp.op2 = op2;

    *stack = comp;
}

void
place_set_variable(char100 var_name) {
    Action *action = action_create_set_variable(var_name);

    scope_add_action(current_scope, action);
    release(action);
}

void
place_operand(Operand op) {
    Action *action;
    switch (op.type) {
        case ask_number:
        case number: action = action_create_number(op); break;
        case variable: action = action_create_get_variable(op); break;
        case magicVariable: action = action_create_get_magic_variable(op); break;
        case string: action = action_create_text(op); break;
        case null: action = NULL; break;
    }

    if (action) {
        scope_add_action(current_scope, action);
        release(action);
    }
}
