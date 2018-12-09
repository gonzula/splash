#ifndef SPLASH_HELPER_H
#define SPLASH_HELPER_H

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

void helper_init();

void append_operand(Operand *, OpType, char100);
void append_operation(Operand *, char, Operand, Operand);
void append_minus_op(Operand *, Operand);
void set_variable(char100, Operand);

void append_comparison(Comparison *, CompOp, Operand, Operand);
void append_conditional(Comparison);

int if_count;

#endif  /* SPLASH_HELPER_H */
