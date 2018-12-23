#ifndef ACTION_H
#define ACTION_H

#include "splash_helper.h"

Action *action_init();
Action *action_create(ActionID id);
Action *action_create_number(Operand op1);
Action *action_create_get_variable(Operand op1);
Action *action_create_get_magic_variable(Operand op1);
Action *action_create_math_operation(char operator, Operand op2);

#endif  /* ACTION_H */
