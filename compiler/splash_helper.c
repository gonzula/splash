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
init_parse(const char *in_file_name, const char *out_file_name) {
    input_file = fopen(in_file_name, "r");
    output_file = fopen(out_file_name, "w");
    output_header(output_file);
    scopes = htable_init();
    current_scope = scope_create();
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
    (*stack).type = type;

    uuid_gen((*stack).uuid);

    switch (type) {
        case op_number:         strcpy((*stack).value.value, operand.value); break;
        case op_variable:       strcpy((*stack).name.value, operand.value); break;
        case op_magic_variable: strcpy((*stack).name.value, operand.value); break;
        case op_string:         strcpy((*stack).value.value, operand.value); break;
        case op_null: break;
    }
}

void
append_null_operand(Operand *stack) {
    (*stack).type = op_null;
}

void
append_func_call(Operand *stack, char100 name, Operand parameter) {
    Action *action = NULL;
    if (strcmp(name.value, "AskNumber") == 0) {
        action = action_create_ask_input(parameter, "Number");

        (*stack).type = op_magic_variable;
        strcpy((*stack).name.value, "Ask for Input");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name.value, "AskText") == 0) {
        action = action_create_ask_input(parameter, "Text");

        (*stack).type = op_magic_variable;
        strcpy((*stack).name.value, "Ask for Input");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name.value, "ShowResult") == 0) {
        append_null_operand(stack);
        action = action_create_show_result(parameter);
    } else if (strcmp(name.value, "Floor") == 0) {
        place_operand(parameter);

        action = action_create_round_number("Always Round Down", "Right of Decimal");
        (*stack).type = op_magic_variable;
        strcpy((*stack).name.value, "Rounded Number");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name.value, "Ceil") == 0) {
        place_operand(parameter);

        action = action_create_round_number("Always Round Up", "Right of Decimal");
        (*stack).type = op_magic_variable;
        strcpy((*stack).name.value, "Rounded Number");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name.value, "Round") == 0) {
        place_operand(parameter);

        action = action_create_round_number("Normal", "Right of Decimal");
        (*stack).type = op_magic_variable;
        strcpy((*stack).name.value, "Rounded Number");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name.value, "GetName") == 0) {
        place_operand(parameter);

        action = action_create_get_item_name();
        (*stack).type = op_magic_variable;
        strcpy((*stack).name.value, "Name");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name.value, "GetType") == 0) {
        place_operand(parameter);

        action = action_create_get_item_type();
        (*stack).type = op_magic_variable;
        strcpy((*stack).name.value, "Type");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name.value, "ViewContentGraph") == 0) {
        place_operand(parameter);

        action = action_create_view_content_graph();

        append_null_operand(stack);
    } else if (strcmp(name.value, "Wait") == 0) {
        action = action_create_wait(parameter);
        append_null_operand(stack);
    } else if (strcmp(name.value, "Exit") == 0) {
        action = action_create_exit();
        append_null_operand(stack);
    } else {
        DEBUGPRINT("uninplemented function");
        append_null_operand(stack);
    }

    if (action) {
        scope_add_action(current_scope, action);
        release(action);
    }
}

int
operation_optimization(Operand *stack, char operator, Operand op1, Operand op2) {
    DEBUGPRINT("optimizing %s %c %s\n", op1.value.value, operator, op2.value.value);
    Operand new_stack;
    new_stack.type = op_number;

    uuid_gen(new_stack.uuid);

    double v1 = atof(op1.value.value);
    double v2 = atof(op2.value.value);
    double result = 0;

    DEBUGPRINT("v1 = %lf\n", v1);
    DEBUGPRINT("v2 = %lf\n", v2);

    switch (operator) {
        case '+': result = v1 + v2; break;
        case '-': result = v1 - v2; break;
        case '*': result = v1 * v2; break;
        case '/': result = v1 / v2; break;
        case '^': result = pow(v1, v2); break;
        default: return 1;
    }
    DEBUGPRINT("ret = %lf\n", result);

    sprintf(new_stack.value.value, "%lf", result);
    (*stack) = new_stack;

    return 0;
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
    if (op1.type == op_number && op2.type == op_number) { // can optimize
        if (!operation_optimization(stack, operator, op1, op2)) {
            return;
        }
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
    new_stack.type = op_magic_variable;

    char name[] = "Calculation Result";
    strcpy(new_stack.name.value, name);
    strcpy(new_stack.uuid, operation_action->uuid);

    *stack = new_stack;
    release(operation_action);
}

void
append_minus_op(Operand *stack, Operand op) {
    Operand temp;
    temp.type = op_number;
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
    comp.operator = comp_op_eq;

    Operand op1;
    op1.type = op_variable;

    uuid_gen(op1.uuid);
    sprintf(op1.name.value, "$splash_if_%d", if_count);

    Operand op2;
    op2.type = op_number;
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
        case op_number: action = action_create_number(op); break;
        case op_variable: action = action_create_get_variable(op); break;
        case op_magic_variable: action = action_create_get_magic_variable(op); break;
        case op_string: action = action_create_text(op); break;
        case op_null: action = action_create_nothing(); break;
    }

    if (action) {
        scope_add_action(current_scope, action);
        release(action);
    }
}
