#include "action.h"
#include "structures/structures.h"
#include "utils.h"
#include "scope.h"
#include "output.h"
#include "interpolated.h"
#include <string.h>
#include <stdio.h>

void _action_release(void *obj);

void
action_set_uuid(Action *action, char uuid[37]) {
    strcpy(action->uuid, uuid);

    String *uuid_string = str_create(action->uuid);
    Serializable *s = serializable_create_str(uuid_string);
    htable_set(action->parameters, "UUID", s);

    release(uuid_string);
    release(s);
}

Action *
action_create(ActionID id) {
    Action *action = (Action *)alloc(sizeof(Action), _action_release);
    action->id = id;
    action->parameters = htable_init();
    char uuid[37];
    uuid_gen(uuid);
    action_set_uuid(action, uuid);

    action->sub_scope = NULL;

    return action;
}

Action *
action_create_number(Operand op) {
    Action *action = action_create(WF_number);
    action_set_uuid(action, op.uuid);

    Serializable *s = serializable_create_float(atof(op.value.value));
    htable_set(action->parameters, "WFNumberActionNumber", s);
    release(s);

    return action;
}

Action *
action_create_text(Operand op) {
    Action *action = action_create(WF_text);
    action_set_uuid(action, op.uuid);

    Interpolated *interpolated = interpolated_create(op.value);

    Serializable *s1 = interpolated_parameters(interpolated);
    htable_set(action->parameters, "WFTextActionText", s1);

    release(interpolated);
    release(s1);
    return action;
}

Action *
action_create_nothing(void) {
    Action *action = action_create(WF_nothing);
    return action;
}

Action *
action_create_get_variable(Operand op) {
    Action *action = action_create(WF_get_variable);
    action_set_uuid(action, op.uuid);

    HashTable *variable = htable_init();
    Serializable *s1 = serializable_create_ht(variable);
    htable_set(action->parameters, "WFVariable", s1);

    HashTable *value = htable_init();
    Serializable *s2 = serializable_create_ht(value);
    htable_set(variable, "Value", s2);

    String *var_type = str_create("Variable");
    Serializable *s3 = serializable_create_str(var_type);
    htable_set(value, "Type", s3);

    String *var_name = str_create(op.name.value);
    Serializable *s4 = serializable_create_str(var_name);
    htable_set(value, "VariableName", s4);

    String *token_attachment = str_create("WFTextTokenAttachment");
    Serializable *s5 = serializable_create_str(token_attachment);
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
    action_set_uuid(action, op.uuid);

    HashTable *variable = htable_init();
    Serializable *s1 = serializable_create_ht(variable);
    htable_set(action->parameters, "WFVariable", s1);

    HashTable *value = htable_init();
    Serializable *s2 = serializable_create_ht(value);
    htable_set(variable, "Value", s2);

    String *var_type = str_create("ActionOutput");
    Serializable *s3 = serializable_create_str(var_type);
    htable_set(value, "Type", s3);

    String *var_name = str_create(op.name.value);
    Serializable *s4 = serializable_create_str(var_name);
    htable_set(value, "OutputName", s4);

    String *uuid = str_create(op.uuid);
    Serializable *s5 = serializable_create_str(uuid);
    htable_set(value, "OutputUUID", s5);


    String *token_attachment = str_create("WFTextTokenAttachment");
    Serializable *s6 = serializable_create_str(token_attachment);
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
        case op_null: fprintf(stderr, "Invalid null inside math operation\n"); break;
        case op_string: fprintf(stderr, "Invalid string inside math operation\n"); break;
        case op_number: {
                         Serializable *s = serializable_create_float(atof(op2.value.value));
                         htable_set(action->parameters, key, s);
                         release(s);
                     }
                     break;
        case op_magic_variable: {
                                HashTable *h1 = htable_init();
                                Serializable *s1 = serializable_create_ht(h1);
                                htable_set(action->parameters, key, s1);

                                HashTable *h2 = htable_init();
                                Serializable *s2 = serializable_create_ht(h2);
                                htable_set(h1, "Value", s2);

                                String *output_name = str_create(op2.name.value);
                                Serializable *s3 = serializable_create_str(output_name);
                                htable_set(h2, "OutputName", s3);

                                String *uuid = str_create(op2.uuid);
                                Serializable *s4 = serializable_create_str(uuid);
                                htable_set(h2, "OutputUUID", s4);

                                String *action_output = str_create("ActionOutput");
                                Serializable *s5 = serializable_create_str(action_output);
                                htable_set(h2, "Type", s5);

                                String *token_attachment = str_create("WFTextTokenAttachment");
                                Serializable *s6 = serializable_create_str(token_attachment);
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
        case op_variable: {
                           HashTable *h1 = htable_init();
                           Serializable *s1 = serializable_create_ht(h1);
                           htable_set(action->parameters, key, s1);

                           HashTable *h2 = htable_init();
                           Serializable *s2 = serializable_create_ht(h2);
                           htable_set(h1, "Value", s2);

                           String *variable = str_create("Variable");
                           Serializable *s3 = serializable_create_str(variable);
                           htable_set(h2, "Type", s3);

                           String *variable_name = str_create(op2.name.value);
                           Serializable *s4 = serializable_create_str(variable_name);
                           htable_set(h2, "VariableName", s4);

                           String *token_attachment = str_create("WFTextTokenAttachment");
                           Serializable *s5 = serializable_create_str(token_attachment);
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
    }
}

void
action_complete_math_simple_operation(Action *action, char operator, Operand op2) {
    String *WF_operation = NULL;
    switch (operator) {
        case '+': WF_operation = str_create("+"); break;
        case '-': WF_operation = str_create("-"); break;
        case '*': WF_operation = str_create("×"); break;
        case '/': WF_operation = str_create("÷"); break;
    }

    Serializable *s = serializable_create_str(WF_operation);
    htable_set(action->parameters, "WFMathOperation", s);
    release(WF_operation);
    release(s);

    action_complete_math_operand(action, "WFMathOperand", op2);
}

void
action_complete_math_scientific_operation(Action *action, char operator, Operand op2) {
    String *operator_string = str_create("…");
    Serializable *s = serializable_create_str(operator_string);

    htable_set(action->parameters, "WFMathOperation", s);
    release(operator_string);
    release(s);

    String *WF_operation = NULL;
    if (operator == '^') {
        if (op2.type == op_number && strcmp("2", op2.value.value) == 0) {
            WF_operation = str_create("x^2");
        } else if (op2.type == op_number && strcmp("3", op2.value.value) == 0) {
            WF_operation = str_create("x^3");
        } else {
            WF_operation = str_create("x^y");
            action_complete_math_operand(action, "WFScientificMathOperand", op2);
        }
    } else if (operator == '%') {
        WF_operation = str_create("Modulus");
        action_complete_math_operand(action, "WFScientificMathOperand", op2);
    }

    s = serializable_create_str(WF_operation);
    htable_set(action->parameters, "WFScientificMathOperation", s);
    release(WF_operation);
    release(s);
}

Action *
action_create_math_operation(char operator, Operand op2) {
    Action *action = action_create(WF_math);

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

    return action;
}

Action *
action_create_set_variable(char100 name) {
    Action *action = action_create(WF_set_variable);

    String *variable_name = str_create(name.value);
    Serializable *s = serializable_create_str(variable_name);

    htable_set(action->parameters, "WFVariableName", s);


    release(s);
    release(variable_name);

    return action;
}


void
action_complete_comp_operand(Action *action, CompOp operator, Operand operand) {
    switch (operator) {
        case comp_op_eq: {
                           String *str = str_create(operand.value.value);
                           Serializable *s = serializable_create_str(str);
                           htable_set(action->parameters,  "WFConditionalActionString", s);
                           release(s);
                           release(str);
                       }
                       break;
        case comp_op_lt:
        case comp_op_gt: action_complete_math_operand(action, "WFNumberValue", operand); break;
    }
}

Action *
action_create_comp(Comparison comp) {
    Action *action = action_create(WF_conditional);
    action->sub_scope = scope_create();

    String *uuid = str_create(action->uuid);
    Serializable *s = serializable_create_str(uuid);
    htable_set(action->parameters, "GroupingIdentifier", s);

    String *operator;
    switch (comp.operator) {
        case comp_op_eq: operator = str_create("Equals"); break;
        case comp_op_lt: operator = str_create("Is Less Than"); break;
        case comp_op_gt: operator = str_create("Is Greater Than"); break;
    }
    Serializable *s1 = serializable_create_str(operator);
    htable_set(action->parameters, "WFCondition", s1);

    Serializable *s2 = serializable_create_int(0);
    htable_set(action->parameters, "WFControlFlowMode", s2);

    action_complete_comp_operand(action, comp.operator, comp.op2);

    release(s);
    release(s1);
    release(s2);
    release(uuid);
    release(operator);

    return action;
}

Action *
action_create_ask_input(Operand op, char *input_type) {
    Action *action = action_create(WF_ask);

    if (op.type == op_string) {
        Interpolated *interpolated = interpolated_create(op.value);

        Serializable *s1 = interpolated_parameters(interpolated);
        htable_set(action->parameters, "WFAskActionPrompt", s1);

        release(interpolated);
        release(s1);
    } else if (op.type == op_variable) {
        Interpolated *interpolated = interpolated_create_from_token(op.name.value);

        Serializable *s1 = interpolated_parameters(interpolated);
        htable_set(action->parameters, "WFAskActionPrompt", s1);
        release(interpolated);
        release(s1);
    } else if (op.type == op_null) {
    } else {
        fprintf(stderr, "Invalid parameter in AskNumber()\n");
    }

    String *empty = str_init();
    Serializable *s2 = serializable_create_str(empty);
    htable_set(action->parameters, "WFAskActionDefaultAnswer", s2);

    String *number = str_create(input_type);
    Serializable *s3 = serializable_create_str(number);
    htable_set(action->parameters, "WFInputType", s3);

    release(number);
    release(s3);
    release(empty);
    release(s2);
    return action;
}

Action *
action_create_show_result(Operand op) {
    Action *action = action_create(WF_show_result);

    if (op.type == op_string) {
        Interpolated *interpolated = interpolated_create(op.value);

        Serializable *s1 = interpolated_parameters(interpolated);
        htable_set(action->parameters, "Text", s1);
        release(interpolated);
        release(s1);
    } else if (op.type == op_variable) {
        Interpolated *interpolated = interpolated_create_from_token(op.name.value);

        Serializable *s1 = interpolated_parameters(interpolated);
        htable_set(action->parameters, "Text", s1);
        release(interpolated);
        release(s1);
    } else {
        fprintf(stderr, "Invalid parameter in ShowResult()\n");
    }

    return action;
}

Action *
action_create_round_number(const char *round_mode, const char *round_type) {
    Action *action = action_create(WF_round_number);

    String *round_mode_value = str_create(round_mode);
    Serializable *s1 = serializable_create_str(round_mode_value);
    htable_set(action->parameters, "WFRoundMode", s1);

    String *round_type_value = str_create(round_type);
    Serializable *s2 = serializable_create_str(round_type_value);
    htable_set(action->parameters, "WFRoundType", s2);

    release(round_mode_value);
    release(s1);
    release(round_type_value);
    release(s2);

    return action;
}

Action *
action_create_get_item_name() {
    Action *action = action_create(WF_get_item_name);

    return action;
}

Action *
action_create_get_item_type() {
    Action *action = action_create(WF_get_item_type);

    return action;
}

Action *
action_create_view_content_graph(void) {
    Action *action = action_create(WF_view_content_graph);

    return action;
}

Action *
action_create_wait(Operand op) {
    Action *action = action_create(WF_wait);

    if (op.type == op_number) {
        Serializable *s = serializable_create_float(atof(op.value.value));
        htable_set(action->parameters, "WFDelayTime", s);
        release(s);
    }

    return action;
}

Action *
action_create_exit() {
    Action *action = action_create(WF_exit);

    return action;
}

List *
action_create_cond_control(int value, int control_count) {
    List * actions = list_init();

    Operand op;
    op.type = op_number;
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
    action_set_uuid(close_action, action->uuid);

    String *uuid = str_create(close_action->uuid);
    Serializable *s = serializable_create_str(uuid);
    htable_set(close_action->parameters, "GroupingIdentifier", s);

    Serializable *s1 = serializable_create_int(2);
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
        case WF_nothing:
        case WF_show_result:
        case WF_ask:
        case WF_round_number:
        case WF_get_item_name:
        case WF_get_item_type:
        case WF_view_content_graph:
        case WF_wait:
        case WF_exit:
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
        case WF_nothing: fprintf(output, "nothing"); break;
        case WF_set_variable: fprintf(output, "setvariable"); break;

        case WF_ask: fprintf(output, "ask"); break;
        case WF_show_result: fprintf(output, "showresult"); break;
        case WF_round_number: fprintf(output, "round"); break;
        case WF_get_item_name: fprintf(output, "getitemname"); break;
        case WF_get_item_type: fprintf(output, "getitemtype"); break;
        case WF_view_content_graph: fprintf(output, "viewresult"); break;
        case WF_wait: fprintf(output, "delay"); break;
        case WF_exit: fprintf(output, "exit"); break;
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
