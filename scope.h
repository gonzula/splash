#ifndef SCOPE_H
#define SCOPE_H

#include "splash_helper.h"

Scope *scope_create(char *name);
void scope_add_action(Scope *scope, Action *action);

#endif  /* SCOPE_H */
