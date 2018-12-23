#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "splash_helper.h"
#include "utils.h"
#include "output.h"

void _action_release(void *obj);
void _scope_release(void *obj);

void
init_parse() {
    scopes = htable_init();
    current_scope = scope_create("main");
}

void
end_parse() {
    release(scopes);
    release(current_scope);
}

Scope *
scope_create(char *name) {
    Scope *scope = (Scope *)alloc(sizeof(Scope), _scope_release);
    scope->actions = list_init();
    scope->parent_name = NULL;

    htable_set(scopes, name, scope);

    return scope;
}

Scope *
scope_add_action(Scope *scope, Action *action) {
    list_append(scope->actions, action);
}

Action *
action_init() {
    Action *action = (Action *)alloc(sizeof(Action), _action_release);
    action->id = 0;
    action->parameters = NULL;
    action->uuid[0] = 0;
    action->sub_scope = NULL;

    return action;
}

Action *
action_create(ActionID id) {
    Action *action = (Action *)alloc(sizeof(Action), _action_release);
    action->id = id;
    action->parameters = htable_init();
    uuid_gen(action->uuid);

    action->sub_scope = NULL;
    switch (id) {
        case WF_conditional:
            action->sub_scope = scope_create(action->uuid);
            break;
        case WF_get_variable:
        case WF_math:
        case WF_number:
        case WF_set_variable: break;
    }

    return action;
}

Action *
action_create_number(Operand op1) {
    Action *action = action_create(WF_number);

    String *value = str_create(op1.value.value);
    htable_set(action->parameters, "WFNumberActionNumber", value);
    release(value);

    return action;
}

Action *
action_create_get_variable(Operand op1) {
    Action *action = action_create(WF_get_variable);

    HashTable *variable = htable_init();
    HashTable *value = htable_init();
    String *var_type = str_create("Variable");
    String *var_name = str_create(op1.name.value);
    htable_set(value, "Type", var_type);
    htable_set(value, "VariableName", var_name);

    htable_set(action->parameters, "WFVariable", variable);

    String *token_attachment = str_create("WFTextTokenAttachment");
    htable_set(action->parameters, "WFVariable", variable);
    htable_set(action->parameters, "WFSerializationType", token_attachment);

    release(token_attachment);
    release(variable);
    release(value);
    release(var_type);


    return action;
}

Action *
action_create_get_magic_variable(Operand op1) {
    Action *action = action_create(WF_get_variable);

    HashTable *variable = htable_init();
    HashTable *value = htable_init();
    String *var_type = str_create("ActionOutput");
    String *var_name = str_create(op1.name.value);
    htable_set(value, "Type", var_type);
    htable_set(value, "OutputName", var_name);
    String *uuid = str_create(op1.uuid);
    htable_set(value, "OutputUUID", uuid);

    htable_set(action->parameters, "WFVariable", variable);

    String *token_attachment = str_create("WFTextTokenAttachment");
    htable_set(action->parameters, "WFVariable", variable);
    htable_set(action->parameters, "WFSerializationType", token_attachment);

    release(uuid);
    release(token_attachment);
    release(variable);
    release(value);
    release(var_type);


    return action;
}

void
_scope_release(void *obj) {
    Scope *scope = (Scope *)obj;

    if (scope->parent_name) {
        release(scope->parent_name);
    }
    release(scope->actions);
}

void
_action_release(void *obj) {
    Action *action = (Action *)obj;
    if (action->sub_scope) {
        release(action->sub_scope);
    }
    if (action->parameters) {
        release(action->parameters);
    }
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
    uuid_gen(temp.uuid);
    char minus_one[] = "-1";
    strcpy(temp.value.value, minus_one);

    append_operation(stack, '*', temp, op);
}
