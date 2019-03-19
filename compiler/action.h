#ifndef ACTION_H
#define ACTION_H

#include <stdio.h>
#include "splash_helper.h"

Action *action_create(ActionID id);
Action *action_create_number(Operand op);
Action *action_create_text(Operand op);
Action *action_create_nothing(void);
Action *action_create_get_variable(Operand op);
Action *action_create_get_magic_variable(Operand op);
Action *action_create_math_operation(char operator, Operand op2);
Action *action_create_set_variable(char100 var_name);
Action *action_create_comp(Comparison comp);

Action *action_create_ask_input(Operand op, char *input_type);
Action *action_create_show_result(Operand op);
Action *action_create_round_number(const char *round_mode, const char *round_type);
Action *action_create_get_item_name(void);
Action *action_create_get_item_type(void);
Action *action_create_view_content_graph(void);
Action *action_create_wait(Operand op);
Action *action_create_exit(void);


List * action_create_cond_control(int value, int control_count);

void action_output(Action *action, FILE *output);

#endif  /* ACTION_H */
