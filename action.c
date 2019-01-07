#include "action.h"
#include "structures/structures.h"
#include "utils.h"
#include "scope.h"
#include <string.h>
#include <stdio.h>

void _action_release(void *obj);

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


    Serializable *s = serializable_init();
    s->type = st_float;
    s->f = atof(op1.value.value);
    htable_set(action->parameters, "WFNumberActionNumber", s);
    release(s);

    return action;
}

Action *
action_create_get_variable(Operand op1) {
    Action *action = action_create(WF_get_variable);

    HashTable *variable = htable_init();
    Serializable *s1 = serializable_create(variable, st_ht);
    htable_set(action->parameters, "WFVariable", s1);

    HashTable *value = htable_init();
    Serializable *s2 = serializable_create(value, st_ht);
    htable_set(variable, "Value", s2);

    String *var_type = str_create("Variable");
    Serializable *s3 = serializable_create(var_type, st_str);
    htable_set(value, "Type", s3);

    String *var_name = str_create(op1.name.value);
    Serializable *s4 = serializable_create(var_name, st_str);
    htable_set(value, "VariableName", s4);

    String *token_attachment = str_create("WFTextTokenAttachment");
    Serializable *s5 = serializable_create(token_attachment, st_str);
    htable_set(action->parameters, "WFSerializationType", s5);

    release(token_attachment);
    release(variable);
    release(value);
    release(var_type);

    release(s1);
    release(s2);
    release(s3);
    release(s4);
    release(s5);

    return action;
}

Action *
action_create_get_magic_variable(Operand op1) {
    Action *action = action_create(WF_get_variable);

    HashTable *variable = htable_init();
    Serializable *s1 = serializable_create(variable, st_ht);
    htable_set(action->parameters, "WFVariable", s1);

    HashTable *value = htable_init();
    Serializable *s2 = serializable_create(value, st_ht);
    htable_set(variable, "Value", s2);

    String *var_type = str_create("ActionOutput");
    Serializable *s3 = serializable_create(var_type, st_str);
    htable_set(value, "Type", s3);

    String *var_name = str_create(op1.name.value);
    Serializable *s4 = serializable_create(var_name, st_str);
    htable_set(value, "OutputName", s4);

    String *uuid = str_create(op1.uuid);
    Serializable *s5 = serializable_create(uuid, st_str);
    htable_set(value, "OutputUUID", s5);


    String *token_attachment = str_create("WFTextTokenAttachment");
    Serializable *s6 = serializable_create(token_attachment, st_str);
    htable_set(action->parameters, "WFSerializationType", s6);

    release(uuid);
    release(token_attachment);
    release(variable);
    release(value);
    release(var_type);

    release(s1);
    release(s2);
    release(s3);
    release(s4);
    release(s5);
    release(s6);
    return action;
}

void
action_complete_math_operand(Action *action, const char *key, Operand op2) {
    switch (op2.type) {
        case number: {
                         Serializable *s = serializable_init();
                         s->type = st_float;
                         s->f = atof(op2.value.value);
                         htable_set(action->parameters, key, s);
                         release(s);
                     }
        case magicVariable: {
                                HashTable *h1 = htable_init();
                                Serializable *s1 = serializable_create(h1, st_ht);
                                htable_set(action->parameters, key, s1);

                                HashTable *h2 = htable_init();
                                Serializable *s2 = serializable_create(h2, st_ht);
                                htable_set(h1, "Value", s2);

                                String *output_name = str_create(op2.name.value);
                                Serializable *s3 = serializable_create(output_name, st_str);
                                htable_set(h2, "OutputName", s3);

                                String *uuid = str_create(op2.uuid);
                                Serializable *s4 = serializable_create(uuid, st_str);
                                htable_set(h2, "OutputUUID", s4);

                                String *action_output = str_create("ActionOutput");
                                Serializable *s5 = serializable_create(action_output, st_str);
                                htable_set(h2, "Type", s5);

                                String *token_attachment = str_create("WFTextTokenAttachment");
                                Serializable *s6 = serializable_create(token_attachment, st_str);
                                htable_set(h1, "WFSerializationType", s6);


                                release(h1);
                                release(s1);
                                release(h2);
                                release(s2);
                                release(output_name);
                                release(s3);
                                release(uuid);
                                release(s4);
                                release(action_output);
                                release(s5);
                                release(token_attachment);
                                release(s6);
                            }
        case variable: {
                           HashTable *h1 = htable_init();
                           Serializable *s1 = serializable_create(h1, st_ht);
                           htable_set(action->parameters, key, s1);

                           HashTable *h2 = htable_init();
                           Serializable *s2 = serializable_create(h2, st_ht);
                           htable_set(h1, "Value", s2);

                           String *variable = str_create("Variable");
                           Serializable *s3 = serializable_create(variable, st_str);
                           htable_set(h2, "Type", s3);

                           String *variable_name = str_create(op2.name.value);
                           Serializable *s4 = serializable_create(variable_name, st_str);
                           htable_set(h2, "VariableName", s4);

                           String *token_attachment = str_create("WFTextTokenAttachment");
                           Serializable *s5 = serializable_create(token_attachment, st_str);
                           htable_set(h1, "WFSerializationType", s5);

                           release(h1);
                           release(s1);
                           release(h2);
                           release(s2);
                           release(variable);
                           release(s3);
                           release(variable_name);
                           release(s4);
                           release(token_attachment);
                           release(s5);
                       }
    }
}

void
action_complete_math_simple_operation(Action *action, char operator, Operand op2) {
    String *WF_operation;
    switch (operator) {
        case '+': WF_operation = str_create("+"); break;
        case '-': WF_operation = str_create("-"); break;
        case '*': WF_operation = str_create("×"); break;
        case '/': WF_operation = str_create("÷"); break;
    }

    Serializable *s = serializable_create(WF_operation, st_str);
    htable_set(action->parameters, "WFMathOperation", s);
    release(WF_operation);
    release(s);

    action_complete_math_operand(action, "WFMathOperand", op2);
}

void
action_complete_math_scientific_operation(Action *action, char operator, Operand op2) {
    if (operator == '^') {
        String *ellipsis = str_create("…");
        Serializable *s = serializable_create(ellipsis, st_str);

        htable_set(action->parameters, "WFMathOperation", s);
        release(ellipsis);
        release(s);

        String *WF_operation;
        if (op2.type == number && strcmp("2", op2.value.value) == 0) {
            WF_operation = str_create("x^2");
        } else if (op2.type == number && strcmp("3", op2.value.value) == 0) {
            WF_operation = str_create("x^3");
        } else {
            WF_operation = str_create("x^y");
            action_complete_math_operand(action, "WFScientificMathOperand", op2);
        }

        s = serializable_create(WF_operation, st_str);
        htable_set(action->parameters, "WFScientificMathOperation", s);
        release(WF_operation);
        release(s);
    }
}

Action *
action_create_math_operation(char operator, Operand op2) {
    Action *action = action_create(WF_math);

    String *uuid = str_create(action->uuid);
    Serializable *s = serializable_create(uuid, st_str);
    htable_set(action->parameters, "UUID", s);

    switch (operator) {
        case '+':
        case '-':
        case '*':
        case '/':
            action_complete_math_simple_operation(action, operator, op2);
            break;
        case '^':
            action_complete_math_scientific_operation(action, operator, op2);
            break;
    }

    release(uuid);
    release(s);

    return action;
}

}

void
action_output(FILE *output, Action *action) {

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
