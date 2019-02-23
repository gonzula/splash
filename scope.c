#include "scope.h"
#include "action.h"
#include "utils.h"
#include "structures/structures.h"

#include <string.h>

void _scope_release(void *obj);

Scope *
scope_create() {
    Scope *scope = (Scope *)alloc(sizeof(Scope), _scope_release);
    char uuid[37];
    uuid_gen(uuid);
    scope->name = str_create(uuid);
    scope->actions = list_init();
    scope->parent_name = NULL;

    htable_set(scopes, uuid, scope);

    scope_clear_last_uuid(scope);

    return scope;
}

void
scope_output(Scope *scope, FILE *output) {
    char last_uuid_output[37];
    *last_uuid_output = 0;
    LIST_LOOP(scope->actions) {
        Action *action = (Action *)node->object;
        if (strcmp(last_uuid_output, action->uuid) == 0) {
            DEBUGPRINT("Skipping action\n");
            continue;
        }

        action_output(action, output);
        strcpy(last_uuid_output, action->uuid);
    }
}

void
scope_clear_last_uuid(Scope *scope) {
    *(scope->last_uuid) = 0;
}

void
scope_add_action(Scope *scope, Action *action) {
    list_append(scope->actions, action);
    strcpy(scope->last_uuid, action->uuid);
}

void
scope_add_actions(Scope *scope, List *actions) {
    LIST_LOOP(actions) {
        Action *action = (Action *)node->object;
        scope_add_action(scope, action);
    }
}

void
_scope_release(void *obj) {
    Scope *scope = (Scope *)obj;

    if (scope->name) {
        release(scope->name);
    }
    release(scope->actions);
}
