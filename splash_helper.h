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


void append_operand(Operand **, OpType, char100);
void append_operation(Operand **, char, Operand *, Operand *);
void append_minus_op(Operand **, Operand *);
void set_variable(char100, Operand *op);

void output_header(FILE *output);
void output_footer(FILE *output);
