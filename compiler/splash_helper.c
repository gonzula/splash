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
    helper_error = 0;
    error_message = NULL;
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
append_operand(Operand *stack, OpType type, String *operand) {
    (*stack).type = type;

    uuid_gen((*stack).uuid);

    switch (type) {
        case op_number:         (*stack).value = str_create(operand->string); break;
        case op_variable:       (*stack).name = str_create(operand->string); break;
        case op_magic_variable: (*stack).name = str_create(operand->string); break;
        case op_string:         (*stack).value = str_create(operand->string); break;
        case op_null: break;
    }
}

void
append_null_operand(Operand *stack) {
    (*stack).type = op_null;
}

int
append_func_call(Operand *stack, String *name, Operand parameter) {
    Action *action = NULL;
    if (strcmp(name->string, "AskNumber") == 0) {
        action = action_create_ask_input(parameter, "Number");

        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Ask for Input");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "AskText") == 0) {
        action = action_create_ask_input(parameter, "Text");

        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Ask for Input");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "ShowResult") == 0) {
        action = action_create_show_result(parameter);
        append_null_operand(stack);
    } else if (strcmp(name->string, "Floor") == 0) {
        place_operand(parameter, true);

        action = action_create_round_number("Always Round Down", "Right of Decimal");
        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Rounded Number");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "Ceil") == 0) {
        place_operand(parameter, true);

        action = action_create_round_number("Always Round Up", "Right of Decimal");
        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Rounded Number");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "Round") == 0) {
        place_operand(parameter, true);

        action = action_create_round_number("Normal", "Right of Decimal");
        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Rounded Number");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "GetName") == 0) {
        place_operand(parameter, true);

        action = action_create_get_item_name();
        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Name");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "GetType") == 0) {
        place_operand(parameter, true);

        action = action_create_get_item_type();
        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Type");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "ViewContentGraph") == 0) {
        place_operand(parameter, true);

        action = action_create_view_content_graph();

        append_null_operand(stack);
    } else if (strcmp(name->string, "Wait") == 0) {
        action = action_create_wait(parameter);
        append_null_operand(stack);
    } else if (strcmp(name->string, "Exit") == 0) {
        action = action_create_exit();
        append_null_operand(stack);
    } else if (strcmp(name->string, "WaitToReturn") == 0) {
        action = action_create_wait_to_return();
        append_null_operand(stack);
    } else if (strcmp(name->string, "GetBatteryLevel") == 0) {
        action = action_create_get_battery_level();

        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Battery Level");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "Date") == 0) {
        action = action_create_date(parameter);

        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Date");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "ExtractArchive") == 0) {
        place_operand(parameter, true);
        action = action_create_extract_archive();

        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Files");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else if (strcmp(name->string, "GetCurrentLocation") == 0) {
        action = action_create_get_current_location();

        (*stack).type = op_magic_variable;
        (*stack).name = str_create("Current Location");
        strcpy((*stack).uuid, action->uuid);
        *stack = (*stack);
    } else {
        DEBUGPRINT("uninplemented function");
        append_null_operand(stack);
        helper_error = 2;
        error_message = malloc(sizeof(char) * (name->len + 256));
        sprintf(error_message, "Unknown function \"%s\"\n", name->string);
        return 1;
    }

    if (action) {
        scope_add_action(current_scope, action);
        release(action);
    }
    return 0;
}

int
operation_optimization(Operand *stack, char operator, Operand op1, Operand op2) {
    DEBUGPRINT("optimizing %s %c %s\n", op1.value->string, operator, op2.value->string);
    Operand new_stack;
    new_stack.type = op_number;

    uuid_gen(new_stack.uuid);

    double v1 = atof(op1.value->string);
    double v2 = atof(op2.value->string);
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

    char buff[100];
    sprintf(buff, "%lf", result);
    new_stack.name = str_create(buff);
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

    place_operand(op1, true);

    Action *operation_action = action_create_math_operation(operator, op2);
    scope_add_action(current_scope, operation_action);

    Operand new_stack;
    new_stack.type = op_magic_variable;

    new_stack.name = str_create("Calculation Result");
    strcpy(new_stack.uuid, operation_action->uuid);

    *stack = new_stack;
    release(operation_action);
}

void
append_minus_op(Operand *stack, Operand op) {
    Operand temp;
    temp.type = op_number;
    uuid_gen(temp.uuid);
    temp.value = str_create("-1");

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
    place_operand(comp.op1, true);

    Action *action = action_create_comp(comp);
    scope_add_action(current_scope, action);
    action->cond_should_close_control = true;

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
    char buff[100];
    sprintf(buff, "$splash_if_%d", if_count);
    op1.name = str_create(buff);

    Operand op2;
    op2.type = op_number;
    op2.value = str_create("0");

    comp.op1 = op1;
    comp.op2 = op2;

    place_operand(comp.op1, true);
    Action *action = action_create_comp(comp);
    scope_add_action(current_scope, action);
    action->cond_should_close_control = false;

    action->sub_scope->parent_name = current_scope->name;
    current_scope = action->sub_scope;
    action->cond_control_count = if_count;

    release(action);
}

void
close_scope() {
    current_scope = htable_retrieve(scopes, current_scope->parent_name->string, false);
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
place_set_variable(String *var_name) {
    Action *action = action_create_set_variable(var_name);

    scope_add_action(current_scope, action);
    release(action);
}

void
place_operand(Operand op, bool force_null) {
    Action *action;
    switch (op.type) {
        case op_number: action = action_create_number(op); break;
        case op_variable: action = action_create_get_variable(op); break;
        case op_magic_variable: action = action_create_get_magic_variable(op); break;
        case op_string: action = action_create_text(op); break;
        case op_null:
            if (force_null) {
                action = action_create_nothing();
            } else {
                action = NULL;
            }
            break;

    }

    if (action) {
        scope_add_action(current_scope, action);
        release(action);
    }
}

void
operand_release(Operand op) {
    if (op.name) {
        release(op.name);
    }
    if (op.value) {
        release(op.value);
    }
}
