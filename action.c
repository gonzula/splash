#include "action.h"
#include "structures/structures.h"
#include "utils.h"
#include "scope.h"
#include "output.h"
#include "interpolated.h"
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

    return action;
}

Action *
action_create_number(Operand op) {
    Action *action = action_create(WF_number);
    strcpy(action->uuid, op.uuid);

    if (op.type == ask_number) {
        HashTable *h1 = htable_init();
        Serializable *s1 = serializable_create(h1, st_ht);

        HashTable *h2 = htable_init();
        Serializable *s2 = serializable_create(h2, st_ht);
        htable_set(h1, "Value", s2);

        String *ask = str_create("Ask");
        Serializable *s3 = serializable_create(ask, st_str);
        htable_set(h2, "Type", s3);

        String *attachment = str_create("WFTextTokenAttachment");
        Serializable *s4 = serializable_create(attachment, st_str);
        htable_set(h1, "WFSerializationType", s4);

        htable_set(action->parameters, "WFNumberActionNumber", s1);

        release(attachment);
        release(s4);
        release(ask);
        release(s3);
        release(s1);
        release(h1);
        release(s2);
        release(h2);
    } else if (op.type == number) {
        Serializable *s = serializable_init();
        s->type = st_float;
        s->f = atof(op.value.value);
        htable_set(action->parameters, "WFNumberActionNumber", s);
        release(s);
    }

    return action;
}

Action *
action_create_text(Operand op) {
    Action *action = action_create(WF_text);
    strcpy(action->uuid, op.uuid);

    String *uuid = str_create(action->uuid);
    Serializable *s = serializable_create(uuid, st_str);
    htable_set(action->parameters, "UUID", s);

    Interpolated *interpolated = interpolated_create(op.value);

    HashTable *action_text = htable_init();
    Serializable *s1 = serializable_create(action_text, st_ht);
    htable_set(action->parameters, "WFTextActionText", s1);

    HashTable *value = htable_init();
    Serializable *s2 = serializable_create(value, st_ht);
    htable_set(action_text, "Value", s2);

    HashTable *attachments = htable_init();
    Serializable *s3 = serializable_create(attachments, st_ht);
    htable_set(value, "attachmentsByRange", s3);

    LIST_LOOP(interpolated->tokens) {
        StringToken *token = (StringToken *)node->object;

        HashTable *dict = htable_init();
        Serializable *s1 = serializable_create(dict, st_ht);

        char range[100];
        sprintf(range, "{%d, 1}", token->position);
        htable_set(attachments, range, s1);

        String *type = str_create("Variable");
        Serializable *s2 = serializable_create(type, st_str);
        htable_set(dict, "Type", s2);

        Serializable * s3 = serializable_create(token->name, st_str);
        htable_set(dict, "VariableName", s3);

        release(s1);
        release(dict);
        release(type);
        release(s2);
        release(s3);
    }

    Serializable *s4 = serializable_create(interpolated->str, st_str);
    htable_set(value, "string", s4);

    String *serialization_type = str_create("WFTextTokenString");
    Serializable *s5 = serializable_create(serialization_type, st_str);
    htable_set(action_text, "WFSerializationType", s5);

    release(uuid);
    release(s);
    release(attachments);
    release(s3);
    release(value);
    release(s2);
    release(action_text);
    release(s1);
    release(s4);
    release(serialization_type);
    release(s5);
    return action;
}

Action *
action_create_get_variable(Operand op) {
    Action *action = action_create(WF_get_variable);
    strcpy(action->uuid, op.uuid);

    HashTable *variable = htable_init();
    Serializable *s1 = serializable_create(variable, st_ht);
    htable_set(action->parameters, "WFVariable", s1);

    HashTable *value = htable_init();
    Serializable *s2 = serializable_create(value, st_ht);
    htable_set(variable, "Value", s2);

    String *var_type = str_create("Variable");
    Serializable *s3 = serializable_create(var_type, st_str);
    htable_set(value, "Type", s3);

    String *var_name = str_create(op.name.value);
    Serializable *s4 = serializable_create(var_name, st_str);
    htable_set(value, "VariableName", s4);

    String *token_attachment = str_create("WFTextTokenAttachment");
    Serializable *s5 = serializable_create(token_attachment, st_str);
    htable_set(variable, "WFSerializationType", s5);

    release(token_attachment);
    release(variable);
    release(value);
    release(var_type);
    release(var_name);

    release(s1);
    release(s2);
    release(s3);
    release(s4);
    release(s5);

    return action;
}

Action *
action_create_get_magic_variable(Operand op) {
    Action *action = action_create(WF_get_variable);
    strcpy(action->uuid, op.uuid);

    HashTable *variable = htable_init();
    Serializable *s1 = serializable_create(variable, st_ht);
    htable_set(action->parameters, "WFVariable", s1);

    HashTable *value = htable_init();
    Serializable *s2 = serializable_create(value, st_ht);
    htable_set(variable, "Value", s2);

    String *var_type = str_create("ActionOutput");
    Serializable *s3 = serializable_create(var_type, st_str);
    htable_set(value, "Type", s3);

    String *var_name = str_create(op.name.value);
    Serializable *s4 = serializable_create(var_name, st_str);
    htable_set(value, "OutputName", s4);

    String *uuid = str_create(op.uuid);
    Serializable *s5 = serializable_create(uuid, st_str);
    htable_set(value, "OutputUUID", s5);


    String *token_attachment = str_create("WFTextTokenAttachment");
    Serializable *s6 = serializable_create(token_attachment, st_str);
    htable_set(variable, "WFSerializationType", s6);

    release(uuid);
    release(token_attachment);
    release(variable);
    release(value);
    release(var_type);
    release(var_name);

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
        case string:
            fprintf(stderr, "Invalid string inside math operation\n");
            break;
        case number: {
                         Serializable *s = serializable_init();
                         s->type = st_float;
                         s->f = atof(op2.value.value);
                         htable_set(action->parameters, key, s);
                         release(s);
                     }
                     break;
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
                            break;
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
                       break;
        case ask_number: {
                           HashTable *h1 = htable_init();
                           Serializable *s1 = serializable_create(h1, st_ht);
                           htable_set(action->parameters, key, s1);

                           HashTable *h2 = htable_init();
                           Serializable *s2 = serializable_create(h2, st_ht);
                           htable_set(h1, "Value", s2);

                           String *ask = str_create("Ask");
                           Serializable *s3 = serializable_create(ask, st_str);
                           htable_set(h2, "Type", s3);

                           String *token_attachment = str_create("WFTextTokenAttachment");
                           Serializable *s5 = serializable_create(token_attachment, st_str);
                           htable_set(h1, "WFSerializationType", s5);

                           release(h1);
                           release(s1);
                           release(h2);
                           release(s2);
                           release(ask);
                           release(s3);
                           release(token_attachment);
                           release(s5);
                         }
                         break;
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
    String *operator_string = str_create("…");
    Serializable *s = serializable_create(operator_string, st_str);

    htable_set(action->parameters, "WFMathOperation", s);
    release(operator_string);
    release(s);

    String *WF_operation;
    if (operator == '^') {
        if (op2.type == number && strcmp("2", op2.value.value) == 0) {
            WF_operation = str_create("x^2");
        } else if (op2.type == number && strcmp("3", op2.value.value) == 0) {
            WF_operation = str_create("x^3");
        } else {
            WF_operation = str_create("x^y");
            action_complete_math_operand(action, "WFScientificMathOperand", op2);
        }
    } else if (operator == '%') {
        WF_operation = str_create("Modulus");
        action_complete_math_operand(action, "WFScientificMathOperand", op2);
    }

    s = serializable_create(WF_operation, st_str);
    htable_set(action->parameters, "WFScientificMathOperation", s);
    release(WF_operation);
    release(s);
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
        case '%':
            action_complete_math_scientific_operation(action, operator, op2);
            break;
    }

    release(uuid);
    release(s);

    return action;
}

Action *
action_create_set_variable(char100 name) {
    Action *action = action_create(WF_set_variable);

    String *variable_name = str_create(name.value);
    Serializable *s = serializable_create(variable_name, st_str);

    htable_set(action->parameters, "WFVariableName", s);


    release(s);
    release(variable_name);

    return action;
}


void
action_complete_comp_operand(Action *action, CompOp operator, Operand operand) {

    switch (operator) {
        case CompOpEQ: {
                           String *str = str_create(operand.value.value);
                           Serializable *s = serializable_create(str, st_str);
                           htable_set(action->parameters,  "WFConditionalActionString", s);
                           release(s);
                           release(str);
                       }
                       break;
        case CompOpLT:
        case CompOpGT: action_complete_math_operand(action, "WFNumberValue", operand); break;
    }
}

Action *
action_create_comp(Comparison comp) {
    Action *action = action_create(WF_conditional);
    action->sub_scope = scope_create(action->uuid);

    String *uuid = str_create(action->uuid);
    Serializable *s = serializable_create(uuid, st_str);
    htable_set(action->parameters, "GroupingIdentifier", s);

    String *operator;
    switch (comp.operator) {
        case CompOpEQ: operator = str_create("Equals"); break;
        case CompOpLT: operator = str_create("Is Less Than"); break;
        case CompOpGT: operator = str_create("Is Greater Than"); break;
    }
    Serializable *s1 = serializable_create(operator, st_str);
    htable_set(action->parameters, "WFCondition", s1);

    Serializable *s2 = serializable_init();
    s2->type = st_int;
    s2->i = 0;
    htable_set(action->parameters, "WFControlFlowMode", s2);

    action_complete_comp_operand(action, comp.operator, comp.op2);

    release(s);
    release(s1);
    release(s2);
    release(uuid);
    release(operator);

    return action;
}

List *
action_create_cond_control(int value, int control_count) {
    List * actions = list_init();

    Operand op;
    op.type = number;
    sprintf(op.value.value, "%d", value);
    uuid_gen(op.uuid);

    Action *number_action = action_create_number(op);
    list_append(actions, number_action);
    release(number_action);

    char100 var_name;
    sprintf(var_name.value, "$splash_if_%d", control_count);

    Action *set_var_action = action_create_set_variable(var_name);
    list_append(actions, set_var_action);
    release(set_var_action);

    return actions;
}

List *
action_create_close_cond(Action *action) {
    List *actions;
    if (action->cond_should_close_control) {
        actions = action_create_cond_control(1, action->cond_control_count);
    } else {
        actions = list_init();
    }

    Action *close_action = action_create(WF_conditional);
    list_append(actions, close_action);
    strcpy(close_action->uuid, action->uuid);

    String *uuid = str_create(close_action->uuid);
    Serializable *s = serializable_create(uuid, st_str);
    htable_set(close_action->parameters, "GroupingIdentifier", s);

    Serializable *s1 = serializable_init();
    s1->type = st_int;
    s1->i = 2;
    htable_set(close_action->parameters, "WFControlFlowMode", s1);

    release(s);
    release(s1);
    release(uuid);
    release(close_action);

    return actions;
}

List *
action_create_close_scope(Action *action) {
    switch (action->id) {
        case WF_conditional: return action_create_close_cond(action);
        case WF_get_variable:
        case WF_math:
        case WF_number:
        case WF_text:
        case WF_set_variable: return list_init();
    }
}

void
action_output(Action *action, FILE *output) {
    fprintf(output, "<dict>");
    fprintf(output, "<key>WFWorkflowActionIdentifier</key>");

    fprintf(output, "<string>");
    fprintf(output, "is.workflow.actions.");

    switch (action->id) {
        case WF_conditional: fprintf(output, "conditional"); break;
        case WF_get_variable: fprintf(output, "getvariable"); break;
        case WF_math: fprintf(output, "math"); break;
        case WF_number: fprintf(output, "number"); break;
        case WF_text: fprintf(output, "gettext"); break;
        case WF_set_variable: fprintf(output, "setvariable"); break;
    }

    fprintf(output, "</string>");

    fprintf(output, "<key>WFWorkflowActionParameters</key>");
    output_htable(output, action->parameters);

    fprintf(output, "</dict>");

    if (action->sub_scope) {
        scope_output(action->sub_scope, output);

        List *close_actions = action_create_close_scope(action);
        LIST_LOOP(close_actions) {
            Action *sub_action = (Action *)node->object;
            action_output(sub_action, output);
        }
        release(close_actions);
    }
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
