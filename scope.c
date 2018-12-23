#include "scope.h"
#include "structures/structures.h"

void _scope_release(void *obj);

Scope *
scope_create(char *name) {
    Scope *scope = (Scope *)alloc(sizeof(Scope), _scope_release);
    scope->actions = list_init();
    scope->parent_name = NULL;

    htable_set(scopes, name, scope);

    return scope;
}

void
scope_add_action(Scope *scope, Action *action) {
    list_append(scope->actions, action);
}

void
_scope_release(void *obj) {
    Scope *scope = (Scope *)obj;

    if (scope->parent_name) {
        release(scope->parent_name);
    }
    release(scope->actions);
}
