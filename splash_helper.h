#ifndef SPLASH_HELPER_H
#define SPLASH_HELPER_H

#include "structures/structures.h"

typedef union {
    char value[100];
} char100;

typedef enum {
    number,
    magicVariable,
    variable
} OpType;

typedef struct {
    OpType type;
    char100 value;
    char100 name;
    char uuid[37];
} Operand;

typedef enum {
    CompOpEQ,
    CompOpLT,
    /* CompOpLE,  not implemented */
    CompOpGT
    /* CompOpGE not implemented */
} CompOp;

typedef struct {
    Operand op1;
    Operand op2;
    CompOp operator;
} Comparison;

typedef struct {
    String *name;
    List *actions;
    String *parent_name;
    char last_uuid[37];
} Scope;

typedef enum {
    WF_conditional,
    WF_get_variable,
    WF_math,
    WF_number,
    WF_set_variable
} ActionID;

typedef struct {
    ActionID id;
    HashTable *parameters;
    char uuid[37];  /* In case of groups, should be the same for all the actions in the group */
    Scope *sub_scope; /* for groups, like if and loop */
} Action;

Scope *current_scope;
HashTable *scopes;
int if_count;

void init_parse();  /* Must be called before starting parse */
void end_parse();  /* Must be called after ending parse */

Action *action_init();
Action *action_create(ActionID id);

void action_add_subaction(Action *this, Action *other);

void append_operand(Operand *, OpType, char100);
void append_operation(Operand *, char, Operand, Operand);
void append_minus_op(Operand *, Operand);
void set_variable(char100, Operand);
void place_set_variable(char100 var_name);
void place_operand(Operand op);

void append_comparison(Comparison *, CompOp, Operand, Operand);
void append_conditional(Comparison);
void close_scope();


#endif  /* SPLASH_HELPER_H */
