#ifndef SPLASH_HELPER_H
#define SPLASH_HELPER_H

#include <stdio.h>
#include <stdbool.h>
#include "structures/structures.h"

typedef struct {
    String *name;
    char uuid[37];
    int position;
} StringToken;

typedef struct {
    String *str;
    List *tokens;
} Interpolated;  // Interpolated String

typedef enum {
    op_number,
    op_magic_variable,  // referenced by uuid
    op_variable, // referenced by name
    op_string,
    op_null
} OpType;

typedef struct {
    OpType type;
    String *value;
    String *name;
    char uuid[37];
} Operand;

typedef enum {
    comp_op_eq,
    comp_op_lt,
    /* comp_op_le,  not implemented */
    comp_op_gt
    /* comp_op_ge not implemented */
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
    WF_text,
    WF_nothing,
    WF_set_variable,

    // functions
    WF_ask,
    WF_show_result,
    WF_round_number,
    WF_get_item_name,
    WF_get_item_type,
    WF_view_content_graph,
    WF_wait,
    WF_exit,
    WF_wait_to_return,
    WF_get_battery_level,
    WF_date,
    WF_extract_archive,
    WF_get_current_location
} ActionID;

typedef struct {
    ActionID id;
    HashTable *parameters;
    char uuid[37];  /* In case of groups, should be the same for all the actions in the group */
    Scope *sub_scope; /* for groups, like if and loop */
    int cond_control_count;
    bool cond_should_close_control;
} Action;

FILE *input_file;
FILE *output_file;
Scope *current_scope;
HashTable *scopes;
int if_count;
int helper_error;
char *error_message;

FILE *init_parse(const char *in_file_name, const char *out_file_name);  /* Must be called before starting parse */
void end_parse(void);  /* Must be called after ending parse */

void increment_if_count(void);

void action_add_subaction(Action *this, Action *other);

void append_operand(Operand *, OpType, String *);
void append_null_operand(Operand *);
int append_func_call(Operand *, String *, Operand parameter);
void append_operation(Operand *, char, Operand, Operand);
void append_minus_op(Operand *, Operand);
void set_variable(String *, Operand);
void place_set_variable(String *var_name);
void place_operand(Operand op, bool force_null);

void append_comparison(Comparison *, CompOp, Operand, Operand);
void append_cond_control(void);
void append_conditional(Comparison);
void append_else(void);
void close_scope(void);

void operand_release(Operand op);


#endif  /* SPLASH_HELPER_H */
