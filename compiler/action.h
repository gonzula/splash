#ifndef ACTION_H
#define ACTION_H

#include <stdio.h>
#include "splash_helper.h"

Action *action_init(void);
Action *action_create(ActionID id);
Action *action_create_number(Operand op);
Action *action_create_text(Operand op);
Action *action_create_get_variable(Operand op);
Action *action_create_get_magic_variable(Operand op);
Action *action_create_math_operation(char operator, Operand op2);
Action *action_create_set_variable(char100 var_name);
Action *action_create_comp(Comparison comp);

Action *action_create_ask_number(Operand op);
Action *action_create_show_result(Operand op);

List * action_create_cond_control(int value, int control_count);

void action_output(Action *action, FILE *output);

#endif  /* ACTION_H */
